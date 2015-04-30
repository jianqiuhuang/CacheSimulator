#include "CacheSimulator.h"

int main(int argc, char** argv){
	if(argc != 2){
		std::cerr << "USAGE: ./cache_simulator <input_file_name>" << std::endl;
		exit(1);
	}
	int cache_line_size = -1, cache_size = -1, associativity = -1;
	std::string policy = "";

	Cache_simulator direct_mapped(DIRECT_MAPPED, argv[1], "out.txt");
	cache_line_size = 32;
	int cache_size_array[] = {1024, 4096, 16384, 32768};

//	for(int i = 0; i < 4; i++){
//		direct_mapped.simulate(cache_size_array[i], cache_line_size, associativity, policy);
//	}	

	Cache_simulator set_associative(SET_ASSOCIATIVE, argv[1], "out.txt");
//	int associativity_array[] = {2, 4, 8, 16};
//	cache_size = 16384;
//	for(int i = 0; i < 4; i++){
//		set_associative.simulate(cache_size1, cache_line_size, associativity_array[i], policy);
//	}	

	Cache_simulator fully_associative(FULLY_ASSOCIATIVE, argv[1], "out.txt");
	cache_line_size = 32;
	cache_size = 16384;
//	policy = "LRU";
//	fully_associative.simulate(cache_size, cache_line_size, associativity, policy);
	policy = "HOT-COLD";
	fully_associative.simulate(cache_size, cache_line_size, associativity, policy);
//	fully_associative.simulate(4, 1, -1, "HOT-COLD");
	return 0;
}
