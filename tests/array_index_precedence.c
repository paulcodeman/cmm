#define MEMSIZE 4096

#jumptomain NONE

struct _image
{
	dword width;
	dword values[64];
	void set_pixel();
	void set_pixel_ref();
	dword get_pixel();
	dword get_pixel_ref();
} image;

dword result, result_ref;

void _image::set_pixel(dword row, column, color)
{
	values[width * row + column] = color;
}

void _image::set_pixel_ref(dword row, column, color)
{
	values[((width * row) + column)] = color;
}

dword _image::get_pixel(dword row, column)
{
	return values[width * row + column];
}

dword _image::get_pixel_ref(dword row, column)
{
	return values[((width * row) + column)];
}

void main()
{
	image.width = 8;
	image.set_pixel(2, 3, 0x12345678);
	image.set_pixel_ref(2, 3, 0x12345678);
	result = image.get_pixel(2, 3);
	result_ref = image.get_pixel_ref(2, 3);
	$ret
}
