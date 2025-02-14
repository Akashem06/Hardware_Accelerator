# Matrix Acceleration ISA

## 1. Register File
- 16 General Purpose Registers (R0-R15)
- 4 Special Purpose Registers:
  - SAC (Systolic Array Control)
  - MAR (Memory Address Register)
  - MDR (Memory Data Register)
  - STATUS (Contains flags: busy, error, overflow)

## 2. Instruction Format
32-bit fixed width instructions:
[31:28] - Opcode (4 bits)
[27:24] - Destination Register (4 bits)
[23:20] - Source Register 1 (4 bits)
[19:16] - Source Register 2 (4 bits)
[15:0]  - Immediate/Offset (16 bits)

## 3. Instruction Set

### Matrix Operations
- MLOAD   - Load matrix from memory to systolic array
  Format: MLOAD Rd, Rs1, imm    // Load matrix starting at address in Rs1, size specified by imm
  
- MSTORE  - Store matrix from systolic array to memory
  Format: MSTORE Rd, Rs1, imm   // Store matrix starting at address in Rs1, size in imm
  
- MMULT   - Matrix multiplication
  Format: MMULT Rd, Rs1, Rs2    // Multiply matrices specified by Rs1 and Rs2
  
- MADD    - Matrix addition
  Format: MADD Rd, Rs1, Rs2     // Add matrices specified by Rs1 and Rs2

### DMA Operations
- DMA_ST  - Start DMA transfer
  Format: DMA_ST Rd, Rs1, Rs2   // Transfer from Rs1 to Rs2, size in Rd
  
- DMA_CHK - Check DMA status
  Format: DMA_CHK Rd            // Store status in Rd

### Control Operations
- CONF    - Configure systolic array
  Format: CONF Rd, imm          // Configure array dimensions/mode
  
- SYNC    - Synchronize operations
  Format: SYNC                  // Wait for all operations to complete

### Memory Operations
- LD      - Load word
  Format: LD Rd, Rs1, imm      // Load from memory at Rs1 + imm to Rd
  
- ST      - Store word
  Format: ST Rs1, Rs2, imm     // Store Rs1 to memory at Rs2 + imm

## 4. Opcode Map
0000 - MLOAD
0001 - MSTORE
0010 - MMULT
0011 - MADD
0100 - DMA_ST
0101 - DMA_CHK
0110 - CONF
0111 - SYNC
1000 - LD
1001 - ST

## 5. Status Register Bits
[0] - Busy Flag
[1] - Error Flag
[2] - Overflow Flag
[3] - DMA Active
[4] - Matrix Operation Complete
[5:31] - Reserved

## 6. Example Program
Multiply two 4x4 matrices

CONF    R0, 0x0404    // Configure 4x4 systolic array
MLOAD   R1, R2, 16    // Load first matrix
MLOAD   R2, R3, 16    // Load second matrix
MMULT   R4, R1, R2    // Multiply matrices
SYNC                  // Wait for completion
MSTORE  R4, R5, 16    // Store result
