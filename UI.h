#ifndef UI_H
#define UI_H

#include <vector>
#include <string>

void enablecolor();
void printHeader(int score, int highScore, int timeLeft);
void colorfulboard(std::vector<std::vector<char> > &panel);
void printSidebar(int bombs, int rockets, int hints);
void renderAll(std::vector<std::vector<char>>& panel, int score, int highScore, int timeLeft, int bombs, int rockets, int hints, const std::string& gameLog);

#endif