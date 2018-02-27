### REPORT

 * Cache simulator
 * GROUP MEMBERS:
 * Kaif Dauwa
 * Arya HajiTaheri
 * Vinh  Lam
 *CODE LANGUAGE: C++


****************************************************************************************************************************


**This C++ project simulates a 32kb L1 cache. The user decides its writing policies and associativity. The cache block is 32/64 byte.**

**The addresses from the provided cache block is converted to binary and then separated to the corresponding bit value (tag, index, offset).** 

**We used a dynamic 2d array to simulate the cache and used the trace file, or the main memory, to fil the cache with the desired addresses.** 

**The final cache content is dependent on the initial variables that the user chose. Not all cache settings were optimal for each trace file.**

****************************************************************************************************************************

 OPTIMAL SETTING			              	associativity     |	cache block size    |Replacement policy| Write policy | accuracy(%)

* optimal settings for swim.trace: 2-way set associative| 64 byte cache block |	FIFO	|  writeback  | 55.709%

* optimal settings for twolf.trace:2-way set associative| 32 byte cache block |	LRU		|  writeback  | 81.147%

* optimal settings for gcc.trace: 2-way set associative | 64 byte cache block |	LRU		|  writeback  | 87.586%

* optimal settings for gzip.trace:2-way set associative | 32 byte cache block |	LRU		| writethrough| 33.598%

* optimal settings for mcf.trace: 2-way set associative | 32 byte cache block |	LRU		|  writeback  | 87.477%
