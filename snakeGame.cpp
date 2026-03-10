#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>

using namespace std;

// Game constants
const int WIDTH = 20;
const int HEIGHT = 20;
const int MAX_SNAKE_LENGTH = 100;

// Direction constants
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

// Game variables
int snakeX[MAX_SNAKE_LENGTH], snakeY[MAX_SNAKE_LENGTH];
int snakeLength;
int fruitX, fruitY;
int score;
Direction dir;
bool gameOver;

// Function prototypes
void Setup();
void Draw();
void Input();
void Logic();
void HideCursor();
void SetCursorPosition(int x, int y);

int main() {
    Setup();
    
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        // Control game speed
        Sleep(100);
    }
    
    cout << "\nGame Over! Final Score: " << score << endl;
    cout << "Press any key to exit...";
    _getch();
    
    return 0;
}

void Setup() {
    gameOver = false;
    dir = STOP;
    
    // Initialize snake in the middle
    snakeLength = 3;
    snakeX[0] = WIDTH / 2;
    snakeY[0] = HEIGHT / 2;
    
    // Initialize snake body
    for (int i = 1; i < snakeLength; i++) {
        snakeX[i] = snakeX[0] - i;
        snakeY[i] = snakeY[0];
    }
    
    // Place first fruit
    srand(time(0));
    fruitX = rand() % WIDTH;
    fruitY = rand() % HEIGHT;
    
    score = 0;
}

void Draw() {
    // Clear screen (platform-dependent)
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
    
    // Hide cursor for cleaner display
    HideCursor();
    
    // Draw top border
    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";
    cout << endl;
    
    // Draw game area
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            // Draw left border
            if (x == 0)
                cout << "#";
            
            // Draw snake head
            if (x == snakeX[0] && y == snakeY[0])
                cout << "O";
            // Draw snake body
            else {
                bool isBodyPart = false;
                for (int i = 1; i < snakeLength; i++) {
                    if (snakeX[i] == x && snakeY[i] == y) {
                        cout << "o";
                        isBodyPart = true;
                        break;
                    }
                }
                
                // Draw fruit
                if (!isBodyPart) {
                    if (x == fruitX && y == fruitY)
                        cout << "F";
                    else
                        cout << " ";
                }
            }
            
            // Draw right border
            if (x == WIDTH - 1)
                cout << "#";
        }
        cout << endl;
    }
    
    // Draw bottom border
    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";
    cout << endl;
    
    // Display score
    cout << "Score: " << score << endl;
    cout << "Controls: WASD or Arrow Keys to move, X to quit" << endl;
}

void Input() {
    // Check if a key is pressed
    if (_kbhit()) {
        switch (_getch()) {
            case 'a':
            case 'A':
            case 75: // Left arrow
                if (dir != RIGHT) // Prevent 180-degree turn
                    dir = LEFT;
                break;
            case 'd':
            case 'D':
            case 77: // Right arrow
                if (dir != LEFT)
                    dir = RIGHT;
                break;
            case 'w':
            case 'W':
            case 72: // Up arrow
                if (dir != DOWN)
                    dir = UP;
                break;
            case 's':
            case 'S':
            case 80: // Down arrow
                if (dir != UP)
                    dir = DOWN;
                break;
            case 'x':
            case 'X':
                gameOver = true;
                break;
        }
    }
}

void Logic() {
    // Store previous tail position for fruit collision
    int prevX = snakeX[snakeLength - 1];
    int prevY = snakeY[snakeLength - 1];
    
    // Move snake body (from tail to head)
    for (int i = snakeLength - 1; i > 0; i--) {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }
    
    // Move snake head based on direction
    switch (dir) {
        case LEFT:
            snakeX[0]--;
            break;
        case RIGHT:
            snakeX[0]++;
            break;
        case UP:
            snakeY[0]--;
            break;
        case DOWN:
            snakeY[0]++;
            break;
    }
    
    // Wall collision detection (wrap around)
    if (snakeX[0] >= WIDTH) snakeX[0] = 0;
    else if (snakeX[0] < 0) snakeX[0] = WIDTH - 1;
    
    if (snakeY[0] >= HEIGHT) snakeY[0] = 0;
    else if (snakeY[0] < 0) snakeY[0] = HEIGHT - 1;
    
    // Self collision detection
    for (int i = 1; i < snakeLength; i++) {
        if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
            gameOver = true;
            return;
        }
    }
    
    // Fruit collision detection
    if (snakeX[0] == fruitX && snakeY[0] == fruitY) {
        // Increase score
        score += 10;
        
        // Grow snake
        if (snakeLength < MAX_SNAKE_LENGTH) {
            snakeLength++;
            snakeX[snakeLength - 1] = prevX;
            snakeY[snakeLength - 1] = prevY;
        }
        
        // Place new fruit at random position
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
        
        // Make sure fruit doesn't appear on snake
        bool onSnake;
        do {
            onSnake = false;
            for (int i = 0; i < snakeLength; i++) {
                if (fruitX == snakeX[i] && fruitY == snakeY[i]) {
                    onSnake = true;
                    fruitX = rand() % WIDTH;
                    fruitY = rand() % HEIGHT;
                    break;
                }
            }
        } while (onSnake);
    }
}

// Helper function to hide cursor for cleaner display
void HideCursor() {
    #ifdef _WIN32
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
    #endif
}

// Helper function to set cursor position
void SetCursorPosition(int x, int y) {
    #ifdef _WIN32
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    #endif
}
