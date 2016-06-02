#pragma once

#include <Maple/FWD.h>
#include <Maple/Gameplay/MapleGameLogic.h>
#include <Bibim/StandardGame.h>
#include <Bibim/GameModule.h>
#include <vector>

namespace Maple
{
    class PatternManager : public Bibim::GameModule
    {
        // Manager of Patterns
        BBThisIsNoncopyableClass(PatternManager);
        BBModuleClass(PatternManager, GameModule, 'P', 'T', 'M', 'G');

        public:
            struct PuzzleInformation;

            typedef std::vector<std::vector<MapleGameLogic::TableState::T> > PuzzlePattern;
            typedef std::vector<PuzzleInformation> PuzzleInformationCollection;

            struct PuzzleInformation
            {
                public:
                    PuzzleInformation();
                    ~PuzzleInformation();
                    void SetCharacter(String characterName);

                public:
                    int index;
                    int puzzleSize;
                    PuzzlePattern puzzlePattern;
                    int targetCharacter; // 0 = Moa, 1 = Maple
                    bool isSolved;
                    bool isUnlocked;
            };

        public:
            PatternManager();
            ~PatternManager();

            void AddPuzzleInformation(const PuzzleInformation& value);

            int GetPuzzlePerPage() const;
            void SetPuzzlePerPage(int value);

            int GetTotalPuzzelCount() const;
            int GetTotalPageCount() const;

            PuzzleInformationCollection GetPuzzleInformationOnPage(int character, int page) const; // page starts 0
            const PuzzleInformation& GetPuzzleInformationOnNumber(int character, int index) const;

            void SetSpecificPuzzleSolved(int index, bool solved);
            void SetSpecificPuzzleUnlocked(int index, bool unlocked);

            void Init(Bibim::StandardGame::LuaBase* lua);

        private:
            bool isInit;
            PuzzleInformationCollection puzzleInformationCollection;
            int puzzlePerPage;
    };
}
