#define MEMSIZE 4096
#jumptomain NONE

// The dead branch of a constant condition is skipped without being compiled.
// A statement other than a plain `x = y;` used to be cut at its first ';':
// the rest of an `else if (...) { ...; ...; }` then closed the function.

#define V 5

dword a, i, r, r2;

void main()
{
	r = 0;
	if (V > 3) r = 1; else if (a) { r2 = 2; r2 = 3; }
	if (V > 3) r = 1; else for (i = 0; i < 3; i++) r2++;
	if (V > 3) r = 1; else while (a) { a--; r2 = 5; }
	if (V > 3) r = 1; else do { r2++; } while (a);
	if (V > 3) r = 1; else if (a) r = 2; else r = 3;
	if (V > 3) r = 1; else ;
	r2 = 40;	// was swallowed by the empty else above
	if (V < 3) if (a) { r = 1; r = 2; } else r = 3;
	IF (V > 3) r = 1; ELSE IF (a) { r2 = 2; r2 = 3; }
	$ret
}
