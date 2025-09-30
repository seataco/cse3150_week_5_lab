#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

using namespace std;

// Forward declaration
int compute_score(const std::vector<std::vector<int>>& board);

#include <iterator>

std::vector<int> compress_row(const std::vector<int>& row) {
    std::vector<int> compressed;
    copy_if(row.begin(), row.end(), std::back_inserter(compressed), [](int x) { return x != 0; });
    compressed.resize(4, 0);
    return compressed;
}

std::vector<int> merge_row(std::vector<int> row) {
    for (auto it = row.begin(); it != row.end() - 1; ++it) {
        if (*it == *(it+1)) {
            *it *= 2;
            *(it+1) = 0;
        }
    }
    
    return compress_row(row);
}




void write_board_csv(const vector<vector<int>>& board, bool first, const string& stage) {
    ios_base::openmode mode = ios::app;
    if (first) mode = ios::trunc;
    ofstream fout("game_output.csv", mode);
    if (!fout) return;

    // Write stage identifier
    fout << stage << ",";

    // Write board data
    for (int r=0;r<4;r++){
        for (int c=0;c<4;c++){
            fout<<board[r][c];
            if (!(r==3 && c==3)) fout<<",";
        }
    }
    fout<<"\n";
}

void read_board_csv(vector<vector<int>>& board) {
    ifstream fin("game_input.csv");

    string line;
    int r = 0;
    while (getline(fin, line) && r < 4) {
        stringstream ss(line);
        string cell;
        int c = 0;
        while (getline(ss, cell, ',') && c < 4) {
            try {
                board[r][c] = stoi(cell);
            } catch (...) {
                board[r][c] = 0;  // Default to 0 for invalid input
            }
            c++;
        }
        r++;
    }
}

void print_board(const vector<vector<int>>& board) {
    cout<<"Score: "<<compute_score(board)<<endl;
    for (auto& row : board) {
        for (auto& cell : row) {
            if (cell == 0) cout<<"."<<"\t";
            else cout<<cell<<"\t";
        }
        cout<<endl;
    }
}

void spawn_tile(std::vector<std::vector<int>>& board) {
    std::vector<std::pair<int,int>> empty;
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            if (board[r][c] == 0) empty.emplace_back(r,c);

    if (empty.empty()) return;

    static std::mt19937 gen(42);  // Fixed seed for deterministic behavior
    std::uniform_int_distribution<> pos_dist(0, empty.size()-1);
    std::uniform_int_distribution<> val_dist(1, 10);

    auto [r, c] = empty[pos_dist(gen)];
    board[r][c] = (val_dist(gen) == 1 ? 4 : 2); // 10% chance of 4
}

bool move_left(std::vector<std::vector<int>>& board) {
    bool moved = false;
    for (auto& row : board) {
        std::vector<int> original_row = row;
        row = compress_row(row);
        row = merge_row(row);
        moved = moved || (row != original_row);
    }
    return moved;
}

bool move_right(std::vector<std::vector<int>>& board) {
    bool moved = false;
    for (auto& row : board) {
        std::vector<int> original_row = row;
        reverse(row.begin(), row.end());
        row = compress_row(row);
        row = merge_row(row);
        reverse(row.begin(), row.end());
        moved = moved || (row != original_row);
    }
    return moved;
}

bool move_up(std::vector<std::vector<int>>& board) {
    bool moved = false;
    // for better practice, consider using auto
    for (int c = 0; c < 4; c++) {
        std::vector<int> column;
        for (int r = 0; r < 4; r++) {
            column.push_back(board[r][c]);
        }
        std::vector<int> original_column = column;
        column = compress_row(column);
        column = merge_row(column);
        moved = moved || (column != original_column);
        for (int r = 0; r < 4; r++) {
            board[r][c] = column[r];
        }
    }
    return moved;
}

bool move_down(std::vector<std::vector<int>>& board) {
    bool moved = false;
    // for better practice, consider using auto
    for (int c = 0; c < 4; c++) {
        std::vector<int> column;
        for (int r = 0; r < 4; r++) {
            column.push_back(board[r][c]);
        }
        std::vector<int> original_column = column;
        reverse(column.begin(), column.end());
        column = compress_row(column);
        column = merge_row(column);
        reverse(column.begin(), column.end());
        moved = moved || (column != original_column);
        for (int r = 0; r < 4; r++) {
            board[r][c] = column[r];
        }
    }
    return moved;
}



int compute_score(const std::vector<std::vector<int>>& board) {
    int score = 0;
    for (const auto& row : board)
        for (int val : row)
            score += val;
    return score;
}


int main(){
    vector<vector<int>> board(4, vector<int>(4,0));

    // Read initial board from CSV
    read_board_csv(board);

    stack<vector<vector<int>>> history;
    bool first=true;

    while(true){
        print_board(board);
        if (first) {
            write_board_csv(board, true, "initial");
            first = false;
        }

        cout<<"Move (w=up, a=left, s=down, d=right), u=undo, q=quit: ";
        char cmd;
        if (!(cin>>cmd)) break;
        if (cmd=='q') break;

        if (cmd=='u') {
            // TODO: Check if history stack is not empty using !history.empty()
            // If not empty:
            //   1. Set board = history.top() to restore the previous state
            //   2. Remove the top element with history.pop()
            //   3. Call print_board(board) to show the restored board
            //   4. Call write_board_csv(board, false, "undo") to log the undo
            // Use 'continue' to skip the rest of the loop iteration
        }

        vector<vector<int>> prev = board;
        bool moved=false;
        if (cmd=='a') moved=move_left(board);
        else if (cmd=='d') moved=move_right(board);
        else if (cmd=='w') moved=move_up(board);
        else if (cmd=='s') moved=move_down(board);

        if (moved) {
            // TODO: Push the previous board state to history stack
            // Use: history.push(prev)

            // Write board after merge but before spawn
            write_board_csv(board, false, "merge");

            spawn_tile(board);
            // Write board after spawn
            write_board_csv(board, false, "spawn");
        } else {
            // No move was made
            write_board_csv(board, false, "invalid");
        }
    }
    return 0;
}
