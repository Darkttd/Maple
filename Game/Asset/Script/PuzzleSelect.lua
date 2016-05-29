
function baseShowPuzzlePage(page, offset, direction, puzzleSelect, buttonWindowName, itemPrefix, countPerPage, totalCount, prevButton, nextButton, show, setup)
    th.settings():SetLastViewPuzzlePage(page);
    offset = offset or 1;
    direction = direction or 0;

    -- local itemsWindow = ui(puzzleSelect, buttonWindowName);
    
    -- if (direction ~= 0) then
    --     local itemsWindowParent = itemsWindow:GetParent();
    --     itemsWindow:SetName('')
    --                :SetPickable(false);
    --     anim.fadeout(itemsWindow, 0.2);
    --     if (direction < 0) then
    --         anim.moveby(itemsWindow, 0,  120, 0.3);
    --     else
    --         anim.moveby(itemsWindow, 0, -120, 0.3);
    --     end
    --     th.timeout(1.0, function () itemsWindowParent:RemoveChild(itemsWindow) end);
        
    --     local layout = th.load(baseAssetURI);
    --     itemsWindowParent:AddChild(ui(layout:GetRoot(), buttonWindowName):Clone());
    --     itemsWindow:SetZOrder(220);
    -- end
    
    local itemsWindow = ui(puzzleSelect, buttonWindowName);
    local lastOffset = math.min(offset + countPerPage - 1, totalCount);
    for i = offset, lastOffset do
        local index = (i - offset + 1);
        setup(i, ui(itemsWindow, itemPrefix .. index):Show(), index == 1);
    end
    
    for i = lastOffset + 1, offset + countPerPage - 1 do
        local index = (i - offset + 1);
        local item = ui(itemsWindow, itemPrefix .. index);
        if (item) then
            item:Hide();
        end
    end
    
    if (direction ~= 0) then
        itemsWindow:SetOpacity(0);
        th.timeout(0.2, function ()
            if (prevButton) then
                prevButton:SetFrozen(prevButton:GetOpacity() < 0.5);
            end
            if (nextButton) then
                nextButton:SetFrozen(nextButton:GetOpacity() < 0.5);
            end
        end);
        if (direction > 0) then
            anim.movein(itemsWindow, 0, -80, 0.2);
            anim.fadein(itemsWindow, 0.2);
        elseif (direction < 0) then
            anim.movein(itemsWindow, 0,  80, 0.2);
            anim.fadein(itemsWindow, 0.2);
        end
    else
        anim.fadein(itemsWindow, 0.2);
    end

    if (prevButton and nextButton) then
        local prevOffset = offset - countPerPage;
        local nextOffset = offset + countPerPage;

        ui.off(prevButton, 'click');
        ui.off(nextButton, 'click');

        -- prevButton:SetZOrder(200);
        -- nextButton:SetZOrder(200);

        if (prevOffset >= 1) then
            ui.on(prevButton, 'click', function () show(page - 1, prevOffset, -1) end);
            ui.effectorize(prevButton);
            anim.fadein(prevButton);
        else
            prevButton:Freeze();
            anim.fadeout(prevButton);
        end

        if (nextOffset <= totalCount) then
            ui.on(nextButton, 'click', function () show(page + 1, nextOffset,  1) end);
            ui.effectorize(nextButton);
            anim.fadein(nextButton);
        else
            nextButton:Freeze();
            anim.fadeout(nextButton);
        end
    end
end

function goToPuzzleSelect()

    th.resetloadingstatus();

    character = 'Moa'; -- <TODO> To load from setting.

    --local session = GamingSession():Reset(3)
    --                                :SetCharacter(character)
    --                                :SetDifficulty(difficulty)

    local root = ui.getroot();
    local world = ui.createpuzzleselect(nil, true):SetName('World');

    setupLoadingScreen(root);
    playMusic(1); -- In puzzle Select UI, always Lobby musix.

    do
        local layout = th.load('Asset/UI/PuzzleSelectUI');
        local window = layout:GetRoot():Clone();
        window:SetXY(0.0, 0.0)
            :SetXYMode('ABS')
            :SetZOrder(10);
        window:SetName('puzzleSelectUI');

        window:AddChild(UILabel():SetXY(215, 194)
                                 :SetXYMode('ABS')
                                 :AlignTo('CENTER')
                                 :SetOrigin(0.5, 0.5)
                                 :SetFont(ui.font({
                                    size=17,
                                    color={255,255,255},
                                    stroke=1,
                                    strokeColor={0,0,0},
                                    weights=1
                                 }))
                                 :SetPickable(false)
                                 :SetName('PageLabel')
                                 :SetText('0/0'));
        
        world:SetUILayer(window);
        world:SetPatternPixel(68.0);

        do
            -- Setting Puzzle
            local puzzleManager = th.patternman();

            world:SetPatternManager(puzzleManager);
        end

        ui(window, 'Background'):SetZOrder(120);
        -- showStanding('moa_closet', -65, 0, window, 125);
    end

    world:GetUnlockedLayer():RemoveAllChildren();
    world:GetLockedLayer():RemoveAllChildren();

    do
        for i = 5, 20 do
            if(i % 5 == 0) then
                local unlockedImage = UIImage():SetTexture(th.load('Asset/UI/unlocked_PuzzleSize_' .. i));
                local lockedImage = UIImage():SetTexture(th.load('Asset/UI/locked_PuzzleSize_' .. i));

                unlockedImage:SetName('' .. i);
                lockedImage:SetName('' .. i);

                world:GetUnlockedLayer():AddChild(unlockedImage);
                world:GetLockedLayer():AddChild(lockedImage);
            end
        end
    end

    root:AddChild(world);

    do
        --[[
        local function go(nav)
            return function (target)
                if (target:GetFrozen()) then return true end

                local window = ui('PuzzleSelectUI');
                local buttons = ui.getAllButtons(window, true);
                
                anim.fadeout(window, 0.2);
                anim.moveby(ui(window, 'OK'), 60, 0, 0.2);

                sceneOut(window, nav, {outs={versionLabel}});
                ui.freezeAll(buttons);
                return true;
            end;
        end 
        --]]

        local function go(nav, a, b)
            return function (target)

                --ui.destroyworld(ui('World'));
                ui():RemoveAllChildren();
                --th.canceltimeout(EyeBlinkingCallback);
                blinkingStopper();

                nav(a, b);
            end;
        end

        local function showPuzzles(page, offset, direction)
            page = page or 0;

            local puzzleButtons = ui(window, 'PuzzleSelect');
            local pageLabel = ui('PageLabel');
            local window = ui('PuzzleSelect');
            local prevButton = ui(window, 'Prev');
            local nextButton = ui(window, 'Next');

            pageLabel:SetText('' .. (page + 1) .. '/' .. math.floor((#PUZZLES + 11) / 12));

            local world = ui(ui.getroot(), 'World');
            world:SetCurrentPage(page);

            local function goPuzzle(puzzleNumber)
                return function(target)
                    local pageNumber = 0; -- Add page number.

                    local f = go(goToGame, pageNumber, puzzleNumber);
                    f(target);
                end;
            end

            local savedata = th.savedata();
            baseShowPuzzlePage(page, offset, direction,
                window, 'Buttons', 'Item',
                12, #PUZZLES, prevButton, nextButton, showPuzzles,
                function (i, item, isFirst)
                    local isUnlocked = savedata:IsUnlocked(PUZZLES[i].StageName);
                    local isCleared = savedata:IsUnlocked(PUZZLES[i].StageName .. '_Clear');
                    local targetSize = #(PUZZLES[i].Pattern);
                    local button = item:FindChild('Button', false);
                    local lock = item:FindChild('Lock', false);

                    for j = 5, 20 do
                        if(j % 5 == 0) then
                            local unSolved = item:FindChild('Unsolved' .. j, false);
                            unSolved:Hide();

                            local solved = item:FindChild('Solved' .. j, false);
                            solved:Hide();
                        end
                    end                    

                    if (isUnlocked) then
                        button:SetFrozen(false)
                              :SetFocusVisible(true);
                        lock:Hide();

                        if (isCleared) then
                            local solved = item:FindChild('Solved' .. targetSize, false);
                            solved:Show();
                            solved:SetPickable(false);
                        else
                            local unsolved = item:FindChild('Unsolved' .. targetSize, false);
                            unsolved:Show();
                            unsolved:SetPickable(false);
                        end

                        ui.off(button, 'click');
                        ui.on(button, 'click', goPuzzle(i - 1));
                        ui.effectorize(button);
                    else
                        button:SetFrozen(true);
                        ui.off(button, 'click');
                        lock:Show();
                        lock:SetPickable(false);
                        item:FindChild('Solved' .. targetSize, false):Show();
                    end
                end);

        end

        local window = world:GetUILayer();
        --local buttons = ui.getAllItems(window, true);

        ui.on(ui(window, 'Back'), 'click', go(goToLobby));
        ui.on(window, 'keydown', function(v, key)
           if(key == 'Escape' or key == 'SystemBack') then
                ui(window, 'Back'):Click();
           end
        end);

        local lastPage = th.settings():GetLastViewPuzzlePage();
        showPuzzles(lastPage, lastPage * 12 + 1);

        --sceneIn(window);
        --for index, value in ipairs(buttons) do
        --    value:SetOpacity(0);
        --    th.timeout(index * 0.03, function()
        --        anim.fadein(value);
                --anim.movein(value, -60, 0, 0.3);
        --    end);
        --end

        ui.effectorize(ui(window, 'Back'));
        ui.focus(ui(window, 'Back'));
    end
end

