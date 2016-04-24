#pragma once
#ifndef __TH_GAMERSAVEDATA_H__
#define __TH_GAMERSAVEDATA_H__

#   include <Maple/FWD.h>
#   include <Bibim/GameModule.h>
#   include <set>

    namespace Maple
    {
        class GamerSavedata : public Bibim::GameModule
        {
            BBThisIsNoncopyableClass(GamerSavedata);
            BBModuleClass(GamerSavedata, GameModule, 'G', 'S', 'A', 'V');

            static const int CurrentSaveDataVersion = 1;

            public:
                GamerSavedata();
                virtual ~GamerSavedata();

                void Save();
                void Unlock(const String& name);
                bool IsUnlocked(const String& name) const;
                void Reset();

                int GetMoaSolvedCount() const;
                int GetMapleSolvedCount() const;

            private:
                void Load();

            private:
                void InitialUnlockedItem();

            private:
                std::set<String> items;
        };
    }

#   include <Maple/GamerSavedata.inl>

#endif