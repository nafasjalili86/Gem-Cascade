#include <windows.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <iomanip>
#include <string> 
#include "gamelogic.h"

using namespace std;

namespace Color {
    const ConsoleColor RESET   = { ConsoleColor::RESET_COLOR, 0 };
    const ConsoleColor BOLD    = { ConsoleColor::ADD_BOLD, 0 };
    const ConsoleColor DIM     = { ConsoleColor::REMOVE_BOLD, 0 };
    const ConsoleColor RED     = { ConsoleColor::SET_COLOR, FOREGROUND_RED | FOREGROUND_INTENSITY };
    const ConsoleColor GREEN   = { ConsoleColor::SET_COLOR, FOREGROUND_GREEN | FOREGROUND_INTENSITY };
    const ConsoleColor YELLOW  = { ConsoleColor::SET_COLOR, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY };
    const ConsoleColor BLUE    = { ConsoleColor::SET_COLOR, FOREGROUND_BLUE | FOREGROUND_INTENSITY };
    const ConsoleColor MAGENTA = { ConsoleColor::SET_COLOR, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY };
    const ConsoleColor CYAN    = { ConsoleColor::SET_COLOR, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY };
    const ConsoleColor GOLD    = { ConsoleColor::SET_COLOR, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY };
}

static WORD g_defaultAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
static bool g_defaultCaptured = false;

std::ostream& operator<<(std::ostream& os, const ConsoleColor& c) {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE || hOut == NULL) return os;

    CONSOLE_SCREEN_BUFFER_INFO info;
    WORD current = g_defaultAttributes;
    if (GetConsoleScreenBufferInfo(hOut, &info)) {
        current = info.wAttributes;
        if (!g_defaultCaptured) {
            g_defaultAttributes = info.wAttributes;
            g_defaultCaptured = true;
        }
    }

    WORD newAttr = current;
    switch (c.kind) {
        case ConsoleColor::SET_COLOR:
            newAttr = (WORD)c.value;
            break;
        case ConsoleColor::ADD_BOLD:
            newAttr = current | FOREGROUND_INTENSITY;
            break;
        case ConsoleColor::REMOVE_BOLD:
            newAttr = current & ~FOREGROUND_INTENSITY;
            break;
        case ConsoleColor::RESET_COLOR:
            newAttr = g_defaultAttributes;
            break;
    }

    os.flush();  // هر چیزی که هنوز چاپ نشده رو اول چاپ کن.
    SetConsoleTextAttribute(hOut, newAttr);
    return os;
}

void enablecolor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE || hOut == NULL) return;

    CONSOLE_SCREEN_BUFFER_INFO info;
    if (GetConsoleScreenBufferInfo(hOut, &info)) {
        g_defaultAttributes = info.wAttributes;
        g_defaultCaptured = true;
    }
}

struct Gem {
    char symbol;
    ConsoleColor color;
};

Gem gems[] = { 
    {'$', Color::RED},
    {'&', Color::GREEN},
    {'%', Color::YELLOW},
    {'#', Color::BLUE},
    {'@', Color::MAGENTA},
};

void colorfulboard(vector<vector<char> > &panel) {
    cout << "\n";
    cout << "      ";
    for (int j = 0; j < 8; j++) cout << j + 1 << "  ";
    cout << "\n" << Color::GOLD << "   +---------------------------+\n" << Color::RESET;
    for (int i = 0; i < 8; i++) {
        cout << " " << i + 1 << " || ";
        for (int j = 0; j < 8; j++) {
            char currentSymbol = panel[i][j];
            ConsoleColor currentColor = Color::RESET;
            for (int k = 0; k < 5; k++) {
                if (gems[k].symbol == currentSymbol) {
                    currentColor = gems[k].color;
                    break;
                }
            }
            cout << currentColor << currentSymbol << Color::RESET << "  ";
        }
        cout << "||\n";
    }
    cout << Color::GOLD << "   +---------------------------+\n" << Color::RESET;
}

void printHeader(int score, int highScore, int timeLeft) {
    int mm = timeLeft / 60;
    int ss = timeLeft % 60;
    ConsoleColor timeColor = (timeLeft > 60) ? Color::GREEN : (timeLeft > 30 ? Color::YELLOW : Color::RED);

    cout << Color::DIM << string(72, '=') << Color::RESET << "\n";
    cout << Color::BOLD << Color::CYAN << "  GEM CASCADE" << Color::RESET
         << "  |  SCORE: " << Color::YELLOW << setw(5) << score << Color::RESET
         << "  |  TIME: " << timeColor << setfill('0') << setw(2) << mm << ":" << setw(2) << ss << setfill(' ') << Color::RESET
         << "  |  BEST: " << Color::GOLD << highScore << Color::RESET << "\n";
    cout << Color::DIM << string(72, '=') << Color::RESET << "\n";
}

void printSidebar(int bombs, int rockets, int hints) {
    cout << "\n  " << Color::BOLD << "[ POWER-UPS ]" << Color::RESET << "\n";
    cout << "  " << Color::RED << "[B] Bomb   x" << bombs << Color::DIM << " (" << COST_BOMB << " pts)" << Color::RESET << "\n";
    cout << "  " << Color::CYAN << "[R] Rocket x" << rockets << Color::DIM << " (" << COST_ROCKET << " pts)" << Color::RESET << "\n";
    cout << "  " << Color::YELLOW << "[H] Hint   x" << hints << Color::DIM << " (" << COST_HINT << " pts)" << Color::RESET << "\n";
}
