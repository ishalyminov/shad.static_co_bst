#include <stdlib.h>
#include <9unit.h>
#include <veb.h>

int
main(int argc, char **argv)
{
	Veb L = vebnew(15);
	test(vebsop(L,1) == 1);
	test(vebsop(L,2) == 2);
	test(vebsop(L,3) == 3);
	test(vebsop(L,4) == 4);
	test(vebsop(L,5) == 8);
	test(vebsop(L,6) == 9);
	test(vebsop(L,7) == 5);
	test(vebsop(L,8) == 10);
	test(vebsop(L,9) == 11);
	test(vebsop(L,10) == 6);
	test(vebsop(L,11) == 12);
	test(vebsop(L,12) == 13);
	test(vebsop(L,13) == 7);
	test(vebsop(L,14) == 14);
	test(vebsop(L,15) == 15);
	vebfree(L);
	return 0;
}
