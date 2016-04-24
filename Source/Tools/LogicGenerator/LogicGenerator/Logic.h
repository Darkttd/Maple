#pragma once

const int ArSize = 20;

enum CellState
{
    Black,
    White,
    Red,
    Yellow,
};

// 성공시 true 리턴
// 실패시 false 리턴 및, 인수로 전달된 cellState 에서
// 문제가 되는 셀은 Red 로 표기
bool Check(CellState cellState[ArSize][ArSize], int size);

