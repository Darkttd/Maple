#pragma once

const int ArSize = 20;

enum CellState
{
    Black,
    White,
    Red,
    Yellow,
};

// ������ true ����
// ���н� false ���� ��, �μ��� ���޵� cellState ����
// ������ �Ǵ� ���� Red �� ǥ��
bool Check(CellState cellState[ArSize][ArSize], int size);

