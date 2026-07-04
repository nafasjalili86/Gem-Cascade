#ifndef BOARD_H
#define BOARD_H

#include <vector>

void matrix(std::vector<std::vector<char> >& panel);
void swapcoordinates(std::vector<std::vector<char> >& panel, int r1, int c1, int r2, int c2);
bool validmovement(std::vector<std::vector<char> >& panel, int r1, int c1, int r2, int c2);
bool useHint(std::vector<std::vector<char>>& panel, int& hintR1, int& hintC1, int& hintR2, int& hintC2);

#endif