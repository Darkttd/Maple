#include <Maple/Gameplay/MapleGameLogic.h>
#include <Bibim/Math.h>
#include <Bibim/Assert.h>
using namespace Bibim;

namespace Maple
{
    MapleGameLogic::MapleGameLogic()
        : restLife(DefaultLife)
    {

    }

    MapleGameLogic::MapleGameLogic(int size)
        : puzzleSize(size),
          restLife(DefaultLife)
    {

    }

    MapleGameLogic::MapleGameLogic(int size, const GamePanel& ansPattern)
    {
        Initialize(size, ansPattern);
    }

    void MapleGameLogic::Initialize(int size, const GamePanel& ansPattern)
    {
        puzzleSize = size;
        restLife = DefaultLife;

        this->ansPattern = ansPattern;
    }

    void MapleGameLogic::Restart()
    {
        for(int i = 0; i < puzzleSize; i++)
        {
            for(int j = 0; j < puzzleSize; j++)
            {
                solvePattern.arr[i][j] = TableState::Uncolored;
            }
        }

        restLife = DefaultLife;
    }

    void MapleGameLogic::Update(float /*dt*/, int /*timestamp*/)
    {
        // <TODO> 시간 측정 로직이 들어갈 경우 사용할 것
    }

    const MapleGameLogic::GamePanel& MapleGameLogic::GetSolvePanel() const
    {
        return solvePattern;
    }

    int MapleGameLogic::GetPuzzleSize() const
    {
        return puzzleSize;
    }

    int MapleGameLogic::GetRestLife() const
    {
        return restLife;
    }

    MapleGameLogic::TryFillColorResult::T MapleGameLogic::TryFillColor()
    {
        BBAssert(cursor.X == cursorTail.X || cursor.Y == cursorTail.Y);

        // dic은 cursorTail에서 cursor을 향하는 방향
        Point2 dic = cursor - cursorTail;

        TryFillColorResult::T result = TryFillColorResult::NonChanged;

        if(dic == Point2::Zero)
        {
            // 한 칸만 선택된 상태에서 O 버튼을 누를 경우,
            // 대상에 X 표식이 있을 경우에도 지우는 식으로 동작합니다.

            if(solvePattern.arr[cursor.X][cursor.Y] == TableState::Uncolored)
            {
                if(ansPattern.arr[cursor.X][cursor.Y] == TableState::Uncolored)
                {
                    // 정답이 아닌 경우, 오답으로 표기합니다

                    solvePattern.arr[cursor.X][cursor.Y] = TableState::ForceErased;

                    Incorrect();
                    result = TryFillColorResult::Incorrect;
                }
                else
                {
                    // 정답인 경우 색칠합니다.

                    solvePattern.arr[cursor.X][cursor.Y] = TableState::Colored;
                    result = TryFillColorResult::Correct;
                }
            }
            else if(solvePattern.arr[cursor.X][cursor.Y] == TableState::Erased)
            {
                // X 표가 되어있는 경우, X 표식을 지웁니다.

                solvePattern.arr[cursor.X][cursor.Y] = TableState::Uncolored;

                result = TryFillColorResult::NonChanged;
            }
        }
        else
        {
            if(dic.X != 0)
                dic.X /= Math::Abs(dic.X);
            else if(dic.Y != 0)
                dic.Y /= Math::Abs(dic.Y);

            result = TryFillColorResult::Correct;

            for(Point2 currentPoint = cursorTail; currentPoint != (cursor + dic); currentPoint += dic)
            {
                if(solvePattern.arr[currentPoint.X][currentPoint.Y] == TableState::Uncolored)
                {
                    // 대상에 아무 표식이 없을 때만 작동합니다

                    if(ansPattern.arr[currentPoint.X][currentPoint.Y] == TableState::Uncolored)
                    {
                        // 정답이 아닌 경우, 오답으로 표기하고 색칠을 중단합니다

                        solvePattern.arr[currentPoint.X][currentPoint.Y] = TableState::ForceErased;

                        Incorrect();
                        result = TryFillColorResult::Incorrect;
                        break;
                    }
                    else
                    {
                        // 정답인 경우 색칠합니다.

                        solvePattern.arr[currentPoint.X][currentPoint.Y] = TableState::Colored;
                    }
                }
            }

            cursorTail = cursor;
        }

        if(GetRestLife() < 0)
        {
            result = TryFillColorResult::GameOver;
        }
        else if(WinCheck())
        {
            result = TryFillColorResult::Clear;
        }
        
        return result;
    }

    void MapleGameLogic::TryFillErased()
    {
        BBAssert(cursor.X == cursorTail.X || cursor.Y == cursorTail.Y);

        // dic은 cursorTail에서 cursor을 향하는 방향
        Point2 dic = cursor - cursorTail;

        if(dic.X != 0)
            dic.X /= Math::Abs(dic.X);
        else if(dic.Y != 0)
            dic.Y /= Math::Abs(dic.Y);
        else
            dic.Y = 1;

        bool eraseMode = true;

        if(solvePattern.arr[cursorTail.X][cursorTail.Y] == TableState::Erased || solvePattern.arr[cursorTail.X][cursorTail.Y] == TableState::ForceErased)
        {
            // 시작점이 X 일 경우, X 를 지우는 형식으로 동작합니다.

            eraseMode = false;
        }

        for(Point2 currentPoint = cursorTail; currentPoint != (cursor + dic); currentPoint += dic)
        {
            if(eraseMode && solvePattern.arr[currentPoint.X][currentPoint.Y] == TableState::Uncolored)
            {
                // 시작점이 X가 아니고, 아무 것도 없는 칸인 경우, X 표를 합니다.
                solvePattern.arr[currentPoint.X][currentPoint.Y] = TableState::Erased;
            }
            else if(!eraseMode && solvePattern.arr[currentPoint.X][currentPoint.Y] == TableState::Erased)
            {
                // 시작점이 X이고, X가 칠해져 있는 칸이면, X를 지웁니다.
                solvePattern.arr[currentPoint.X][currentPoint.Y] = TableState::Uncolored;
            }
        }

        cursorTail = cursor;
    }

    std::vector<std::vector<int> > MapleGameLogic::GetHorizontalNumberCount() const
    {
        std::vector<std::vector<int> > pattern;

        // <TODO> Draw() 함수의 수행시간을 줄여야 한다면, pattern 을 한번만 계산하도록 변경해야 합니다.

        for(int i = 0; i < puzzleSize; i++)
        {
            std::vector<int> current;

            int j = puzzleSize - 1;
            int counter = 0;

            while(j >= 0)
            {
                if(ansPattern.arr[i][j] == TableState::Colored)
                    counter++;
                else
                {
                    if(counter > 0)
                    {
                        current.push_back(counter);
                        counter = 0;
                    }
                }

                j--;
            }

            if(counter > 0)
                current.push_back(counter);

            if(current.size() == 0)
                current.push_back(0);

            pattern.push_back(current);
        }

        return pattern;
    }

    std::vector<std::vector<int> > MapleGameLogic::GetVerticalNumberCount() const
    {
        std::vector<std::vector<int> > pattern;

        // <TODO> Draw() 함수의 수행시간을 줄여야 한다면, pattern 을 한번만 계산하도록 변경해야 합니다.

        for(int i = 0; i < puzzleSize; i++)
        {
            std::vector<int> current;

            int j = 0;
            int counter = 0;
            
            while(j < puzzleSize)
            {
                if(ansPattern.arr[j][i] == TableState::Colored)
                    counter++;
                else
                {
                    if(counter > 0)
                    {
                        current.push_back(counter);
                        counter = 0;
                    }
                }

                j++;
            }

            if(counter > 0)
                current.push_back(counter);

            if(current.size() == 0)
                current.push_back(0);

            pattern.push_back(current);
        }

        return pattern;
    }

    Point2 MapleGameLogic::GetCursor() const
    {
        return cursor;
    }

    void MapleGameLogic::SetCursor(const Bibim::Point2& value)
    {
        cursor.X = Math::Clamp(value.X, 0, puzzleSize - 1);
        cursor.Y = Math::Clamp(value.Y, 0, puzzleSize - 1);
    }

    Point2 MapleGameLogic::GetCursorTail() const
    {
        return cursorTail;
    }

    void MapleGameLogic::SetCursorTail(const Bibim::Point2& end)
    {
        Point2 clamped;
        clamped.X = Math::Clamp(end.X, 0, puzzleSize - 1);
        clamped.Y = Math::Clamp(end.Y, 0, puzzleSize - 1);

        cursorTail = clamped;
        cursor = clamped;
    }

    void MapleGameLogic::CursorMove(const Bibim::Point2& dPoint)
    {
        if(Math::Abs(dPoint.X) + Math::Abs(dPoint.Y) != 1)
            return;

        SetCursorTail(cursor + dPoint);
    }

    bool MapleGameLogic::WinCheck()
    {
        // 이겼는지 체크하는 로직입니다.
        // 체크된 칸이 모두 일치하면 통과입니다.

        bool result = true;

        for(int i = 0; i < puzzleSize; i++)
        {
            for(int j = 0; j < puzzleSize; j++)
            {
                if(ansPattern.arr[i][j] == TableState::Colored && solvePattern.arr[i][j] != TableState::Colored)
                {
                    result = false;
                    break;
                }
            }

            if(!result)
                break;
        }

        return result;
    }

    void MapleGameLogic::Incorrect()
    {
        // 라이프만 깍습니다.

        // 남은 라이프는 UIGameLogic 쪽에서 꺼내서 사용합니다.

        restLife--;
    }
}
