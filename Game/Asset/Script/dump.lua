function ui.dump(v, indent)
    indent = indent or 0;
    
    local name = v:GetName();
    if (name == nil or name == '') then
        name = '<nameless>'
    end
    
    local output = '';
    local count;
    for i = 0, indent do
        output = output .. '  '
    end
    if (v:IsPanel()) then
        count = v:GetNumberOfChildren();
        output = output .. string.format('%s (%d)', name, count)
    else
        output = output .. name
    end
    
    _ALERT(output);
    
    if (count) then
        for i = 0, count - 1 do
            local child = v:GetChildAt( i );
            ui.dump(child, indent + 1);
        end
    end
end

function ui.dumponly(v, indent)
    indent = indent or 0;

    if(indent > 1) then
        return;
    end;
    
    local name = v:GetName();
    if (name == nil or name == '') then
        name = '<nameless>'
    end
    
    local output = '';
    local count;
    for i = 0, indent do
        output = output .. '  '
    end
    if (v:IsPanel()) then
        count = v:GetNumberOfChildren();
        output = output .. string.format('%s (%d)', name, count) .. ', z=' .. v:GetZOrder();
    else
        output = output .. name .. ', z=' .. v:GetZOrder();
    end
    
    _ALERT(output);

    if (count) then
        for i = 0, count - 1 do
            local child = v:GetChildAt( i );
            ui.dumponly(child, indent + 1);
        end
    end
end