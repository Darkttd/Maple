#pragma once

#include <Maple/FWD.h>
#include <Bibim/StandardGame.h>

namespace Maple
{
    class Lua : public Bibim::StandardGame::LuaBase
    {
        BBThisIsNoncopyableClass(Lua);
        BBModuleClass(Lua, Bibim::StandardGame::LuaBase, 'L', 'U', 'A', '_');
        public:
            Lua(Game* game);
            virtual ~Lua();

            inline Game* GetGame() const;
            inline GamerSettings* GetGamerSettings() const;
            void SetGamerSettings(GamerSettings* value);
            inline GamerSavedata* GetGamerSavedata() const;
            void SetGamerSavedata(GamerSavedata* value);
            inline PatternManager* GetPatternManager() const;
            void SetPatternManager(PatternManager* value);

        private:
            void LoadLibraries();
            void LoadClasses();

            int Settings();
            int Savedata();
            int PatternMan();
            int Version();
            int LongVersion();

            //int CreateVN();
            int CreatePuzzleSelect();
            int CreateWorld();
            int DestroyWorld();

        private:
            Game* game;
            GamerSettings* gamerSettings;
            GamerSavedata* gamerSavedata;
            PatternManager* patternManager;

            struct Internal;

            friend class ScriptCallback;
            friend class UIScriptEventHandler;
            friend class ScriptHttpCallback;
    };
}

#include <Maple/Lua.inl>
