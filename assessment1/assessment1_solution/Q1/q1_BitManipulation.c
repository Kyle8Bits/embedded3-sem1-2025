unsigned int X, r, temp1, temp2;
r = X; //read the value from X
temp1 = (r & (0b1111 << 4)) >> 4;    //get value of bits 7-4
temp2 = (r & (0b1111 << 10)) >> 10;  //get value of bits 13-10
r &= ~( (0b1111 << 4) | (0b1111 << 10) ); //clear bits 7-4 and 13-10
r |= (temp1 << 10 | temp2 << 4);   //write temp1 to bits 13-10, and temp2 to bits 7-4
X = r; //write the value back


/* ALTERNATIVE SOLUTION: */
unsigned int X, r, temp1, temp2;
r = X;  //read the value from X
temp1 = (r >> 4) & 0xF;      //get value of bits 7-4
temp2 = (r >> 10) & 0xF;    //get value of bits 13-10
r &= ~( (0xF << 4) | (0xF << 10) ); //clear bits 7-4 and 13-10
r |= (temp1 << 10 | temp2 << 4);   //write temp1 to bits 13-10, and temp2 to bits 7-4
X = r;   //write the value back

