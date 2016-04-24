#include "Logic.h"
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

// ��ȭ�� ������ ���ϰ��� true
// ��ȭ�� ������ ���ϰ��� false
// ��ȭ�� �ִ� ��� line �� ���� ����� ���
bool LineAction(CellState line[ArSize], const int size, const vector<int>& counter)
{
    // ���� �ϳ��� �׼�

    if(counter.size() == 0)
    {
        // counter�� ���Ұ� ���� ���� ���� �ڵ�

        // counter�� ���Ұ� �����Ƿ� ���� ���� ��� ĭ�� White �� �����Ǿ�� �Ѵ�
        bool changed = false;
        
        for(int i = 0; i < size; i++)
        {
            if(line[i] != White)
            {
                line[i] = White;
                changed = true;
            }
        }

        return changed;
    }

    // �̹� �ذ�� ��� �н�
    {
        bool isSolved = true;

        for(int i = 0; i < size; i++)
        {
            if(line[i] == Red)
                isSolved = false;
        }

        if(isSolved)
            return false;
    }

    int tester[ArSize];

    for(int i = 0; i < size; i++)
        tester[i] = 0;

    // tester �� 0���� �ʱ�ȭ
    // �׽�Ʈ�ϴ� ����, ������ line ���� Ȯ���� ĭ�� ���� �ʴ� ��츦 ������ ��� ���
    // �׽�Ʈ�ϴ� ��쿡 ���ؼ�, Black �� ��� +1, White �� ��� -1 �� ����
    // ����� ���� n �̸� +n �̰ų� -n �̸� �� ������ Ȯ��
    // Ȯ���� ��찡 �ִ� ��� true ���� �� line �� ��ȭ�� ��� ������ ����

    int sum = 0;
    
    sum = accumulate(counter.begin(), counter.end(), 0);

    int restLimit = size - sum - (counter.size() - 1);

    vector<int> offsets;
    
    for(int i = 0; i < static_cast<int>(counter.size()); i++)
        offsets.push_back(0);

    (*offsets.rbegin())--;

    int caseCount = 0;

    do
    {
        (*offsets.rbegin())++;
        if(*offsets.rbegin() > restLimit)
        {
            auto rit = offsets.rbegin();
            rit++;
            
            while(true)
            {
                (*rit)++;
                if((*rit) > restLimit)
                {
                    rit++;
                }
                else
                    break;
            }

            int initValue = *rit;
            auto it = rit.base();

            for(; it != offsets.end(); it++)
                *it = initValue;
        }

        CellState current[ArSize];
        for(int i = 0; i < size; i++)
            current[i] = White;

        int currentOffset = 0; // �̰���, ��ĥ�� �Ͱ� �⺻ ��ĭ(1ĭ)�� ���� ������

        for(int i = 0; i < static_cast<int>(offsets.size()); i++)
        {
            int startPoint = currentOffset + offsets[i];
            int endPoint = currentOffset + offsets[i] + counter[i];

            for(int p = startPoint; p < endPoint; p++)
            {
                current[p] = Black;
            }

            currentOffset += counter[i] + 1;
        }

        bool validCase = true;

        // ������ ����. ����ġ ����� ��ȿ�� �ѱ��
        for(int i = 0; i < size; i++)
        {
            if(line[i] != Red)
            {
                if(line[i] != current[i])
                    validCase = false;
            }
        }

        if(validCase)
        {
            caseCount++;

            // ��ġ�� tester �� ���� �߰�
            // Black�� +1
            // White�� -1
            
            for(int i = 0; i < size; i++)
            {
                switch(current[i])
                {
                    case Black:
                        tester[i] += 1;
                        break;

                    case White:
                        tester[i] -= 1;
                        break;
                }
            }
        }

    }while(offsets[0] != restLimit);

    // +caseCount �� -caseCount �� �����ϴ��� üũ
    // �����ϸ�, ���� �� true ����
    // ��, ���� Ȯ���Ǿ��ִ� ���� �н�
    bool isUpdated = false;

    for(int i = 0; i < size; i++)
    {
        if(tester[i] == caseCount)
        {
            if(line[i] == Red)
            {
                isUpdated = true;
                line[i] = Black;
            }
        }

        if(tester[i] == -caseCount)
        {
            if(line[i] == Red)
            {
                isUpdated = true;
                line[i] = White;
            }
        }
    }

    return isUpdated;
}


bool Check(CellState cellState[ArSize][ArSize], const int size)
{
    CellState testState[ArSize][ArSize];

    // Red �� �����Ǹ�, Black ���� ġȯ
    // Yellow �� �����Ǹ�, White �� ġȯ
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            if(cellState[i][j] == Red)
                cellState[i][j] = Black;

            if(cellState[i][j] == Yellow)
                cellState[i][j] = White;
        }
    }

    // �ʱ�ȭ ���� ����.
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            testState[i][j] = Red;
        }
    }

    // Black �̳� White �� ���� Ȯ��
    // Red �̸� Ȯ������ ���� ĭ�̴�

    // �Ʒ��� holizontalCounter �� verticalCounter �� ����
    // "����" �� �ش��ϴ� ���ο� ���� ���� �迭
    // ��, ��� ĭ�� ������� ��� vector ���� 0�� ������� �ʴ�.
    // ��, {0} �� �ƴ϶� {} �̴�
    vector<int> holizontalCounter[ArSize];
    vector<int> verticalCounter[ArSize];

    // holizontalCounter, verticalCounter �� ä���
    for(int i = 0; i < size; i++)
    {
        // holizontal
        int counter = 0;

        for(int j = 0; j < size; j++)
        {
            if(cellState[j][i] == Black)
                counter++;
            else
            {
                if(counter > 0)
                {
                    holizontalCounter[i].push_back(counter);
                    counter = 0;
                }
            }
        }

        if(counter > 0)
        {
            holizontalCounter[i].push_back(counter);
            counter = 0;
        }
    }

    for(int i = 0; i < size; i++)
    {
        // vertical
        int counter = 0;

        for(int j = 0; j < size; j++)
        {
            if(cellState[i][j] == Black)
                counter++;
            else
            {
                if(counter > 0)
                {
                    verticalCounter[i].push_back(counter);
                    counter = 0;
                }
            }
        }

        if(counter > 0)
        {
            verticalCounter[i].push_back(counter);
            counter = 0;
        }
    }

    bool isChanged;

    do
    {
        isChanged = false;

        //holizontal
        for(int i = 0; i < size; i++)
        {
            CellState line[ArSize];

            for(int j = 0; j < size; j++)
            {
                line[j] = testState[j][i];
            }

            bool result = LineAction(line, size, holizontalCounter[i]);

            if(result)
            {
                isChanged = true;

                for(int j = 0; j < size; j++)
                {
                    testState[j][i] = line[j];
                }
            }
        }

        //vertical
        for(int i = 0; i < size; i++)
        {
            CellState line[ArSize];

            for(int j = 0; j < size; j++)
            {
                line[j] = testState[i][j];
            }

            bool result = LineAction(line, size, verticalCounter[i]);

            if(result)
            {
                isChanged = true;

                for(int j = 0; j < size; j++)
                {
                    testState[i][j] = line[j];
                }
            }
        }

    }while(isChanged);

    bool result = true;

    {
        // Red ���� ī��Ʈ
        // ī��Ʈ ��, Red �� 1 �̻��̸� false ����, ������ 0 ����
        // Red �� 1 �̻��� ���, �ش��ϴ� Red �� ������ Black �� ���� ������ ������, ������ White �� ���� Yellow �� ����

        int counter = 0;

        for(int i = 0; i < size; i++)
        {
            for(int j = 0; j < size; j++)
            {
                if(testState[i][j] == Red)
                    counter++;
            }
        }
        
        if(counter > 0)
        {
            result = false;

            for(int i = 0; i < size; i++)
            {
                for(int j = 0; j < size; j++)
                {
                    if(testState[i][j] == Red)
                    {
                        if(cellState[i][j] == Black)
                            cellState[i][j] = Red;
                        else if(cellState[i][j] == White)
                            cellState[i][j] = Yellow;
                    }
                }
            }
        }
    }

    return result;
}