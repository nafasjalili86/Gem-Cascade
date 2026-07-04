#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <fstream> // برای کار با فایل
#include "gamelogic.h"
#include "board.h"

using namespace std;

int mainmenu(){
    cout << endl;
    cout << Color::MAGENTA << " ***************************************\n" << Color::RESET ;
    cout << Color::MAGENTA <<" ||            GEM CASCADE            ||\n"<< Color::RESET ;
    cout << Color::MAGENTA <<" ||                                   ||  \n" << Color::RESET ;
    cout << Color::MAGENTA <<" *************************************** \n" <<Color::RESET ;
    cout << "  " << Color::MAGENTA << "[1]" << "  new game \n" << Color::RESET;
    cout << "  " << Color::GREEN << "[2]" <<  "  load game \n"<< Color::RESET;
    cout << "  " << Color::BLUE << "[0]" << "  exit\n\n" << Color::RESET;
    cout << "  " << Color::GREEN << "please select a number: " << Color::RESET << endl;

    char selection;
    while (true) {
        cin >> selection;
        if (selection == '0') return 0;
        if (selection == '1') return 1;
        if (selection == '2') return 2;
        cout << "  please select again: ";
    }
}

vector<vector<bool>> findmatches(vector<vector<char>>& panel, bool checkSpecial) {
    int rows = 8, cols = 8;
    vector<vector<bool>> marked(rows, vector<bool>(cols, false));
    vector<vector<bool>> visited(rows, vector<bool>(cols, false));

    int dr[] = {-1, 1, 0, 0};  // هر دوتا ایندکسش باهم جهت میسازن
    int dc[] = {0, 0, -1, 1};  // مثلا منقی یک و صفر میشه بالا .. بعدی پایین .. چپ ... راست

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (visited[i][j] || panel[i][j] == ' ') continue;
            // برمیداره همه جم های شبیه رو ی گروه میکنه
            char gem = panel[i][j];
            vector<pair<int, int>> group;
            queue<pair<int, int>> q;

            q.push({i, j});   
            visited[i][j] = true;

            while (!q.empty()) {
                auto [r, c] = q.front();
                q.pop();
                group.push_back({r, c});
                 // اولین خونه ی صف رو برمیداره و عضو گروه میکندش
                for (int d = 0; d < 4; d++) {
                    int nr = r + dr[d];  // مختصات همسایه هاشو حساب میکنه
                    int nc = c + dc[d];

                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {  // چک میکنه ک از برد بیرون نزده باشه
                        if (!visited[nr][nc] && panel[nr][nc] == gem) {
                            visited[nr][nc] = true;
                            q.push({nr, nc});
                        }
                    }
                }
            }

            if (checkSpecial && group.size() >= 5) {  // مچ 5 تایی همه جم های هم نوع وی صفحه رو حذف میکنه
                char targetGem = gem;
                cout << Color::BOLD << Color::MAGENTA << "\n 5-Match detected! Clear all '" << targetGem << "' gems!\n" << Color::RESET;
                for (int r = 0; r < rows; r++) {
                    for (int c = 0; c < cols; c++) {
                        if (panel[r][c] == targetGem) {
                            marked[r][c] = true;
                        }
                    }
                }
            }
            else if (checkSpecial && group.size() == 4) {  // مچ 4 تایی راکت داره
                for (auto [r, c] : group) {
                    marked[r][c] = true;
                }
                int centerRow = group[0].first;
                int centerCol = group[0].second;

                cout << Color::BOLD << Color::CYAN << "\n[ ROCKET REWARD ] 4-Match detected! Choose Direction (R for Row / C for Column): " << Color::RESET;
                char choice;
                cin >> choice;

                if (choice == 'R' || choice == 'r') {
                    for (int c = 0; c < 8; c++) marked[centerRow][c] = true;
                } else {
                    for (int r = 0; r < 8; r++) marked[r][centerCol] = true;
                }
            }
            else if (group.size() >= 3) { // مچ 3 تایی رو هم حذف میکنه
                for (auto [r, c] : group) {
                    marked[r][c] = true;
                }
            }
        }
    }
    return marked;
}

void removematches(vector<vector<char>>& panel, vector<vector<bool>>& marked) {
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (marked[i][j]) panel[i][j] = ' ';
}

void gravity(vector<vector<char>>& panel) {  
    for (int j = 0; j < 8; j++) {
        int empty = 7;  // پایین‌ترین جای خالی ستون.  اول از آخرین سطر شروع می‌کته
        for (int i = 7; i >= 0; i--) {
            if (panel[i][j] != ' ') {
                panel[empty][j] = panel[i][j];
                if (empty != i) panel[i][j] = ' ';
                empty--;
            }
        }
    }
}

void fillempty(vector<vector<char>>& panel) { 
    char gems[] = {'$', '&', '%', '@', '#'};
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (panel[i][j] == ' ') {
                panel[i][j] = gems[rand() % 5];
            }
        }
    }
}

bool isstable(vector<vector<char>>& panel) { // هیچ مچ آماده ای روی برد نباشه
    auto marked = findmatches(panel, false);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (marked[i][j]) return false;
        }
    }
    return true;
}

bool deadlock(vector<vector<char>>& panel) {
    for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 8; j++) {
            if (j + 1 <= 8 && validmovement(panel, i, j, i, j + 1)) return true;
            if (i + 1 <= 8 && validmovement(panel, i, j, i + 1, j)) return true;
        }
    }
    return false;
}

int countmarked(vector<vector<bool>>& marked) {
    int count = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            if (marked[i][j]) count++;
    return count;
}

void savegame(vector<vector<char>>& panel, int score, int timeLeft, int bombs, int rockets, int hints) {
    ofstream file("savegame.txt");  
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            file << panel[i][j];  
            if (j < 7) file << " ";  
        }
        file << "\n";  
    }
    file << score << "\n";
    file << timeLeft << "\n";
    file << bombs << " " << rockets << " " << hints << "\n";
    file.close();
    cout << Color::GREEN << "Game saved!" << Color::RESET << endl;
}

bool loadgame(vector<vector<char>>& panel, int& score, int& timeLeft, int& bombs, int& rockets, int& hints) {
    ifstream file("savegame.txt");  
    if (!file) {  
        cout << Color::RED << "No save file found!" << Color::RESET << endl;
        return false;
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            file >> panel[i][j];
        }
    }
    file >> score;
    file >> timeLeft;
    file >> bombs >> rockets >> hints;
    file.close();
    return true;
}

void applybomb(vector<vector<char>>& panel, int r, int c) {
    r--; c--; // مختصات ورودی از کاربر یک بیسه ولی کد من صفر بیسه .. برا همین یدونه کم میکنم
    for (int i = r - 1; i <= r + 1; i++) {
        for (int j = c - 1; j <= c + 1; j++) {
            if (i >= 0 && i < 8 && j >= 0 && j < 8) {
                panel[i][j] = ' '; 
            }
        }
    }
}

void applyrocket(vector<vector<char>>& panel, int idx, char type) {
    idx--; 
    if (type == 'r' || type == 'R') {
        if (idx >= 0 && idx < 8) {
            for (int j = 0; j < 8; j++) panel[idx][j] = ' ';
        }
    } else {
        if (idx >= 0 && idx < 8) {
            for (int i = 0; i < 8; i++) panel[i][idx] = ' ';
        }
    }
}