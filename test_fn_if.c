dword a,b;
dword foo(dword x) { return x * 2; }
dword bar(dword y) { return y + 3; }
void main()
{
    a = 10; b = 2;
    if (foo(a) + bar(b) * 2 > 15)
        a = 1;
    if ((foo(a) + bar(b)) * 2 > 15)
        a = 2;
    if (foo(a + b) > 15)
        a = 3;
}
