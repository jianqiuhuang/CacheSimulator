#ifndef CACHESIMULATOR_H
#define CACHESIMULATOR_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <chrono>
#include <math.h>
#include <iomanip>
//Identify which cache policy to use
enum cache_policy{DIRECT_MAPPED, SET_ASSOCIATIVE, FULLY_ASSOCIATIVE, SET_ASSOCIATIVE_NO_ALLOC_ON_WRITE_MISS, SET_ASSOCIATIVE_NEXTLINE_PREFETCHING, PREFETCH_ON_MISS};

struct info{
	double last_used_time;
	std::vector<long long> cache_line;
};

//All cache_policy simulators will return the cache miss rate in type double
class Cache_simulator{
	public:
		Cache_simulator(cache_policy type, std::string infile_name);
		
		long long simulate(int cache_size, int cache_line_size, int ways, std::string policy);

		long long get_total_referenced();
	private:
		long long direct_mapped(int cache_size, int cache_line_size);

		long long set_associative(int cache_size, int cache_line_size, int associativity);

		long long fully_associative(int cache_size, int cache_line_size, std::string replacement_policy);

		long long set_associative_no_alloc_on_write_miss(int cache_size, int cache_line_size, int associativity);

		long long set_associative_nextline_prefetching (int cache_size, int cache_line_size, int associativity);

		long long prefetch_on_miss (int cache_size, int cache_line_size, int associativity);

		long long total_referenced;		

		std::string input_file_name;
		
		cache_policy cache_type;
		
		std::chrono::high_resolution_clock::time_point start_time;
};
#endif

