#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

using namespace std;

struct Position {
    int x, y;
};

struct Block {
    int width, height;
    vector<vector<bool>> shape;
};

class Board {
private:
    bool cells[8][8];

public:
    Board() {
        for (auto& row : cells)
            for (bool& cell : row)
                cell = false;
    }

    Board(const Board& other) {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                cells[i][j] = other.cells[i][j];
    }

    void setCell(int row, int col, bool value) {
        cells[row][col] = value;
    }

    bool canPlace(const Block& block, int x, int y) const {
        if (x < 0 || y < 0 || x + block.width > 8 || y + block.height > 8)
            return false;
        
        for (int i = 0; i < block.height; ++i)
            for (int j = 0; j < block.width; ++j)
                if (block.shape[i][j] && cells[y + i][x + j])
                    return false;
        
        return true;
    }

    void placeBlock(const Block& block, int x, int y) {
        for (int i = 0; i < block.height; ++i)
            for (int j = 0; j < block.width; ++j)
                if (block.shape[i][j])
                    cells[y + i][x + j] = true;
    }

    vector<int> getFilledRows() const {
        vector<int> filled;
        for (int row = 0; row < 8; ++row) {
            bool full = true;
            for (int col = 0; col < 8; ++col)
                if (!cells[row][col]) {
                    full = false;
                    break;
                }
            if (full) filled.push_back(row);
        }
        return filled;
    }

    vector<int> getFilledColumns() const {
        vector<int> filled;
        for (int col = 0; col < 8; ++col) {
            bool full = true;
            for (int row = 0; row < 8; ++row)
                if (!cells[row][col]) {
                    full = false;
                    break;
                }
            if (full) filled.push_back(col);
        }
        return filled;
    }

    void clearRows(const vector<int>& rows) {
        for (int row : rows)
            for (int col = 0; col < 8; ++col)
                cells[row][col] = false;
    }

    void clearColumns(const vector<int>& cols) {
        for (int col : cols)
            for (int row = 0; row < 8; ++row)
                cells[row][col] = false;
    }

    void print(const vector<Position>* newCells = nullptr) const {
        cout << "  ";
        for (int col = 0; col < 8; ++col) cout << col << " ";
        cout << "\n";

        for (int row = 0; row < 8; ++row) {
            cout << row << " ";
            for (int col = 0; col < 8; ++col) {
                bool isNew = newCells ? find_if(newCells->begin(), newCells->end(),
                    [&](const Position& p){ return p.x == col && p.y == row; }) != newCells->end() : false;
                
                cout << (cells[row][col] ? (isNew ? "□ " : "■ ") : ". ");
            }
            cout << endl;
        }
    }
};

Block parseBlock(const string& s) {
    vector<string> rows;
    size_t pos = 0;
    while (pos < s.size()) {
        size_t next = s.find('-', pos);
        if (next == string::npos) {
            rows.push_back(s.substr(pos));
            break;
        } else {
            rows.push_back(s.substr(pos, next - pos));
            pos = next + 1;
        }
    }

    if (rows.empty()) return Block{0, 0, {}};

    int max_width = 0;
    for (const auto& row : rows)
        if (row.size() > max_width)
            max_width = row.size();

    for (auto& row : rows)
        if (row.size() < max_width)
            row += string(max_width - row.size(), '.');

    int height = rows.size();
    int width = max_width;
    vector<vector<bool>> shape(height, vector<bool>(width, false));
    
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            if (rows[i][j] == 'o')
                shape[i][j] = true;

    return Block{width, height, shape};
}

vector<Position> generateValidPositions(const Board& board, const Block& block) {
    vector<Position> positions;
    for (int x = 0; x <= 8 - block.width; ++x) {
        for (int y = 0; y <= 8 - block.height; ++y) {
            if (board.canPlace(block, x, y)) {
                positions.push_back({x, y});
            }
        }
    }
    return positions;
}

struct State {
    Board board;
    int score;
    vector<Position> moves;

    State() : score(-1) {}
    State(Board b, int s, vector<Position> m) : board(b), score(s), moves(m) {}
};

State dfs(const Board& currentBoard, int currentScore, int blockIdx, const vector<Block>& blocks) {
    if (blockIdx >= blocks.size()) {
        return State(currentBoard, currentScore, {});
    }

    const Block& block = blocks[blockIdx];
    vector<Position> positions = generateValidPositions(currentBoard, block);

    State bestState;

    for (const Position& pos : positions) {
        Board newBoard = currentBoard;
        newBoard.placeBlock(block, pos.x, pos.y);

        vector<int> filledRows = newBoard.getFilledRows();
        vector<int> filledCols = newBoard.getFilledColumns();
        int addedScore = filledRows.size() + filledCols.size();

        newBoard.clearRows(filledRows);
        newBoard.clearColumns(filledCols);

        State nextState = dfs(newBoard, currentScore + addedScore, blockIdx + 1, blocks);

        if (nextState.score > bestState.score) {
            nextState.moves.insert(nextState.moves.begin(), pos);
            bestState = nextState;
        }
    }

    return bestState;
}

void simulateAndDisplay(Board initialBoard, const vector<Block>& blocks, const vector<Position>& moves) {
    Board currentBoard = initialBoard;
    int totalScore = 0;

    for (size_t i = 0; i < blocks.size(); ++i) {
        system(CLEAR);
        vector<Position> newCells;
        
        const Block& block = blocks[i];
        const Position& pos = moves[i];
        
        // Store original board state
        Board beforePlacement = currentBoard;
        
        // Place the block and record new cells
        currentBoard.placeBlock(block, pos.x, pos.y);
        for (int y = 0; y < block.height; ++y) {
            for (int x = 0; x < block.width; ++x) {
                if (block.shape[y][x]) {
                    newCells.push_back({pos.x + x, pos.y + y});
                }
            }
        }

        cout << "Placement " << (i+1) << " at (" << pos.x << "," << pos.y << ")\n";
        currentBoard.print(&newCells);

        // Get filled rows/columns AFTER placement
        vector<int> filledRows = currentBoard.getFilledRows();
        vector<int> filledCols = currentBoard.getFilledColumns();
        
        // Clear them and update score
        currentBoard.clearRows(filledRows);
        currentBoard.clearColumns(filledCols);
        totalScore += filledRows.size() + filledCols.size();

        cout << "\nCleared " << filledRows.size() << " row(s) and " 
             << filledCols.size() << " column(s)\n";
        cout << "New board state (■ = existing, □ = new, . = empty):\n";
        currentBoard.print();
        cout << "Total score: " << totalScore << "\n\n";
        system("pause");
    }
}

int main() {
    Board currentBoard;
    
    // Set up UTF-8 output
    #ifdef _WIN32
    system("chcp 65001");
    #endif

    // Custom board initialization
    cout << "Load custom starting board? (y/n): ";
    char loadChoice;
    cin >> loadChoice;
    if (tolower(loadChoice) == 'y') {
        bool valid = false;
        while (!valid) {
            cout << "Enter 8x8 board (use '-' between rows, e.g., ooo.....-ooo.....-...):\n";
            string input;
            cin >> input;

            vector<string> rows;
            size_t pos = 0;
            while (pos < input.size()) {
                size_t next = input.find('-', pos);
                if (next == string::npos) {
                    rows.push_back(input.substr(pos));
                    break;
                } else {
                    rows.push_back(input.substr(pos, next - pos));
                    pos = next + 1;
                }
            }

            if (rows.size() != 8) {
                cout << "Need exactly 8 rows!\n";
                continue;
            }

            valid = true;
            for (int i = 0; i < 8; ++i) {
                if (rows[i].size() != 8) {
                    cout << "Row " << i << " must be 8 characters!\n";
                    valid = false;
                    break;
                }
                for (int j = 0; j < 8; ++j) {
                    currentBoard.setCell(i, j, rows[i][j] == 'o');
                }
            }
        }
    }

    while (true) {
        system(CLEAR);
        cout << "Current board:\n";
        currentBoard.print();
        cout << endl;

        vector<Block> blocks;
        bool validBlocks = true;
        for (int i = 0; i < 3; ++i) {
            string input;
            cout << "Enter block " << (i+1) << " (e.g., ooo-ooo-ooo): ";
            cin >> input;

            Block block = parseBlock(input);
            if (block.width == 0 || block.height == 0) {
                cout << "Invalid block format. Please try again.\n";
                validBlocks = false;
                break;
            }
            blocks.push_back(block);
        }

        if (!validBlocks) continue;

        State bestOverallState;
        vector<Block> bestBlocksOrder;

        vector<int> indices = {0, 1, 2};
        do {
            vector<Block> permutedBlocks;
            for (int idx : indices) {
                permutedBlocks.push_back(blocks[idx]);
            }

            State currentState = dfs(currentBoard, 0, 0, permutedBlocks);

            if (currentState.score > bestOverallState.score) {
                bestOverallState = currentState;
                bestBlocksOrder = permutedBlocks;
            }
        } while (next_permutation(indices.begin(), indices.end()));

        if (bestOverallState.moves.size() != bestBlocksOrder.size()) {
            cout << "No valid sequence found for any permutation of the blocks.\n";
            continue;
        }

        cout << "\nBest sequence of moves considering all permutations:\n";
        simulateAndDisplay(currentBoard, bestBlocksOrder, bestOverallState.moves);
        currentBoard = bestOverallState.board;

        cout << "Continue? (y/n): ";
        char choice;
        cin >> choice;
        if (tolower(choice) != 'y') break;
    }

    return 0;
}