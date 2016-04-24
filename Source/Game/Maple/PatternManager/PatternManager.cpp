#include <Maple/PatternManager/PatternManager.h>
#include <Bibim/Assert.h>
using namespace Bibim;
using namespace std;

namespace Maple
{
    ///////////////////////////////////////////////////////////////////////
    PatternManager::PuzzleInformation::PuzzleInformation()
        : index(-1),
          puzzleSize(-1),
          targetCharacter(-1),
          isSolved(false),
          isUnlocked(false)
    {
    }

    PatternManager::PuzzleInformation::~PuzzleInformation()
    {
    }

    void PatternManager::PuzzleInformation::SetCharacter(String characterName)
    {
        if(characterName == "Moa")
            targetCharacter = 0;
        else if(characterName == "Maple")
            targetCharacter = 1;
        else
            BBAssert(false);
    }

    ///////////////////////////////////////////////////////////////////////

	PatternManager::PatternManager()
		: puzzlePerPage(12)
          , isInit(false)
	{
	}

	PatternManager::~PatternManager()
	{
	}

	void PatternManager::AddPuzzleInformation( const PuzzleInformation& value )
	{
		puzzleInformationCollection.push_back(value);
	}

	int PatternManager::GetPuzzlePerPage() const
	{
		return puzzlePerPage;
	}

	void PatternManager::SetPuzzlePerPage( int value )
	{
		puzzlePerPage = value;
	}

	int PatternManager::GetTotalPuzzelCount() const
	{
		return puzzleInformationCollection.size();
	}

	int PatternManager::GetTotalPageCount() const
	{
		BBAssert(puzzlePerPage > 0);

		return (puzzleInformationCollection.size() + puzzlePerPage - 1) / puzzlePerPage;
	}

	//std::vector<PatternManager::PuzzlePattern> PatternManager::GetPuzzlePatternOnPage( int character, int page ) const
	//{
	//	std::vector<PuzzlePattern> puzzlePattern;

	//	// <TODO> character 에 따라 분류하는 로직을 추가해야 합니다.

	//	int offset = page * puzzlePerPage;

	//	for(int i = 0; i < puzzlePerPage; i++)
	//	{
	//		if(offset + i >= static_cast<int>(puzzleInformationCollection.size()))
	//			break;

	//		puzzlePattern.push_back(puzzleInformationCollection[i].puzzlePattern);
	//	}

	//	return puzzlePattern;
	//}

	PatternManager::PuzzleInformationCollection PatternManager::GetPuzzleInformationOnPage( int character, int page ) const
	{
		PuzzleInformationCollection puzzleInformation;

		// <TODO> character 에 따라 분류하는 로직을 추가해야 합니다.

		int offset = page * puzzlePerPage;

		for(int i = 0; i < puzzlePerPage; i++)
		{
			if(offset + i >= static_cast<int>(puzzleInformationCollection.size()))
				break;

			puzzleInformation.push_back(puzzleInformationCollection[offset + i]);
		}

		return puzzleInformation;
	}

    const PatternManager::PuzzleInformation& PatternManager::GetPuzzleInformationOnNumber(int character, int index) const
    {
		// <TODO> character 에 따라 분류하는 로직을 추가해야 합니다.

        BBAssert(index < puzzleInformationCollection.size());

		return puzzleInformationCollection[index];
    }

	void PatternManager::SetSpecificPuzzleSolved( int index, bool solved )
	{
		BBAssert(index < static_cast<int>(puzzleInformationCollection.size()));

		puzzleInformationCollection[index].isSolved = solved;
	}

	void PatternManager::SetSpecificPuzzleUnlocked( int index, bool unlocked )
	{
		BBAssert(index < static_cast<int>(puzzleInformationCollection.size()));

		puzzleInformationCollection[index].isUnlocked = unlocked;
	}

    void PatternManager::Init(StandardGame::LuaBase* lua)
    {
        // 이미 로드된 lua 로부터 수동으로 데이터를 읽어오도록 합니다.

        lua_State* state = lua->GetState();

        //lua_getglobal(state, "PUZZLES");

        lua_tinker::table puzzles = lua_tinker::get<lua_tinker::table>(state, "PUZZLES");
        const int numberOfPuzzles = puzzles.len();
        for(int i = 1; i <= numberOfPuzzles; i++)
        {
            PuzzleInformation info;
            PuzzleInformation xyInverter;
            info.index = i;
            xyInverter.index = i;

            lua_tinker::table puzzle = puzzles.get<lua_tinker::table>(i);

            //info.SetCharacter(puzzle.get<const char*>("TargetCharacter"));
            //xyInverter.SetCharacter(puzzle.get<const char*>("TargetCharacter"));

            lua_tinker::table pattern = puzzle.get<lua_tinker::table>("Pattern");
            int puzzleSize = pattern.len();

            info.puzzleSize = puzzleSize;
            xyInverter.puzzleSize = puzzleSize;

            for(int j = 1; j <= puzzleSize; j++)
            {
                const char* p = pattern.get<const char*>(j);

                vector<MapleGameLogic::TableState::T> line;
                BBAssert(puzzleSize == static_cast<int>(strlen(p)));

                for(int k = 0; k < puzzleSize; k++)
                {
                    MapleGameLogic::TableState::T d;

                    if(p[k] == '0')
                        d = MapleGameLogic::TableState::Uncolored;
                    else
                        d = MapleGameLogic::TableState::Colored;

                    line.push_back(d);
                }

                info.puzzlePattern.push_back(line);
            }

            for(int j = 0; j < puzzleSize; j++)
            {
                vector<MapleGameLogic::TableState::T> line;

                for(int k = 0; k < puzzleSize; k++)
                {
                    line.push_back(info.puzzlePattern[k][j]);
                }

                xyInverter.puzzlePattern.push_back(line);
            }

            puzzleInformationCollection.push_back(xyInverter);
        }
    }

}
