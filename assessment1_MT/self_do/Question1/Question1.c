unsigned int X, r, temp1, temp2;
r = X;
temp1 = (r & ( 0b1111 << 4)) >> 4;
temp2 = (r & (0b1111 << 10)) >> 10;
r &= ~(0b1111 << 4);
r &= ~(0b1111 << 10);
r |= ((temp1 << 10) | (temp2 << 4));
X = r;

