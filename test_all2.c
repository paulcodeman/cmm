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
    a = 2; b = 3; c = 5; d = 7;

    if ((a + b) * c > 20)
        a = a + b + c;

    e = (a + b) * (c + d);

    e = fn_arg((a + b) * c);

    e = fn_arg(a) + fn_mul(b, c) * d;

    e = (fn_arg(a) + fn_mul(b, c)) * d;

    e = fn_mul(a + b, c) * d + a;
}
