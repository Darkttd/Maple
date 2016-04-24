#include "Logic.h"
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

// 변화가 있으면 리턴값은 true
// 변화가 없으면 리턴값은 false
// 변화가 있는 경우 line 을 통해 결과물 출력
bool LineAction(CellState line[ArSize], const int size, const vector<int>& counter)
{
    // 라인 하나당 액션

    if(counter.size() == 0)
    {
        // counter의 원소가 없을 때의 예외 코드

        // counter의 원소가 없으므로 현재 줄의 모든 칸은 White 로 고정되어야 한다
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

    // 이미 해결된 경우 패스
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

    // tester 는 0으로 초기화
    // 테스트하는 경우는, 기존의 line 에서 확정된 칸과 맞지 않는 경우를 제외한 모든 경우
    // 테스트하는 경우에 대해서, Black 인 경우 +1, White 인 경우 -1 을 시행
    // 경우의 수가 n 이면 +n 이거나 -n 이면 그 색으로 확정
    // 확정된 경우가 있는 경우 true 리턴 및 line 에 변화된 대로 데이터 변경

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

        int currentOffset = 0; // 이것은, 색칠된 것과 기본 빈칸(1칸)에 의한 오프셋

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

        // 원본과 대조. 불일치 존재시 무효로 넘긴다
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

            // 일치시 tester 에 패턴 추가
            // Black은 +1
            // White는 -1
            
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

    // +caseCount 와 -caseCount 가 존재하는지 체크
    // 존재하면, 변경 후 true 리턴
    // 단, 본래 확정되어있던 경우는 패스
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

    // Red 가 감지되면, Black 으로 치환
    // Yellow 가 감지되면, White 로 치환
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

    // 초기화 이후 시작.
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            testState[i][j] = Red;
        }
    }

    // Black 이나 White 면 답이 확정
    // Red 이면 확정되지 않은 칸이다

    // 아래의 holizontalCounter 와 verticalCounter 는 각각
    // "문제" 에 해당하는 가로와 세로 숫자 배열
    // 단, 모든 칸이 비어있을 경우 vector 에는 0도 들어있지 않다.
    // 즉, {0} 이 아니라 {} 이다
    vector<int> holizontalCounter[ArSize];
    vector<int> verticalCounter[ArSize];

    // holizontalCounter, verticalCounter 값 채우기
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
        // Red 갯수 카운트
        // 카운트 후, Red 가 1 이상이면 false 리턴, 없으면 0 리턴
        // Red 가 1 이상인 경우, 해당하는 Red 중 원본이 Black 인 쪽은 원본에 덮어씌우고, 원본이 White 인 쪽은 Yellow 로 쓴다

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