# MIPS instructions

In this C++ code I've implemented basic R, I & J type instructions using MIPS style architecture.

Assumptions:
1. Memory is byte addressable
2. Has 32 registers each with 32-bit address length
3. Total memory of 65536 bit (to reduce complexities)


Following is the data path of MIPS architecture:

1. Instruction Fetch (IF) from imem.txt
2. Instruction Decode (ID) / Read Register File (RF)
3. ALU Operation execution (EX), as required by instrcutions
4. Data Memory (DM) access from dmem.txt
5. Write Back (WB) to Register File, if required


Using these above steps I implemented these instructions:

Name		Type		Opcode(Hex)	Func(Hex)
addu		R-Type 		00			21
subu 		R-Type 		00 			23
addiu 		I-Type 		09
and 		R-Type 		00 			24
or 		R-Type 		00 			25
nor 		R-Type 		00 			27
beq 		I-Type 		04
j 		J-Type 		02
lw 		I-Type 		23
sw 		I-Type 		2B
halt 		J-Type 		3F

All of the result of operation(s) are stored in RFresult.txt which is generated after each execution. Where text is appended to each execution of it.
I've attached my result file.

Different values in dmem.txt and imem.txt files ,alter names for initializing data memory and instruction memory, will result in different output in Result file.