dword a,b;

dword foo(dword x)
{
	return x * 2;
}

dword bar(dword x, dword y)
{
	return x + y;
}

void main()
{
	b = 3;
	a = foo(b) + 1;
	a = b + foo(b) * 2;
	a = b * foo(b) + 1;
	a = foo(b) + bar(b, 5);
	a = foo(b) * 2 + b;
	a = foo(b) * bar(b, 2) + 1;
	a = foo(b);
}
