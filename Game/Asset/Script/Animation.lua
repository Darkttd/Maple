anim = anim or {};
anim.ease = anim.ease or {};
function anim.ease.linear(t) return t; end
function anim.ease.i2(t) return t * t; end
function anim.ease.o2(t) return -t * (t - 2); end
function anim.ease.io2(t)
    t = t * 2;
    if (t < 1) then
        return 0.5 * t * t;
    else
        return -0.5 * ((t - 1) * (t - 3) - 1);
    end
end
function anim.ease.i3(t) return t * t * t; end
function anim.ease.o3(t) t = t - 1; return t * t * t + 1; end
function anim.ease.io3(t)
    t = t * 2;
    if (t < 1) then
        return 0.5 * t * t * t;
    else
        t = t - 2;
        return 0.5 * (t * t * t + 2);
    end
end
function anim.ease.isin(t) return -1 * math.cos(t * math.pi * 0.5) + 1; end
function anim.ease.osin(t) return math.sin(t * math.pi * 0.5); end
function anim.ease.iosin(t) return -0.5 * (math.cos(t * math.pi) - 1); end
function anim.ease.iexp(t) return t > 0 and 2 ^ (10 * (t - 1)) or 0; end
function anim.ease.oexp(t) return t < 1 and -(2 ^ (-10 * t)) + 1 or 1; end
function anim.ease.ioexp(t)
    if (t <= 0) then return 0 end
    if (t >= 1) then return 1 end
    t = t * 2;
    if (t < 1) then
        return 0.5 * (2 ^ (10 * (t - 1)));
    else
        return 0.5 * (-2 ^ (-10 * (t - 1)) + 2);
    end
end
function anim.ease.icir(t) return -1 * (math.sqrt(1 - t * t) - 1); end;
function anim.ease.ocir(t) t = t - 1; return math.sqrt(1 - t * t); end;
function anim.ease.iocir(t)
    t = t * 2;
    if (t < 1) then
        return -0.5 * (math.sqrt(1 - t * t) - 1);
    else
        t = t - 2;
        return 0.5 * (math.sqrt(1 - t * t) + 1);
    end
end
function anim.ease.generateioback(overshoot)
    return function (t)
        local o = overshoot * 1.525;
        t = t * 2;
        if (t < 1) then
            return 0.5 * (t * t * ((o + 1) * t - o));
        else
            t = t - 2;
            return 0.5 * (t * t * ((o + 1) * t + o) + 2);
        end
    end
end;
anim.ease.ioback = anim.ease.generateioback(1.70158);

function anim.ease.generateoelastic(amplitude, period)
    return function (t)
        if (t <= 0) then
            return 0;
        end
        if (t >= 1) then
            return 1;
        end
        if (period == 0) then
            period = 0.3;
        end
        
        local s = 0;
        if (amplitude < 1) then
            amplitude = 1;
            s = period * 0.2;
        else
            s = period / (math.pi * 2) * math.asin(1 / amplitude);
        end
        
        return amplitude * (2 ^ (-10 * t)) * math.sin((t - s) * (math.pi * 2) / period) + 1;
    end
end
anim.ease.oelastic = anim.ease.generateoelastic(4, 3);

function anim.moveby(v, x, y, duration, options)
    if (v == nil) then return end
    
    x = x or 0;
    y = y or 0;
    
    if (x == 0 and y == 0) then return end
    
    options = options or {};
    options.id     = options.id or nil;
    options.interp = options.interp or anim.ease.iosin;
    
    duration = duration or 0.5;
    local sx, sy = v:GetXY();
    local interp = options.interp;
    th.alarm(1/60, duration, options.id,
            function (t) v:SetXY(sx + x * interp(t), sy + y * interp(t)); end,
            function ()  v:SetXY(sx + x,             sy + y);             end);
            
    return duration;
end

function anim.movein(v, x, y, duration, options)
    if (v == nil) then return end
    
    x = x or 0;
    y = y or 0;
    
    if (x == 0 and y == 0) then return end
    
    v:SetXY(v:GetX() - x,
            v:GetY() - y);
    anim.moveby(v, x, y, duration, options);
    
    return duration;
end

function anim.scalein(v, duration, scale, interp)
    if (v == nil) then return end
    
    duration = duration or 0.3;
    scale = scale or 0.1;
    interp = interp or anim.ease.o3;
    th.alarm(1/60, duration, nil,
            function (t)
                local s = interp(t) * scale + (1 - scale);
                v:GetTransform():SetScale(s, s);
            end,
            function () v:GetTransform():SetScale(1, 1) end);
    
    return duration;
end

function anim.scaleout(v, duration, scale, interp)
    if (v == nil) then return end
    
    duration = duration or 0.6;
    scale = scale or 0.1;
    interp = interp or anim.ease.i3;
    th.alarm(1/60, duration, nil,
            function (t) local s = (interp(1 - t) * scale + (1 - scale)); v:GetTransform():SetScale(s, s) end,
            function ()  v:GetTransform():SetScale(0, 0) end);
            
    return duration;
end

function anim.fadein(v, duration, options)
    if (v == nil) then return end
    
    options = options or {};
    options.id = options.id or nil;
    options.delay = options.delay or 0;
    
    duration = duration or 0.3;
    local so = v:GetOpacity();
    local function inner()
        th.alarm(1/60, duration, options.id,
                 function (t) v:SetOpacity(so + (1 - so) * t); end,
                 function ()  v:SetOpacity(1); end);
    end
    
    if (options.delay <= 0.001) then
        inner();
    else
        th.timeout(options.delay, inner);
    end

    return duration;
end

function anim.fadeout(v, duration, options)
    if (v == nil) then return end

    options = options or {};
    options.id = options.id or nil;
    options.delay = options.delay or 0;

    duration = duration or 0.3;
    local so = v:GetOpacity();
    local function inner()
        th.alarm(1/60, duration, options.id,
                 function (t) v:SetOpacity(so - (so * t)); end,
                 function ()  v:SetOpacity(0); end);
    end
    
    if (options.delay <= 0.001) then
        inner();
    else
        th.timeout(options.delay, inner);
    end
            
    return duration;
end

function anim.screenoff(v, duration)
    if (v == nil) then return; end
    
    local scale = 0.5;
    
    duration = duration or 0.3;
    v:GetTransform():SetScale(scale, scale)
    th.alarm(1/60, duration, nil,
            function (t)
                local sy = 1 + anim.ease.o2(t);
                local sx = anim.ease.i2(1 - t);
                v:GetTransform():SetScale(sx * scale, sy * scale);
            end,
            function ()  v:GetTransform():SetScale(0, 0) end);
            
    return duration;
end

function anim.expand(v, duration)
    if (v == nil) then return; end
    
    local scale = 2;
    
    duration = duration or 0.5;
    v:GetTransform():SetScale(0, 0)
    th.alarm(1/60, duration, nil,
            function (t)
                local s = anim.ease.o2(t) * scale;
                v:GetTransform():SetScale(s, s);
            end,
            function () end);
            
    return duration;
end

function anim.popup(v)
    if (v == nil) then return; end

    v:GetTransform():SetScale(0, 0);
    local duration = duration or 0.3;
    th.alarm(1/60, duration, nil,
            function (t)
                local s = anim.ease.oelastic(t);
                local transform = v:GetTransform();
                transform:SetScale(s, s);
                transform:SetGlobalOffset(0, (math.sin(t*math.pi)) * -24);
            end,
            function ()
                local transform = v:GetTransform();
                v:GetTransform():SetScale(1, 1);
                v:GetTransform():SetGlobalOffset(0, 0);
            end);
    
    return duration;
end