#include "Board.h"

int IS_BOARD_CORRECT() {
    bool IsCorrect = false;
    if (boardParameters.blue_pawns_count > boardParameters.red_pawns_count) {
        IsCorrect = false;
    }
    else if (boardParameters.red_pawns_count == boardParameters.blue_pawns_count) {
        IsCorrect = true;
    }
    else if (boardParameters.red_pawns_count > boardParameters.blue_pawns_count) {
        if (boardParameters.blue_pawns_count + 1 != boardParameters.red_pawns_count) {
            IsCorrect = false;
        }
        else {
            IsCorrect = true;
        }
    }
    return IsCorrect;
}


bool checkCorners(Stack& field_sizes, char square[][11], bool blue, bool& borderFirst, bool& borderSecond) {
    if ((!borderFirst || !borderSecond) && blue) {
        return false;
    }
    else if (borderFirst && borderSecond && blue) {
        return true;
    }
    else if ((!borderFirst || !borderSecond) && field_sizes.topValue() > 1 && !blue) {
        return false;
    }
    else if (!borderFirst && field_sizes.topValue() == 1 && !blue) {
        return false;
    }
    else {
        return true;
    }
}

bool checkBlueCorners(Stack& field_sizes, char square[][11]) {
    bool borderFirstLineB = false;
    bool borderLastLineB = false;
    for (int j = 0; j < field_sizes.topValue(); ++j) {
        for (int i = 0; i < field_sizes.topValue(); ++i) {
            if (square[j][i] == BLUE && j == 0) {
                borderFirstLineB = true;
                continue;
            }
            else if (square[j][i] == BLUE && j == field_sizes.topValue() - 1) {
                borderLastLineB = true;
                continue;
            }
        }
    }
    return checkCorners(field_sizes, square, true, borderFirstLineB, borderLastLineB);
}

bool checkRedCorners(Stack& field_sizes, char square[][11]) {
    bool borderFirstLineR = false;
    bool borderLastLineR = false;
    for (int j = 0; j < field_sizes.topValue(); ++j) {
        if (square[j][field_sizes.topValue() - 1] == RED) {
            borderFirstLineR = true;
        }
        if (square[j][0] == RED) {
            borderLastLineR = true;
            continue;
        }
    }
    return checkCorners(field_sizes, square, false, borderFirstLineR, borderLastLineR);
}

void createClearSquare(Stack& field_sizes, char square[][11]) {
    int size = field_sizes.topValue();
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            square[i][j] = ' ';
        }
    }
}

void clearBoard(const int& rows, const int& cols, char board[][11]) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            board[i][j] = ' ';
        }
    }
}

void clearCount() {
    boardParameters.blue_pawns_count = 0;
    boardParameters.red_pawns_count = 0;
    boardParameters.row_index = 0;
    boardParameters.col_index = 0;
}

void correctPlacingForSquare(int line, Stack& field_sizes, char square[][11], char board[][11], int& i, int& k, int first, int second) {
    int size = field_sizes.topValue();
    int result = first + second;
    if (line + first == size / 2) {
        square[i][k] = board[i + k][k - (k - result)];
    }
}

bool isValidCell(int& x, int& y, int rows, int cols) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

void resetStateVariables(bool visited[][11], int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            visited[i][j] = false;
        }
    }
}

void DFS(Stack& field_sizes, char square[][11], int& newX, int& newY, int dx[], int dy[], vector<pair<int, int>>& connectedComponent) {
    for (int j = 0; j < 6; ++j) {
        int neighbourX = newX + dx[j];
        int neighbourY = newY + dy[j];
        if (isValidCell(neighbourX, neighbourY, field_sizes.topValue(), field_sizes.topValue()) && square[neighbourX][neighbourY] == square[newX][newY] && !boardParameters.visited[neighbourX][neighbourY]) {
            boardParameters.visited[neighbourX][neighbourY] = true;
            connectedComponent.push_back({ neighbourX, neighbourY });
            DFS(field_sizes, square, neighbourX, neighbourY, dx, dy, connectedComponent);
        }
    }
}

int checkWinner(Stack& field_sizes, vector<pair<int, int>> connectedComponent, char& color, bool& result) {
    if (connectedComponent.size() >= field_sizes.topValue()) {
        for (auto& cell : connectedComponent) {
            if (cell.first == field_sizes.topValue() - 1 && color == BLUE) {
                if(boardParameters.firstCheck) {
                    boardParameters.countBlueLines++;
                    if (boardParameters.isBoardPossibleCommand && connectedComponent.size() >= field_sizes.topValue()) {
                        for (auto& cell : connectedComponent) {
                            boardParameters.winP.push_back({ cell.first, cell.second });
                        }
                    }
                }
                result = true;
                boardParameters.longestConnect.push(connectedComponent.size());
                boardParameters.symbol.push(color);
                return true;
            }
            if (cell.second == field_sizes.topValue() - 1 && color == RED) {
                if (boardParameters.firstCheck) {
                    boardParameters.countRedLines++;
                    if (boardParameters.isBoardPossibleCommand && connectedComponent.size() >= field_sizes.topValue()) {
                        for (auto& cell : connectedComponent) {
                            boardParameters.winP.push_back({ cell.first, cell.second });
                        }
                    }
                }
                boardParameters.longestConnect.push(connectedComponent.size());
                boardParameters.symbol.push(color);
                result = true;
                return true;
            }
            else {
                continue;
            }
        }
    }
    return result;
}

int checkNeighbors(Stack& field_sizes, char square[][11], int& x, int& y, bool& result) {
    
    char color = square[x][y];

    if ((color == BLUE) && x != 0) {
        return false;
    }
    else if (color == RED && y != 0) {
        return false;
    }

    boardParameters.visited[x][y] = true;

    vector<pair<int, int>> connectedComponent;
    connectedComponent.push_back({ x, y });

    DFS(field_sizes, square, x, y, neiborsCoordinates.dx, neiborsCoordinates.dy, connectedComponent);

    checkWinner(field_sizes, connectedComponent, color, result);
    return result;
}


void whoWin(bool& result, int& first, int& second, int& first_symbol, int& second_symbol, bool& redWin, bool& blueWin){
    if (result) {
        if (first > second) {
            if (first_symbol == RED) {
                if (boardParameters.isGameOverCommand) {
                    cout << "YES RED" << endl;
                }
                redWin = true;
            }
            else {
                if (boardParameters.isGameOverCommand) {
                    cout << "YES BLUE" << endl;
                }
                blueWin = true;
            }
        }
        else {
            if (second_symbol == RED) {
                if (boardParameters.isGameOverCommand) {
                    cout << "YES RED" << endl;
                }
                redWin = true;
            }
            else {
                if (boardParameters.isGameOverCommand) {
                    cout << "YES BLUE" << endl;
                }
                blueWin = true;

            }
        }
    }
    else if (boardParameters.isGameOverCommand && !result) {
        cout << "NO" << endl;
    }
}

void proccesingIsMoreThenOneWay(Stack& field_sizes, char square[][11], bool& result) {
    int countLines = 0;
    int countTrue = 0;
    int size = field_sizes.topValue();
    bool afterResult = false;
    for (const auto& coord : boardParameters.winP) {
        int i = coord.x;
        int j = coord.y;
        if (square[i][j] != '0') {
            countLines++;
            resetStateVariables(boardParameters.visited, size, size);
            char deletedSymbol = square[i][j];
            square[i][j] = '0';
            afterResult = false;
            for (int h = 0; h < size; ++h) {
                for (int g = 0; g < size; ++g) {
                    if (square[h][g] != '0') {
                        checkNeighbors(field_sizes, square, h, g, afterResult);
                    }
                }
            }
            square[i][j] = deletedSymbol;
            if (afterResult == true) {
                countTrue++;
            }
        }
    }
    if (countLines == countTrue) {
        boardParameters.isPossible = false;
    }
    else {
        boardParameters.isPossible = true;
    }
    boardParameters.winP.clear();
}

void proccesingGameOver_BoardPossible(Stack& field_sizes, char square[][11], bool& result, int& first, int& second, int& first_symbol, int& second_symbol, bool& redWin, bool& blueWin) {
    whoWin(result, first, second, first_symbol, second_symbol, redWin, blueWin);
    if (!result && boardParameters.isBoardPossibleCommand) {
        boardParameters.isPossible = true;
    }
    else if (boardParameters.isBoardPossibleCommand && result && ((redWin && boardParameters.red_pawns_count == boardParameters.blue_pawns_count) || (blueWin && boardParameters.blue_pawns_count != boardParameters.red_pawns_count))) {
        boardParameters.isPossible = false;
    }
    else if (boardParameters.isBoardPossibleCommand && result && (boardParameters.countRedLines > 1 || boardParameters.countBlueLines > 1)) {
        boardParameters.isPossible = false;
    }
    else if (boardParameters.isBoardPossibleCommand && result) {
        boardParameters.firstCheck = false;
        proccesingIsMoreThenOneWay(field_sizes, square, result);
    }
}

void IS_GAME_OVER(Stack& field_sizes, char square[][11]) {
    if (!IS_BOARD_CORRECT() && boardParameters.isGameOverCommand) {
        cout << "NO" << endl;
    }
    else if (!checkBlueCorners(field_sizes, square) && !checkRedCorners(field_sizes, square) && boardParameters.isGameOverCommand) {
        cout << "NO" << endl;
    }
    else {
        bool result = false;
        bool redWin = false;
        bool blueWin = false;
        boardParameters.countBlueLines = 0;
        boardParameters.countRedLines = 0;
        for (int i = 0; i < field_sizes.topValue(); ++i) {
            for (int j = 0; j < field_sizes.topValue(); ++j) {
                if (square[i][j] != '0') {
                    if (square[i][j] == BLUE || square[i][j] == RED) {
                        boardParameters.firstCheck = true;
                        checkNeighbors(field_sizes, square, i, j, result);
                    }
                }
            }

        }
        int firstLongestConnect = boardParameters.longestConnect.pop();
        int secondLongestConnect = boardParameters.longestConnect.pop();

        int first_symbol = boardParameters.symbol.pop();
        int second_symbol = boardParameters.symbol.pop();

        proccesingGameOver_BoardPossible(field_sizes,square,result, firstLongestConnect, secondLongestConnect, first_symbol,second_symbol, redWin, blueWin);
        resetStateVariables(boardParameters.visited, field_sizes.topValue(), field_sizes.topValue());
    }
}

void transformateToSquare(Stack& field_sizes, char board[][11], char square[][11]) {
    int line = 0;
    bool even_number = false;
    int half = field_sizes.topValue() - 1;
    int topValue = field_sizes.topValue();

    if (field_sizes.topValue() % 2 == 0) even_number = true;
    else even_number = false;

    for (int i = 0; i < topValue; ++i) {
        for (int j = 0; j < topValue; ++j) {
            if (line <= topValue - 2) {
                if (line < 2) {
                    if (board[i + j][j] != ' ') {
                        square[i][j] = board[i + j][j];
                    }
                    else {
                        square[i][j] = board[i + j][j - 1];
                    }
                }
                else {
                    for (int k = 0; k <= topValue; ++k) {
                        if (k < half + 1 && even_number) {
                            square[i][k] = board[i + k][k];
                        }
                        else if (k <= half && !even_number) {
                            square[i][k] = board[i + k][k];
                        }
                        else
                        {
                            if (topValue >= 5 && topValue < 7) {
                                for (int g = 1; g >= -1; g--) {
                                    correctPlacingForSquare(line, field_sizes, square, board, i, k, g, 2);
                                }
                            }
                            else if (topValue >= 7 && topValue < 9) {
                                for (int g = 2; g >= -2; g--) {
                                    correctPlacingForSquare(line, field_sizes, square, board, i, k, g, 3);
                                }
                            }
                            else if (topValue == 9) {
                                for (int g = 2; g >= -3; g--) {
                                    correctPlacingForSquare(line, field_sizes, square, board, i, k, g, 4);
                                }
                            }
                            else if (topValue == 10) {
                                for (int g = 3; g >= -3; g--) {
                                    correctPlacingForSquare(line, field_sizes, square, board, i, k, g, 4);
                                }
                            }
                            else if (topValue == 11) {
                                for (int g = 3; g >= -4; g--) {
                                    correctPlacingForSquare(line, field_sizes, square, board, i, k, g, 5);
                                }
                            }
                            else {
                                square[i][k] = board[i + k][k - (k - 1)];
                            }
                        }
                    }
                }
            }
            else {
                for (int k = 0; k <= field_sizes.topValue(); ++k) {
                    square[i][k] = board[i + k][0];
                }
            }
        }
        half--;
        line++;
    }
}

void commands(Stack& size, char line[], char board[][11], char square[][11], const int rows) {
    if (strcmp(line, "BOARD_SIZE") == 0) {
        while (!size.isEmpty()) {
            cout << size.topValue() << " ";
            size.pop();
        }
        clearBoard(rows, size.topValue(), board);
        clearCount();

    }
    else if (strcmp(line, "PAWNS_NUMBER") == 0) {
        cout << boardParameters.blue_pawns_count + boardParameters.red_pawns_count << " ";

        clearBoard(rows, size.topValue(), board);
        clearCount();
    }

    else if (strcmp(line, "IS_BOARD_CORRECT") == 0) {
        if (IS_BOARD_CORRECT()) {
            cout << "YES" << endl;
        }
        else {
            cout << "NO" << endl;
        }
        clearBoard(rows, size.topValue(), board);
        clearCount();

    }
    else if (strcmp(line, "IS_GAME_OVER") == 0) {
        boardParameters.isGameOverCommand = true;
        boardParameters.isBoardPossibleCommand = false;
        createClearSquare(size, square);
        transformateToSquare(size, board, square);

        IS_GAME_OVER(size, square);

        clearBoard(rows, size.topValue(), board);
        createClearSquare(size, square);
        clearCount();

    }
    else if (strcmp(line, "IS_BOARD_POSSIBLE") == 0) {
        boardParameters.longestConnect.pop();
        while (!boardParameters.symbol.isEmpty()) {
            boardParameters.symbol.pop();
        }
        boardParameters.isGameOverCommand = false;
        boardParameters.isBoardPossibleCommand = true;
        createClearSquare(size, square);
        transformateToSquare(size, board, square);

        if (IS_BOARD_CORRECT()) {
            if (boardParameters.red_pawns_count + boardParameters.blue_pawns_count == 0) {
                cout << "YES" << endl;
            }
            else if (boardParameters.red_pawns_count <= size.topValue()) {
                cout << "YES" << endl;
            }
            else if (!checkBlueCorners(size, square) && !checkRedCorners(size, square) && boardParameters.isBoardPossibleCommand) {
                cout << "YES" << endl;
            }
            else {
                IS_GAME_OVER(size, square);
                if (boardParameters.isPossible) {
                    cout << "YES" << endl;
                }
                else {
                    cout << "NO" << endl;
                }
            }
        }
        else {
            cout << "NO" << endl;
        }
        clearBoard(rows, size.topValue(), board);
        createClearSquare(size, square);
        clearCount();

    }
}

void proccesingBoard(Stack& field_sizes, char board[][11], char line[]) {

    boardParameters.prev_sizeBoard = boardParameters.sizeBoard;
    boardParameters.sizeBoard = 0;
    boardParameters.col_index = 0;

    for (size_t i = 0; line[i] != '\0'; ++i) {
        if (line[i] == '<') {
            boardParameters.sizeBoard++;
            i += 2;
            if (line[i] != ' ') {
                board[boardParameters.row_index - 1][boardParameters.col_index++] = line[i];
            }
            else {
                board[boardParameters.row_index - 1][boardParameters.col_index++] = '0';
            }
        }
        if (line[i] == RED) {
            boardParameters.red_pawns_count++;
        }
        if (line[i] == BLUE) {
            boardParameters.blue_pawns_count++;
        }
    }
    boardParameters.row_index++;


    if (boardParameters.sizeBoard > boardParameters.prev_sizeBoard) {
        if (!field_sizes.isEmpty()) {
            field_sizes.pop();
        }
        field_sizes.push(boardParameters.sizeBoard);
    }
}


int main() {
    Stack field_sizes;
    char line[1024];

    const int rows = 21;
    const int rows_square = 11;
    const int cols = 11;
    char board[rows][cols];
    char square[rows_square][cols];

    clearBoard(rows, cols, board);

    while (cin.getline(line, 1024)) {
        if (strlen(line) == 0) continue;

        proccesingBoard(field_sizes, board, line);

        commands(field_sizes, line, board, square, rows);
    }

    return 0;
}