#pragma once
#include "Stack.h"

enum PlayerColor { RED = 'r', BLUE = 'b' };

struct Coordinate {
    int x;
    int y;
};

struct Board {
    Stack longestConnect;
    Stack symbol;

    vector<Coordinate> winP;

    bool visited[11][11] = { false };

    bool isGameOverCommand = false;
    bool isBoardPossibleCommand = false;
    bool isPossible = false;

    bool firstCheck = false;

    int countBlueLines = 0;
    int countRedLines = 0;

    int sizeBoard = 0;
    int prev_sizeBoard = 0;
    int red_pawns_count = 0;
    int blue_pawns_count = 0;
    int row_index = 0;
    int col_index = 0;
};

struct NeiboursCoordinates {
    int dx[6] = { -1, -1, 0, 0, 1, 1 };
    int dy[6] = { -1, 0, -1, 1, 0, 1 };
};

Board boardParameters;
NeiboursCoordinates neiborsCoordinates;