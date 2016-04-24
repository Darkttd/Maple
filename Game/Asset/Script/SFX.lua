do
    __sfx__ =
    {
        path = function (name)
            return 'Asset/SFX/'  .. name .. '.wav';
        end,
        random = function (group, ...)
            group = group or 0;
            
            local t = {...};
            for i = 1, #t do
                t[i] = __sfx__.path(t[i]);
            end
            
            if (#t > 1) then
                return function ()
                    sfx.play(t[math.random(#t)], group);
                end
            else
                local name = t[1];
                return function ()
                    sfx.play(name, group);
                end
            end
        end,
        
        urandom = function (group, ...)
            group = group or 0;
        
            local tw = 0; -- total weights
            local names = {};
            local weights = {};
            for i = 1, select('#', ...), 2 do
                local name = select(i + 0, ...);
                local weight = select(i + 1, ...);
                names[#names + 1] = __sfx__.path[name];
                weights[#weights + 1] = weight;
                tw = tw + weight;
            end
            
            return function ()
                local w = math.random(tw)
                for i = 1, #weights do
                    local weight = weights[i];
                    if (w <= weight) then
                        sfx.play(names[i], group);
                        return;
                    else
                        w = w - weight;
                    end
                end
            end
        end,

        install_singles = function (self, group, t)
            for _, v in ipairs(t) do
                local path = __sfx__.path(v);
                self[v] = function () sfx.play(path, group); end;
            end
        end,
    };

    sfx.install_singles = __sfx__.install_singles;
    sfx:install_singles(0,
    {
    });
    sfx.ClockChannel = 10;
    sfx:install_singles(sfx.ClockChannel, {'Clock'})

    __sfx__ = nil;
end