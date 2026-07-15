dword a, b, c, d, e;

dword fn_arg(dword x)
{
    return x * 10;
}

dword fn_mul(dword x, y)
{
    return x * y;
}

void main()
{
    extern dword something();

    a = something() mod 10 + 2;
    b = something() mod 10 + 3;
    c = something() mod 10 + 5;
    d = something() mod 10 + 7;

    if ((a + b) * c > 20)
        e = a + b + c;

    e = (a + b) * (c + d);

    e = fn_arg((a + b) * c);

    e = fn_arg(a) + fn_mul(b, c) * d;

    e = (fn_arg(a) + fn_mul(b, c)) * d;

    e = fn_mul(a + b, c) * d + a;
}
