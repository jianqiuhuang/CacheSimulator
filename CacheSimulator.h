#ifndef CACHESIMULATOR_H
#define CASHESIMULATOR_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>
#include <stdlib.h>
//Identify which cache policy to use
enum cache_policy{DIRECT_MAPPED, SET_ASSOCIATIVE, FULLY_ASSOCIATIVE, SET_ASSOCIATIVE_NO_ALLOC_ON_WRITE_MISS, SET_ASSOCIATIVE_NEXTLINE_PREFETCHING, PREFETCH_ON_MISS};

//All cache_policy simulators will return the cache miss rate in type double
class Cache_simulator{
	public:
		Cache_simulator(cache_policy type, std::string infile_name, std::string outfile_name);
		
		void simulate();
	private:
		double direct_mapped(int cache_size, int cache_line_size);

		double set_associative(int cache_size, int cache_line_size, int associativity);

		double fully_associative(int cache_size, int cache_line_size, char *replacement_policy);

		double set_associative_no_alloc_on_wirte_miss(int cache_size, int cache_line_size, int associativity);

		double set_associative_nextline_prefetching (int cache_size, int cache_line_size, int associativity);

		double prefetch_on_miss (int cache_size, int cache_line_size, int associativity);

		std::string input_file_name, output_file_name;
		
		cache_policy cache_type;

};
#endif

