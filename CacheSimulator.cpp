#include "CacheSimulator.h"

#define DEBUG 0 

Cache_simulator::Cache_simulator(cache_policy type, std::string infile_name, std::string outfile_name){
	cache_type = type;
	input_file_name = infile_name;	
	output_file_name = outfile_name;
	start_time = std::chrono::high_resolution_clock::now();
}

void Cache_simulator::simulate(int cache_size, int cache_line_size, int associativity, std::string policy){
	if(cache_type == DIRECT_MAPPED)
		std::cout << "direct-mapped: " << direct_mapped(cache_size, cache_line_size) * 100 << "%" << std::endl;
	else if(cache_type == SET_ASSOCIATIVE)
		std::cout << "set-associative: " << set_associative(cache_size, cache_line_size, associativity) * 100 << "%" << std::endl;
	else if(cache_type == FULLY_ASSOCIATIVE)
		std::cout << "fully accociative: " << fully_associative(cache_size, cache_line_size, policy) * 100 << "%" << std::endl;
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
	long long total_referenced = 0, cache_hit = 0;
	
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
			
			if(DEBUG){
				std::cerr << "the reference was a cache miss" << std::endl;
				std::cerr << "cache line with data: " << std::endl;
				for(unsigned int i = 0; i < cache[cache_index].size(); i++){
					std::cerr << cache[cache_index][i] << " ";
				}
				std::cerr << std::endl;
			}

			//populate the cache line
			for(int i = 0; i < cache_line_size; i++){
				cache[cache_index][i] = base*cache_line_size+i;
			}
			
			if(DEBUG){
				std::cerr << "Replace cache line with data: " << std::endl;
				for(unsigned int i = 0; i < cache[cache_index].size(); i++){
					std::cerr << cache[cache_index][i] << " ";
				}
				std::cerr << std::endl;
			}

		}else{
			cache_hit++;
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
	if(DEBUG) std::cout << "total referenced: " << total_referenced << " --- cache hit: " << cache_hit << std::endl;
	return (double) cache_hit / total_referenced;
}

double Cache_simulator::set_associative(int cache_size, int cache_line_size, int associativity){
	int cache_entries = cache_size / cache_line_size / associativity;
	if(DEBUG){
		std::cerr << "the cache has " << associativity << " ways and " << cache_entries << " entries in each way, and " << cache_line_size << " elements per cache line" << std::endl;
	}
	
	//Initialize cache entries
	struct info tmp;
	tmp.last_used_time = -1;
	tmp.cache_line.resize(cache_line_size, -1);

	std::vector< std::vector< struct info > > cache (associativity, std::vector < struct info> (cache_entries,  tmp));
	
	std::ifstream infile (input_file_name);
	std::string line;
	char instruction_type;
	long long address, cache_line_index, base, cache_index;
	long long total_referenced = 0, cache_hit = 0;

	while(std::getline(infile, line)){
		bool hit = false;
		total_referenced++;
		
		std::stringstream ss;
		ss << line;
		ss >> instruction_type >> std::hex >> address;
		
		if(DEBUG){
			std::cerr << "address read from file: " << instruction_type << "_ 0x" << std::hex << address << std::endl;
		}
		
		base = address / cache_line_size;
		cache_index = base % cache_entries;
		cache_line_index = address % cache_line_size;

		if(DEBUG){
			std::cerr << "referencing memory address: 0x:" << std::hex << address << std::endl;
			std::cerr << "caculated cache index at: " << cache_index << " and cache line index at: " << cache_line_index << std::endl;
		}

		for(unsigned int i = 0; i < cache.size(); i++){
			
			if(DEBUG){
				std::cerr << "Before updating:\nway " << i << ":" << std::endl;
				std::cerr << "last used time of this cache line: " <<  cache[i][cache_index].last_used_time << std::endl << "data: " << std::endl;
				for(unsigned int z = 0; z < cache[i][cache_index].cache_line.size(); z++){
					std::cerr << std::hex << cache[i][cache_index].cache_line[z] << " ";
				}
				std::cerr << std::endl;
			}
			
			if(cache[i][cache_index].cache_line[cache_line_index] == address){
				cache_hit++;
				//Cache hit -- update last_used_time
				cache[i][cache_index].last_used_time = std::chrono::duration<double, std::milli> (std::chrono::high_resolution_clock::now() - start_time).count();

				if(DEBUG){
					std::cerr << "Cache hit!!\nin way " << i <<  "\nAfter updating: " << std::endl;
					std::cerr << "last used time of this cache line: " <<  cache[i][cache_index].last_used_time << std::endl << "data: " << std::endl;;
					for(unsigned int z = 0; z < cache[i][cache_index].cache_line.size(); z++){
						std::cerr << std::hex << cache[i][cache_index].cache_line[z] << " ";
					}
					std::cerr << std::endl;
				}

				hit = true;
				break;	
			}
		}
			
		//Procedure for cache miss --- cache hit took care immediately after realize it is a hit
		if(!hit){

			if(DEBUG){
				std::cerr << "cache missed --- looking for appropriate slot to perform swap" << std::endl;	
			}

			double min = 99999999999999;
			int victim_index = -1;

			//Looking for the right slot to replace
	
			for(unsigned int i = 0; i < cache.size(); i++){
				if(cache[i][cache_index].last_used_time < min){
					min = cache[i][cache_index].last_used_time;
					victim_index = i;
				}
			}	
				
			if(DEBUG){
				std::cerr << "perform swap in way " << victim_index << std::endl;
			}

			//replacing element at victim index			
			cache[victim_index][cache_index].last_used_time = std::chrono::duration<double, std::milli> (std::chrono::high_resolution_clock::now() - start_time).count();
			for(unsigned int i = 0; i < cache[victim_index][cache_index].cache_line.size(); i++){
				cache[victim_index][cache_index].cache_line[i] = base * cache_line_size + i;
			}
			
			if(DEBUG){
				std::cerr << "after swapping, way " << victim_index << " at line " << cache_index << " contains ";
				for(unsigned int i = 0; i < cache[victim_index][cache_index].cache_line.size(); i++)
					std::cerr << "0x" << std::hex << cache[victim_index][cache_index].cache_line[i] << " ";
				std::cerr << std::endl << std::endl;
			}
		}
	}
		
		if(DEBUG) std::cerr << "total referenced: " << total_referenced << " --- cache hit: " << cache_hit << std::endl;

	infile.close();	
	return (double) cache_hit / total_referenced;
}

double Cache_simulator::fully_associative(int cache_size, int cache_line_size, std::string policy){
	double return_value = -1;
	//Least recently used
	if(policy.compare("LRU") == 0){
		return_value = set_associative(cache_size, cache_line_size, cache_size/cache_line_size);
	}
	//Hot-cold 
	else if(policy.compare("HOT-COLD") == 0){
		int cache_entries = cache_size / cache_line_size;
		if(DEBUG) std::cerr << "number of entries: " << std::dec << cache_entries << std::endl;
		//array implementation of hot-cold tree
		std::vector<int> hot_cold (cache_entries-1, 0);
		if(DEBUG) std::cerr << "size of hot_cold tree: " << hot_cold.size() << std::endl;
		struct info tmp;
		tmp.last_used_time = -1;
		tmp.cache_line.resize(cache_line_size, -1);
		if(DEBUG) std::cerr << "size of cache line: " << tmp.cache_line.size() << std::endl;
		std::vector<struct info> cache (cache_entries, tmp);
		if(DEBUG) std::cerr << "size of cache: " << cache.size() << std::endl;
		std::ifstream infile (input_file_name);
		char instruction_type;
		long long address, base;
		long long total_referenced =0, cache_hit = 0;
		std::string line;

		while(std::getline(infile, line)){
			total_referenced++;
		
			std::stringstream ss;
			ss << line;
			ss >> instruction_type >> std::hex >> address;
		
			if(DEBUG) std::cerr << "address read from file: " << instruction_type << "_0x" << std::hex << address << std::endl;
				
			base = (address / cache_line_size) * cache_line_size;
			
			bool hit = false;
			int hit_index = -1;
			int cache_line_index = address % cache_line_size;
			//Checking whether it is a cache hit or miss
			for(unsigned int i = 0; i < cache.size(); i++){
				//cache hit 
				if(cache[i].cache_line[cache_line_index] == address){
					hit = true;
					hit_index = i;
					break;
				}
			}
			int hot_cold_index = -1;
			//Hit--update hot-cold bits
			if(hit){
				if(DEBUG){
				std::cout << "before updating tree:" << std::endl;
				for(int i = 0; i < cache_entries -1; i++){
					std::cout << "at index: " << i << "--" << hot_cold[i] << " ";
				}
				std::cout << std::endl;
				}
				cache_hit++;
				//location in hot-cold tree
				hot_cold_index = hit_index + cache_entries -1;
				if(DEBUG)std::cerr << "hit!!! at index:" << hit_index << " hot_cold_index: " << hot_cold_index << std::endl;
				while(hot_cold_index != 0){
					//Backtrace the tree to update hot-cold bits
					//Right child
					if(hot_cold_index % 2 == 0){
						hot_cold_index = (hot_cold_index - 2) / 2;
						hot_cold[hot_cold_index] = 0;
					//Left child
					}else{
						hot_cold_index = (hot_cold_index - 1) / 2;
						hot_cold[hot_cold_index] = 1;
					}
					if(DEBUG) std::cerr << "Updating hot-cold bits at hot_cold_index: " << std::dec << hot_cold_index << std::endl;
				}
				if(DEBUG){
				std::cout << "after updating tree:" << std::endl;
				for(int i = 0; i < cache_entries -1; i++){
					std::cout << "at index: " << i << "--" << hot_cold[i] << " ";
				}
				std::cout << std::endl;
				}
			}
			//cache miss
			else{
				if(DEBUG) std::cerr << "Missed!!!" << std::endl;
				//Updating the hot-cold bits (binary tree)
				//Variable hot_cold_index is holding the index (tree representation) of the cache line that is going to be replaced
				int victim_index = -1;
				hot_cold_index = 0;
				for(int i = 0; i < log2(cache_entries); i++){
					if(hot_cold[hot_cold_index] == 0){
						hot_cold[hot_cold_index] = 1;
						hot_cold_index = hot_cold_index * 2 + 1;
					}else{
						hot_cold[hot_cold_index] = 0;
						hot_cold_index = hot_cold_index * 2 + 2;
					}
				}	

				//Convert hot_cold_index to 0~cache_entries
				victim_index = hot_cold_index - (cache_entries - 1);
				if(DEBUG) std::cerr << "victim at index(cache): " << std::dec << victim_index << std::endl;	
				if(DEBUG) std::cerr << "hot_cold_index: " << hot_cold_index << std::endl;
				//Putting information into designated cache line
				for(unsigned int i = 0; i < cache[victim_index].cache_line.size(); i++){
					cache[victim_index].cache_line[i] = base + i;
				}
			}
			if(DEBUG){
			std::cerr << "Verifying content in cache" << std::endl;
			for(unsigned int i = 0; i < cache.size(); i++){
				for(unsigned int j = 0; j < cache[i].cache_line.size(); j++){
					if(cache[i].cache_line[j] == -1)
						std::cerr << std::dec << cache[i].cache_line[j] << " ";
					else
						std::cerr << "0x" << std::hex << cache[i].cache_line[j] << " ";
					
				}
				std::cerr << std::endl;
			}
			}
		}
		std::cerr << "total referenced: " << total_referenced << " cache hit: " << cache_hit << std::endl;
		return_value = (double) cache_hit / total_referenced;
		infile.close();
	}else{
		std::cerr << "UNKNOW REPLACEMENT POLICY.. PLEASE USE EITHER LRU OR HOT-COLD" << std::endl;
		exit(1);
	}
	return return_value;
}

//double Cache_simulator::set_associative_no_alloc_on_write_miss(int cache_size, int cache_line_size, int associatiity){
	
//}
