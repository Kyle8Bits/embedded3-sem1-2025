#include <stdio.h>

// Function to print out number in binary format
void printBinary(unsigned int num) {
    unsigned int mask = 1 << 31;  // start with the MSB bit

    printf("0b");
    while (mask > 0) {
        printf("%d", (num & mask) ? 1 : 0);
        mask >>= 1;
    }
    printf("\n");
}

int main() {
    unsigned int X = 0b10101111001110000011101101010111;
    unsigned int Y = 0;
    
    Y = X;
    // Extract bits 7-4 and bits 13-10
    unsigned int bits27_6 = (Y & (0x3FFFFF << 6)) >>6;  // >> 0;

    Y &= ~(0x3FFFFFF << 6);

    Y |= (bits27_6 << 10);
    Y |= (0b1010 << 6);

    printf("   X = ");
    printBinary(X);
    printf("   Y2 = ");
    printBinary(Y);
}
