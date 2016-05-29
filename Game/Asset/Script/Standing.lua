
Cloths =
{
    [1] = 'SchoolUniform',
    [2] = 'Hanbok',
    [3] = 'Gimono',
    [4] = 'ChinaDress',
    [5] = 'Nurse',
    [6] = 'SwimSuits',
    [7] = 'Maid',
};

Emotions =
{
    [1] = 'Default',
    [2] = 'Laugh',
    [3] = 'Angry',
    [4] = 'Shying',
    [5] = 'Crying',
    [6] = 'Awkwarden',
};


function showStanding(uiName, xpos, ypos, w, zorder, targetChar, targetCloth)
    local root = ui.getroot();
    local settings = th.settings();

    zorder = zorder or 10;

    if (targetChar == nil) then
        if(settings:GetStandingCharacter() == 'Unselected') then
            settings:SetStandingCharacter('Kradmoa');
        end

        -- If character is Maple, reposition position
        if(settings:GetStandingCharacter() == 'Maple') then
            xpos = xpos + 10;
        end

        targetChar = settings:GetStandingCharacter();
    end

    local layout = th.load('Asset/UI/standing_' .. targetChar .. '_layer_fixed');
    local w = w or ui.getroot();
    local window = layout:GetRoot():Clone();

    --_ALERT('zorder = ' .. zorder);

    window:SetXY(xpos, ypos)
          :SetXYMode('ABS')
          :SetZOrder(zorder);

    window:SetName(uiName);
    w:AddChild(window);
    ui.focus(window);

    local maple = ui(w, uiName);
    maple:SetOpacity(0.0);
    maple:HideChildren();
    maple:SetPickable(false);


    if(targetChar == 'Maple') then
        targetCloth = targetCloth or settings:GetMapleStandingCharacterCloth();

        -- Do not draw Maple's tied hair when ChinaDress
        if(targetCloth ~= 'ChinaDress') then
            ui(maple, 'Basic'):Show();
        else
            ui(maple, 'Basic_for_swimsuits'):Show();
        end
    else
        targetCloth = targetCloth or settings:GetMoaStandingCharacterCloth();

        -- Do not draw Moa's tied hair when SwimSuits
        if(targetCloth ~= 'SwimSuits') then
            ui(maple, 'Basic'):Show();
        else
            ui(maple, 'Basic_for_swimsuits'):Show();
        end
    end

    local top = ui(maple, 'BasicTop');
    if(top ~= nil) then
        top:Show();
    end

    do
        local cloth = ui(maple, 'Cloth');
        cloth:Show();
        cloth:HideChildren();
        ui(cloth, targetCloth):Show();
    end

    blinkingStarter(maple);

    --window:SetOpacity(1.0);
    --anim.fadein(window, 0.3);
    --anim.eyeblink(window);
    th.timeout(0.3, nil, function()
        window:SetOpacity(1.0);
    end);
end

function blinkingStarter(w)
    local emotion = ui(w, 'Emotion');
    emotion:Show();
    emotion:HideChildren();
    ui(emotion, 'Default1'):Show();

    local eyeblinkset = {};

    for i=1,6 do
        eyeblinkset[i] = 
        {
            layer = ui(emotion, 'Default' .. i),
            keyframe = 0.06
        };
    end

    eyeblinkset[1].keyframe = 3.0;
    eyeblinkset[7] = {layer = ui(emotion, 'Default5'), keyframe = 0.06};
    eyeblinkset[8] = {layer = ui(emotion, 'Default3'), keyframe = 0.06};

    anim.eyeblink(eyeblinkset);
end

function blinkingPause()
    th.timescale(EyeBlinkingCallback, 0.0);
end

function blinkingUnpause()
    th.timescale(EyeBlinkingCallback, 1.0);
end

function blinkingStopper()
    th.canceltimeout(EyeBlinkingCallback);
end

function showEmotion(w, showTime, emotionName)
    if (emotionName == nil) then return end

    th.timescale(EyeBlinkingCallback, 0.0);
    blinkingStopper();

    local emotion = ui(w, 'Emotion');

    for i=1,6 do
        ui(emotion, 'Default' ..i):Hide();
    end

    local targetEmotion = ui(emotion, emotionName);
    local anotherTarget = nil;
    targetEmotion:Show();

    if (emotionName == 'Shying') then
        anotherTarget = ui(emotion, 'Shying2')

        if (anotherTarget ~= nil) then
            anotherTarget:Show();
        end
    end

    th.canceltimeout(EmotionCallback);
    th.timeout(showTime, EmotionCallback, function()
        targetEmotion:Hide();
        if (anotherTarget ~= nil) then
            anotherTarget:Hide();
        end
        blinkingStarter(w);
    end);

end
