#include <stdio.h>

//Function to print out number in binary format
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
    unsigned int X = 0b111000000;  // Initial value of X
    unsigned int Y = 0;  // Initial value of Y
    printf("   X = ") ; printBinary(X);
    printf("   Y = ") ; printBinary(Y);

    // a. Set bit 3 of X
    X |= (1 << 3);  
    //printf("The binary equivalent is %d \n",  X);
    printf("a) X = ") ; printBinary(X);

    // b. Set bits 5-3 of X
    X |= (0b111 << 3);
    printf("b) X = ") ; printBinary(X);

    // c. Clear bit 6 of X
    X &= ~(1 << 6);
    printf("c) X = ") ; printBinary(X);

    // d. Clear bits 8-6 of X
    X &= ~(0b111 << 6); 
    printf("d) X = ") ; printBinary(X);

    /* e. Both set bits 5-3 and clear bits 8-6 of X 
    (should write to X only one time) */
    unsigned int tmp = X;
    tmp |= (0b111 << 3);  //set bits 5-3
    tmp &= ~(0b111 << 6); //clear bits 8-6
    X = tmp;
    printf("e) X = ") ; printBinary(X);

    // f. Toggle (reverse) bits 7 and 4 of X
    X ^= ((1 << 7) | (1 << 4));  
    printf("f) X = ") ; printBinary(X);

    // g. Copy bits 7-4 of X to bits 11-8 of Y
    tmp = (X >> 4) & 0b1111; //store only bits 7-4 of X to tmp
    unsigned int tmpY;
    tmpY = Y & ~(0b1111 << 8);  // Clear bits 11-8 of Y's value
    tmpY |= tmp << 8;           // Update bits 11-8 by tmp value
    Y = tmpY;                   // Write back to Y
    printf("g) X = ") ; printBinary(X);
    printf("   Y = ") ; printBinary(Y);

    return 0;
}
