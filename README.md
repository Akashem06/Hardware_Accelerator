# Hardware Accelerator [WIP]

This is a mega project I really wanted to take on. I think the future of hardware/embedded will be in hardware acceleration.
Being able to speed up AI processes, common mathematical operations and more will be key!

Here are the goals:
- System verilog simulation of the hardware accelerator
  - This needs to avoid stalls like the plague, and be as fast as possible. The memory hiearchy will play a huge role
  - This will also need to support DMA for rapid memory transfers
  - The systolic array will perform matrix multiplications hopefully at a rapid speed!
- Defining a custom ISA for Systolic array operations
- Creating a custom compiler to work with my hardware accelerator

Resources:
ECE320 Waterloo Notes + Textbook
