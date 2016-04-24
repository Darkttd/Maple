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
		// 여러 패턴의 퍼즐들을 관리하는 클래스
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
                    int targetCharacter; // 0 = 모아, 1 = 단풍
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

			//std::vector<PuzzlePattern> GetPuzzlePatternOnPage(int character, int page) const; // page 는 0부터 시작합니다.
			PuzzleInformationCollection GetPuzzleInformationOnPage(int character, int page) const; // page 는 0부터 시작합니다.
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
