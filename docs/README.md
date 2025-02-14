# Project Overview

## Hardware Accelerator Pipeline Architecture

### 1. Pipeline Stages & Components (In Implementation Order)

#### Stage 1: Instruction Frontend
Components:
- Instruction Cache (I-Cache)
  - Direct-mapped, 32KB
  - Single-cycle access for common instructions
- Instruction Fetch Unit
  - Branch prediction unit
  - Prefetch buffer (4 instructions)
- Instruction Decoder
  - Parallel decode of matrix vs scalar operations

#### Stage 2: Matrix Operation Scheduler
Components:
- Matrix Operation Queue (MOQ)
  - 8-entry deep queue
  - Tracks dependencies between operations
- DMA Controller
  - Multiple channels (4x)
  - Address generation unit
  - Scatter-gather support
- Memory Management Unit
  - Page table walker
  - TLB for matrix operations

#### Stage 3: Systolic Array Control
Components:
- Systolic Array Controller
  - Configuration registers
  - State machine for operation sequencing
- Data Router
  - Crossbar switch for data distribution
  - Buffer management
- Weight Buffer
  - Double-buffered for continuous operation
  - Direct path to systolic array

#### Stage 4: Execution Units
Components:
- Main Systolic Array (16x16)
  - Processing Elements (PEs) with FP32/INT8 support
  - Local accumulation registers
  - Bypass networks
- Scalar ALU
  - Basic arithmetic operations
  - Address calculation unit
- Matrix Load/Store Unit
  - Address generation
  - Data alignment logic

#### Stage 5: Memory Hierarchy
Components:
- L1 Matrix Cache
  - Scratchpad memory (256KB)
  - Multiple banks for parallel access
- DMA Engine
  - Burst transfer support
  - Streaming buffers
- Memory Interface
  - AXI4 interface
  - Quality of Service (QoS) management
