Gameplay = 
{
    TimerID = 
    {
        Default = 34534,
    },

    Timer = {},
};

function Gameplay.Timer.once(duration, complete)
    th.timeout(duration, Gameplay.TimerID.Default, complete);
end

function Gameplay.Timer.loop(interval, duration, update, complete, cancel)
    th.alarm(interval, duration, Gameplay.TimerID.Default, update, complete, cancel);
end

function Gameplay.Timer.forever(interval, update)
    if (type(interval) == 'function') then
        update = interval;
        interval = 1/60;
    end

    th.alarm(interval, 9999, Gameplay.TimerID.Default, update);
end

function Gameplay.Timer.cancel(id)
    if (type(id) == 'string') then
        id = Gameplay.TimerID[id];
    end

    th.cancelalarm(id);
end

function Gameplay.Timer.cancelAll()
    for _, v in pairs(Gameplay.TimerID) do
        th.cancelalarm(v);
    end
end

function goToGame(pageNumber, puzzleNumber)
    --_ALERT('goToGame... puzzleNumber = ' .. puzzleNumber);

    th.resetloadingstatus();
    
    character = character or 'Moa';
    
    --local session = GamingSession():Reset(3)
    --                               :SetCharacter(character)
    --                               :SetDifficulty(difficulty)

    local root  = ui.getroot();
    local world = ui.createworld(nil, true):SetName('World');
    local puzzleSize = -1;
    
    setupLoadingScreen(root);

    local stageCharacter = string.sub(PUZZLES[puzzleNumber + 1].StageName, 1, 3);
    local targetMusicNumber;
    if (stageCharacter == 'MOA') then
        targetMusicNumber = math.random(2) + 1;
    else
        targetMusicNumber = math.random(2) + 3;
    end

    world:Initialize(th.patternman(), puzzleNumber);
    playMusic(targetMusicNumber);

    do
        local layout = th.load('Asset/Gameplay/ingameUI');
        local window = layout:GetRoot():Clone();
        window:SetXY(0.0, 0.0)
              :SetXYMode('ABS')
              :SetZOrder(10);
        window:SetName('ingameUI');
        world:SetUILayer(window);

        do
            -- <TODO> 임시 코드입니다. Session 개념이 들어간다면,
            -- 퍼즐 사이즈는 그쪽으로부터 불러와야 합니다.

            puzzleSize = world:GetPuzzleSize();
        end

        do
            local rightPanel = ui(window, 'RightPanel');

            rightPanel:AddChild(UILabel():SetXY(3, 135)
                                         :SetXYMode('ABS')
                                         :AlignTo('CENTERTOP')
                                         :SetOrigin(0.5, 0.5)
                                         :SetFont(ui.font({
                                            size=12,
                                            color={255,255,255},
                                            stroke=1,
                                            strokeColor={0,0,0},
                                            weights=1
                                         }))
                                         :SetText(PUZZLES[puzzleNumber + 1].StageName)
                                         :SetPickable(false)
                                         :SetName('StageLabel'));

            rightPanel:AddChild(UILabel():SetXY(0, 212)
                                         :SetXYMode('ABS')
                                         :AlignTo('CENTERTOP')
                                         :SetOrigin(0.5, 0.5)
                                         :SetFont(ui.font({
                                            size=17,
                                            color={255,255,255},
                                            stroke=1,
                                            strokeColor={0,0,0},
                                            weights=1
                                         }))
                                         :SetText('5')
                                         :SetPickable(false)
                                         :SetName('RestLifeLabel'));
        end

        local buttons = ui.getAllButtons(window, true);

        local function controlPanel(target, x, y)
            local w = ui.parent(target, "UIGameLogic");


            local panelX, panelY = target:GetXY();
            local panelWidth, panelHeight = target:GetSize();

            local contextOffsetX, contextOffsetY = w:GetContextOffset();

            x, y = w:GetInvertedContextCoordinate(x, y);

            --_ALERT('....contextOffset=(' .. contextOffsetX .. ', ' .. contextOffsetY .. ')');
            --_ALERT('....target=(' .. panelX .. ', ' .. panelY .. '), pos = (' .. x .. ', ' .. y .. ')');

            local dx = (x - contextOffsetX) - panelX - panelWidth / 2;
            local dy = (y - contextOffsetY) - panelY - panelHeight / 2;

            if (math.abs(dx) < math.abs(dy)) then
                dx = 0;
            else
                dy = 0;
            end

            if (dx ~= 0) then dx = dx / math.abs(dx); end
            if (dy ~= 0) then dy = dy / math.abs(dy); end

            w:CursorMove(dx, dy);
        end

        local function tryFillColor(target)
            local w = ui.parent(target, "UIGameLogic");

            w:TryFillColor();
        end

        local function tryFillErased(target)
            local w = ui.parent(target, "UIGameLogic");

            w:TryFillErased();
        end

        --th.canceltimeout(EyeBlinkingCallback);
        blinkingStopper();

        if (stageCharacter == 'MOA') then
            showStanding('moa_closet', -65, 0, window, 130, 'Kradmoa');
        else
            showStanding('moa_closet', -65, 0, window, 130, 'Maple');
        end

        -- ui(window, 'CheatWin'):SetZOrder(140);
        -- ui.on(ui(window, 'CheatWin'), 'click',
        --     function (target)
        --         _ALERT('Cheat_Clear');
        --         Gameplay.showClearUI();
        --     end);

        ui(window, 'PauseMenuPanel'):Hide();
        ui(window, 'GameOverPanel'):Hide();
        ui(window, 'ClearUI'):Hide();
        ui(window, 'UnlockUI'):Hide();
        ui(window, 'ClothUnlockUI'):Hide();

        ui(window, 'Background'):SetZOrder(128);
        ui(window, 'ControlPanel'):SetZOrder(132);
        ui(window, 'GamePanel'):SetZOrder(132);
        ui(window, 'PauseMenuPanel'):SetZOrder(135);
        ui(window, 'GameOverPanel'):SetZOrder(135);

        --업밸류로 접근하지 말고, 람다 안에서는 ui() 함수로 접근해서 처리할 것
        --다시말해, 람다에서는 window 변수 쓰지 말것
        ui.on(ui(window, 'ControlPanel'), 'click', controlPanel);
        ui.on(ui(window, 'PauseButton'), 'click', Gameplay.showPauseMenu);
        ui.on(ui(window, 'OButton'), 'click', tryFillColor);
        ui.on(ui(window, 'XButton'), 'click', tryFillErased);
        ui.on(ui(window, 'moa_closet'), 'click',
            function()
                local standingWindow = ui(ui(world, 'ingameUI'), 'moa_closet');
                local targetEmotion = (math.random(2) == 1) and 'Shying' or 'Angry';
                showEmotion(standingWindow, 1.0, targetEmotion);
            end);

        ui.effectorize(ui(window, 'ControlPanel'));
        ui.effectorize(ui(window, 'PauseButton'));
        ui.effectorize(ui(window, 'OButton'));
        ui.effectorize(ui(window, 'XButton'));

        --PauseMenuPanel 을 world의 자식으로 등록하면서, ingameUI의 자식에서 해제되므로,
        --위의 이벤트 등록이 끝난 뒤에 배치합니다.
        world:SetPauseUILayer(ui(window, 'PauseMenuPanel'));
        world:SetGameOverUILayer(ui(window, 'GameOverPanel'));
    end
    
    world:GetCursorLayer():RemoveAllChildren();

    do
        local cursorImage = UIImage():SetTexture(th.load('Asset/Gameplay/Cursor' .. puzzleSize));
        cursorImage:SetPickable(false);
        cursorImage:SetFocusable(false);
        local childCursor = UIWindow();
        childCursor:SetSize(cursorImage:GetSource():GetWidth(), cursorImage:GetSource():GetHeight())
            :SetSizeMode('ABS'):SetTransform(UITransform3D());
        childCursor:SetName('AnimatedCursor');
        childCursor:AddChild(cursorImage)
        childCursor:SetOrigin(0.5, 0.5);
        childCursor:SetPickable(false);
        childCursor:SetFocusable(false);

        local stageCharacter = string.sub(PUZZLES[puzzleNumber + 1].StageName, 1, 3);
        local targetMusicNumber;
        if (stageCharacter == 'MOA') then
            targetMusicNumber = math.random(2) + 1;
        else
            targetMusicNumber = math.random(2) + 3;
        end

        local bpm = MUSICS[targetMusicNumber].BPM;
        local blinkingTime = 60.0 / bpm;
        --_ALERT('blinkingTime = ' .. blinkingTime);

        th.alarm(blinkingTime, 999999.0, CursorBlinkingCallback, function()
            anim.cursorblink(world:GetCursorLayer(), childCursor, blinkingTime);
        end);

        world:GetCursorLayer():AddChild(childCursor);
    end

    local testLabel = UIWindow():SetName('LoadingScreen')
                              :AddChild(UIRect():SetSize(1, 1)
                                                :SetSizeMode('REL')
                                                :SetFillColor(0, 0, 0))
                              :AddChild(UILabel():SetName('LoadingLabel')
                                                 :SetX(40)
                                                 :SetY(40)
                                                 :SetText('1234567890')
                                                 :SetFont(ui.font({size=24,color={255,255,255}, weights=1.5, spacing=1.1}))
                                                 :AlignTo('RIGHTBOTTOM'));
    root:AddChild(testLabel);
    testLabel:BringToFront();

    world:SetZeroPointOffset(301.0, 108.0);

    if(puzzleSize == 5) then
        world:SetBlockPixelSize(73.0);
    elseif(puzzleSize == 10) then
        world:SetBlockPixelSize(36.0);
    elseif(puzzleSize == 15) then
        world:SetZeroPointOffset(303.0, 111.0);
        world:SetBlockPixelSize(23.5);
    elseif(puzzleSize == 20) then
        world:SetZeroPointOffset(302.0, 110.0);
        world:SetBlockPixelSize(17.5);
    end

    world:SetInterBlockOffset(1.0);
    root:AddChild(world);
    world:UpdateCursorLayer(0, 0);

    do
        local numbers;

        if(puzzleSize == 20) then
            numbers = th.load('Asset/Gameplay/PuzzleNumber20');
        elseif(puzzleSize == 15) then
            numbers = th.load('Asset/Gameplay/PuzzleNumber15');
        end

        if(numbers ~= nil) then
            local window = numbers:GetRoot():Clone();
            window:SetXY(0.0, 0.0)
                  :SetXYMode('ABS')
                  :SetZOrder(8);
            window:SetName('puzzleNumbers');
            window:SetPickable(false);
            window:SetFocusable(false);

            world:SetNumberLayer(window);
        end
    end

    do
        -- n x n 의 퍼즐 UI 만 남기고 모두 Visible 을 끄는 로직입니다.

        local gamePanel = ui(world, 'ingameUI');
        gamePanel = ui(gamePanel, 'GamePanel');

        ui(gamePanel, 'Panel55'):SetVisibility('Invisible');
        ui(gamePanel, 'Panel1010'):SetVisibility('Invisible');
        ui(gamePanel, 'Panel1515'):SetVisibility('Invisible');
        ui(gamePanel, 'Panel2020'):SetVisibility('Invisible');

        ui(gamePanel, 'Panel' .. puzzleSize .. puzzleSize):SetVisibility('Visible');
    end

    do
        local standingWindow = ui(ui(world, 'ingameUI'), 'moa_closet');

        ui.on(world, 'GameClear', function (world)
            --_ALERT('Game Clear');
            Gameplay.showClearUI();
        end);

        ui.on(world, 'GameOver', function (world)
            --_ALERT('Game Over');
            Gameplay.showGameOverUI();
        end);

        ui.on(world, 'OnIncorrect', function (world, restLife)
            showEmotion(standingWindow, 1.0, 'Crying');
            local rightPanel = ui(world, 'RightPanel');
            ui(rightPanel, 'RestLifeLabel'):SetText('' .. restLife);
        end);

        ui.on(world, 'OnCorrect', function (world)
            showEmotion(standingWindow, 1.0, 'Laugh');
        end);

        ui.on(world, 'keydown', function(v, key)

            -- if(key == 'D1') then
            --     -- <TODO> 디버그 코드입니다. 완성되면 지워야 합니다.
            --     _ALERT('cheat_clear');
            --     Gameplay.showClearUI();
            -- end

            if(key == 'SystemMenu') then
                Gameplay.showPauseMenu();
            elseif(key == 'Escape' or key == 'SystemBack') then
                Gameplay.togglePauseMenu();
            end
        end);
    end

    --[[
    do
        -- 현재 코드쪽에서 켜고꺼주고 있으므로 여기에서는 작동하지 않습니다.
        -- 코드쪽의 켜고끄는 코드를 스크립트로 뺄 예정이라면, 이 코드의 주석이 해제되어야 합니다.
        local current = ui(world, 'ingameUI');
        current = ui(current, 'GamePanel');
        current = ui(current, 'Panel55');

        local function invisibleAll(window, count)
            for i=0,4 do
                ui(window, '' .. i):SetVisibility('Invisible');
            end
        end

        invisibleAll(ui(current, 'Horizontal'));
        invisibleAll(ui(current, 'Vertical'));
    end
    --]]

	--SetSource(images:Find("1"));

    ui.focus(world);
end

function Gameplay.togglePauseMenu(target)
    local world = ui('World');
    local pauseMenu = ui(world, 'PauseMenuPanel');

    if(pauseMenu:IsVisible()) then 
        Gameplay.hidePauseMenu(target);
    else
        Gameplay.showPauseMenu(target);
    end
end

function Gameplay.showPauseMenu(target)
    local world = ui('World');
    local pauseMenu = ui(world, 'PauseMenuPanel');

    anim.fadein(pauseMenu);
    pauseMenu:Show();
    blinkingPause();
    th.timescale(CursorBlinkingCallback, 0.0);
    th.timescale(EmotionCallback, 0.0);
    world:SetPauseMenuShowing(true);

    ui.on(ui(pauseMenu, 'Resume'), 'click', Gameplay.hidePauseMenu);
    ui.on(ui(pauseMenu, 'Restart'), 'click', Gameplay.restartGamePuzzle);
    ui.on(ui(pauseMenu, 'Retire'), 'click', Gameplay.navigateToPuzzleSelect);

    ui.effectorize(ui(pauseMenu, 'Resume'));
    ui.effectorize(ui(pauseMenu, 'Restart'));
    ui.effectorize(ui(pauseMenu, 'Retire'));
end

function Gameplay.hidePauseMenu()
    local world = ui('World');
    local pauseMenu = ui(world, 'PauseMenuPanel');
    anim.fadeout(pauseMenu);
    blinkingUnpause();
    th.timescale(CursorBlinkingCallback, 1.0);
    th.timescale(EmotionCallback, 1.0);
    world:SetPauseMenuShowing(false);

    ui.off(ui(pauseMenu, 'Resume'), 'click');
    ui.off(ui(pauseMenu, 'Restart'), 'click');
    ui.off(ui(pauseMenu, 'Retire'), 'click');
end

function Gameplay.showClearUI()
    local world = ui('World');
    th.savedata():Unlock(PUZZLES[world:GetPatternNumber() + 1].StageName .. '_Clear');
    local ingameUI = ui(world, 'ingameUI');
    local clearUI = ui(ingameUI, 'ClearUI');
    th.timescale(CursorBlinkingCallback, 0.0);
    th.timescale(EmotionCallback, 0.0);

    local nextUnlockClothChar = nil;
    local nextUnlockCloth = nil;
    local nextUnlockCG = nil;
    local nextUnlockCG2 = nil;

    do
        -- Unlock Calc Area

        local moaClearCount = th.savedata():GetMoaSolvedCount();
        local nextUnlockTarget = nil;
        local nextUnlockTarget2 = nil;

        local moaC_ct = moaClearCount % 10;
        if (moaC_ct == 0) then
            moaC_ct = 10;
        end
        local moaC_st = (moaClearCount - moaC_ct) / 10 + 1;
        if (moaC_ct < 10) then
            moaC_ct = '0' .. moaC_ct;
        end
        --_ALERT('unlock: ' .. 'MOA' .. moaC_st .. moaC_ct .. 'moaClearCount = ' .. moaClearCount);
        th.savedata():Unlock('MOA' .. moaC_st .. moaC_ct);

        if (moaClearCount == 5) then nextUnlockTarget = 'cg_monicanc_moa';
        elseif (moaClearCount == 7) then nextUnlockTarget = 'KradmoaHanbok';
        elseif (moaClearCount == 10) then nextUnlockTarget = 'cg_ratio_moa';
        elseif (moaClearCount == 15) then nextUnlockTarget = 'cg_sida_moa';
        elseif (moaClearCount == 17) then nextUnlockTarget = 'KradmoaGimono';
        elseif (moaClearCount == 20) then nextUnlockTarget = 'cg_miruslt_moa';
        elseif (moaClearCount == 23) then nextUnlockTarget = 'KradmoaChinaDress';
        elseif (moaClearCount == 25) then nextUnlockTarget = 'cg_sinneoru_moa';
        elseif (moaClearCount == 27) then nextUnlockTarget = 'KradmoaNurse';
        elseif (moaClearCount == 30) then nextUnlockTarget = 'cg_miruslt_moa2';
        elseif (moaClearCount == 33) then nextUnlockTarget = 'KradmoaSwimSuits';
        elseif (moaClearCount == 35) then nextUnlockTarget = 'cg_kaylin_moa';
        elseif (moaClearCount == 37) then nextUnlockTarget = 'KradmoaMaid';
        elseif (moaClearCount == 40) then nextUnlockTarget = 'cg_rnkodd_moa';
        end

        local mapleClearCount = th.savedata():GetMapleSolvedCount();

        local mplC_ct = mapleClearCount % 10;
        if (mplC_ct == 0) then
            mplC_ct = 10;
        end
        local mplC_st = (mapleClearCount - mplC_ct) / 10 + 1;
        if (mplC_ct < 10) then
            mplC_ct = '0' .. mplC_ct;
        end
        --_ALERT('unlock: ' .. 'MPL' .. mplC_st .. mplC_ct .. 'mapleClearCount = ' .. mapleClearCount);
        th.savedata():Unlock('MPL' .. mplC_st .. mplC_ct);

        if (mapleClearCount == 7) then nextUnlockTarget2 = 'MapleHanbok';
        elseif (mapleClearCount == 10) then nextUnlockTarget2 = 'cg_danhobak_mm';
        elseif (mapleClearCount == 15) then nextUnlockTarget2 = 'cg_kaylin_maple';
        elseif (mapleClearCount == 17) then nextUnlockTarget2 = 'MapleGimono';
        elseif (mapleClearCount == 20) then nextUnlockTarget2 = 'cg_monicanc_maple';
        elseif (mapleClearCount == 23) then nextUnlockTarget2 = 'MapleChinaDress';
        elseif (mapleClearCount == 25) then nextUnlockTarget2 = 'cg_sinneoru_maple';
        elseif (mapleClearCount == 27) then nextUnlockTarget2 = 'MapleNurse';
        elseif (mapleClearCount == 30) then nextUnlockTarget2 = 'cg_ssal_maple';
        elseif (mapleClearCount == 33) then nextUnlockTarget2 = 'MapleSwimSuits';
        elseif (mapleClearCount == 35) then nextUnlockTarget2 = 'cg_renian_maple';
        elseif (mapleClearCount == 37) then nextUnlockTarget2 = 'MapleMaid';
        elseif (mapleClearCount == 40) then nextUnlockTarget2 = 'cg_leeori_maple';
        end

        if (nextUnlockTarget == nil and nextUnlockTarget2 ~= nil) then
            nextUnlockTarget = nextUnlockTarget2;
            nextUnlockTarget2 = nil;
        elseif (nextUnlockTarget ~= nil and nextUnlockTarget2 ~= nil) then
            local isAlready1 = th.savedata():IsUnlocked(nextUnlockTarget);

            if isAlready1 then
                nextUnlockTarget = nextUnlockTarget2;
                nextUnlockTarget2 = nil;
            else
                nextUnlockTarget2 = nil;
            end
        end

        if (moaClearCount + mapleClearCount == 1) then
            nextUnlockTarget = 'cg_kaylin_mm';
        end

        if (moaClearCount + mapleClearCount == 80) then
            nextUnlockTarget2 = 'cg_kaylin_moa2';
        end

        --_ALERT('moaClearCount = ' .. moaClearCount .. ', mapleClearCount = ' .. mapleClearCount .. ', nextUnlockTaret = ' .. (nextUnlockTarget or ''));

        -- End Unlock Calc Area

        if (nextUnlockTarget ~= nil) then
            local isAlreadyUnlocked = th.savedata():IsUnlocked(nextUnlockTarget);

            if (isAlreadyUnlocked) then
                -- Do nothing
            else
                th.savedata():Unlock(nextUnlockTarget);

                if (string.sub(nextUnlockTarget, 1, 2) == 'cg') then
                    nextUnlockCG = nextUnlockTarget;
                else
                    if (string.sub(nextUnlockTarget, 1, 2) == 'Kr') then
                        nextUnlockClothChar = 'Kradmoa';
                        nextUnlockCloth = string.sub(nextUnlockTarget, 8);
                    else
                        nextUnlockClothChar = 'Maple';
                        nextUnlockCloth = string.sub(nextUnlockTarget, 6);
                    end
                end
            end
        end

        if (nextUnlockTarget2 ~= nil) then
            local isAlreadyUnlocked = th.savedata():IsUnlocked(nextUnlockTarget2);

            if (isAlreadyUnlocked) then
                -- Do nothing
            else
                th.savedata():Unlock(nextUnlockTarget2);

                -- nextUnlockTarget2 is always cg
                nextUnlockCG2 = nextUnlockTarget2;
            end
        end
    end

    th.savedata():Save();
    anim.fadein(clearUI);
    clearUI:Show();
    ui(ingameUI, 'RightPanel'):Hide();
    ui(ingameUI, 'moa_closet'):Hide();
    ui(ingameUI, 'GamePanel'):Hide();
    ui(ingameUI, 'ControlPanel'):Hide();
    -- ui(ingameUI, 'CheatWin'):Hide();
    ui(world, 'Cursor'):Hide();

    world:SetClearUIShowing(true);

    world:SetZeroPointOffset(204.0, 100.0);

    local nextFunction = Gameplay.navigateToPuzzleSelect;
    local nextPuzzleFunction = Gameplay.navigateToNextPuzzle;

    if (nextUnlockCloth ~= nil) then
        local function showCloth(next, clothName)
            return function (target)
                clearUI:Hide();

                world:SetUnlockUIShowing(true);
                local world = ui('World');
                local ingameUI = ui(world, 'ingameUI');
                local unlockUI = ui(ingameUI, 'ClothUnlockUI');

                _ALERT('showCloth.. clothName = ' .. nextUnlockClothChar .. ' . ' .. nextUnlockCloth);

                showStanding('cloth_Unlock', 400, 30, world:GetUILayer(), 130, nextUnlockClothChar, nextUnlockCloth);

                local interStr = '';

                if (STR.INTERKR_RUR ~= nil) then
                    if (nextUnlockCloth == 'Gimono' or nextUnlockCloth == 'ChinaDress') then
                        interStr = STR.INTERKR_RUR;
                    else
                        interStr = STR.INTERKR_EUR;
                    end
                end

                local targetX = 250;

                if (nextUnlockCloth == 'ChinaDress') then
                    targetX = 270;
                end

                local unlockLabel = UILabel():SetXY(targetX, 350)
                                        :SetXYMode('ABS')
                                        :AlignTo('BOTTOM')
                                        :SetOrigin(0.5, 0.5)
                                        :SetFont(ui.font({
                                            size=25,
                                            color={255,255,255},
                                            stroke=1,
                                            strokeColor={0,0,0},
                                            weights=1,
                                            spacing=1.05
                                        }))
                                        :SetPickable(false)
                                        :SetName('UnlockLabel')
                                        :SetText(STR.CLOTHS[nextUnlockCloth] .. interStr .. STR.UNLOCK_DRESS)
                                        :SetZOrder(141);

                ui(world, 'ingameUI'):AddChild(unlockLabel);

                ui.on(ui(unlockUI, 'Back'), 'click', next);
                ui.effectorize(ui(unlockUI, 'Back'));

                unlockUI:Show();
                return true;
            end;
        end;

        nextFunction = showCloth(nextFunction, nextUnlockCloth);
        nextPuzzleFunction = showCloth(nextPuzzleFunction, nextUnlockCloth);
    elseif (nextUnlockCG ~= nil) then
        local function showCGThumbnail(next, thumbNailName)
            return function (target)
                clearUI:Hide();
                world:SetUnlockUIShowing(true);
                local world = ui('World');
                local ingameUI = ui(world, 'ingameUI');
                local unlockUI = ui(ingameUI, 'UnlockUI');

                _ALERT('showCGThumbnail.. thumbNailName = ' .. thumbNailName);

                local thumbnail = th.load('Asset/Gameplay/RewardIllust/' .. thumbNailName .. '_thumb');

                local thumbnailImage = UIImage():SetTexture(thumbnail)
                                                :SetOpacity(1)
                                                :SetFocusable(false)
                                                :SetTransform(UITransform3D())
                                                :SetZOrder(140);

                local unlockLabel = UILabel():SetXY(420, 380)
                                        :SetXYMode('ABS')
                                        :AlignTo('BOTTOM')
                                        :SetOrigin(0.5, 0.5)
                                        :SetFont(ui.font({
                                            size=25,
                                            color={255,255,255},
                                            stroke=1,
                                            strokeColor={0,0,0},
                                            weights=1,
                                            spacing=1.05
                                        }))
                                        :SetPickable(false)
                                        :SetName('UnlockLabel')
                                        :SetText(STR.UNLOCK_CG)
                                        :SetZOrder(141);

                thumbnailImage:SetXY(349, 189);

                ui(world, 'ingameUI'):AddChild(thumbnailImage);
                ui(world, 'ingameUI'):AddChild(unlockLabel);

                local function showCGFull (target)
                    local fullscreenTexture = th.load('Asset/Gameplay/RewardIllust/' .. thumbNailName);

                    local fullscreenCG = UIImage():SetTexture(fullscreenTexture)
                                                  :SetSizeMode('REL')
                                                  :SetSize(1, 1)
                                                  :SetOpacity(0)
                                                  :SetFocusable(true)
                                                  :SetTransform(UITransform3D())
                                                  :SetZOrder(230);

                    local Artist;

                    for i = 1, #CGS do
                        if CGS[i].Name == thumbNailName then
                            Artist = CGS[i].Artist;
                        end
                    end

                    local artistLabel = UILabel():SetXY(0, 0)
                                            :SetXYMode('ABS')
                                            :AlignTo('RIGHTBOTTOM')
                                            :SetOrigin(1.0, 1.0)
                                            :SetFont(ui.font({
                                                size=13,
                                                color={255,255,255},
                                                stroke=1,
                                                strokeColor={0,0,0},
                                                weights=1,
                                                spacing=1.05
                                            }))
                                            :SetPickable(false)
                                            :SetName('ArtistLabel')
                                            :SetText('Illustrated by ' .. Artist)
                                            :SetZOrder(240);

                    ui(world, 'ingameUI'):AddChild(fullscreenCG);
                    ui(world, 'ingameUI'):AddChild(artistLabel);
                    ui.focus(fullscreenCG);
                    
                    anim.fadein(fullscreenCG);
                    anim.fadein(artistLabel);
                    anim.scalein(fullscreenCG);

                    local function closeFullscreenCG()
                        anim.fadeout(fullscreenCG);
                        anim.fadeout(artistLabel);
                        ui.off(fullscreenCG, 'click');
                        ui.off(fullscreenCG, 'keydown');
                        ui.focus(target);
                        th.timeout(anim.scaleout(fullscreenCG), function ()
                            ui(world, 'ingameUI'):RemoveChild(fullscreenCG);
                            ui(world, 'ingameUI'):RemoveChild(artistLabel);
                            next();
                        end);

                        return true;
                    end
                    ui.on(fullscreenCG, 'click',   closeFullscreenCG);
                    ui.on(fullscreenCG, 'keydown', closeFullscreenCG);
                    ui.effectorize(fullscreenCG);
                end

                ui.on(ui(unlockUI, 'Back'), 'click', showCGFull);
                ui.effectorize(ui(unlockUI, 'Back'));
                unlockUI:Show();
                return true;
             end;
        end;

        if (nextUnlockCG2 ~= nil) then
            nextFunction = showCGThumbnail(nextFunction, nextUnlockCG2);
            nextPuzzleFunction = showCGThumbnail(nextPuzzleFunction, nextUnlockCG2);
        end

        nextFunction = showCGThumbnail(nextFunction, nextUnlockCG);
        nextPuzzleFunction = showCGThumbnail(nextPuzzleFunction, nextUnlockCG);
    else
        -- do nothing
    end

    ui.on(ui(clearUI, 'Back'), 'click', nextFunction);
    ui.effectorize(ui(clearUI, 'Back'));
    th.timeout(0.1, function() sfx.play('Asset/SFX/Congraturation.wav', 3) end);
    local savedata = th.savedata();

    if ((world:GetPatternNumber() ~= #PUZZLES - 1)
        and (savedata:IsUnlocked(PUZZLES[world:GetPatternNumber() + 2].StageName))) then
        ui.on(ui(clearUI, 'Next'), 'click', nextPuzzleFunction);
        ui.effectorize(ui(clearUI, 'Next'));
    else
        ui(clearUI, 'Next'):Hide();
    end
end


function Gameplay.showGameOverUI()
    local world = ui('World');
    local gameOverPanel = ui(world, 'GameOverPanel');

    anim.fadein(gameOverPanel);
    gameOverPanel:Show();
    blinkingPause();
    th.timescale(CursorBlinkingCallback, 0.0);
    th.timescale(EmotionCallback, 0.0);
    world:SetGameOverUIShowing(true);

    ui.on(ui(gameOverPanel, 'Restart'), 'click', Gameplay.restartGamePuzzleByGameOver);
    ui.on(ui(gameOverPanel, 'Retire'), 'click', Gameplay.navigateToPuzzleSelect);

    ui.effectorize(ui(gameOverPanel, 'Restart'));
    ui.effectorize(ui(gameOverPanel, 'Retire'));
end

function Gameplay.restartGamePuzzle()
    local world = ui('World');
    world:Restart();

    Gameplay.hidePauseMenu();
    local rightPanel = ui(world, 'RightPanel');
    ui(rightPanel, 'RestLifeLabel'):SetText('' .. 5);
end

function Gameplay.restartGamePuzzleByGameOver()
    local world = ui('World');
    world:Restart();

    local world = ui('World');
    local gameOverPanel = ui(world, 'GameOverPanel');
    anim.fadeout(gameOverPanel);
    blinkingUnpause();
    th.timescale(CursorBlinkingCallback, 1.0);
    th.timescale(EmotionCallback, 1.0);
    world:SetGameOverUIShowing(false);

    ui.off(ui(gameOverPanel, 'Restart'), 'click');
    ui.off(ui(gameOverPanel, 'Retire'), 'click');

    local rightPanel = ui(world, 'RightPanel');
    ui(rightPanel, 'RestLifeLabel'):SetText('' .. 5);
end

function Gameplay.navigateToPuzzleSelect()
    Gameplay._navigateTo(goToPuzzleSelect);
end

function Gameplay.navigateToNextPuzzle()
    local world = ui('World');
    local nextPuzzle = world:GetPatternNumber() + 1;
    Gameplay._navigateTo(goToGame, 0, nextPuzzle);
end

function Gameplay._navigateTo(next, a, b)
    Gameplay.Timer.cancelAll();
    --th.canceltimeout(EyeBlinkingCallback);
    blinkingStopper();
    th.canceltimeout(CursorBlinkingCallback);
    ui.destroyworld(ui('World'));
    ui():RemoveAllChildren();

    next(a, b);

    th.timeout(1.0, function() th.gc(); end);
end

function setupLoadingScreen(parent)
    parent = parent or ui.getroot();
    parent:AddChild(UIWindow():SetName('LoadingScreen')
                              :AddChild(UIRect():SetSize(1, 1)
                                                :SetSizeMode('REL')
                                                :SetFillColor(0, 0, 0))
                              :AddChild(UILabel():SetName('LoadingLabel')
                                                 :SetX(40)
                                                 :SetY(40)
                                                 :SetText(STR.NowLoading)
                                                 :SetFont(ui.font({size=24,color={255,255,255}, weights=1.5, spacing=1.1}))
                                                 :AlignTo('RIGHTBOTTOM')));

    local loadingAlarmID = 100001;
    th.alarm(0.05, 10000, loadingAlarmID, function ()
        local total, loaded, ct, cl = th.loadingstatus();
        local percentage = (total > 0) and math.floor(loaded / total * 100) or 100
        local s = string.format('%s (%d%%, %d/%d)', STR.NowLoading, percentage, cl, ct);
        ui('LoadingScreen', 'LoadingLabel'):SetText(s);
    
        if (loaded >= total) then
            local loadingScreen = ui('LoadingScreen');
            ui.selfdetach(loadingScreen);
            th.cancelalarm(loadingAlarmID);
        end
        
    end);
end