#!/bin/bash
# Plays a short path through Maple on a virtual X display and fails if the game
# crashes, exits with an error, logs an error, does not quit in time, or does
# not save the cleared puzzle:
# title -> settings -> title -> puzzle select -> first 5x5 puzzle -> pause and
# resume -> solve it -> unlocked picture -> picture -> puzzle select -> title -> quit.
#
# Needs Xvfb, xdotool and ImageMagick (import). Screenshots of each step, the
# game log and the save data go to the output directory.
#
# Clicks use fixed positions in the 800x480 window, so a change to the UI layout
# or to the first puzzle needs a change here too.
set -euo pipefail

if [ $# -ne 2 ]; then
    echo "usage: $0 <Maple executable> <output directory>" >&2
    exit 2
fi
game=$(realpath "$1")
out=$(realpath -m "$2")
deploy=$(realpath "$(dirname "$0")/../Deploy")
mkdir -p "$out/data"

error_log='<ERROR>|AddressSanitizer|runtime error:'
# Known error: Deploy/ lacks Asset/Script/Localization-en (Darkttd/bibim#8 phase 6).
known_error='Couldn.t open lua file\. \(Asset/Script/Localization-en\)'

Xvfb -displayfd 3 -screen 0 1024x768x24 -nolisten tcp 3>"$out/display" 2>"$out/xvfb.log" &
xvfb_pid=$!
game_pid=
cleanup() {
    [ -n "$game_pid" ] && kill "$game_pid" 2>/dev/null || true
    kill "$xvfb_pid" 2>/dev/null || true
}
trap cleanup EXIT
for _ in $(seq 50); do
    [ -s "$out/display" ] && break
    sleep 0.1
done
export DISPLAY=:$(cat "$out/display")

(
    cd "$deploy"
    # EGL instead of GLX: SDL 3.4.2 on the CI runner found no matching GLX visual
    # under Xvfb, while EGL works with the same Mesa.
    export SDL_VIDEO_DRIVER=x11 SDL_VIDEO_FORCE_EGL=1 BIBIM_AUDIO_DRIVER=null XDG_DATA_HOME="$out/data"
    exec "$game"
) >"$out/game.log" 2>&1 &
game_pid=$!

check_running() {
    if ! kill -0 "$game_pid" 2>/dev/null; then
        echo "FAIL: the game exited before step $((step + 1)) ($1):" >&2
        cat "$out/game.log" >&2
        game_pid=
        exit 1
    fi
}

step=0
window=
for _ in $(seq 300); do
    check_running "start"
    window=$(xdotool search --onlyvisible --pid "$game_pid" 2>/dev/null | head -1 || true)
    [ -n "$window" ] && break
    sleep 0.1
done
if [ -z "$window" ]; then
    echo "FAIL: the game did not open its window in 30 seconds" >&2
    exit 1
fi

settle() {
    sleep "$1"
    check_running "$2"
    step=$((step + 1))
    import -window root "$out/$(printf '%02d' "$step")-$2.png"
}

# The engine reads the mouse state once a frame, so press and release must land
# in different frames.
click() {
    check_running "click at $1,$2"
    xdotool mousemove --window "$window" "$1" "$2"
    sleep 0.05
    xdotool mousedown 1
    sleep 0.15
    xdotool mouseup 1
}

key() {
    check_running "key $1"
    xdotool keydown "$1"
    sleep 0.15
    xdotool keyup "$1"
}

settle 5 title
click 666 375; settle 2 settings
key Escape; settle 2 title
click 666 196; settle 2 puzzle-select
click 266 130; settle 3 puzzle
key Escape; settle 1 paused
key Escape; settle 1 resumed

# First puzzle's answer as <row><column> digits. Clicking a cell moves the
# cursor there; the O button (728,424) fills it.
cell_x=(337 411 485 559 633)
cell_y=(145 219 293 367 441)
for cell in 11 15 21 22 24 25 31 33 35 41 43 45 51 55; do
    click "${cell_x[${cell:1:1} - 1]}" "${cell_y[${cell:0:1} - 1]}"
    sleep 0.2
    click 728 424
    sleep 0.2
done
settle 5 cleared

click 728 424; settle 2 picture-unlocked
click 728 424; settle 2 picture
# Any click closes the picture.
click 400 240; settle 2 puzzle-select
click 728 424; settle 2 title
click 666 434

status=0
if ! timeout 30 tail --pid="$game_pid" -f /dev/null; then
    echo "FAIL: the game did not quit after 'Exit' was clicked" >&2
    import -window root "$out/timeout.png"
    exit 1
fi
wait "$game_pid" || status=$?
game_pid=
if [ "$status" -ne 0 ]; then
    echo "FAIL: the game exited with status $status" >&2
    exit 1
fi
if ! grep -q -a 'MOA001_Clear' "$out/data/Maple/maple_savedata"; then
    echo "FAIL: the save data does not record the first puzzle as cleared" >&2
    exit 1
fi
errors=$(grep -E "$error_log" "$out/game.log" | grep -E -v "$known_error" || true)
if [ -n "$errors" ]; then
    echo "FAIL: errors in the game log:" >&2
    echo "$errors" >&2
    exit 1
fi
echo "Smoke run passed."
