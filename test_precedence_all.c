dword a, b, c, d, x, y;
dword r01, r02, r03, r04, r05;
dword r06, r07, r08, r09, r10;

dword box(dword v)
{
    return v;
}

void main()
{
    a = 2; b = 3; c = 5; d = 7; x = 11; y = 13;

    if ((a + b) * c > 20)
        r01 = a + b + c;

    r02 = (a + b) * (c + d);

    r03 = box((a + b) * c);

    r04 = ((a + b) * c) + d;

    r05 = ((a + b) * (c + d)) - a;

    if ((a + b) * c > d * 2)
        r06 = a;

    if ((a + b) > 3) && ((c + d) < 15)
        r07 = b;

    r08 = box((a + b) * (c - b));

    r09 = a + b * c + d;

    r10 = (a + b) * c + d;
}
