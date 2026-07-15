dword a,b,c;

void main()
{
	b = 2;
	c = 3;
	a = EAX + b * 3;
	a = BX + b * 3;
	a = CX + b * 3;
	a = DX + b * 3;
	a = SI + b * 3;
	a = DI + b * 3;
	a = EAX + 2 * 3;
}
