
typedef unsigned char BYTE; // 1 byte
typedef char SIGNED_BYTE; // 1 byte
typedef unsigned short WORD; // 2 bytes
typedef signed short SIGNED_WORD; // 2 bytes

#define MEMORY_BYTES 0x10000

#define A 0
#define F 1
#define B 2
#define C 3
#define D 4
#define E 5
#define H 6
#define L 7

class MMU {
public:

    void write(WORD address, BYTE b) { M[address] = b; }
    BYTE load(WORD address) { return M[address]; }

private:
    WORD M[MEMORY_BYTES];


};
