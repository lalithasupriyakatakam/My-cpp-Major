#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <chrono>
using namespace std;

class SudokuSolver {
private:
    static const int SIZE = 9;
    static const int SUBGRID = 3;
    vector<vector<char>> board;
    
    // For optimization: track which numbers are used in each row, column, and subgrid
    bitset<SIZE> rows[SIZE];
    bitset<SIZE> cols[SIZE];
    bitset<SIZE> boxes[SIZE];
    
    // Statistics
    int backtrackCount;
    int recursiveCalls;
    
public:
    SudokuSolver(vector<vector<char>> inputBoard) 
        : board(inputBoard), backtrackCount(0), recursiveCalls(0) {
        initializeConstraints();
    }
    
    void initializeConstraints() {
        for (int i = 0; i < SIZE; i++) {
            rows[i].reset();
            cols[i].reset();
            boxes[i].reset();
        }
        
        for (int r = 0; r < SIZE; r++) {
            for (int c = 0; c < SIZE; c++) {
                if (board[r][c] != '.') {
                    int num = board[r][c] - '1';
                    int boxIndex = (r / SUBGRID) * SUBGRID + (c / SUBGRID);
                    
                    rows[r].set(num);
                    cols[c].set(num);
                    boxes[boxIndex].set(num);
                }
            }
        }
    }
    
    bool solve() {
        return solveSudoku(0, 0);
    }
    
    bool solveSudoku(int row, int col) {
        recursiveCalls++;
        
        // If we've reached the end of the board, puzzle is solved
        if (row == SIZE) {
            return true;
        }
        
        // Move to next row if at end of column
        if (col == SIZE) {
            return solveSudoku(row + 1, 0);
        }
        
        // Skip already filled cells
        if (board[row][col] != '.') {
            return solveSudoku(row, col + 1);
        }
        
        int boxIndex = (row / SUBGRID) * SUBGRID + (col / SUBGRID);
        
        // Try numbers 1-9
        for (int num = 0; num < SIZE; num++) {
            // Check if number is valid in current position
            if (!rows[row][num] && !cols[col][num] && !boxes[boxIndex][num]) {
                // Place number
                board[row][col] = num + '1';
                rows[row].set(num);
                cols[col].set(num);
                boxes[boxIndex].set(num);
                
                // Recursively solve the rest
                if (solveSudoku(row, col + 1)) {
                    return true;
                }
                
                // Backtrack if solution not found
                backtrackCount++;
                board[row][col] = '.';
                rows[row].reset(num);
                cols[col].reset(num);
                boxes[boxIndex].reset(num);
            }
        }
        
        return false;
    }
    
    // Alternative solving method with constraint propagation (Minimum Remaining Values heuristic)
    bool solveWithOptimization() {
        int row, col;
        
        if (!findEmptyCellWithMRV(row, col)) {
            return true; // Puzzle solved
        }
        
        recursiveCalls++;
        int boxIndex = (row / SUBGRID) * SUBGRID + (col / SUBGRID);
        
        // Try possible numbers
        for (int num = 0; num < SIZE; num++) {
            if (!rows[row][num] && !cols[col][num] && !boxes[boxIndex][num]) {
                // Place number
                board[row][col] = num + '1';
                rows[row].set(num);
                cols[col].set(num);
                boxes[boxIndex].set(num);
                
                if (solveWithOptimization()) {
                    return true;
                }
                
                // Backtrack
                backtrackCount++;
                board[row][col] = '.';
                rows[row].reset(num);
                cols[col].reset(num);
                boxes[boxIndex].reset(num);
            }
        }
        
        return false;
    }
    
    // Find empty cell using Minimum Remaining Values heuristic
    bool findEmptyCellWithMRV(int& row, int& col) {
        int minOptions = SIZE + 1;
        row = col = -1;
        
        for (int r = 0; r < SIZE; r++) {
            for (int c = 0; c < SIZE; c++) {
                if (board[r][c] == '.') {
                    int options = countValidOptions(r, c);
                    if (options < minOptions) {
                        minOptions = options;
                        row = r;
                        col = c;
                    }
                }
            }
        }
        
        return row != -1 && col != -1;
    }
    
    int countValidOptions(int row, int col) {
        int boxIndex = (row / SUBGRID) * SUBGRID + (col / SUBGRID);
        int count = 0;
        
        for (int num = 0; num < SIZE; num++) {
            if (!rows[row][num] && !cols[col][num] && !boxes[boxIndex][num]) {
                count++;
            }
        }
        
        return count;
    }
    
    void printBoard() const {
        cout << "\nSolved Sudoku:\n";
        cout << "+-------+-------+-------+\n";
        
        for (int i = 0; i < SIZE; i++) {
            cout << "| ";
            for (int j = 0; j < SIZE; j++) {
                cout << board[i][j] << " ";
                if ((j + 1) % SUBGRID == 0) {
                    cout << "| ";
                }
            }
            cout << "\n";
            if ((i + 1) % SUBGRID == 0) {
                cout << "+-------+-------+-------+\n";
            }
        }
    }
    
    void printStatistics() const {
        cout << "\nSolver Statistics:\n";
        cout << "Backtrack operations: " << backtrackCount << "\n";
        cout << "Recursive calls: " << recursiveCalls << "\n";
    }
    
    bool isValidSudoku() const {
        // Check rows
        for (int i = 0; i < SIZE; i++) {
            bitset<SIZE> rowCheck;
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] != '.') {
                    int num = board[i][j] - '1';
                    if (rowCheck[num]) return false;
                    rowCheck.set(num);
                }
            }
        }
        
        // Check columns
        for (int j = 0; j < SIZE; j++) {
            bitset<SIZE> colCheck;
            for (int i = 0; i < SIZE; i++) {
                if (board[i][j] != '.') {
                    int num = board[i][j] - '1';
                    if (colCheck[num]) return false;
                    colCheck.set(num);
                }
            }
        }
        
        // Check subgrids
        for (int box = 0; box < SIZE; box++) {
            bitset<SIZE> boxCheck;
            int startRow = (box / SUBGRID) * SUBGRID;
            int startCol = (box % SUBGRID) * SUBGRID;
            
            for (int i = 0; i < SUBGRID; i++) {
                for (int j = 0; j < SUBGRID; j++) {
                    if (board[startRow + i][startCol + j] != '.') {
                        int num = board[startRow + i][startCol + j] - '1';
                        if (boxCheck[num]) return false;
                        boxCheck.set(num);
                    }
                }
            }
        }
        
        return true;
    }
    
    vector<vector<char>> getBoard() const {
        return board;
    }
};

// Example puzzles
vector<vector<char>> createHardPuzzle() {
    return {
        {'8', '.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '3', '6', '.', '.', '.', '.', '.'},
        {'.', '7', '.', '.', '9', '.', '2', '.', '.'},
        {'.', '5', '.', '.', '.', '7', '.', '.', '.'},
        {'.', '.', '.', '.', '4', '5', '7', '.', '.'},
        {'.', '.', '.', '1', '.', '.', '.', '3', '.'},
        {'.', '.', '1', '.', '.', '.', '.', '6', '8'},
        {'.', '.', '8', '5', '.', '.', '.', '1', '.'},
        {'.', '9', '.', '.', '.', '.', '4', '.', '.'}
    };
}

vector<vector<char>> createMediumPuzzle() {
    return {
        {'5', '3', '.', '.', '7', '.', '.', '.', '.'},
        {'6', '.', '.', '1', '9', '5', '.', '.', '.'},
        {'.', '9', '8', '.', '.', '.', '.', '6', '.'},
        {'8', '.', '.', '.', '6', '.', '.', '.', '3'},
        {'4', '.', '.', '8', '.', '3', '.', '.', '1'},
        {'7', '.', '.', '.', '2', '.', '.', '.', '6'},
        {'.', '6', '.', '.', '.', '.', '2', '8', '.'},
        {'.', '.', '.', '4', '1', '9', '.', '.', '5'},
        {'.', '.', '.', '.', '8', '.', '.', '7', '9'}
    };
}

void testSolver(const vector<vector<char>>& puzzle, const string& difficulty) {
    cout << "\n=====================================";
    cout << "\nSolving " << difficulty << " Sudoku puzzle\n";
    cout << "=====================================\n";
    
    auto start = chrono::high_resolution_clock::now();
    
    SudokuSolver solver(puzzle);
    
    if (!solver.isValidSudoku()) {
        cout << "Invalid initial puzzle!\n";
        return;
    }
    
    // Solve with optimization (MRV heuristic)
    bool solved = solver.solveWithOptimization();
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    
    if (solved) {
        solver.printBoard();
        solver.printStatistics();
        cout << "Time taken: " << duration.count() << " ms\n";
        
        // Verify solution
        if (solver.isValidSudoku()) {
            cout << "✓ Solution is valid!\n";
        } else {
            cout << "✗ Solution is invalid!\n";
        }
    } else {
        cout << "No solution exists!\n";
    }
}

int main() {
    cout << "SUDOKU SOLVER USING BACKTRACKING ALGORITHM\n";
    cout << "==========================================\n";
    
    // Test with different puzzles
    testSolver(createMediumPuzzle(), "Medium");
    testSolver(createHardPuzzle(), "Hard");
    
    // Custom puzzle input example
    cout << "\n\nEnter your own Sudoku puzzle (9x9, use '.' for empty cells):\n";
    vector<vector<char>> customPuzzle(9, vector<char>(9));
    
    for (int i = 0; i < 9; i++) {
        string row;
        cout << "Row " << i + 1 << ": ";
        cin >> row;
        for (int j = 0; j < 9; j++) {
            customPuzzle[i][j] = row[j];
        }
    }
    
    testSolver(customPuzzle, "Custom");
    
    return 0;
}
