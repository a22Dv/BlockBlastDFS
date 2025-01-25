#include <iostream>
#include <vector>
#include <unordered_set>

using Int2DVector = std::vector<std::vector<int>>;
using Int3DVector = std::vector<Int2DVector>;

std::vector<std::string> getVectorFromArgv(int argc, char* argv[]);
Int2DVector getBinaryRepresentation(std::string argument);
Int2DVector getUpdatedBoardState(Int2DVector board);

int main(int argc, char* argv[]) {
    // Prepare arguments.
    std::vector<std::string> args = getVectorFromArgv(argc, argv);
    Int3DVector argsArrays;
    for (int i = 1; i < args.size(); ++i) {
        argsArrays.push_back(getBinaryRepresentation(args[i]));
    }
    getUpdatedBoardState(argsArrays[0]);
    return 0;
}
std::vector<std::string> getVectorFromArgv(int argc, char* argv[]) {
    std::vector<std::string> return_vector;
    for (int i = 0; i < argc; ++i) {
        return_vector.push_back(std::string(argv[i]));
    }
    return return_vector;
}
Int2DVector getBinaryRepresentation(std::string argument) {
    Int2DVector return_vector;
    std::vector<int> current_row;
    for (int i = 0; i < argument.length(); ++i) {
        if (argument[i] == '_') {
            return_vector.push_back(current_row);
            current_row.clear();
        } else {
            current_row.push_back(argument[i] == 'o' ? 1 : 0);
            i == argument.length() - 1 && (return_vector.push_back(current_row), true);
        }
    }
    return return_vector;
}
Int2DVector getUpdatedBoardState(Int2DVector board) {
    Int2DVector updatedBoard = board;
    
    for (int i = 0; i < updatedBoard.size(); ++i) {
        std::vector<int> c_row, c_col;
        for (int j = 0; j < updatedBoard.size(); ++j) {
            c_row.push_back(updatedBoard[i][j]);
            c_col.push_back(updatedBoard[j][i]);
        }
        if (boardSectionForDeletion(c_row)) {
            for (int j = 0; j < updatedBoard.size(); ++j) {
                updatedBoard[i][j] = 0;
            }
        }
        if (boardSectionForDeletion(c_col)) {
            for (int j = 0; j < updatedBoard.size(); ++j) {
                updatedBoard[j][i] = 0;
            }
        }
    }
    return updatedBoard;
}

bool boardSectionForDeletion(std::vector<int> section) {
    for (int i = 0; i < section.size(); ++i) {
        if (section[i] == 0) {
            return false;
        } 
    }
    return true;
}