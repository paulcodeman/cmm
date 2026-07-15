dword a,b,c;
dword foo(dword x) { return x * 2; }
dword bar(dword y) { return y + 3; }
void main()
{
    a = 10; b = 2;
    c = foo(a) + bar(b) * 2;
}
