#pragma once
#ifndef __TH_GAMERSETTINGS_H__
#define __TH_GAMERSETTINGS_H__

#   include <Maple/FWD.h>
#   include <Bibim/GameModule.h>
#   include <Bibim/Listeners.h>
#   include <map>

    namespace Maple
    {
        class GamerSettings : public Bibim::GameModule
        {
            BBThisIsNoncopyableClass(GamerSettings);
            BBModuleClass(GamerSettings, GameModule, 'G', 'S', 'E', 'T');
            public:
                class IListener
                {
                    public:
                        virtual ~IListener() { }
                        virtual void OnApply(GamerSettings* settings) = 0;
                };

            public:

                enum StandingCharacter
                {
                    Unselected,
                    Kradmoa,
                    Maple,
                };

                enum StandingCharacterCloth
                {
                    SchoolUniform,
                    Hanbok,
                    Gimono,
                    ChinaDress,
                    Nurse,
                    SwimSuits,
                    Maid,
                };

            public:
                GamerSettings();
                virtual ~GamerSettings();

                inline void AddListener(IListener* item);
                inline void AddListener(IListener* item, Bibim::SharedObject* object);
                inline bool RemoveListener(IListener* item);

                void Reset();
                void Apply();

                inline StandingCharacter GetStandingCharacter() const;
                void SetStandingCharacter(StandingCharacter value);

                inline StandingCharacterCloth GetMoaStandingCharacterCloth() const;
                void SetMoaStandingCharacterCloth(StandingCharacterCloth value);

                inline StandingCharacterCloth GetMapleStandingCharacterCloth() const;
                void SetMapleStandingCharacterCloth(StandingCharacterCloth value);

                inline bool GetBGMMute() const;
                void SetBGMMute(bool value);

                inline bool GetSFXMute() const;
                void SetSFXMute(bool value);

                inline int GetLastViewPuzzlePage() const;
                void SetLastViewPuzzlePage(int value);

                const char* Get(const char* key) const;
                void Set(const char* key, const char* value);

            public:
                static const char* ConvertFromStandingCharacterToChars(StandingCharacter value);
                static StandingCharacter ConvertFromCharsToStandingCharacter(const char* value);

                static const char* ConvertFromStandingCharacterClothToChars(StandingCharacterCloth value);
                static StandingCharacterCloth ConvertFromCharsToStandingCharacterCloth(const char* value);

            private:
                void Load();
                void Save();

                void SetDefaults();

            private:
                StandingCharacter standingCharacter;
                StandingCharacterCloth moaStandingCharacterCloth;
                StandingCharacterCloth mapleStandingCharacterCloth;
                bool bgmMute;
                bool sfxMute;
                int lastViewPuzzlePage;
                std::map<Bibim::String, Bibim::String> values;

                Bibim::Listeners<IListener> listeners;
        };
    }

#   include <Maple/GamerSettings.inl>

    BBBindLuaEnum(Maple::GamerSettings::StandingCharacter,
                  Maple::GamerSettings::ConvertFromCharsToStandingCharacter,
                  Maple::GamerSettings::ConvertFromStandingCharacterToChars);
    BBBindLuaEnum(Maple::GamerSettings::StandingCharacterCloth,
                  Maple::GamerSettings::ConvertFromCharsToStandingCharacterCloth,
                  Maple::GamerSettings::ConvertFromStandingCharacterClothToChars);

#endif