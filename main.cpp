#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib> // برای rand 
#include <chrono> // کار با زمان
#include <thread> // برای مکث کوتاه
#include "board.h"
#include "UI.h"
#include "gamelogic.h"

using namespace std;

int main() {
    srand(time(0));
    enablecolor();
    int choice = mainmenu();

    if (choice == 1 || choice == 2) {
        vector<vector<char>> panel(8, vector<char>(8));
        int score = 0;
        int highScore = 1200;
        int totalDuration = 180; 
        int bombs = 1, rockets = 1, hints = 3;

        if (choice == 1) {
            matrix(panel);
            while (!isstable(panel)) {
                auto marked = findmatches(panel, false);
                removematches(panel, marked);
                gravity(panel);
                fillempty(panel);
            }
        } else {
            if (!loadgame(panel, score, totalDuration, bombs, rockets, hints)) {
                cout << Color::YELLOW << "Starting a new game instead...\n" << Color::RESET;
                matrix(panel);
                totalDuration = 180;
                while (!isstable(panel)) {
                    auto marked = findmatches(panel, false);
                    removematches(panel, marked);
                    gravity(panel);
                    fillempty(panel);
                }
            }
        }

        auto startTime = chrono::steady_clock::now();  // زمان شروع بازی رو ذخیره می‌کنه.

        while (true) {
            auto currentTime = chrono::steady_clock::now();
            int elapsedSeconds = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();  //اختلاف زمان رو برحسب ثانیه حساب می‌کنه.
            int timeLeft = totalDuration - elapsedSeconds;

            if (timeLeft <= 0) {
                cout << Color::RED << "\nTime is up! Game Over!\n" << Color::RESET;
                break;
            }

            system("cls");
            printHeader(score, highScore, timeLeft);
            colorfulboard(panel);
            printSidebar(bombs, rockets, hints);

            cout << Color::CYAN << "\n[1] Move Gem | [2] Use Power-up | [3] Buy | [4] Save | [0] Exit: " << Color::RESET;
            string actionStr;
            cin >> actionStr;

            if (actionStr == "0"){
                savegame(panel, score, timeLeft, bombs, rockets, hints);
            break;}

            if (actionStr == "1") {
                int r1, c1, r2, c2;
                cout << Color::GREEN << "Enter coordinate (r1 c1 r2 c2): " << Color::RESET;
                if (!(cin >> r1 >> c1 >> r2 >> c2)) {
                    cin.clear(); cin.ignore(1000, '\n');
                    continue;
                }

                if (!validmovement(panel, r1, c1, r2, c2)) {
                    cout << Color::RED << "Invalid Movement!\n" << Color::RESET;
                    this_thread::sleep_for(chrono::milliseconds(800));
                    continue;
                }

                swapcoordinates(panel, r1, c1, r2, c2);
                int chain = 0;

                while (true) {
                    auto marked = findmatches(panel, true);
                    int count = countmarked(marked);
                    if (count == 0) break;

                    int multiplier = 1 << chain; // 2**chain هر بار شیفت به چپ ضربدر دو میشه
                    score += count * 10 * multiplier;

                    removematches(panel, marked);
                    system("cls");
                    printHeader(score, highScore, timeLeft);
                    colorfulboard(panel);
                    this_thread::sleep_for(chrono::milliseconds(600));

                    gravity(panel);
                    system("cls");
                    printHeader(score, highScore, timeLeft);
                    colorfulboard(panel);
                    this_thread::sleep_for(chrono::milliseconds(600));

                    chain++;
                }
                fillempty(panel);
            }
            else if (actionStr == "2") {
                cout << Color::YELLOW << "Select: [B] Bomb | [R] Rocket | [H] Hint: " << Color::RESET;
                char p; cin >> p;

                if ((p == 'B' || p == 'b') && bombs > 0) {
                    int r, c; cout << Color::RED << "Enter target (row col): " << Color::RESET; cin >> r >> c;
                    applybomb(panel, r, c);
                    bombs--;
                    gravity(panel); fillempty(panel);
                }
                else if ((p == 'R' || p == 'r') && rockets > 0) {
                    int idx; char type;
                    cout << Color::CYAN << "Row or Column? (r/c) and Index: " << Color::RESET; cin >> type >> idx;
                    applyrocket(panel, idx, type);
                    rockets--;
                    gravity(panel); fillempty(panel);
                }
                else if (p == 'H' || p == 'h') {
                    if (hints > 0) {
                        int hR1 = 0, hC1 = 0, hR2 = 0, hC2 = 0;
                        if (useHint(panel, hR1, hC1, hR2, hC2)) {
                            hints--;
                            cout << Color::YELLOW << "Hint: Swap (" << hR1 << "," << hC1 << ") with (" << hR2 << "," << hC2 << ")\n" << Color::RESET;
                            this_thread::sleep_for(chrono::seconds(2));
                        }
                    }
                }
            }
            else if (actionStr == "3") {
                cout << Color::GOLD << "Buy: [B] Bomb | [R] Rocket | [H] Hint: " << Color::RESET;
                char buy; cin >> buy;
                if ((buy == 'B' || buy == 'b') && score >= COST_BOMB) { score -= COST_BOMB; bombs++; }
                if ((buy == 'R' || buy == 'r') && score >= COST_ROCKET) { score -= COST_ROCKET; rockets++; }
                if ((buy == 'H' || buy == 'h') && score >= COST_HINT) { score -= COST_HINT; hints++; }
            }
            
            else if (actionStr == "4") {
                auto currentTime = chrono::steady_clock::now();
                int elapsedSeconds = chrono::duration_cast<chrono::seconds>(currentTime - startTime).count();
                int timeLeft = totalDuration - elapsedSeconds;
                savegame(panel, score, timeLeft, bombs, rockets, hints);
            }

            if (!deadlock(panel)) {
                cout << Color::MAGENTA << "Deadlock detected! Refreshing board...\n" << Color::RESET;
                do {
                    matrix(panel);
                    while (!isstable(panel)) {
                        auto m = findmatches(panel, false);
                        removematches(panel, m);
                        gravity(panel);
                        fillempty(panel);
                    }
                } while (!deadlock(panel));
            }
        }
    }
    return 0;
}