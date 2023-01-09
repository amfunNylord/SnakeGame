#include <iostream>
#include <vector>
#include <chrono>
#include <conio.h>
#include <Windows.h>
#include <stdlib.h>

struct FieldPoint {
    int x, y;
};

struct AppleType {
    FieldPoint coordinates;
    const char SIGN = '@';
};

struct SnakeType {
    std::vector <FieldPoint> body;
    const char SIGN = '#';
};

AppleType apple = { 2, 3 };
SnakeType snake = { {{3, 6}, {3, 7}} };
const int DELAY = 400;
const char FIELD_BACKGROUND = '*';
const int FIELD_X = 8, FIELD_Y = 4;
const int MINUS_BORDER = -1;
const char UP_DIRECTION = 'w', DOWN_DIRECTION = 's', RIGHT_DIRECTION = 'd', LEFT_DIRECTION = 'a';
std::vector <std::vector <char>> field(FIELD_Y, std::vector <char>(FIELD_X, FIELD_BACKGROUND));
char direction = 'a';
int saveLastX = 0, saveLastY = 0;
char key;

void SetCur(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void PrintField(std::vector <std::vector <char>> &field) {
    SetCur(0, 0);
    for (auto& row : field) {
        for (auto& el : row) {
            std::cout << el << ' ';
        }
        std::cout << '\n';
    }
}

void PlaceApple(std::vector <std::vector <char>> &field) {
    field[apple.coordinates.x][apple.coordinates.y] = apple.SIGN;
}

void PlaceSnake(std::vector <std::vector <char>> &field) {
    for (int i = 0; i < snake.body.size(); i++) {
        field[snake.body[i].x][snake.body[i].y] = snake.SIGN;
    }
}

bool IsSnakeBody() {
    for (int i = 1; i < snake.body.size(); i++) {
        if (snake.body[0].x == snake.body[i].x && snake.body[0].y == snake.body[i].y) {
            return true;
        }
    }
    return false;
}

void Direct(char pos, int move, char direct) {
    int saveX = 0, saveY = 0, saveX1 = 0, saveY1 = 0;
    FieldPoint newPoint;
    if (direction == direct)
    {
        for (int i = 0; i < snake.body.size(); i++)
        {
            if (i + 1 == snake.body.size())
            {
                saveLastX = snake.body[i].x;
                saveLastY = snake.body[i].y;
                field[snake.body[i].x][snake.body[i].y] = '*';
                snake.body.erase(end(snake.body) - 1);
                snake.body.insert(end(snake.body), newPoint);
            }
            else
            {
                if (i == 0)
                {
                    saveX = snake.body[i].x;
                    saveY = snake.body[i].y;
                    newPoint = { saveX, saveY };
                    if (pos == 'x') {
                        snake.body[i].x = snake.body[i].x + move;
                    }
                    else {
                        snake.body[i].y = snake.body[i].y + move;
                    }
                }
                else
                {
                    saveX1 = snake.body[i].x;
                    saveY1 = snake.body[i].y;
                    snake.body[i].x = saveX;
                    snake.body[i].y = saveY;
                    saveX = saveX1;
                    saveY = saveY1;
                    newPoint = { saveX, saveY };
                }
            }
        }
    }
}

void Move()
{
    Direct('x', -1, UP_DIRECTION);
    Direct('y', -1, LEFT_DIRECTION);
    Direct('x', 1, DOWN_DIRECTION);
    Direct('y', 1, RIGHT_DIRECTION);
}

bool IsWall() {
    if (snake.body[0].y == FIELD_X || snake.body[0].y == MINUS_BORDER || snake.body[0].x == FIELD_Y || snake.body[0].x == MINUS_BORDER) {
        std::cout << "Borders";
        return true;
    }
    return false;
}

void EatApple() {
    FieldPoint newCoord = {};
    newCoord = { saveLastX, saveLastY };
    snake.body.insert(end(snake.body), newCoord);
}

bool IsApple() {
    return ((snake.body[0].x == apple.coordinates.x) && (snake.body[0].y == apple.coordinates.y)) ? true : false;
}

void GenerateApple() {
    while (1) {
        int samePosition = 0;
        srand(time(NULL));
        int numX = rand() % (FIELD_Y);
        apple.coordinates.x = numX;
        srand(time(NULL));
        int numY = rand() % (FIELD_X);
        apple.coordinates.y = numY;
        for (int i = 0; i < snake.body.size(); i++) {
            if ((apple.coordinates.x == snake.body[i].x) && (apple.coordinates.y == snake.body[i].y)) {
                samePosition += 1;
            }
        }
        if (samePosition == 0) {
            break;
        }
    }
}

void Victory() {
    std::cout << "Victory";
}

int main() {
    PrintField(field);
    Sleep(DELAY);
    PlaceSnake(field);
    PrintField(field);
    Sleep(DELAY);
    PlaceApple(field);
    PrintField(field);
    Sleep(DELAY);
    while (snake.body.size() != 32) {
        if (_kbhit()) {
            key = _getch();
            if ((key == UP_DIRECTION && direction != DOWN_DIRECTION) || (key == DOWN_DIRECTION && direction != UP_DIRECTION) || (key == LEFT_DIRECTION && direction != RIGHT_DIRECTION) || (key == RIGHT_DIRECTION && direction != LEFT_DIRECTION)) {
                direction = key;
            }
        }
        Move();
        if (IsApple()) {
            EatApple();
            GenerateApple();
            PlaceApple(field);
        }
        if (IsWall())
        {
            exit(0);
        }
        if (IsSnakeBody()) 
        {
            exit(0);
        }
        PlaceSnake(field);
        PrintField(field);
        Sleep(DELAY);
    }
    Victory();
}