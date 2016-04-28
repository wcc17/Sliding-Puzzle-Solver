#include <iostream>
#include "TileGame.h"

using namespace std;

int main() {
    srand(unsigned(time(NULL)));

    TileGame tileGame;

    cout << "Starting game..." << endl;

    return tileGame.onExecute();
}