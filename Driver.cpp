#include "CacheSimulator.h"

int main(int argc, char** argv){
	Cache_simulator test(DIRECT_MAPPED, "in.txt", "out.txt");
	test.simulate();
	
	return 0;
}
