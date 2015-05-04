#include "CacheSimulator.h"

int main(int argc, char** argv){
	if(argc != 3){
		std::cerr << "USAGE: ./cache_simulator <input_file_name> <output-file-name>" << std::endl;
		exit(1);
	}

	std::ofstream outfile(argv[2]);

	int cache_line_size = -1, cache_size = -1, associativity = -1;
	std::string policy = "";

	Cache_simulator direct_mapped(DIRECT_MAPPED, argv[1]);
	cache_line_size = 32;
	int cache_size_array[] = {1024, 4096, 16384, 32768};

	for(int i = 0; i < 4; i++){
		outfile << direct_mapped.simulate(cache_size_array[i], cache_line_size, associativity, policy);
		outfile <<  " " << direct_mapped.get_total_referenced() << " ";
	}	
	outfile << std::endl;

	Cache_simulator set_associative(SET_ASSOCIATIVE, argv[1]);
	int associativity_array[] = {2, 4, 8, 16};
	cache_size = 16384;
	for(int i = 0; i < 4; i++){
		outfile << set_associative.simulate(cache_size, cache_line_size, associativity_array[i], policy);
		outfile << " " << set_associative.get_total_referenced() << " ";
	}	
	outfile << std::endl;

	Cache_simulator fully_associative(FULLY_ASSOCIATIVE, argv[1]);
	policy = "LRU";
	outfile << fully_associative.simulate(cache_size, cache_line_size, associativity, policy);
	outfile << " " << fully_associative.get_total_referenced() << std::endl;
	policy = "HOT-COLD";
	outfile << fully_associative.simulate(cache_size, cache_line_size, associativity, policy);
	outfile << " " << fully_associative.get_total_referenced() << std::endl;

	Cache_simulator set_associative_no_alloc_write_miss(SET_ASSOCIATIVE_NO_ALLOC_ON_WRITE_MISS, argv[1]);
	for(int i = 0; i < 4; i++){
		outfile << set_associative_no_alloc_write_miss.simulate(cache_size, cache_line_size, associativity_array[i], policy);
		outfile << " " << set_associative_no_alloc_write_miss.get_total_referenced() << " ";
	}	
	outfile << std::endl;
	
	Cache_simulator set_associative_nextline_prefetching(SET_ASSOCIATIVE_NEXTLINE_PREFETCHING, argv[1]);
	for(int i = 0; i < 4; i++){
		outfile << set_associative_nextline_prefetching.simulate(cache_size, cache_line_size, associativity_array[i], policy);
		outfile << " " << set_associative_nextline_prefetching.get_total_referenced() << " ";
	}	
	outfile << std::endl;

	Cache_simulator prefetch_on_miss(PREFETCH_ON_MISS, argv[1]);
	for(int i = 0; i < 4; i++){
		outfile << prefetch_on_miss.simulate(cache_size, cache_line_size, associativity_array[i], policy);
		outfile << " " << prefetch_on_miss.get_total_referenced() << " ";
	}	
	outfile << std::endl;

//Extra credit
	Cache_simulator test(FIFO, argv[1]);
	std::cout << "Extra credit: FIFO replacement\n" << "Cache hit: " << test.simulate(16*1024, 32, -1, "") << ", total referenced: " << test.get_total_referenced() << std::endl;
	outfile.close();
	return 0;
}
