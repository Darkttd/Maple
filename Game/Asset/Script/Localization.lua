function _(id, ...)
    if (select('#', ...) == 0) then
        return STR[id];
    else
        local s = STR[id];
        local args = {...};
        return s:gsub('{(%d+)}', function (n) return args[tonumber(n)] end);
    end
end