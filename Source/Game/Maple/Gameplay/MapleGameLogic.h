#pragma once

#include <Maple/FWD.h>
#include <Bibim/Point2.h>
#include <Bibim/IUpdateable.h>
#include <vector>

namespace Maple
{
    class MapleGameLogic : public Bibim::IUpdateable
    {
        // "네모로직" 의 내부 로직을 정의합니다.

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
                    Uncolored, // 기본 상태. (칠해지지 않은 최초의 상태)
                    Erased, // 유저에 의해 지워진 상태
                    ForceErased, // 오기로 인해 강제로 지워진 상태. 수동으로 기본 상태로 돌릴 수 없음
                    Colored, // 유저에 의해 색칠된 상태. 정답임이 확인된 상태이므로, 다른 상태가 될 수 없음

                    // 정답 패턴의 경우, Uncolored 와 Colored 만 사용합니다
                };
            };

            struct TryFillColorResult
            {
                enum T
                {
                    NonChanged, // 변경이 없을 경우.
                    Correct, // 맞은 경우.
                    Incorrect, // 틀린 경우.
                    Clear, // 클리어한 경우.
                    GameOver, // 게임 오버한 경우.
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

            TryFillColorResult::T TryFillColor(); // O 버튼을 눌렀을 경우
            void TryFillErased(); // X 버튼을 눌렀을 경우

            int GetPuzzleSize() const;

            std::vector<std::vector<int> > GetHorizontalNumberCount() const;
            std::vector<std::vector<int> > GetVerticalNumberCount() const;

            Bibim::Point2 GetCursor() const;
            void SetCursor(const Bibim::Point2& value);

            Bibim::Point2 GetCursorTail() const;
            void SetCursorTail(const Bibim::Point2& end); // 주의, 이 함수는, cursorTail뿐만 아니라 cursor에도 덮어씌웁니다
            void CursorMove(const Bibim::Point2& dPoint);

            //void MoveCursor(const Bibim::Point2& direction);

        private:
            bool WinCheck(); // 이겼는지 체크
            void Incorrect(); // 오답일 경우 호출

        private:
            GamePanel ansPattern; // 정답 패턴
            GamePanel solvePattern; // 현재 시도중인 패턴
            int restLife; // 남은 Life
            int puzzleSize; // 퍼즐의 사이즈 (5, 10, 15, 20)
            float elapsedTime; // 게임이 시작되고 경과한 시간
            Bibim::Point2 cursor; // 현재 포커스를 가지는 커서
            Bibim::Point2 cursorTail; // 여러 블록을 선택했 을 때, 커서를 가지지 않는 반대쪽 끝 포인트
    };
}
