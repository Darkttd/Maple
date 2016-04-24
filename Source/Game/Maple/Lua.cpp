#include <Maple/Lua.h>
#include <Bibim/All.h>
#include <Maple/Game.h>
#include <Maple/Version.h>
#include <Maple/GamerSettings.h>
#include <Maple/GamerSavedata.h>
#include <Maple/Gameplay/UIGameLogic.h>
#include <Maple/Gameplay/UIPuzzleSelect.h>
using namespace Bibim;

namespace Maple
{
    struct Lua::Internal
    {
#       define INVOKE_GAME(func)    void* ud = nullptr; \
                                    lua_getallocf(state, &ud); \
                                    if (ud) \
                                    { \
                                        Lua* lua = reinterpret_cast<Lua*>(ud); \
                                        BBAssert(lua->GetState() == state); \
                                        return lua->func(); \
                                    } \
                                    else \
                                        return 0;

        static int Settings(lua_State* state)               { INVOKE_GAME(Settings); }
        static int Savedata(lua_State* state)               { INVOKE_GAME(Savedata); }
        static int PatternMan(lua_State* state)             { INVOKE_GAME(PatternMan); }
        static int Version(lua_State* state)                { INVOKE_GAME(Version); }

        //static int CreateVN(lua_State* state)               { INVOKE_GAME(CreateVN); }
        static int CreatePuzzleSelect(lua_State* state)     { INVOKE_GAME(CreatePuzzleSelect); }
        static int CreateWorld(lua_State* state)            { INVOKE_GAME(CreateWorld); }
        static int DestroyWorld(lua_State* state)           { INVOKE_GAME(DestroyWorld); }

#       undef INVOKE_GAME
    };

    namespace
    {
        static inline StandardGame* GetGame(lua_State* L)
        {
            StandardGame::LuaBase* lua = nullptr;
            if (lua_getallocf(L, reinterpret_cast<void**>(&lua)))
                return lua->GetGame();
            else
                return nullptr;
        }
    }

    Lua::Lua(Game* game)
        : Base(game),
          game(game),
          gamerSettings(nullptr),
          gamerSavedata(nullptr)
    {
        LoadLibraries();
        LoadClasses();
    }

    Lua::~Lua()
    {
    }

    void Lua::SetGamerSettings(GamerSettings* value)
    {
        gamerSettings = value;
    }

    void Lua::SetGamerSavedata(GamerSavedata* value)
    {
        gamerSavedata = value;
    }

    void Lua::SetPatternManager(PatternManager* value)
    {
        patternManager = value;
    }

    void Lua::LoadLibraries()
    {
       lua_State* state = GetState();

        const struct luaL_Reg thLib [] = {
            { "settings", &Internal::Settings },
            { "savedata", &Internal::Savedata },
            { "patternman", &Internal::PatternMan },
            { "version", &Internal::Version },
            { NULL, NULL}  /* sentinel */
        };

        const struct luaL_Reg uiLib [] = {
            //{ "createvn", &Internal::CreateVN },
            { "createpuzzleselect", &Internal::CreatePuzzleSelect },
            { "createworld", &Internal::CreateWorld },
            { "destroyworld", &Internal::DestroyWorld },
            { NULL, NULL}  /* sentinel */
        };

        lua_getglobal(state, "th");
        luaL_setfuncs(state, thLib, 0);
        lua_pop(state, 1);

        lua_getglobal(state, "ui");
        luaL_setfuncs(state, uiLib, 0);
        lua_pop(state, 1);
    }

    void Lua::LoadClasses()
    {
        lua_State* L = GetState();
        
        #ifdef BIBIM_PLATFORM_IOS
        lua_tinker::set(L, "PLATFORM", "IOS");
        #else
        // PC platform 은 타깃이 아니므로, 안드로이드로 간주합니다.
        lua_tinker::set(L, "PLATFORM", "ANDROID");
        #endif

        lua_tinker::class_add<UIGameLogic>(L, "UIGameLogic");
            lua_tinker::class_inh<UIGameLogic, UIPanel>(L);
            lua_tinker::class_def<UIGameLogic>(L, "Initialize", &UIGameLogic::Initialize);
            lua_tinker::class_def<UIGameLogic>(L, "Restart", &UIGameLogic::Restart);
            lua_tinker::class_def<UIGameLogic>(L, "GetTimeline", &UIGameLogic::GetTimeline);
            lua_tinker::class_def<UIGameLogic>(L, "GetUILayer", &UIGameLogic::GetUILayer);
            lua_tinker::class_def<UIGameLogic>(L, "SetUILayer", &UIGameLogic::SetUILayer);
            lua_tinker::class_def<UIGameLogic>(L, "GetPauseUILayer", &UIGameLogic::GetPauseUILayer);
            lua_tinker::class_def<UIGameLogic>(L, "SetPauseUILayer", &UIGameLogic::SetPauseUILayer);
            lua_tinker::class_def<UIGameLogic>(L, "GetGameOverUILayer", &UIGameLogic::GetGameOverUILayer);
            lua_tinker::class_def<UIGameLogic>(L, "SetGameOverUILayer", &UIGameLogic::SetGameOverUILayer);
            lua_tinker::class_def<UIGameLogic>(L, "GetCursorLayer", &UIGameLogic::GetCursorLayer);
            lua_tinker::class_def<UIGameLogic>(L, "GetNumberLayer", &UIGameLogic::GetNumberLayer);
            lua_tinker::class_def<UIGameLogic>(L, "SetNumberLayer", &UIGameLogic::SetNumberLayer);
            lua_tinker::class_def<UIGameLogic>(L, "GetZeroPointOffset", &UIGameLogic::GetZeroPointOffset);
            lua_tinker::class_def<UIGameLogic>(L, "SetZeroPointOffset", &UIGameLogic::SetZeroPointOffsetXY);
            lua_tinker::class_def<UIGameLogic>(L, "GetContextOffset", &UIGameLogic::GetContextOffset);
            lua_tinker::class_def<UIGameLogic>(L, "GetInvertedContextCoordinate", &UIGameLogic::GetInvertedContextCoordinate);
            lua_tinker::class_def<UIGameLogic>(L, "GetBlockPixelSize", &UIGameLogic::GetBlockPixelSize);
            lua_tinker::class_def<UIGameLogic>(L, "SetBlockPixelSize", &UIGameLogic::SetBlockPixelSize);
            lua_tinker::class_def<UIGameLogic>(L, "GetInterBlockOffset", &UIGameLogic::GetInterBlockOffset);
            lua_tinker::class_def<UIGameLogic>(L, "SetInterBlockOffset", &UIGameLogic::SetInterBlockOffset);
            lua_tinker::class_def<UIGameLogic>(L, "IsPauseMenuShowing", &UIGameLogic::IsPauseMenuShowing);
            lua_tinker::class_def<UIGameLogic>(L, "SetPauseMenuShowing", &UIGameLogic::SetPauseMenuShowing);
            lua_tinker::class_def<UIGameLogic>(L, "IsClearUIShowing", &UIGameLogic::IsClearUIShowing);
            lua_tinker::class_def<UIGameLogic>(L, "SetClearUIShowing", &UIGameLogic::SetClearUIShowing);
            lua_tinker::class_def<UIGameLogic>(L, "IsUnlockUIShowing", &UIGameLogic::IsUnlockUIShowing);
            lua_tinker::class_def<UIGameLogic>(L, "SetUnlockUIShowing", &UIGameLogic::SetUnlockUIShowing);
            lua_tinker::class_def<UIGameLogic>(L, "IsGameOverUIShowing", &UIGameLogic::IsGameOverUIShowing);
            lua_tinker::class_def<UIGameLogic>(L, "SetGameOverUIShowing", &UIGameLogic::SetGameOverUIShowing);
            lua_tinker::class_def<UIGameLogic>(L, "UpdateCursorLayer", &UIGameLogic::UpdateCursorLayerXY);
            lua_tinker::class_def<UIGameLogic>(L, "CursorMove", &UIGameLogic::CursorMove);
            lua_tinker::class_def<UIGameLogic>(L, "TryFillColor", &UIGameLogic::TryFillColor);
            lua_tinker::class_def<UIGameLogic>(L, "TryFillErased", &UIGameLogic::TryFillErased);
			lua_tinker::class_def<UIGameLogic>(L, "GetPuzzleSize", &UIGameLogic::GetPuzzleSize);
			lua_tinker::class_def<UIGameLogic>(L, "GetPatternNumber", &UIGameLogic::GetPatternNumber);

        lua_tinker::class_add<UIPuzzleSelect>(L, "UIPuzzleSelect");
            lua_tinker::class_inh<UIPuzzleSelect, UIPanel>(L);
            lua_tinker::class_def<UIPuzzleSelect>(L, "GetUILayer", &UIPuzzleSelect::GetUILayer);
            lua_tinker::class_def<UIPuzzleSelect>(L, "SetUILayer", &UIPuzzleSelect::SetUILayer);
            lua_tinker::class_def<UIPuzzleSelect>(L, "GetUnlockedLayer", &UIPuzzleSelect::GetUnlockedLayer);
            lua_tinker::class_def<UIPuzzleSelect>(L, "GetLockedLayer", &UIPuzzleSelect::GetLockedLayer);
            lua_tinker::class_def<UIPuzzleSelect>(L, "GetPatternPixel", &UIPuzzleSelect::GetPatternPixel);
            lua_tinker::class_def<UIPuzzleSelect>(L, "SetPatternPixel", &UIPuzzleSelect::SetPatternPixel);
            lua_tinker::class_def<UIPuzzleSelect>(L, "GetCurrentPage", &UIPuzzleSelect::GetCurrentPage);
            lua_tinker::class_def<UIPuzzleSelect>(L, "SetCurrentPage", &UIPuzzleSelect::SetCurrentPage);
            lua_tinker::class_def<UIPuzzleSelect>(L, "GetPatternManager", &UIPuzzleSelect::GetPatternManager);
            lua_tinker::class_def<UIPuzzleSelect>(L, "SetPatternManager", &UIPuzzleSelect::SetPatternManager);

        lua_tinker::class_add<GamerSettings>(L, "GamerSettings");
            lua_tinker::class_inh<GamerSettings, GameModule>(L);
            lua_tinker::class_def<GamerSettings>(L, "Reset", &GamerSettings::Reset);
            lua_tinker::class_def<GamerSettings>(L, "Apply", &GamerSettings::Apply);
            lua_tinker::class_def<GamerSettings>(L, "GetStandingCharacter", &GamerSettings::GetStandingCharacter);
            lua_tinker::class_def<GamerSettings>(L, "SetStandingCharacter", &GamerSettings::SetStandingCharacter);
            lua_tinker::class_def<GamerSettings>(L, "GetMoaStandingCharacterCloth", &GamerSettings::GetMoaStandingCharacterCloth);
            lua_tinker::class_def<GamerSettings>(L, "SetMoaStandingCharacterCloth", &GamerSettings::SetMoaStandingCharacterCloth);
            lua_tinker::class_def<GamerSettings>(L, "GetMapleStandingCharacterCloth", &GamerSettings::GetMapleStandingCharacterCloth);
            lua_tinker::class_def<GamerSettings>(L, "SetMapleStandingCharacterCloth", &GamerSettings::SetMapleStandingCharacterCloth);
            lua_tinker::class_def<GamerSettings>(L, "GetBGMMute", &GamerSettings::GetBGMMute);
            lua_tinker::class_def<GamerSettings>(L, "SetBGMMute", &GamerSettings::SetBGMMute);
            lua_tinker::class_def<GamerSettings>(L, "GetSFXMute", &GamerSettings::GetSFXMute);
            lua_tinker::class_def<GamerSettings>(L, "SetSFXMute", &GamerSettings::SetSFXMute);
            lua_tinker::class_def<GamerSettings>(L, "GetLastViewPuzzlePage", &GamerSettings::GetLastViewPuzzlePage);
            lua_tinker::class_def<GamerSettings>(L, "SetLastViewPuzzlePage", &GamerSettings::SetLastViewPuzzlePage);
            lua_tinker::class_def<GamerSettings>(L, "Get", &GamerSettings::Get);
            lua_tinker::class_def<GamerSettings>(L, "Set", &GamerSettings::Set);

        lua_tinker::class_add<GamerSavedata>(L, "GamerSavedata");
            lua_tinker::class_inh<GamerSavedata, GameModule>(L);
            lua_tinker::class_def<GamerSavedata>(L, "Save", &GamerSavedata::Save);
            lua_tinker::class_def<GamerSavedata>(L, "Unlock", &GamerSavedata::Unlock);
            lua_tinker::class_def<GamerSavedata>(L, "Reset", &GamerSavedata::Reset);
            lua_tinker::class_def<GamerSavedata>(L, "IsUnlocked", &GamerSavedata::IsUnlocked);
            lua_tinker::class_def<GamerSavedata>(L, "GetMoaSolvedCount", &GamerSavedata::GetMoaSolvedCount);
            lua_tinker::class_def<GamerSavedata>(L, "GetMapleSolvedCount", &GamerSavedata::GetMapleSolvedCount);

        lua_tinker::class_add<PatternManager>(L, "PatternManager");
            lua_tinker::class_inh<PatternManager, GameModule>(L);
    }

    int Lua::Settings()
    {
        if (gamerSettings)
            lua_tinker::push(GetState(), gamerSettings);
        else
            lua_pushnil(GetState());

        return 1;
    }

    int Lua::Savedata()
    {
        if (gamerSavedata)
            lua_tinker::push(GetState(), gamerSavedata);
        else
            lua_pushnil(GetState());

        return 1;
    }

    int Lua::PatternMan()
    {
        if (patternManager)
            lua_tinker::push(GetState(), patternManager);
        else
            lua_pushnil(GetState());

        return 1;
    }

    int Lua::Version()
    {
        lua_pushstring(GetState() , Version::ToString().CStr());
        return 1;
    }

    //int Lua::CreateVN()
    //{
    //    //if (game && game->GetModules())
    //    //    lua_tinker::push(GetState(), new UIVisualNovel(game->GetModules()));
    //    //else
    //    //    lua_pushnil(GetState());
    //    return 1;
    //}

    int Lua::CreatePuzzleSelect()
    {
        if (game && game->GetModules())
            lua_tinker::push(GetState(), new UIPuzzleSelect(game->GetModules()));
        else
            lua_pushnil(GetState());
        return 1;
    }

    int Lua::CreateWorld()
    {
        if (game && game->GetModules())
        {
            UIGameLogic* world = nullptr;

            world = new UIGameLogic(game->GetModules());

            //world->SetSession(session);
            world->SetTimeline(game->GetAlarmClock()->GetTimeline());
            lua_tinker::push(GetState(), world);
        }
        else
            lua_pushnil(GetState());

        return 1;
    }

    int Lua::DestroyWorld()
    {
        if (game && game->GetModules())
        {
            UIGameLogicPtr world = lua_tinker::read<UIGameLogic*>(GetState(), 1);
            if (world->GetEventMap())
                world->GetEventMap()->RaiseEvent(UIEventID("DESTROY"), UIEventArgs(world));
            world->SetTimeline(nullptr);
            GC();
        }

        return 0;
    }
}
