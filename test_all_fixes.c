dword a,b,c,d,e;

dword fn_arg(dword x) { return x * 10; }
dword fn_mul(dword a, dword b) { return a * b; }

void main()
{
    a = 2; b = 3; c = 5; d = 7;

    // 1: paren chain in if
    e = (a + b) * c;

    // 2: nested parens
    e = (a + b) * (c + d);

    // 3: paren chain in return (via fn_arg)
    e = fn_arg((a + b) * c);

    // 4: function call inside math expression
    e = fn_arg(a) + fn_mul(b, c) * d;

    // 5: parens override precedence with function calls
    e = (fn_arg(a) + fn_mul(b, c)) * d;

    // 6: if with paren chain + cmp
    if ((a + b) * c > 20)
        e = a + b + c;

    // 7: return with function + paren chain
    e = fn_mul(a + b, c) * d + a;
}
