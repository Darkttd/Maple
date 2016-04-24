#pragma once

#include <Bibim/FWD.h>
#include <Bibim/Color.h>
#include <Bibim/Matrix4.h>
#include <Bibim/Point2.h>
#include <Bibim/Point3.h>
#include <Bibim/Point4.h>
#include <Bibim/Rect.h>
#include <Bibim/RectF.h>
#include <Bibim/Vector2.h>
#include <Bibim/Vector3.h>
#include <Bibim/Vector4.h>

namespace Maple
{
    extern const char* ProjectName;

    using Bibim::byte;
    using Bibim::longint;
    using Bibim::Color;
    using Bibim::Matrix4;
    using Bibim::Point2;
    using Bibim::Point3;
    using Bibim::Point4;
    using Bibim::Rect;
    using Bibim::RectF;
    using Bibim::Vector2;
    using Bibim::Vector3;
    using Bibim::Vector4;
    using Bibim::String;

#   ifndef BIBIM_CPLUSPLUS_0X
    using Bibim::nullptr;
#   endif
    
    using Bibim::StaticCast;
    using Bibim::DynamicCast;

    BBForwardDeclareSmartPointerClass(UIGameLogic);

    class Game;
    class Lua;
    class GamerSavedata;
    class GamerSettings;
    class PatternManager;
}
