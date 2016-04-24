#include <Maple/Gameplay/MapleGameLogic.h>
#include <Bibim/Math.h>
#include <Bibim/Assert.h>
using namespace Bibim;

namespace Maple
{
    MapleGameLogic::MapleGameLogic()
        : restLife(DefaultLife)
    {

    }

    MapleGameLogic::MapleGameLogic(int size)
        : puzzleSize(size),
          restLife(DefaultLife)
    {

    }

    MapleGameLogic::MapleGameLogic(int size, const GamePanel& ansPattern)
    {
        Initialize(size, ansPattern);
    }

    void MapleGameLogic::Initialize(int size, const GamePanel& ansPattern)
    {
        puzzleSize = size;
        restLife = DefaultLife;

        this->ansPattern = ansPattern;
    }

    void MapleGameLogic::Restart()
    {
        for(int i = 0; i < puzzleSize; i++)
        {
            for(int j = 0; j < puzzleSize; j++)
            {
                solvePattern.arr[i][j] = TableState::Uncolored;
            }
        }

        restLife = DefaultLife;
    }

    void MapleGameLogic::Update(float /*dt*/, int /*timestamp*/)
    {
        // <TODO> �ð� ���� ������ �� ��� ����� ��
    }

    const MapleGameLogic::GamePanel& MapleGameLogic::GetSolvePanel() const
    {
        return solvePattern;
    }

    int MapleGameLogic::GetPuzzleSize() const
    {
        return puzzleSize;
    }

    int MapleGameLogic::GetRestLife() const
    {
        return restLife;
    }

    MapleGameLogic::TryFillColorResult::T MapleGameLogic::TryFillColor()
    {
        BBAssert(cursor.X == cursorTail.X || cursor.Y == cursorTail.Y);

        // dic�� cursorTail���� cursor�� ���ϴ� ����
        Point2 dic = cursor - cursorTail;

        TryFillColorResult::T result = TryFillColorResult::NonChanged;

        if(dic == Point2::Zero)
        {
            // �� ĭ�� ���õ� ���¿��� O ��ư�� ���� ���,
            // ��� X ǥ���� ���� ��쿡�� ����� ������ �����մϴ�.

            if(solvePattern.arr[cursor.X][cursor.Y] == TableState::Uncolored)
            {
                if(ansPattern.arr[cursor.X][cursor.Y] == TableState::Uncolored)
                {
                    // ������ �ƴ� ���, �������� ǥ���մϴ�

                    solvePattern.arr[cursor.X][cursor.Y] = TableState::ForceErased;

                    Incorrect();
                    result = TryFillColorResult::Incorrect;
                }
                else
                {
                    // ������ ��� ��ĥ�մϴ�.

                    solvePattern.arr[cursor.X][cursor.Y] = TableState::Colored;
                    result = TryFillColorResult::Correct;
                }
            }
            else if(solvePattern.arr[cursor.X][cursor.Y] == TableState::Erased)
            {
                // X ǥ�� �Ǿ��ִ� ���, X ǥ���� ����ϴ�.

                solvePattern.arr[cursor.X][cursor.Y] = TableState::Uncolored;

                result = TryFillColorResult::NonChanged;
            }
        }
        else
        {
            if(dic.X != 0)
                dic.X /= Math::Abs(dic.X);
            else if(dic.Y != 0)
                dic.Y /= Math::Abs(dic.Y);

            result = TryFillColorResult::Correct;

            for(Point2 currentPoint = cursorTail; currentPoint != (cursor + dic); currentPoint += dic)
            {
                if(solvePattern.arr[currentPoint.X][currentPoint.Y] == TableState::Uncolored)
                {
                    // ��� �ƹ� ǥ���� ���� ���� �۵��մϴ�

                    if(ansPattern.arr[currentPoint.X][currentPoint.Y] == TableState::Uncolored)
                    {
                        // ������ �ƴ� ���, �������� ǥ���ϰ� ��ĥ�� �ߴ��մϴ�

                        solvePattern.arr[currentPoint.X][currentPoint.Y] = TableState::ForceErased;

                        Incorrect();
                        result = TryFillColorResult::Incorrect;
                        break;
                    }
                    else
                    {
                        // ������ ��� ��ĥ�մϴ�.

                        solvePattern.arr[currentPoint.X][currentPoint.Y] = TableState::Colored;
                    }
                }
            }

            cursorTail = cursor;
        }

        if(GetRestLife() < 0)
        {
            result = TryFillColorResult::GameOver;
        }
        else if(WinCheck())
        {
            result = TryFillColorResult::Clear;
        }
        
        return result;
    }

    void MapleGameLogic::TryFillErased()
    {
        BBAssert(cursor.X == cursorTail.X || cursor.Y == cursorTail.Y);

        // dic�� cursorTail���� cursor�� ���ϴ� ����
        Point2 dic = cursor - cursorTail;

        if(dic.X != 0)
            dic.X /= Math::Abs(dic.X);
        else if(dic.Y != 0)
            dic.Y /= Math::Abs(dic.Y);
        else
            dic.Y = 1;

        bool eraseMode = true;

        if(solvePattern.arr[cursorTail.X][cursorTail.Y] == TableState::Erased || solvePattern.arr[cursorTail.X][cursorTail.Y] == TableState::ForceErased)
        {
            // �������� X �� ���, X �� ����� �������� �����մϴ�.

            eraseMode = false;
        }

        for(Point2 currentPoint = cursorTail; currentPoint != (cursor + dic); currentPoint += dic)
        {
            if(eraseMode && solvePattern.arr[currentPoint.X][currentPoint.Y] == TableState::Uncolored)
            {
                // �������� X�� �ƴϰ�, �ƹ� �͵� ���� ĭ�� ���, X ǥ�� �մϴ�.
                solvePattern.arr[currentPoint.X][currentPoint.Y] = TableState::Erased;
            }
            else if(!eraseMode && solvePattern.arr[currentPoint.X][currentPoint.Y] == TableState::Erased)
            {
                // �������� X�̰�, X�� ĥ���� �ִ� ĭ�̸�, X�� ����ϴ�.
                solvePattern.arr[currentPoint.X][currentPoint.Y] = TableState::Uncolored;
            }
        }

        cursorTail = cursor;
    }

    std::vector<std::vector<int> > MapleGameLogic::GetHorizontalNumberCount() const
    {
        std::vector<std::vector<int> > pattern;

        // <TODO> Draw() �Լ��� ����ð��� �ٿ��� �Ѵٸ�, pattern �� �ѹ��� ����ϵ��� �����ؾ� �մϴ�.

        for(int i = 0; i < puzzleSize; i++)
        {
            std::vector<int> current;

            int j = puzzleSize - 1;
            int counter = 0;

            while(j >= 0)
            {
                if(ansPattern.arr[i][j] == TableState::Colored)
                    counter++;
                else
                {
                    if(counter > 0)
                    {
                        current.push_back(counter);
                        counter = 0;
                    }
                }

                j--;
            }

            if(counter > 0)
                current.push_back(counter);

            if(current.size() == 0)
                current.push_back(0);

            pattern.push_back(current);
        }

        return pattern;
    }

    std::vector<std::vector<int> > MapleGameLogic::GetVerticalNumberCount() const
    {
        std::vector<std::vector<int> > pattern;

        // <TODO> Draw() �Լ��� ����ð��� �ٿ��� �Ѵٸ�, pattern �� �ѹ��� ����ϵ��� �����ؾ� �մϴ�.

        for(int i = 0; i < puzzleSize; i++)
        {
            std::vector<int> current;

            int j = 0;
            int counter = 0;
            
            while(j < puzzleSize)
            {
                if(ansPattern.arr[j][i] == TableState::Colored)
                    counter++;
                else
                {
                    if(counter > 0)
                    {
                        current.push_back(counter);
                        counter = 0;
                    }
                }

                j++;
            }

            if(counter > 0)
                current.push_back(counter);

            if(current.size() == 0)
                current.push_back(0);

            pattern.push_back(current);
        }

        return pattern;
    }

    Point2 MapleGameLogic::GetCursor() const
    {
        return cursor;
    }

    void MapleGameLogic::SetCursor(const Bibim::Point2& value)
    {
        cursor.X = Math::Clamp(value.X, 0, puzzleSize - 1);
        cursor.Y = Math::Clamp(value.Y, 0, puzzleSize - 1);
    }

    Point2 MapleGameLogic::GetCursorTail() const
    {
        return cursorTail;
    }

    void MapleGameLogic::SetCursorTail(const Bibim::Point2& end)
    {
        Point2 clamped;
        clamped.X = Math::Clamp(end.X, 0, puzzleSize - 1);
        clamped.Y = Math::Clamp(end.Y, 0, puzzleSize - 1);

        cursorTail = clamped;
        cursor = clamped;
    }

    void MapleGameLogic::CursorMove(const Bibim::Point2& dPoint)
    {
        if(Math::Abs(dPoint.X) + Math::Abs(dPoint.Y) != 1)
            return;

        SetCursorTail(cursor + dPoint);
    }

    bool MapleGameLogic::WinCheck()
    {
        // �̰���� üũ�ϴ� �����Դϴ�.
        // üũ�� ĭ�� ��� ��ġ�ϸ� ����Դϴ�.

        bool result = true;

        for(int i = 0; i < puzzleSize; i++)
        {
            for(int j = 0; j < puzzleSize; j++)
            {
                if(ansPattern.arr[i][j] == TableState::Colored && solvePattern.arr[i][j] != TableState::Colored)
                {
                    result = false;
                    break;
                }
            }

            if(!result)
                break;
        }

        return result;
    }

    void MapleGameLogic::Incorrect()
    {
        // �������� ����ϴ�.

        // ���� �������� UIGameLogic �ʿ��� ������ ����մϴ�.

        restLife--;
    }
}
