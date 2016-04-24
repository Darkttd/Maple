gfx = gfx or {};

function gfx.cherryBlossoms(parent, timeline, x, y)
    if (timeline == nil) then
        return;
    end

    x = x or 100;
    y = y or 100;
    
    local images = th.load('Asset/GFX/Common');
    do
        local spark = UISpark({
            {
                Opacity = { XAxis='Lifetime', [0] = 0, [0.1] = 1, [0.6] = 1, [1] = 0 },
                Lifetime = 4.5,
                Size = '0.04~0.08',
                Image = '0~1',
                Angle = gfx.rad(0,360),
                AngleAxis = 'RANDOM',
                AngularSpeed = { XAxis='SpeedSq', [0] = math.rad(180), [300000] = math.rad(360) },
                Emitters =
                {
                    {
                        Direction = {0,-1},
                        Angles = gfx.rad(0, 150),
                        Flow = 60,
                        Force = '100~200',
                        Zone = { 0, 0, Radius=200 },
                        InitialParticles = 20,
                    },
                },
                Modifiers = 
                {
                    { Force={-60, 0}, Trigger='ALWAYS' },
                    -- { Zone={-500, 400, Radius=300}, Mass=-50000000, Distance=10 },
                    -- { Eye={0, 0, Radius=0}, Attraction=10, Rotation=300, Trigger='ALWAYS' },
                },
                Gravity = { 0, 100 },
            },
            Images =
            {
                images:Find('CB1'),
                images:Find('CB2'),
            },
        });
        spark:SetX(x);
        spark:SetY(y);
        spark:SetOrigin(0.5, 0.5);
        spark:SetPickable(false);
        --spark:InsertEffect(UIBlendingEffect('ADD'));
        spark:SetTimeline(timeline);
        spark:SetZOrder(0);
        parent:AddChild(spark);
    end
    do
        local spark = UISpark({
            {
                Opacity = { XAxis='Lifetime', [0] = 0, [0.1] = 1, [0.6] = 1, [1] = 0 },
                Lifetime = 4.5,
                Size = '0.1~0.2',
                Image = '0~1',
                Angle = gfx.rad(0,360),
                AngleAxis = 'RANDOM',
                AngularSpeed = { XAxis='SpeedSq', [0] = math.rad(360), [300000] = math.rad(540) },
                Emitters =
                {
                    {
                        Direction = {0,-1},
                        Angles = gfx.rad(0, 150),
                        Flow = 70,
                        Force = '100~200',
                        Zone = { {-200, -200}, {200, 200} },
                        InitialParticles = 30,
                    },
                },
                Modifiers = 
                {
                    { Force={-200, 0}, Trigger='ALWAYS' },
                    -- { Zone={-500, 400, Radius=300}, Mass=-50000000, Distance=10 },
                    -- { Eye={0, 0, Radius=0}, Attraction=10, Rotation=300, Trigger='ALWAYS' },
                },
                Gravity = { 0, 200 },
            },
            Images =
            {
                images:Find('CB1'),
                images:Find('CB2'),
            },
        });
        spark:SetX(x);
        spark:SetY(y);
        spark:SetOrigin(0.5, 0.5);
        spark:SetPickable(false);
        --spark:InsertEffect(UIBlendingEffect('ADD'));
        spark:SetTimeline(timeline);
        spark:SetZOrder(0);
        parent:AddChild(spark);
    end
    do
        local spark = UISpark({
            {
                Opacity = { XAxis='Lifetime', [0] = 0, [0.1] = 1, [0.6] = 1, [1] = 0 },
                Lifetime = 4.5,
                Size = '0.3~0.4',
                Image = '0~1',
                Angle = gfx.rad(0,360),
                AngleAxis = 'RANDOM',
                AngularSpeed = { XAxis='SpeedSq', [0] = math.rad(360), [300000] = math.rad(540) },
                Emitters =
                {
                    {
                        Direction = {0,-1},
                        Angles = gfx.rad(0, 150),
                        Flow = 1,
                        Force = '200~400',
                        Zone = { 0, 0, Radius=100 },
                        InitialParticles = 0,
                    },
                },
                Modifiers = 
                {
                    { Force={-300, 0}, Trigger='ALWAYS' },
                    -- { Zone={-500, 400, Radius=300}, Mass=-50000000, Distance=10 },
                    -- { Eye={0, 0, Radius=0}, Attraction=10, Rotation=300, Trigger='ALWAYS' },
                },
                Gravity = { 0, 400 },
            },
            Images =
            {
                images:Find('CB1'),
                images:Find('CB2'),
            },
        });
        spark:SetX(x);
        spark:SetY(y);
        spark:SetOrigin(0.5, 0.5);
        spark:SetPickable(false);
        --spark:InsertEffect(UIBlendingEffect('ADD'));
        spark:SetTimeline(timeline);
        spark:SetZOrder(200);
        parent:AddChild(spark);
    end
end

function gfx.hello(world, x, y)
    x = x or 100;
    y = y or 100;
    
    local images = th.load('Asset/GFX/Feathers');
    for i = 1, 2 do
        local aaa = i == 1 and 1 or -1;
        local spark = UISpark({
            {
                Opacity = { XAxis='Lifetime', [0] = 0, [0.1] = 1, [0.6] = 1, [1] = 0 },
                Lifetime = 4.5,
                Size = '0.8~1.2',
                Image = '0~3',
                Angle = gfx.rad(0,360),
                AngleAxis = 'RANDOM',
                AngularSpeed = { XAxis='SpeedSq', [0] = aaa*math.rad(360), [300000] = aaa*math.rad(540) },
                Emitters =
                {
                    {
                        Direction = {0,-1},
                        Angles = gfx.rad(0, 150),
                        Flow = 20,
                        Force = '100~200',
                        Zone = { 0, 0, Radius=100 },
                        InitialParticles = 10,
                    },
                },
                Modifiers = 
                {
                    { Force={-300, 0}, Trigger='ALWAYS' },
                    -- { Zone={-500, 400, Radius=300}, Mass=-50000000, Distance=10 },
                    -- { Eye={0, 0, Radius=0}, Attraction=10, Rotation=300, Trigger='ALWAYS' },
                },
                Gravity = { 0, 360 },
            },
            Images =
            {
                images:Find('1'),
                images:Find('2'),
                images:Find('3'),
                images:Find('4'),
            },
        });
        spark:SetX(x);
        spark:SetY(y);
        spark:SetOrigin(0.5, 0.5);
        spark:SetPickable(false);
        --spark:InsertEffect(UIBlendingEffect('ADD'));
        spark:SetTimeline(world:GetAnimationTimeline());
        world:GetHUDLayer():AddChild(spark);
    end
    for i = 1, 2 do
        local aaa = i == 1 and 1 or -1;
        local spark = UISpark({
            {
                Opacity = { XAxis='Lifetime', [0] = 0, [0.1] = 1, [0.6] = 1, [1] = 0 },
                Lifetime = 4.5,
                Size = '0.3~0.5',
                Image = '0~3',
                Angle = gfx.rad(0,360),
                AngleAxis = 'RANDOM',
                AngularSpeed = { XAxis='SpeedSq', [0] = aaa*math.rad(180), [300000] = aaa*math.rad(360) },
                Emitters =
                {
                    {
                        Direction = {0,-1},
                        Angles = gfx.rad(0, 150),
                        Flow = 10,
                        Force = '100~200',
                        Zone = { 0, 0, Radius=100 },
                        InitialParticles = 10,
                    },
                },
                Modifiers = 
                {
                    { Force={-100, 0}, Trigger='ALWAYS' },
                    -- { Zone={-500, 400, Radius=300}, Mass=-50000000, Distance=10 },
                    -- { Eye={0, 0, Radius=0}, Attraction=10, Rotation=300, Trigger='ALWAYS' },
                },
                Gravity = { 0, 100 },
            },
            Images =
            {
                images:Find('1'),
                images:Find('2'),
                images:Find('3'),
                images:Find('4'),
            },
        });
        spark:SetX(x);
        spark:SetY(y);
        spark:SetOrigin(0.5, 0.5);
        spark:SetPickable(false);
        --spark:InsertEffect(UIBlendingEffect('ADD'));
        spark:SetTimeline(world:GetAnimationTimeline());
        world:GetHUDLayer():AddChild(spark);
    end
end

function gfx.ssss(world, x, y)
    x = x or 100;
    y = y or 100;

    local images = th.load('Asset/GFX/Common');
    
    local image = UIImage();
    image:SetX(x);
    image:SetY(y);
    image:SetOrigin(0.5, 0.5);
    image:SetPickable(false);
    image:InsertEffect(UIBlendingEffect('ADD'));
    image:SetTransform(UITransform3D());
    image:SetSource(images:Find('Laser1'));
    world:GetHUDLayer():AddChild(image);
    
    local index = {value = 1};
    th.alarm(1/30, 10, nil,
            function (t)
                image:SetSource(images:Find('Laser' .. index.value));
                index.value = index.value + 1;
                if (index.value >= 9) then
                    index.value = 1;
                end
            end,
            function () end);

    image:GetTransform():SetScale(1, 0)
    th.alarm(1/60, 0.2, nil,
            function (t)
                local sy = anim.ease.o3(t);
                image:GetTransform():SetScale(1, sy);
            end,
            function ()  image:GetTransform():SetScale(1, 1) end);
    th.timeout(10, nil, function ()
        th.alarm(1/60, 0.2, nil,
                function (t)
                    local sy = 1 - anim.ease.o3(t);
                    image:GetTransform():SetScale(1, sy);
                end,
                function ()  image:GetTransform():SetScale(1, 0) end);
    end);

    
    local spark = UISpark({
        {
            Opacity = 1,
            Lifetime = 2,
            Size = { XAxis='Lifetime', SineCurve={0.025, 1.5, 0,2,0,40,512} } ,
            Image = {0, 3},
            Emitters =
            {
                {
                    Direction = {1,0},
                    Flow = 100,
                    Force = 3000,
                    Zone = { {0, -10}, {0, 10} },
                    InitialParticles = 1,
                },
            },
            Modifiers = 
            {
                -- { Force={-1000, 0, Factor='Image**'}, Trigger='ALWAYS' }
                -- { Zone={0, 0, Radius=1000}, Mass=10000, Distance=10 },
                -- { Eye={0, 0, Radius=0}, Attraction=10, Rotation=300, Trigger='ALWAYS' },
            },
            Gravity = { 0, 0 },
        },
        Images =
        {
            images:Find('Ball1'),
            images:Find('Ball2'),
            images:Find('Ball3'),
            images:Find('Ball4')
        },
    });
    spark:SetX(x);
    spark:SetY(y);
    spark:SetOrigin(0.5, 0.5);
    spark:SetPickable(false);
    spark:InsertEffect(UIBlendingEffect('ADD'));
    spark:SetTimeline(world:GetAnimationTimeline());
    world:GetHUDLayer():AddChild(spark);
end

function gfx.helloss(world, x, y)
    local images = th.load('Asset/GFX/Common');   
    local spark = UISpark({
        {
            Opacity = { XAxis='Lifetime', [0] = 1, [0.01] = 1, [0.1] = 0, [1] = 0 },
            Lifetime = 1,
            Size = 0.2,
            Image = 0,
            Emitters =
            {
                {
                    Direction = {1,0},
                    Angles = gfx.rad(0, 90),
                    Flow = 100,
                    Force = 1000,
                    Zone = { {0, -20}, {0, 20} },
                    InitialParticles = 10,
                },
            },
            Modifiers = 
            {
                -- { Force={-1000, 0, Factor='Image**'}, Trigger='ALWAYS' }
                -- { Zone={0, 0, Radius=1000}, Mass=10000, Distance=10 },
                -- { Eye={0, 0, Radius=0}, Attraction=10, Rotation=300, Trigger='ALWAYS' },
            },
            Gravity = { 0, 0 },
        },
        Images =
        {
            images:Find('Focus'),
        },
    });
    spark:SetX(x);
    spark:SetY(y);
    spark:SetOrigin(0.5, 0.5);
    spark:SetPickable(false);
    spark:InsertEffect(UIBlendingEffect('ADD'));
    spark:SetTimeline(world:GetAnimationTimeline());
    world:GetHUDLayer():AddChild(spark);
end

function gfx.hellffo(world, x, y)
    local images = th.load('Asset/GFX/Common');   
    local spark = UISpark({
        {
            Opacity = { XAxis='Lifetime', [0] = 0, [0.3] = 1, [0.6] = 1, [1] = 0 },
            Lifetime = 0.7,
            Size = 0.3,
            Image = 0,
            Emitters =
            {
                {
                    Direction = 'INVNORMAL',
                    NormalZone = { 200, 200, Radius=100 },
                    Flow = 300,
                    Force = '400',
                    Zone = { 0, 0, Radius='300~350' },
                    InitialParticles = 10,
                },
            },
            Modifiers = 
            {
                -- { Force={-1000, 0, Factor='Image**'}, Trigger='ALWAYS' }
                -- { Zone={0, 0, Radius=1000}, Mass=10000, Distance=10 },
                -- { Eye={0, 0, Radius=0}, Attraction=10, Rotation=300, Trigger='ALWAYS' },
            },
            Gravity = { 0, 0 },
        },
        Images =
        {
            images:Find('Focus'),
        },
    });
    spark:SetX(x);
    spark:SetY(y);
    spark:SetOrigin(0.5, 0.5);
    spark:SetPickable(false);
    spark:InsertEffect(UIBlendingEffect('ADD'));
    spark:SetTimeline(world:GetAnimationTimeline());
    world:GetHUDLayer():AddChild(spark);
end

function gfx.helloee(world, x, y)
    x = x or 100;
    y = y or 100;

    local images = th.load('Asset/GFX/Common');   
    local image = UIImage();
    image:SetX(x);
    image:SetY(y);
    image:SetOrigin(0.5, 0.5);
    image:SetPickable(false);
    image:InsertEffect(UIBlendingEffect('ADD'));
    image:SetTransform(UITransform3D());
    image:GetTransform():SetRotation(0, 0, math.random(math.pi * 2));
    image:SetSource(images:Find('Explosion2A'));
    world:GetHUDLayer():AddChild(image);
    anim.expand(image);
    anim.fadeout(image, 0.3);
    
    local spark = UISpark({
        {
            Opacity = { XAxis='Lifetime', [0] = 0, [0.1] = 1, [0.6] = 1, [1] = 0 },
            Lifetime = 0.5,
            Size = 0.3,
            Image = 0,
            Emitters =
            {
                {
                    Direction = {0,-1},
                    Angles = gfx.rad(0, 150),
                    Flow = 0,
                    Force = '100~200',
                    Zone = { 0, 0 },
                    InitialParticles = 10,
                },
            },
            Modifiers = 
            {
                -- { Force={-1000, 0, Factor='Image**'}, Trigger='ALWAYS' }
                -- { Zone={0, 0, Radius=1000}, Mass=10000, Distance=10 },
                -- { Eye={0, 0, Radius=0}, Attraction=10, Rotation=300, Trigger='ALWAYS' },
            },
            Gravity = { 0, 0 },
        },
        Images =
        {
            images:Find('Explosion2B'),
        },
    });
    spark:SetX(x);
    spark:SetY(y);
    spark:SetOrigin(0.5, 0.5);
    spark:SetPickable(false);
    spark:InsertEffect(UIBlendingEffect('ADD'));
    spark:SetTimeline(world:GetAnimationTimeline());
    world:GetHUDLayer():AddChild(spark);
    
    for i = 1, 3 do
        local image = UIImage();
        image:SetX(x);
        image:SetY(y);
        image:SetOrigin(0.5, 0.5);
        image:SetPickable(false);
        image:InsertEffect(UIBlendingEffect('ADD'));
        image:SetTransform(UITransform3D());
        image:GetTransform():SetRotation(0, 0, math.random(math.pi * 2));
        image:SetSource(images:Find('Explosion2C'));
        world:GetHUDLayer():AddChild(image);
        anim.screenoff(image);
    end
end

function gfx.aroundbat(world, x, y)
    x = x or 0;
    y = y or 0;

    local images = th.load('Asset/GFX/Common');
    local spark = UISpark({
        {
            Opacity = { XAxis='Lifetime', [0] = 1, [0.1] = 1, [0.9] = 1, [1] = 0 },
            Lifetime = 2,
            Angle = 0,
            Size = 1,
            Image = '0~1',
            Emitters =
            {
                {
                    --Force = '20~50',
                    Flow = 10,
                    Zone = { 0, 0, Radius='50~100' },
                    InitialParticles = 5,
                },
            },
            Modifiers = 
            {
                -- { Force={-1000, 0, Factor='Image**'}, Trigger='ALWAYS' }
                { Zone={0, 0, Radius=1000}, Mass=10000, Distance=10 },
                { Eye={0, 0, Radius=0}, Attraction=10, Rotation=300, Trigger='ALWAYS' },
            },
            Gravity = { 0, 0 },
        },
        Images = 
        {
            images:Find('Explosion'),
            images:Find('Explosion')
        },
    });
    spark:SetX(x);
    spark:SetY(y);
    spark:InsertEffect(UIBlendingEffect('ADD'));
    spark:SetTimeline(world:GetAnimationTimeline());
    return spark;
end

function gfx.rad(min, max)
    if (min and max) then
        return string.format('%f~%f', math.rad(min), math.rad(max))
    else
        return math.rad(min or max);
    end
end