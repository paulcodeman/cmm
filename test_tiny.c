dword a, b, c, d;
dword r6;

void main()
{
    a = 2; b = 3; c = 5; d = 7;

    r6 = ((a + b) * (c + d)) - a;

    if ((a + b) * c > d * 3)
        r6 = a;
}
