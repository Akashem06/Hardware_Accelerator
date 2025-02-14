# Memory Hiearchy

## Cache
Opted for a single L1 cache for simplified design. May expand this into multiple layers like L2 and L3 cache in the future.
Opted for harvard architecture to split up instruction cache and data cache. This allows simultaneous instruction fetch and data access for better pipelining.

Instruction cache
Size: 32kb
Line size: 64 bytes (512 bits per line)
Associativity: 2-way set-associative
Replacement Rules: LRU-Cache

## CPU Frontend
Typical applications have these ports:
- Address port = Provides address of the instruction/data to fetch
- Data port = Reading or writing data, this is only for data caches
- Control port = Read/write, invalidate and flush signals etc.
- Hit/Miss signal = Indicate if cache miss has occurred or not

## Memory Backend
- Address port = Requests missing instruction/data from main memory
- Data port = Receives fetched instructions from memory
- Control port = Handles memory access requests

### Cache Design Considerations
- Fully associative cache (Resource: https://www.youtube.com/watch?v=A0vR-ks3hsQ) **NOT USED**
  - Structure: Any memory block can be placed in any cache line. Tons of flexibility
  - Pros:
    - Eliminates conflict misses completely
    - Maximizes cache hit rate
  - Cons:
    - Requires a complex replacement policy (e.g., LRU, FIFO, Random)
    - Higher power consumption due to searching all cache lines in parallel (Fetches block ID from address and checks all tags in parallel)
    - Increased access time and hardware cost

- Direct-mapped cache (Resource: https://www.youtube.com/watch?v=zocwH0g-qQM) **NOT USED**
  - Structure: Each memory block maps to exactly one cache line. No flexibility
  - Pros:
    - Simplest to implement (requires only an index lookup and tag comparison)
    - No replacement policy!
    - Fastest access time due to single-line mapping (Really easy to lookup, get offset, get the line number, see if the tag is the same. If not, its a miss)
  - Cons:
    - High chance of having a conflict miss. This is when multiple blocks map to the same cache location 
      causing wasted space and frequent replacements. Typically causes thrashing (rapid allocation/deallocation in cache)
      - What if two blocks are fighting over the same line? We have two perfectly spaced blocks (Depends on the tag), they will be swapped in and out as they are accessed
    - Well you could increase the number of lines to reduce the number of blocks with the same line number! But this is wasted space
  - Lets not use this :)

- Set-associative cache (Resource: https://www.youtube.com/watch?v=gr5M9CULUZw) **USED**
  - Structure: Divides the cache into N-way sets, where each memory block can be placed in any line within a specific set
    - Instead of line IDs like direct-mapped caches, we have set IDs. For each set ID, there are N slots available for different tags
    - This means N different memory addresses with the same Set ID can be stored in the cache
  - Pros:
    - Reduces conflict misses compared to direct-mapped caches because of N number of addresses being stored
    - Balances performance and complexity
    - Uses a simpler replacement policy than fully associative caches, since you only need to compare N number of addresses
      - For example a 2-way associatve cache, we only need to check 2 addresses for LRU/FIFO/etc.
  - Cons:
    - Higher hardware complexity compared to direct-mapped caches
    - Still susceptible to some conflict misses, depending on associativity level

- Additional stuff I added
  - Flags for each line of cache
    - ONLY FOR UNIFIED CACHES. NOT USED IN MY DESIGN: Type bit (Data/Instruction)
      - When instruction loads in, I must decode it differently. Instructions are read only
      - Data doesn't get decoded, and can be written to
    - Valid bit (If the line has valid data)
      - When you start up the processor, everything should be invalid
      - If the line of the cache has updated data, and the main memory hasn't been updated, we must update main memory before invalidating line
    - Lock bit
      - Lock a line of cache to prevent the cache from being replaced
  - Data:
    - Dirty bit
      - Identifies a line of **data** that has been written to, but not updated in main memory
    - Write policies:
      - Write through:
        - Every write to the cache will immediately writes through to the main memory
        - This is really important for multi-core systems since every core may rely on main memory for validating personal caches
      - Write back:
        - Update main memory only when a dirty line is to be replaced
        - Reduces bus traffic greatly, but might cause issues with shared data
  - Instructions:
    - Once instructions are fetched it must be decoded based on ISA spec

CACHE SELECTION: 2-way associatve cache. Opted for this because it has the best of both worlds, it is fast, but isn't too expensive
