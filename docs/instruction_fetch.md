# Instruction Fetch Unit

## CPU Frontend
- Branch taken signal
- Branch target instruction
## Cache Backend
- Program counter output for instruction request
- Fetch request signal
- Fetched instruction input from cache (Will be forwarded to 'fetch_instruction' through prefetch buffer)
- Cache hit signal
## Pipeline Backend
- Pipeline stall input signal
- Fetched instruction output for instruction decoder
- Valid fetch signal

## Detailed Architecture

### Program Counter (PC) Management
- **Reset and Initialization:**  
  - Active-low reset
  - Resets the program counter, prefetch write/read pointer and the valid bitmap
- **Sequential PC Update:**  
  - Increment logic based on instruction word size
- **Branch Resolution and Target Selection:**  
  - Immediate PC update upon branch signal assertion
  - In the future I could do integration with branch prediction units
- **Stall Handling:**  
  - Preventing PC updates, instruction fetch request, and buffering prefetched instructions during pipeline stalls

### 2.2. Prefetch Buffer Design
- **Circular Buffer Implementation:**  
  - Configurable prefetch depth (e.g., parameterized by `PREFETCH_DEPTH`)
  - Management of write and read pointers with automatic wrap-around
- **Valid Bit Management:**  
  - Use of a bitmap to track valid entries
  - Read/write pointers help avoid race conditions between read and write operations
