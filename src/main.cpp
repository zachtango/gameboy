#include "gameboy.h"
#include <iostream>
#include <bitset>

using namespace std;

int main(){

    // Gameboy gb = Gameboy();

    // for(int i = 0; i < 256; i++){
    //     gb.step();
    // }

    int n = (true << 7u);
    bitset<8> x(n);
    bitset<8> y(-n);

    cout << x << ' ' << y << endl;

    return 0;
}

