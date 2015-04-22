#include "CacheSimulator.h"

int main(int argc, char** argv){
	if(argc != 2){
		std::cerr << "USAGE: ./cache_simulator <input_file_name>" << std::endl;
		exit(1);
	}
	
	Cache_simulator direct_mapped(DIRECT_MAPPED, argv[1], "out.txt");
	int cache_line_size = 32, cache_size[] = {1024, 4096, 16384, 32768};

//	for(int i = 0; i < 4; i++){
//		direct_mapped.simulate(cache_size[i], cache_line_size, 0);
//	}	

	Cache_simulator set_associative(SET_ASSOCIATIVE, argv[1], "out.txt");
	int associativity[] = {2, 4, 8, 16};
	int cache_size1 = 16384;
	for(int i = 0; i < 4; i++){
		set_associative.simulate(cache_size1, cache_line_size, associativity[i]);
	}	

//	set_associative.simulate(4, 2, 2);
	return 0;
}
