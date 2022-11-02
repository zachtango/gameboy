#include "gameboy.h"

using namespace std;

int main(){

    Gameboy gb = Gameboy();

    for(int i = 0; i < 256; i++){
        gb.step();
    }

    return 0;
}

