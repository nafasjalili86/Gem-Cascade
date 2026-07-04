#ifndef GAMELOGIC_H
#define GAMELOGIC_H

#include <vector>
#include <string>
#include <iostream>

struct ConsoleColor {
    enum Kind { SET_COLOR, ADD_BOLD, REMOVE_BOLD, RESET_COLOR }; // یک نوع داده با چند مقدار مشخص میسازه
    Kind kind; // نوع عملیات رنگ
    int value; // کد رنگ ویندوز
};

std::ostream& operator<<(std::ostream& os, const ConsoleColor& c);

namespace Color {
    extern const ConsoleColor RESET;
    extern const ConsoleColor BOLD;
    extern const ConsoleColor DIM;
    extern const ConsoleColor RED;   // متغیر RED وجود داره، تعریف کاملش جای دیگه‌ست (UI.cpp)
    extern const ConsoleColor GREEN;
    extern const ConsoleColor YELLOW;
    extern const ConsoleColor BLUE;
    extern const ConsoleColor MAGENTA;
    extern const ConsoleColor CYAN;
    extern const ConsoleColor GOLD;
}

const int COST_BOMB = 120;
const int COST_ROCKET = 100;
const int COST_HINT = 70;

int mainmenu();
std::vector<std::vector<bool>> findmatches(std::vector<std::vector<char>>& panel, bool checkSpecial);
void removematches(std::vector<std::vector<char>>& panel, std::vector<std::vector<bool>>& marked);
void gravity(std::vector<std::vector<char>>& panel);
void fillempty(std::vector<std::vector<char>>& panel);
bool isstable(std::vector<std::vector<char>>& panel);
int countmarked(std::vector<std::vector<bool>>& marked);
bool deadlock(std::vector<std::vector<char>>& panel); 

void savegame(std::vector<std::vector<char>>& panel, int score, int timeLeft, int bombs, int rockets, int hints);
bool loadgame(std::vector<std::vector<char>>& panel, int& score, int& timeLeft, int& bombs, int& rockets, int& hints);
void applybomb(std::vector<std::vector<char>>& panel, int r, int c);
void applyrocket(std::vector<std::vector<char>>& panel, int idx, char type);

#endif
