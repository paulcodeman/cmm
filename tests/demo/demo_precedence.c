// C-- Math Expression Precedence Demo
// All expressions below compile and produce correct results
// after our operator precedence fixes.

dword a, b, c, d, x, y;
dword r01, r02, r03, r04, r05;
dword r06, r07, r08, r09, r10;

dword box(dword v)
{
    return v + 1;
}

void main()
{
    a = 2; b = 3; c = 5; d = 7; x = 11; y = 13;

    // 1: paren chain in if condition
    if ((a + b) * c > 20)
        r01 = a + b + c;

    // 2: nested parens in assignment
    r02 = (a + b) * (c + d);

    // 3: paren chain in function argument
    r03 = box((a + b) * c);

    // 4: operator precedence: * before +
    r04 = a + b * c + d;

    // 5: parens override precedence
    r05 = (a + b) * c + d;

    // 6: double nested parens
    r06 = ((a + b) * (c + d)) - a;

    // 7: chained if with && after parens
    if ((a + b) > 3) && ((c + d) < 15)
        r07 = a;

    // 8: nested parens in function call argument
    r08 = box((a + b) * (c + d));

    // 9: paren chain with comparison on both sides
    if ((a + b) * c > d * 3)
        r09 = b;

    // 10: a + b * (c + d) — precedence with parens
    r10 = a + b * (c + d);
}
