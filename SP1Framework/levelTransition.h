#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
using namespace std;

COORD mapTransition(COORD spawn, int x, int y, int *level, bool *transisted);