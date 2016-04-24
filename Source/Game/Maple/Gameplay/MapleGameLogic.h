#pragma once

#include <Maple/FWD.h>
#include <Bibim/Point2.h>
#include <Bibim/IUpdateable.h>
#include <vector>

namespace Maple
{
    class MapleGameLogic : public Bibim::IUpdateable
    {
        // "�׸����" �� ���� ������ �����մϴ�.

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
                    Uncolored, // �⺻ ����. (ĥ������ ���� ������ ����)
                    Erased, // ������ ���� ������ ����
                    ForceErased, // ����� ���� ������ ������ ����. �������� �⺻ ���·� ���� �� ����
                    Colored, // ������ ���� ��ĥ�� ����. �������� Ȯ�ε� �����̹Ƿ�, �ٸ� ���°� �� �� ����

                    // ���� ������ ���, Uncolored �� Colored �� ����մϴ�
                };
            };

            struct TryFillColorResult
            {
                enum T
                {
                    NonChanged, // ������ ���� ���.
                    Correct, // ���� ���.
                    Incorrect, // Ʋ�� ���.
                    Clear, // Ŭ������ ���.
                    GameOver, // ���� ������ ���.
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

            TryFillColorResult::T TryFillColor(); // O ��ư�� ������ ���
            void TryFillErased(); // X ��ư�� ������ ���

            int GetPuzzleSize() const;

            std::vector<std::vector<int> > GetHorizontalNumberCount() const;
            std::vector<std::vector<int> > GetVerticalNumberCount() const;

            Bibim::Point2 GetCursor() const;
            void SetCursor(const Bibim::Point2& value);

            Bibim::Point2 GetCursorTail() const;
            void SetCursorTail(const Bibim::Point2& end); // ����, �� �Լ���, cursorTail�Ӹ� �ƴ϶� cursor���� �����ϴ�
            void CursorMove(const Bibim::Point2& dPoint);

            //void MoveCursor(const Bibim::Point2& direction);

        private:
            bool WinCheck(); // �̰���� üũ
            void Incorrect(); // ������ ��� ȣ��

        private:
            GamePanel ansPattern; // ���� ����
            GamePanel solvePattern; // ���� �õ����� ����
            int restLife; // ���� Life
            int puzzleSize; // ������ ������ (5, 10, 15, 20)
            float elapsedTime; // ������ ���۵ǰ� ����� �ð�
            Bibim::Point2 cursor; // ���� ��Ŀ���� ������ Ŀ��
            Bibim::Point2 cursorTail; // ���� ����� ������ �� ��, Ŀ���� ������ �ʴ� �ݴ��� �� ����Ʈ
    };
}
