
function main()
    --_ALERT('main() called');
    STR = STR_JA;
    th.title(STR.Title);
    --th.alarm(1, 999999,10, function() _ALERT(ui.focus():GetName()); end);
    goToLobby();
    --goToGameStory01();
    --goToPuzzleSelect();
end

IMAGE_FONT_CACHES = 15;
function setupImageFont(window, texture, number)
    local offset = offset or 4;
    window:SetY(window:GetY() - offset)
          :SetSize(1, 1)
          :SetSizeMode('CONTENT')
          :SetPickable(false);

    for i = 1, IMAGE_FONT_CACHES do
        window:AddChild(UIImage());
    end

    setImageText(window, texture, number)
end

function setImageText(window, texture, number)
    local characters = characters or {'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
    local cw = texture:GetWidth() / #characters; -- character width
    local ch = texture:GetHeight(); -- character height

    number = intcomma(number);

    local x = 0;
    for i = 1, #number do
        local c = number:sub(i, i);
        local index = table.indexof(characters, c);
        if (index) then
            ui(window, i - 1):SetX(x)
                             :SetTexture(texture, (index - 1) * cw, 0, cw, ch)
                             :Show();

            x = x + cw * 0.95;
        end
    end

    for i = #number, IMAGE_FONT_CACHES - 1 do
        ui(window, i):Hide();
    end
end

function sceneIn(window, t)
    local duration = 0.3;

    window:SetOpacity(0);
    anim.fadein(window, duration);
    
    if (t == nil) then
        return;
    end

    if (t.title) then
        anim.movein(t.title, 400, 0, duration);
    end

    if (t.rights) then
        for _, v in ipairs(t.rights) do
            anim.movein(v, -200, 0, duration);
        end
    end
end

function sceneOut(window, nav, t)
    local duration = 0.3;
    local focusingTime = 0.3;
    local args = {};
    
    if (nav == goToStoryMode or nav == goToGame) then
        t = t or {};
        t.outs = t.outs or {};
        t.outs[#t.outs + 1] = ui('Background');
        t.selected = nil;
    end

    if (t) then
        if (t.title) then
            anim.fadeout(t.title, duration);
            anim.moveby(t.title, -800, 0, duration);
        end
        
        local selected = t.selected;
        
        if (t.outs) then
            for _, v in ipairs(t.outs) do
                if (v) then
                    if (v == selected) then
                        anim.fadeout(v, duration, {delay=focusingTime});
                    else
                        anim.fadeout(v, duration);
                    end
                end
            end
        end

        if (t.rights) then
            for _, v in ipairs(t.rights) do
                if (v) then
                    if (v == selected) then
                        th.timeout(focusingTime, function()
                            anim.fadeout(v, duration);
                            anim.moveby(v, 400, 0, duration);
                        end);
                    else
                        anim.fadeout(v, duration);
                        anim.moveby(v, 400, 0, duration);
                    end
                end
            end
        end
        
        if (t.args) then
            args = t.args
        end
    end

    th.timeout(0.5, function() nav(table.unpack(args)); end);
    th.timeout(1.0, function() window:GetParent():RemoveChild(window); th.timeout(0.1, nil, function () collectgarbage('collect') end); end);
end

function baseShowPage(page, offset, direction, root, windowName, itemPrefix, baseAssetURI, countPerPage, totalCount, prevButton, nextButton, show, setup)
    offset = offset or 1;
    direction = direction or 0;
    
    local itemsWindow = ui(root, windowName);
    
    if (direction ~= 0) then
        local itemsWindowParent = itemsWindow:GetParent();
        itemsWindow:SetName('')
                   :SetPickable(false);
        anim.fadeout(itemsWindow, 0.2);
        if (direction < 0) then
            anim.moveby(itemsWindow, 0,  120, 0.3);
        else
            anim.moveby(itemsWindow, 0, -120, 0.3);
        end
        th.timeout(1.0, function () itemsWindowParent:RemoveChild(itemsWindow) end);
        
        local layout = th.load(baseAssetURI);
        itemsWindowParent:AddChild(ui(layout:GetRoot(), windowName):Clone());
        itemsWindow:SetZOrder(220);
    end
    
    local itemsWindow = ui(root, windowName);
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
    end

    if (prevButton and nextButton) then
        local prevOffset = offset - countPerPage;
        local nextOffset = offset + countPerPage;

        ui.off(prevButton, 'click');
        ui.off(nextButton, 'click');

        prevButton:SetZOrder(200);
        nextButton:SetZOrder(200);

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

function ui.setFrozenAll(buttons, value)
    if (buttons) then
        for _, v in ipairs(buttons) do
            v:SetFrozen(value);
        end
    else
        ui.all('UIButton', function (o)
            o:SetFrozen(value);
            return true;
        end);
    end
end

function ui.freezeAll(buttons)   ui.setFrozenAll(buttons, true) end
function ui.unfreezeAll(buttons) ui.setFrozenAll(buttons, false) end

function ui.font(t)
    local font = ui.createfont();
    font:SetFaceURI(t.face or 'Asset/NanumGothic')
        :SetSize(t.size or 20)
        :SetStrokeSize(t.stroke or 0)
        :SetWeights(t.weights or 0)
        :SetShear(t.shear or 0)
        :SetSpacing(t.spacing or 1);
        
    if (t.color) then
        font:SetColor(t.color[1], t.color[2], t.color[3]);
    end
    if (t.strokeColor) then
        font:SetStrokeColor(t.strokeColor[1], t.strokeColor[2], t.strokeColor[3]);
    end
    
    return font;
end


function playMusic(number)
    number = number or 1;

    local ext = '.ogg';

    if (PLATFORM == 'IOS') then
        ext = '.mp3';
    end

    bgm.change('Asset/BGM/' .. MUSICS[number].URI .. ext);
end


function ui.effectorize(buttons)
    if (type(buttons) ~= 'table') then
        buttons = { buttons };
    end

    for _, v in ipairs(buttons) do
        ui.on(v, 'click',      function () sfx.play('Asset/SFX/ButtonClick.wav', 2) end);
    end
end


function baseGoTo(uri, onOpen)
    local root = ui.getroot();

    local window = nil;
    if (uri) then
        local layout = th.load('Asset/UI/' .. uri);
        window = layout:GetRoot():Clone();
    else
        window = UIWindow();
    end
    
    window:SetXY(0, 0)
          :SetXYMode('ABS')
          :SetZOrder(10);

    if (uri == 'StoryModeUI') then
        -- Need change bgm - which character selected and more half of puzzle cleared 
        playMusic(2);
        --bgm.duration(0.00);

        local vn = ui.createvn();
        vn:GetDialogueWindow():AddChild(window);
        window = vn;

        local backgroundImage = ui(root, 'Background');
        if (backgroundImage) then
            root:RemoveChild(backgroundImage);
        end
    else
        playMusic(1);
        --bgm.duration(0.00);
        --_ALERT('Duration = ' .. bgm.duration('Asset/BGM/Title.ogg'));

        if (uri) then
            if (ui(root, 'Background') == nil) then
                root:AddChild(UIImage():SetName('Background')
                                       :SetTexture(th.load('Asset/UI/BackDefault'))
                                       :SetPickable(false)
                                       :SetZOrder(0));
            end
        end
    end
    
    window:SetName(uri)    
    root:AddChild(window);
    ui.focus(window);

    -- if(window == nil) then
    --     _ALERT('basegoto, setfocus -> nil');
    -- else
    --     _ALERT('basegoto, setfocus -> ' .. window:GetName());
    -- end

    if (onOpen) then
        onOpen(window);
    end
end



function goToLobby()
    baseGoTo('titleUI', function(window)
        window:AddChild(UILabel():SetXY(4, 4)
                                 :SetXYMode('ABS')
                                 :AlignTo('LEFTBOTTOM')
                                 :SetFont(ui.font({
                                     size=12,
                                     color={255,255,255},
                                     stroke=1,
                                     strokeColor={0,0,0},
                                     weights=1
                                 }))
                                 :SetText('v' .. th.version())
                                 :SetPickable(false)
                                 :SetName('VersionLabel'));

        local function go(nav, next)
            return function (target)
                if (target:GetFrozen()) then return true end

                local window = ui('titleUI');
                local buttons = ui.getAllButtons(window, true);
                
                anim.fadeout(window, 0.3);

                for index, value in ipairs(buttons) do
                    th.timeout(index * 0.03, function()
                        anim.fadeout(value, 0.2);
                        anim.moveby(value, 60, 0, 0.2);
                    end);
                end
                
                sceneOut(window, nav, {outs={versionLabel}, args={next}});
                ui.freezeAll(buttons);
                return true;
            end;
        end

        local function test(str)
            return function (target)
                _ALERT(str);
            end;
        end

        local buttons = ui.getAllButtons(window, true);
        local settings = th.settings();

        -- if (settings:GetStandingCharacter() == 'Kradmoa' or settings:GetStandingCharacter() == 'Maple') then
            ui.on(ui(window, 'GameStart'), 'click', go(goToPuzzleSelect));
        -- else
        --     ui.on(ui(window, 'GameStart'), 'click', go(goToCharacterSelect, goToPuzzleSelect));
        -- end

        ui.on(ui(window, 'Closet'),    'click', go(goToCharacterSelect, goToCloset));
        ui.on(ui(window, 'Gallery'),   'click', go(goToGallery));
        ui.on(ui(window, 'Setting'),   'click', go(goToSetting));
        ui.on(ui(window, 'Quit'),      'click', th.exit);
        ui.on(window, 'keydown', function(v, key)
           if(key == 'Escape' or key == 'SystemBack') then
                ui(window, 'Quit'):Click();
           end
        end);

        if (PLATFORM == 'IOS') then
            ui(window, 'Quit'):Hide();
        end

        sceneIn(window);
        for index, value in ipairs(buttons) do
            value:SetOpacity(0);
            th.timeout(index * 0.03, function()
                anim.fadein(value);
                anim.movein(value, -60, 0, 0.3);
            end);
        end

        ui.effectorize(buttons);
        ui.focus(ui(window, 'Quit'));
    end);
end


function goToCharacterSelect(afterScene)
    baseGoTo('CharacterSelectUI', function(window)

        local moa = ui(window, 'Moa');
        local maple  = ui(window, 'Maple');

        moa:InsertEffect(UIColorMatrixEffect():SetName('Grayscale'));
        moa:SetOpacity(0);

        maple:InsertEffect(UIColorMatrixEffect():SetName('Grayscale'));
        maple:SetOpacity(0);

        window:AddChild(UILabel():SetXY(250, -100)
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
                                 :SetName('CharacterNameLabel'));

        window:AddChild(UILabel():SetXY(540, 170)
                                 :SetXYMode('ABS')
                                 :AlignTo('LEFTTOP')
                                 :SetOrigin(0.0, 0.0)
                                 :SetFont(ui.font({
                                    size=13,
                                    color={255,255,255},
                                    stroke=1,
                                    strokeColor={0,0,0},
                                    weights=1,
                                    spacing=1.05
                                 }))
                                 :SetPickable(false)
                                 :SetName('CharacterDescriptionLabel'));

        local charName = ui(window, 'CharacterNameLabel');
        local charDesc = ui(window, 'CharacterDescriptionLabel');

        local changeCharacter;

        local function characterIn()
            local settings = th.settings();
            anim.fadein(moa, 0.2);
            anim.fadein(maple,  0.2);
            anim.movein(moa,  20, 0, 0.2, {interp=anim.ease.o2});
            anim.movein(maple,  -20, 0, 0.2, {interp=anim.ease.o2});
            
            local sc = settings:GetStandingCharacter();


            if (sc == 'Kradmoa') then
                moa:BringToFront();
                moa:FindEffect('Grayscale'):Reset();
                maple:FindEffect('Grayscale'):Grayscale(0.123, 0.456, 0.012);
                ui(window, 'CharacterNameLabel'):SetText(STR.CHARACTERS[1]);
                ui(window, 'CharacterDescriptionLabel'):SetText(STR.CHARACTER_DESCRIPTION[1]);
            elseif (sc == 'Maple') then
                maple:BringToFront();
                maple:FindEffect('Grayscale'):Reset();
                moa:FindEffect('Grayscale'):Grayscale(0.123, 0.456, 0.012);
                ui(window, 'CharacterNameLabel'):SetText(STR.CHARACTERS[2]);
                ui(window, 'CharacterDescriptionLabel'):SetText(STR.CHARACTER_DESCRIPTION[2]);
            else
                moa:FindEffect('Grayscale'):Grayscale(0.123, 0.456, 0.012);
                maple:FindEffect('Grayscale'):Grayscale(0.123, 0.456, 0.012);
                ui(window, 'CharacterNameLabel'):SetText(STR.NOT_YET_SELECTED_CHARACTERS);
                ui(window, 'CharacterDescriptionLabel'):SetText(STR.NOT_YET_SELECTED_CHARACTERS_DESCRIPTION);
            end

            th.timeout(1/60, function()
                anim.fadein(charName,  0.2);
                anim.fadein(charDesc,  0.2);
            end);

            th.timeout(0.2, function()
                ui.unfreezeAll();
                ui.on(moa, 'click', function() changeCharacter('Kradmoa') end);
                ui.on(maple, 'click', function() changeCharacter('Maple') end);
                ui.effectorize(moa);
                ui.effectorize(maple);
            end);
        end
        
        local function characterOut()
            ui.freezeAll()
            ui.off(moa, 'click');
            ui.off(maple, 'click');;

            anim.fadeout(moa, 0.2);
            anim.fadeout(maple,  0.2);
            anim.fadeout(charName,  0.2);
            anim.fadeout(charDesc,  0.2);
            anim.moveby(moa, -20, 0, 0.2);
            anim.moveby(maple,   20, 0, 0.2);
        end

        changeCharacter = function (character)
            local settings = th.settings();
            local sc = selectedCharacter;

            if (type(character) == 'string') then
                if (settings:GetStandingCharacter() == character) then
                    return;
                end
                
                settings:SetStandingCharacter(character);
            else
                if (sc[1] == 'Kradmoa') then
                    sc[1] = 'Maple';
                else
                    sc[1] = 'Kradmoa';
                end
            end

            characterOut();
            th.timeout(0.3, function()
                moa:SetX(moa:GetX() + 20);
                maple:SetX(maple:GetX() - 20);
                characterIn();
            end);
        end

        local function go(nav)
            return function (target)
                local settings = th.settings();
                local sc = settings:GetStandingCharacter();

                if (sc ~= 'Kradmoa') then
                    if (sc ~= 'Maple') then
                        return true
                    end
                end

                if (target:GetFrozen()) then return true end

                anim.fadeout(ui(window, 'BackPanel'));

                characterOut();

                th.timeout(0.1, function()
                    sceneOut(window, nav, {outs={ui(window, 'OK'), ui(window, 'BackPanel'), ui(window, 'Description')}});
                    ui.freezeAll(buttons);
                end);
                return true;
            end;
        end

        local function forceGo(nav)
            return function (target)
                anim.fadeout(ui(window, 'BackPanel'));

                characterOut();

                th.timeout(0.1, function()
                    sceneOut(window, nav, {outs={ui(window, 'OK'), ui(window, 'BackPanel'), ui(window, 'Description')}});
                    ui.freezeAll(buttons);
                end);
                return true;
            end;
        end

        sceneIn(window);
        anim.fadein(ui(window, 'BackPanel'));
        ui.on(ui(window, 'OK'), 'click', go(afterScene));
        ui.on(ui(window, 'Moa'), 'click', function() changeCharacter('Kradmoa') end);
        ui.on(ui(window, 'Maple'), 'click', function() changeCharacter('Maple') end);
        ui.on(window, 'keydown', function(v, key)
           if(key == 'Escape' or key == 'SystemBack') then
                --ui(window, 'OK'):Click();
                forceGo(goToLobby)(nil);
           end
        end);

        th.timeout(0.1, characterIn);

        ui.effectorize(ui(window, 'OK'));
        ui.effectorize(ui(window, 'Moa'));
        ui.effectorize(ui(window, 'Maple'));
        ui.focus(ui(window, 'OK'));
    end);
end

function goToCloset()
    baseGoTo('ClosetUI', function(window)
        local function go(nav)
            return function (target)
                if (target:GetFrozen()) then return true end

                local window = ui('ClosetUI');
                local buttons = ui.getAllButtons(window, true);
                
                anim.fadeout(window, 0.2);
                anim.moveby(ui(window, 'OK'), 60, 0, 0.2);
                --th.canceltimeout(EyeBlinkingCallback);
                blinkingStopper();

                sceneOut(window, nav, {outs={versionLabel}});
                ui.freezeAll(buttons);
                return true;
            end;
        end

        local function ChangeCloth(dir)
            return function (target)
                if (target:GetFrozen()) then return true end

                local current = 1;
                local settings = th.settings();
                local currentCharacterName = settings:GetStandingCharacter();
                local currentClothName;

                if (currentCharacterName == 'Maple') then
                    currentClothName = settings:GetMapleStandingCharacterCloth();
                else
                    currentClothName = settings:GetMoaStandingCharacterCloth();
                end

                for i=1, #Cloths do
                    if(Cloths[i] == currentClothName) then
                        current = i;
                    end
                end

                local savedata = th.savedata();

                repeat
                    current = current + dir;

                    if(current > #Cloths) then
                        current = 1;
                    elseif(current < 1) then
                        current = #Cloths;
                    end
                until savedata:IsUnlocked(settings:GetStandingCharacter() .. Cloths[current]);

                local standing = ui(window, 'moa_closet');

                anim.fadeout(standing, 0.2);
                anim.moveby(standing, -60 * dir, 0, 0.2);
                anim.fadeout(ui(window, 'ClothNameLabel'), 0.2);
                anim.fadeout(ui(window, 'ClothDescriptionLabel'), 0.2);

                local window = ui('ClosetUI');
                local buttons = ui.getAllButtons(window, true);
                ui.freezeAll(buttons);


                th.timeout(0.3, function()
                    standing:GetParent():RemoveChild(standing);
                    local char = settings:GetStandingCharacter();

                    if (char == 'Maple') then
                        settings:SetMapleStandingCharacterCloth(Cloths[current]);
                    else
                        settings:SetMoaStandingCharacterCloth(Cloths[current]);
                    end

                    ui(window, 'ClothNameLabel'):SetText(STR.CLOTHS[Cloths[current]]);
                    ui(window, 'ClothDescriptionLabel'):SetText(STR.CLOTHS_DESCRIPTION[Cloths[current]]);
                    anim.fadein(ui(window, 'ClothNameLabel'), 0.2);
                    anim.fadein(ui(window, 'ClothDescriptionLabel'), 0.2);

                    --th.canceltimeout(EyeBlinkingCallback);
                    blinkingStopper();
                    showStanding('moa_closet', 260, 30, window);
                    standing = ui(window, 'moa_closet');

                    th.timeout(1/60, function()
                        anim.fadein(standing, 0.2);
                        anim.movein(standing, -60 * dir, 0, 0.2);
                    end);

                    -- manual z order setting
                    -- default z value is 128
                    standing:SetZOrder(130);
                    ui(window, 'Description'):SetZOrder(140);

                    th.timeout(0.2, function()
                        ui.unfreezeAll(buttons);
                    end);
                end);

            end;
        end
 
        if (th.savedata():IsUnlocked('Kradmoa' .. th.settings():GetMoaStandingCharacterCloth())) then
            -- do nothing
        else
            _ALERT('initialized'.. 'Kradmoa' .. th.settings():GetMoaStandingCharacterCloth());
            th.settings():SetMoaStandingCharacterCloth(Cloths[1]);
        end

        if (th.savedata():IsUnlocked('Maple' .. th.settings():GetMapleStandingCharacterCloth())) then
            -- do nothing
        else
            _ALERT('Maple Init...');
            th.settings():SetMapleStandingCharacterCloth(Cloths[1]);
        end


        window:AddChild(UILabel():SetXY(28, 340)
                                 :SetXYMode('ABS')
                                 :SetZOrder(140)
                                 :AlignTo('CENTERTOP')
                                 :SetOrigin(0.5, 0.5)
                                 :SetFont(ui.font({
                                    size=17,
                                    color={255,255,255},
                                    stroke=1,
                                    strokeColor={0,0,0},
                                    weights=1
                                 }))
                                 :SetPickable(false)
                                 :SetName('ClothNameLabel'));

        window:AddChild(UILabel():SetXY(55, 365)
                                 :SetXYMode('ABS')
                                 :SetZOrder(140)
                                 :AlignTo('CENTERTOP')
                                 :SetOrigin(0.5, 0.5)
                                 :SetFont(ui.font({
                                    size=13,
                                    color={255,255,255},
                                    stroke=1,
                                    strokeColor={0,0,0},
                                    weights=1,
                                    spacing=1.05
                                 }))
                                 :SetPickable(false)
                                 :SetName('ClothDescriptionLabel')); 

        if (th.settings():GetStandingCharacter() == 'Maple') then
            ui(window, 'ClothNameLabel'):SetText(STR.CLOTHS[th.settings():GetMapleStandingCharacterCloth()]);
            ui(window, 'ClothDescriptionLabel'):SetText(STR.CLOTHS_DESCRIPTION[th.settings():GetMapleStandingCharacterCloth()]);
        else
            ui(window, 'ClothNameLabel'):SetText(STR.CLOTHS[th.settings():GetMoaStandingCharacterCloth()]);
            ui(window, 'ClothDescriptionLabel'):SetText(STR.CLOTHS_DESCRIPTION[th.settings():GetMoaStandingCharacterCloth()]);
        end

        local buttons = ui.getAllButtons(window, true);
        ui(window, 'Description'):SetOpacity(0.7);
        ui.on(ui(window, 'Back'),  'click', go(goToLobby));
        ui.on(ui(window, 'Right'), 'click', ChangeCloth(1));
        ui.on(ui(window, 'Left'),  'click', ChangeCloth(-1));
        ui.on(window, 'keydown', function(v, key)
           if(key == 'Escape' or key == 'SystemBack') then
                ui(window, 'Back'):Click();
           end
        end);

        sceneIn(window);
        anim.fadein(ui(window, 'OK'));
        anim.movein(ui(window, 'OK'), -60, 0, 0.3);

        showStanding('moa_closet', 270, 30, window);

        -- manual z order setting
        -- default z value is 128
        ui(window, 'moa_closet'):SetZOrder(130);
        ui(window, 'Description'):SetZOrder(140);

        ui.effectorize(buttons);
        ui.focus(ui(window, 'Back'));
    end);
end



function goToSetting()
    baseGoTo('SettingUI', function(window)
        local function updateUI()
            local window = ui('SettingUI');
            local settings = th.settings();
            
            if (settings:GetBGMMute()) then
                ui(window, 'BGMOff'):SetChecked(true);
            else
                ui(window, 'BGMOn'):SetChecked(true);
            end
            
            if (settings:GetSFXMute()) then
                ui(window, 'EffectOff'):SetChecked(true);
            else
                ui(window, 'EffectOn'):SetChecked(true);
            end
        end

        local function go(nav)
            return function (target)
                if (target:GetFrozen()) then return true end

                local window = ui('SettingUI');
                local buttons = ui.getAllButtons(window, true);
                
                anim.fadeout(window, 0.3);

                for index, value in ipairs(buttons) do
                    th.timeout(index * 0.03, function()
                        anim.fadeout(value, 0.2);
                        anim.moveby(value, 60, 0, 0.2);
                    end);
                end
                
                sceneOut(window, nav, {outs={versionLabel}});
                ui.freezeAll(buttons);
                return true;
            end;
        end

-- -------------------------------------------------------------
-- ---------- CHEAT Code                                      --
--         local function cheat_UnlockAll (target)            --
--             local savedata = th.savedata();                --
--             _ALERT('Debug Code - Unlock All Activated');   --
--                                                            --
--             -- first, Unlock all of cloth                  --
--             for i = 1, #Cloths do                          --
--                 savedata:Unlock('Kradmoa' .. Cloths[i]);   --
--                 savedata:Unlock('Maple' .. Cloths[i]);     --
--             end                                            --
--                                                            --
--             -- next, Unlock all of CGs                     --
--             for i = 1, #CGS do                             --
--                 savedata:Unlock(CGS[i].Name);              --
--             end                                            --
--                                                            --
--             -- next, Unlock all of Puzzle to solve         --
--             for i = 1, 10 do                               --
--                 local j = i;                               --
--                 if (j < 10) then                           --
--                     j = '0' .. j;                          --
--                 end                                        --
--                 savedata:Unlock('MOA0' .. j);              --
--                 savedata:Unlock('MOA1' .. j);              --
--                 savedata:Unlock('MOA2' .. j);              --
--                 savedata:Unlock('MOA3' .. j);              --
--                 savedata:Unlock('MPL0' .. j);              --
--                 savedata:Unlock('MPL1' .. j);              --
--                 savedata:Unlock('MPL2' .. j);              --
--                 savedata:Unlock('MPL3' .. j);              --
--             end                                            --
--                                                            --
--             th.savedata():Save();                          --
--         end                                                --
-- -------------------------------------------------------------

        local function ResetUnlockStatus (target)
            th.savedata():Reset();
            th.settings():SetMoaStandingCharacterCloth('SchoolUniform');
            th.settings():SetMapleStandingCharacterCloth('SchoolUniform');
            th.savedata():Save();
        end

        local function ShowResetConfirmWindow (target)
            local resetConfirm = ui(window, 'ResetConfirm');

            resetConfirm:Show();

            local function CloseConfirmWindow (target)
                resetConfirm:Hide();

                ui.off(ui(resetConfirm, 'Confirm'), 'click');
                ui.off(ui(resetConfirm, 'Cancel'), 'click');
            end

            ui.on(ui(resetConfirm, 'Confirm'), 'click', function() ResetUnlockStatus(); CloseConfirmWindow(); end);
            ui.on(ui(resetConfirm, 'Cancel'),  'click', CloseConfirmWindow);

            ui.effectorize(ui(resetConfirm, 'Confirm'));
            ui.effectorize(ui(resetConfirm, 'Cancel'));
        end

        local buttons = ui.getAllButtons(window, true);
        ui.on(ui(window, 'BGMOn'),     'click', function () local s = th.settings(); s:SetBGMMute(false); s:Apply(); end);
        ui.on(ui(window, 'BGMOff'),    'click', function () local s = th.settings(); s:SetBGMMute(true); s:Apply(); end);
        ui.on(ui(window, 'EffectOn'),  'click', function () local s = th.settings(); s:SetSFXMute(false); s:Apply(); end);
        ui.on(ui(window, 'EffectOff'), 'click', function () local s = th.settings(); s:SetSFXMute(true); s:Apply(); end);
        ui.on(ui(window, 'Credit'),    'click', go(goToCredit));
        -- ui.on(ui(window, 'SolveAll'),  'click', cheat_UnlockAll) -- <TODO> cheat code. need to delete when release.
        ui.on(ui(window, 'Reset'),     'click', ShowResetConfirmWindow); -- <TODO> temp code. if ui enable, need to fix it.
        ui.on(ui(window, 'Back'),      'click', go(goToLobby));
        ui.on(window, 'keydown', function(v, key)
           if(key == 'Escape' or key == 'SystemBack') then
                ui(window, 'Back'):Click();
           end
        end);
 
        updateUI();
        
        sceneIn(window);

        ui(window, 'ResetConfirm'):Hide();

        for index, value in ipairs(buttons) do
            value:SetOpacity(0);
            th.timeout(index * 0.03, function()
                anim.fadein(value);
                anim.movein(value, -60, 0, 0.3);
            end);
        end

        ui.effectorize(buttons);
        ui.focus(ui(window, 'Back'));
    end);
end



function goToCredit()
    baseGoTo('CreditUI', function(window)
        local function go(nav)
            return function (target)
                if (target:GetFrozen()) then return true end

                local window = ui('CreditUI');
                local buttons = ui.getAllButtons(window, true);
                
                anim.fadeout(window, 0.2);
                anim.moveby(ui(window, 'OK'), 60, 0, 0.2);

                sceneOut(window, nav, {outs={versionLabel}});
                ui.freezeAll(buttons);
                return true;
            end;
        end

        local buttons = ui.getAllButtons(window, true);
        ui.on(ui(window, 'Back'),    'click', go(goToSetting));
        ui.on(window, 'keydown', function(v, key)
            _ALERT('clicked.. window=' .. window:GetName());
            if(key == 'Escape' or key == 'SystemBack') then
                ui(window, 'Back'):Click();
            end
        end);
  
        sceneIn(window);
        anim.fadein(ui(window, 'Back'));
        anim.movein(ui(window, 'Back'), -60, 0, 0.3);

        ui.effectorize(buttons);
        ui.focus(ui(window, 'Back'));
    end);
end




function goToGallery()
    baseGoTo('GalleryUI', function(window)
        local function go(nav)
            return function (target)
                if (target:GetFrozen()) then return true end

                local window = ui('GalleryUI');
                local buttons = ui.getAllButtons(window, true);
                
                anim.fadeout(window, 0.2);
                anim.moveby(ui(window, 'OK'), 60, 0, 0.2);

                sceneOut(window, nav, {outs={versionLabel}});
                ui.freezeAll(buttons);
                return true;
            end;
        end

        local function showCGs(page, offset, direction)
            page = page or 0;
            local savedata = th.savedata();
            local cgs = CGS;
            local window = ui('GalleryUI');
            local pageLabel = ui(window, 'PageLabel');
            local prevButton = ui(window, 'Prev');
            local nextButton = ui(window, 'Next');
            local stageLabel       = ui(window, 'Stage');
            local illustratorLabel = ui(window, 'Illustrator');
            local commentLabel     = ui(window, 'Comment');

            pageLabel:SetText('' .. (page + 1) .. '/' .. math.floor((#CGS + 11) / 12));

            baseShowPage(page, offset, direction,
                         window, 'Thumbnails', 'Item', 'Asset/UI/GalleryUI',
                         12, #cgs, prevButton, nextButton, showCGs,
                         function (i, item, isFirst)
                            local name = cgs[i].Name;
                            local Artist = cgs[i].Artist;
                            local thumbnail = th.load('Asset/Gameplay/RewardIllust/' .. name .. '_thumb');

                            local isUnlocked = savedata:IsUnlocked(name);
                            local lock = item:FindChild('Lock', false);
                            local button = item:FindChild('Button', false);
                            if (isUnlocked) then
                                button:SetFrozen(false)
                                      :SetFocusVisible(true);
                                lock:Hide();
                                if (thumbnail) then
                                    ui(button:GetNormal(), 'Normal'):SetTexture(thumbnail);
                                    ui(button:GetPushed(), 'Normal'):SetTexture(thumbnail);
                                    ui(button:GetHovering(), 'Normal'):SetTexture(thumbnail);

                                    ui.on(button, 'click', function (target)
                                        local fullscreenTexture = th.load('Asset/Gameplay/RewardIllust/' .. name);

                                        local fullscreenCG = UIImage():SetTexture(fullscreenTexture)
                                                                      :SetSizeMode('REL')
                                                                      :SetSize(1, 1)
                                                                      :SetOpacity(0)
                                                                      :SetFocusable(true)
                                                                      :SetTransform(UITransform3D())
                                                                      :SetZOrder(230);

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

                                        window:AddChild(fullscreenCG);
                                        window:AddChild(artistLabel);
                                        ui.focus(fullscreenCG);
                                        
                                        th.timeout(1/60, function()
                                            anim.fadein(fullscreenCG);
                                            anim.fadein(artistLabel);
                                            anim.scalein(fullscreenCG);
                                        end);

                                        local function closeFullscreenCG()
                                            anim.fadeout(fullscreenCG);
                                            anim.fadeout(artistLabel);
                                            ui.off(fullscreenCG, 'click');
                                            ui.off(fullscreenCG, 'keydown');
                                            ui.focus(target);
                                            th.timeout(anim.scaleout(fullscreenCG), function ()
                                                window:RemoveChild(fullscreenCG);
                                                window:RemoveChild(artistLabel);
                                            end);

                                            return true;
                                        end
                                        ui.on(fullscreenCG, 'click',   closeFullscreenCG);
                                        ui.on(fullscreenCG, 'keydown', closeFullscreenCG);
                                        ui.effectorize(fullscreenCG);                                        
                                    end);

                                    ui.effectorize(button);
                                end
                            else
                                button:SetFrozen(true);
                                lock:SetPickable(false);
                            end
                         end);

            --ui.off(window, 'keydown');
            --ui.on(window,  'keydown', keynav.escape('CGModeUI', 'Back'));
            -- local buttons = {};
            -- for _, v in ipairs(ui.getAllButtons(window)) do
            --     if (v:GetName() ~= 'Button' or
            --         v:GetParent():GetParent():GetName() == 'Thumbnails') then
            --         buttons[#buttons + 1] = v;
            --     end
            -- end
            --ui.on(window, 'keydown', keynav.free(buttons));
        end

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
                                 :SetText('0/0')
                                 :SetZOrder(130));

        do
            local buttons = ui.getAllButtons(window, true);
            ui.on(ui(window, 'Back'),    'click', go(goToLobby));
            ui.on(window, 'keydown', function(v, key)
               if(key == 'Escape' or key == 'SystemBack') then
                    ui(window, 'Back'):Click();
               end
            end);

            ui.effectorize(ui(window, 'Back'));
        end

        showCGs(); 

        sceneIn(window);
        anim.fadein(ui(window, 'OK'));
        anim.movein(ui(window, 'OK'), -60, 0, 0.3);

        ui.focus(ui(window, 'Back'));
    end);
end

function ui.getAllButtons(window, reversed)
    local t = {};
    ui.all(window, 'UIButton', function (o)
        t[#t + 1] = o;
        return true;
    end);
    
    if (reversed) then
        local new = {};
        for i, v in ipairs(t) do
            new[#t - i + 1] = v;
        end
        
        t = new;
    end
    
    return t;
end

function ui.getAllItems(window, reversed)
    local t = {};
    ui.all(window, 'UIButton', function (o)
        t[#t + 1] = o;
        return true;
    end);
    ui.all(window, 'USprite', function (o)
        t[#t + 1] = o;
        return true;
    end);
    
    if (reversed) then
        local new = {};
        for i, v in ipairs(t) do
            new[#t - i + 1] = v;
        end
        
        t = new;
    end
    
    return t;
end

function ui.parent(v, className)
    if (v == nil) then return end

    local parent = v:GetParent();
    if (parent:__is__(className)) then
        return parent;
    else
        return ui.parent(parent, className);
    end
end
