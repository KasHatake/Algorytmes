#pragma once
#ifndef OTHER_H
#define OTHER_H

#include <string>

using namespace std;

class Graph;

void setupConsole();
string cleanFilename(const string& input);
int inputInt(const string& prompt, int minVal, int maxVal);
int showMainMenu();
Graph* createNewGraph();
Graph* loadGraph();

#endif