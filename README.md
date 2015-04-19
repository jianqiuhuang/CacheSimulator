# CacheSimulator

The goal of this project is to measure the effectiveness of various cache subsystem organizations using the traces of memory instructions obtained from the realistic programs. Each trace contains 10 million memory instructions with two values provided for each instruction: a flag indicating whether this is a load or a store (L stands for a load, S stands for a store), and the byte memory address targeted by this instruction. Three traces are provided, each corresponding to one of the SPEC 2006 benchmarks.   Each trace name is annotated with the corresponding benchmark name.
Your goal is to write a program in C or C++ that would use these traces to measure the cache hit rate of various data cache organizations and prefetching techniques (note: we are not estimating the instruction cache performance in this project, only the data cache).. Specifically, the following cache designs have to be implemented. 

1)	Direct-Mapped Cache. Assume that each cache line has a size of 32 bytes and model the caches sized at 1KB, 4KB, 16KB and 32KB
#
2)	Set-Associative Cache. Again, assume that the cache line size is 32 bytes and model a 16KB cache with associativity of 2, 4, 8 and 16. Assume that the least recently used (LRU) replacement policy is implemented.
#
3)	Fully-Associative cache.  Assume that each cache line is 32 bytes and the total cache size is 16KB. Implement Least Recently Used (LRU) and hot-cold LRU approximation policies.  
#
4)	Set-Associative Cache with no Allocation on a Write Miss. In this design, if a store instruction misses into the cache, then the missing line is not written into the cache, but instead is written directly to memory. Evaluate this design for the same configurations as in question (2) above.  
#
5)	Set-Associative Cache with Next-line Prefetching. In this design, the next memory line will be brought into the cache with every cache access. For example, if current access is to line X, then line (x+1) is also brought into the cache, replacing the cache’s previous content. Evaluate this design for the same configurations as in question (2) above. 
#
6)	Prefetch-on-a-Miss. This is similar to part (5) above, but prefetching is only triggered on a cache miss.
Extra credit problem [30%]: Propose and implement a new cache replacement or prefetching mechanism that outperforms either the LRU replacement or the next-line prefetcher. Evaluate your proposal on the designs listed in question (2) above. Feel free to use any supplementary literature that you can find on this subject.

