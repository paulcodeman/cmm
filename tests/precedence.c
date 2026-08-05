#define MEMSIZE 4096

#jumptomain NONE

dword a,b,c,d,e,f;

void main()
{
	a = 29;
	b = 5;
	c = 3;
	d = 2;
	e = 7;
	f = 11;

	p01_raw(); p01_ref();
	p02_raw(); p02_ref();
	p03_raw(); p03_ref();
	p04_raw(); p04_ref();
	p05_raw(); p05_ref();
	p06_raw(); p06_ref();
	p07_raw(); p07_ref();
	p08_raw(); p08_ref();
	p09_raw(); p09_ref();
	p10_raw(); p10_ref();
	p11_raw(); p11_ref();
	p12_raw(); p12_ref();
	p13_raw(); p13_ref();
	p14_raw(); p14_ref();
	p15_raw(); p15_ref();
	p16_raw(); p16_ref();
	p17_raw(); p17_ref();
	p18_raw(); p18_ref();
	p19_raw(); p19_ref();
	p20_raw(); p20_ref();
	p21_raw(); p21_ref();
	p22_raw(); p22_ref();
	p23_raw(); p23_ref();
	p24_raw(); p24_ref();
	p25_raw(); p25_ref();
	p26_raw(); p26_ref();
	p27_raw(); p27_ref();
	p28_raw(); p28_ref();
	p29_raw(); p29_ref();
	p30_raw(); p30_ref();
	p31_raw(); p31_ref();
	p32_raw(); p32_ref();
	p33_raw(); p33_ref();
	p34_raw(); p34_ref();
	p35_raw(); p35_ref();
	$ret
}

:dword p01_raw()
{
	return a + b * c;
}

:dword p01_ref()
{
	return a + (b * c);
}

:dword p02_raw()
{
	return a - b / c;
}

:dword p02_ref()
{
	return a - (b / c);
}

:dword p03_raw()
{
	return a + b % c;
}

:dword p03_ref()
{
	return a + (b % c);
}

:dword p04_raw()
{
	return a * b + c;
}

:dword p04_ref()
{
	return (a * b) + c;
}

:dword p05_raw()
{
	return a / b * c;
}

:dword p05_ref()
{
	return (a / b) * c;
}

:dword p06_raw()
{
	return a - b + c;
}

:dword p06_ref()
{
	return (a - b) + c;
}

:dword p07_raw()
{
	return a << b + c;
}

:dword p07_ref()
{
	return a << (b + c);
}

:dword p08_raw()
{
	return a + b << c;
}

:dword p08_ref()
{
	return (a + b) << c;
}

:dword p09_raw()
{
	return a >> b - d;
}

:dword p09_ref()
{
	return a >> (b - d);
}

:dword p10_raw()
{
	return a & b + c;
}

:dword p10_ref()
{
	return a & (b + c);
}

:dword p11_raw()
{
	return a + b & c;
}

:dword p11_ref()
{
	return (a + b) & c;
}

:dword p12_raw()
{
	return a ^ b & c;
}

:dword p12_ref()
{
	return a ^ (b & c);
}

:dword p13_raw()
{
	return a & b ^ c;
}

:dword p13_ref()
{
	return (a & b) ^ c;
}

:dword p14_raw()
{
	return a | b ^ c;
}

:dword p14_ref()
{
	return a | (b ^ c);
}

:dword p15_raw()
{
	return a ^ b | c;
}

:dword p15_ref()
{
	return (a ^ b) | c;
}

:dword p16_raw()
{
	return -a * b;
}

:dword p16_ref()
{
	return (-a) * b;
}

:dword p17_raw()
{
	return a * -b + c;
}

:dword p17_ref()
{
	return (a * (-b)) + c;
}

:dword p18_raw()
{
	return a + b * c - d / e % f;
}

:dword p18_ref()
{
	return (a + (b * c)) - ((d / e) % f);
}

:dword p19_raw()
{
	return (a + b) * (c - d);
}

:dword p19_ref()
{
	return (a + b) * (c - d);
}

:dword p20_raw()
{
	return a & b + c | d;
}

:dword p20_ref()
{
	return (a & (b + c)) | d;
}

:dword p21_raw()
{
	return a ^ b & c | d;
}

:dword p21_ref()
{
	return (a ^ (b & c)) | d;
}

:dword p22_raw()
{
	return a << b + c & d;
}

:dword p22_ref()
{
	return (a << (b + c)) & d;
}

:dword p23_raw()
{
	return a >> b - d ^ c;
}

:dword p23_ref()
{
	return (a >> (b - d)) ^ c;
}

:dword p24_raw()
{
	return a & b + c & d;
}

:dword p24_ref()
{
	return (a & (b + c)) & d;
}

:dword p25_raw()
{
	return a << b + c << d;
}

:dword p25_ref()
{
	return (a << (b + c)) << d;
}

:dword p26_raw()
{
	return a - b / c + d;
}

:dword p26_ref()
{
	return (a - (b / c)) + d;
}

:dword p27_raw()
{
	return a | b & c ^ d;
}

:dword p27_ref()
{
	return a | ((b & c) ^ d);
}

:dword p28_raw()
{
	return a ^ b + c & d;
}

:dword p28_ref()
{
	return a ^ ((b + c) & d);
}

:dword p29_raw()
{
	return a & b << c | d;
}

:dword p29_ref()
{
	return (a & (b << c)) | d;
}

:dword p30_raw()
{
	return a | b << c & d;
}

:dword p30_ref()
{
	return a | ((b << c) & d);
}

:dword p31_raw()
{
	return a & (b + c | d);
}

:dword p31_ref()
{
	return a & ((b + c) | d);
}

:dword p32_raw()
{
	return a << (b + c & d);
}

:dword p32_ref()
{
	return a << ((b + c) & d);
}

:dword p33_raw()
{
	return a & b + (c | d) | e;
}

:dword p33_ref()
{
	return (a & (b + (c | d))) | e;
}

:dword p34_raw()
{
	return a << b + (c & d) & e;
}

:dword p34_ref()
{
	return (a << (b + (c & d))) & e;
}

:dword p35_raw()
{
	if (((a - b) / c) + d == 10) return 1;
	return 0;
}

:dword p35_ref()
{
	if ((((a - b) / c) + d) == 10) return 1;
	return 0;
}
