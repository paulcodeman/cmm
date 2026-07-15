dword a, b, c, d, e;

dword box(dword v)
{
    return v;
}

void main()
{
    a = 2; b = 3; c = 5; d = 7;

    if ((a + b) * c > 20)
        e = a + b + c;

    e = (a + b) * (c + d);

    e = box((a + b) * c);
}
