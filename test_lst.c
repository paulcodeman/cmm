dword a,b,c,d;
void main()
{
    a = 10; b = 2; c = 3;
    if ((a + b) * c > 15)
        d = 1;
    if (a * (b + c) > 15)
        d = 2;
    if ((a + b) * (c + d) > 15)
        d = 3;
}
