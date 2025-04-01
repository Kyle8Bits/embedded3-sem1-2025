#include <stdio.h>


void printBinary(unsigned int num) {
    unsigned int mask = 1 << 31;  // start with the MSB bit

    printf("0b");
    while (mask > 0) {
        printf("%d", (num & mask) ? 1 : 0);
        mask >>= 1;
    }
    printf("\n");
}

int main(){
    unsigned int X = 0b10101111001110010011101101010110;
    unsigned int Y = 0;
    Y = X;

    unsigned int temp26_12, temp11_6;

    temp26_12 =  (Y >> 12) & 0x7FFF;
    temp11_6 = (Y >> 6) & 0b111111;

    temp11_6 ^= (0b111111 << 0);

    Y &= ~(0x1FFFFF << 6);

    Y |= (temp26_12 << 6);
    Y |= (temp11_6 << 21);

    printBinary(Y);
    printBinary(temp11_6);
    printBinary(temp26_12);

    return 0;
}
