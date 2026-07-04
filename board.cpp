#include <iostream>
#include <vector>
#include <cstdlib>  // توابع آماده  rand system exit 
#include <cmath>
#include "board.h"   // تو این فقط اعلام میکنیم تو فالای سی پی پی پیاده سازی میکنیم
#include "gamelogic.h"

using namespace std;

bool validposition(vector<vector<char> >& panel , int i , int j){
    if (j >= 2){  // بیشتر از دوتا تطابق ایجاد نشه در هر سطرو ستونی
        if (panel[i][j] == panel[i][j-1] && panel[i][j] == panel[i][j-2])       
            return false;
    }
    if (i >= 2) {    
        if (panel[i][j] == panel[i-1][j] && panel[i][j] == panel[i-2][j])       
            return false;
    }
    return true;
}

void matrix(vector<vector<char> >& panel){
    char gems[] = {'$', '&', '%', '@', '#'};
    for (int i = 0; i < panel.size(); i++) {
        for (int j = 0; j < panel[i].size(); j++) {
            panel[i][j] = gems[rand() % 5];
            while (!validposition(panel ,i , j)){
                panel[i][j] = gems[rand() % 5];
            }
        }
    } 
}


void swapcoordinates(vector<vector<char> >& panel, int r1, int c1, int r2, int c2){
    swap(panel[r1-1][c1-1] , panel[r2-1][c2-1]);
}

bool threematch(vector<vector<char> >& panel, int r, int c) {
    char gem = panel[r][c];
    int count = 1; 
    for (int k = c-1; k >= 0 && panel[r][k] == gem; k--) count++; 
    for (int k = c+1; k < 8 && panel[r][k] == gem; k++) count++; 
    if (count >= 3) return true;
    
    count = 1;  
    for (int k = r-1; k >= 0 && panel[k][c] == gem; k--) count++; 
    for (int k = r+1; k < 8 && panel[k][c] == gem; k++) count++; 
    if (count >= 3) return true;
    
    return false;
}

bool validmovement(vector<vector<char> >& panel, int r1, int c1, int r2, int c2){
    if(r1 < 1 || r1 > 8 || c1 < 1 || c1 > 8 || r2 < 1 || r2 > 8 || c2 < 1 || c2 > 8) return false;
    bool adjacent = (abs(r1-r2) == 1 && c1 == c2) || (abs(c1-c2) == 1 && r1 == r2);
    if (!adjacent) return false;
    
    swap(panel[r1-1][c1-1], panel[r2-1][c2-1]);
    bool match = threematch(panel, r1-1, c1-1) || threematch(panel, r2-1, c2-1);
    swap(panel[r1-1][c1-1], panel[r2-1][c2-1]); 
    return match;
}

bool useHint(vector<vector<char>>& panel, int& hintR1, int& hintC1, int& hintR2, int& hintC2) {
    int maxdeleted = 0;
    bool foundmove = false;

    for (int i = 1; i <= 8; i++) {
        for (int j = 1; j <= 8; j++) {
            if (j + 1 <= 8 && validmovement(panel, i, j, i, j + 1)) { // حرکت چپ و راستی
                swap(panel[i-1][j-1], panel[i-1][j]); 
                auto marked = findmatches(panel, false); 
                swap(panel[i-1][j-1], panel[i-1][j]);   // سوپ میکنه بعد برمیگرده به حالت قبلی و صرفا فقط تست میکنه

                int currentdeleted = 0;  
                for (int r = 0; r < 8; r++)
                    for (int c = 0; c < 8; c++)
                        if (marked[r][c]) currentdeleted++;

                if (currentdeleted > maxdeleted) { //اگ جم های بیشتری رو حذف کنه .. بهتره برامون
                    maxdeleted = currentdeleted;
                    hintR1 = i; hintC1 = j;
                    hintR2 = i; hintC2 = j + 1;
                    foundmove = true; 
                }
            }  // حرکتی رو نگه میداره ک بیشترین جم رو حذف کنه

            if (i + 1 <= 8 && validmovement(panel, i, j, i + 1, j)) {  // حرکت بالا پایینی
                swap(panel[i-1][j-1], panel[i][j-1]);    
                auto marked = findmatches(panel, false); 
                swap(panel[i-1][j-1], panel[i][j-1]); 

                int currentdeleted = 0;
                for (int r = 0; r < 8; r++)
                    for (int c = 0; c < 8; c++)
                        if (marked[r][c]) currentdeleted++;

                if (currentdeleted > maxdeleted) {
                    maxdeleted = currentdeleted;
                    hintR1 = i; hintC1 = j;
                    hintR2 = i + 1; hintC2 = j;
                    foundmove = true; 
                }
            }
        }
    }
    return foundmove;
}