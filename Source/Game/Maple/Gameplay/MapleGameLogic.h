#pragma once

#include <Maple/FWD.h>
#include <Bibim/Point2.h>
#include <Bibim/IUpdateable.h>
#include <vector>

namespace Maple
{
    class MapleGameLogic : public Bibim::IUpdateable
    {
        // Define Inner Game Logkc

        public:

            enum
            {
                MaxPanelSize = 20,
                DefaultLife = 5,
            };

            struct TableState
            {
                enum T
                {
                    Uncolored, // Default State. (Not Filled any marker)
                    Erased, // Deleted by User. (Black X marker)
                    ForceErased, //  Deleted by Incorrect Answer. Cannot convert to Uncolored State. (Red X marker)
                    Colored, // Colored by User. Because this is correct Answer, Cannot convert other State. (Black filled marker)

                    // Answer Pattern only uses Uncolored and Colored state
                };
            };

            struct TryFillColorResult
            {
                enum T
                {
                    NonChanged, // Not to change
                    Correct, // Correct Case
                    Incorrect, // Incorrect Case
                    Clear, // Cleared Game
                    GameOver, // Game Over
                };
            };

            struct GamePanel
            {
                TableState::T arr[MaxPanelSize][MaxPanelSize];

                GamePanel()
                {
                    for(int i = 0; i < MaxPanelSize; i++)
                    {
                        for(int j = 0; j < MaxPanelSize; j++)
                        {
                            arr[i][j] = TableState::Uncolored;
                        }
                    }
                }
            };

        public:
            MapleGameLogic();
            MapleGameLogic(int size);
            MapleGameLogic(int size, const GamePanel& ansPattern);

            void Initialize(int size, const GamePanel& ansPattern);
            void Restart();
            void Finalize();

            void Update(float dt, int timestamp);

            const GamePanel& GetAnsPanel() const;
            void SetAnsPanel(const GamePanel& value);

            const GamePanel& GetSolvePanel() const;
            void SetSolvePanel(const GamePanel& value);

            int GetRestLife() const;
            //void SetRestLife(int value);

            TryFillColorResult::T TryFillColor(); // Pressed O Button
            void TryFillErased(); // Pressed X Button

            int GetPuzzleSize() const;

            std::vector<std::vector<int> > GetHorizontalNumberCount() const;
            std::vector<std::vector<int> > GetVerticalNumberCount() const;

            Bibim::Point2 GetCursor() const;
            void SetCursor(const Bibim::Point2& value);

            Bibim::Point2 GetCursorTail() const;
            void SetCursorTail(const Bibim::Point2& end); // Warning. This function change cursorTail and also overwrite cursor
            void CursorMove(const Bibim::Point2& dPoint);

            //void MoveCursor(const Bibim::Point2& direction);

        private:
            bool WinCheck(); // Check for Win Game
            void Incorrect(); // Call when Incorrect

        private:
            GamePanel ansPattern; // Answer Pattern
            GamePanel solvePattern; // Current Solving Pattern
            int restLife; // Rest Life
            int puzzleSize; // Size of Puzzle. Current Only (5, 10, 15, 20)
            float elapsedTime;
            Bibim::Point2 cursor; // Current Cursor which have focus
            Bibim::Point2 cursorTail; // When Select multiple block, other side of the point which have no focus
    };
}
