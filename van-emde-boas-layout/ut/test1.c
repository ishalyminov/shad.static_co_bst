#include <stdlib.h>
#include <9unit.h>
#include <veb.h>

int
main(int argc, char **argv)
{
	Veb L = vebnew(15);
	test(vebpos(L,1) == 1);
	test(vebpos(L,2) == 2);
	test(vebpos(L,3) == 3);
	test(vebpos(L,4) == 4);
	test(vebpos(L,5) == 7);
	test(vebpos(L,6) == 10);
	test(vebpos(L,7) == 13);
	test(vebpos(L,8) == 5);
	test(vebpos(L,9) == 6);
	test(vebpos(L,10) == 8);
	test(vebpos(L,11) == 9);
	test(vebpos(L,12) == 11);
	test(vebpos(L,13) == 12);
	test(vebpos(L,14) == 14);
	test(vebpos(L,15) == 15);
	vebfree(L);
	return 0;
}

