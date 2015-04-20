#include "CacheSimulator.h"

const static int DEBUG = 1;

Cache_simulator::Cache_simulator(cache_policy type, std::string infile_name, std::string outfile_name){
	cache_type = type;
	input_file_name = infile_name;	
	output_file_name = outfile_name;
}

void Cache_simulator::simulate(){
	if(cache_type == DIRECT_MAPPED)
		std::cout << "cache missed rate: " << direct_mapped(6, 2) * 100 << "%" << std::endl;
	else{
		std::cerr << "Unknown cache type" << std::endl;
		exit(1);
	}
}

double Cache_simulator::direct_mapped(int cache_size, int cache_line_size){
	int cache_entries = cache_size / cache_line_size;
	if(DEBUG){
		std::cerr << "the cache has " << cache_entries << " entries" << std::endl;
	}
	//Initialize cache entries
	std::vector<std::vector<long long> > cache (cache_entries, std::vector<long long> (cache_line_size, -1)); 
		
	std::ifstream infile (input_file_name);
	std::string line;
	char instruction_type;
	long long address, cache_line_index, base, cache_index;
	long long total_referenced = 0, cache_missed = 0;
	
	while(std::getline(infile, line)){
		total_referenced++;

		std::stringstream ss;
		ss << line;	
		ss >> instruction_type >> std::hex >>  address;
		if(DEBUG){
			std::cerr << "address read from file: " << instruction_type << "_ 0x" << std::hex << address << std::endl;		
		}
	
		base = address / cache_line_size;
		cache_index = base % cache_entries;
		cache_line_index = address % cache_line_size;
		
		if(DEBUG){
			std::cerr << "referencing memory address: 0x" << std::hex << address << std::endl;
			std::cerr << "caculated cache index at: " << cache_index << " and cache line index at: " << cache_line_index << std::endl;
		}
		
		//cache missed
		if(cache[cache_index][cache_line_index] != address){
			cache_missed++;
			//populate the cache line
			std::vector<long long> tmp;
			for(int i = 0; i < cache_line_size; i++){
				tmp.push_back( base*cache_line_size+i );
			}
			
			if(DEBUG){
				std::cerr << "the reference was a cache miss" << std::endl;
				std::cerr << "cache line with data: " << std::endl;
				for(unsigned int i = 0; i < cache[cache_index].size(); i++){
					std::cerr << cache[cache_index][i] << " ";
				}
				std::cerr << std::endl << "replaced cache line with data: " << std::endl;
				for(unsigned int i = 0; i < tmp.size(); i++){
					std::cerr << tmp[i] << " ";
				}
				std::cerr << std::endl;
			}

			cache[cache_index] = tmp;	
		}else{
			if(DEBUG){
				std::cerr << "the reference was a cache hit" << std::endl;
				std::cerr << "cache line with data: " << std::endl;
				for(unsigned int i = 0; i < cache[cache_index].size(); i++){
					std::cerr << cache[cache_index][i] << " ";
				}
				std::cerr << std::endl;
			}
		}
	}
	infile.close();
	std::cout << "total referenced: " << total_referenced << " --- cache missed: " << cache_missed << std::endl;
	return (double) cache_missed / total_referenced;
}
