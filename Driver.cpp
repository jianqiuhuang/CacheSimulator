#include "CacheSimulator.h"

int main(int argc, char** argv){
	Cache_simulator test(SET_ASSOCIATIVE, "in.txt", "out.txt");
	test.simulate(8, 2, 2);
	
	return 0;
}
