# ESE501
Digital System Specification and Modeling Using SystemC

## Project 1
### Date: 3/13/19
Author: Anubhav Tomar

Description: MINI RISC Processorr Archetichure with 5-Stage Pipeline Execution

Language: SystemC

#### Files:
* [PM.h](https://github.com/anubhavtomar/ESE501/blob/master/Project1/PM.h) - Program Memory stores program

* [DM.h](https://github.com/anubhavtomar/ESE501/blob/master/Project1/DM.h) - Data Memory (For Load and Store Instructions)

* [RF.h](https://github.com/anubhavtomar/ESE501/blob/master/Project1/RF.h) - Register File (Register Bank)

* [ALU.h](https://github.com/anubhavtomar/ESE501/blob/master/Project1/ALU.h) - Execution module that handles executing all arithmatics and logical instructions

* [controller.cpp](https://github.com/anubhavtomar/ESE501/blob/master/Project1/controller.cpp) - FSM is used to determine what stage shoudl be executed. Instruction queue is used to store and handle multiple instructions in different stages. Generate control signals for all the modules. Synchorization done using ```wait``` in main FSM thread.

* [testbench.cpp](https://github.com/anubhavtomar/ESE501/blob/master/Project1/testbench.cpp) - Initilizes all modules, interconnect port of different modules using signals. Generate clock signals. Load the program memory and start simulation.

* [log.txt](https://github.com/anubhavtomar/ESE501/blob/master/Project1/log.txt) - Log File logged at different stages in the pipelne while execution.


```
5-Stages 
1st Rising edge : Read Instrution from PM using PC and store the instruction in IR(Instruction Register)
1st Falling : Decode IR and generate required control signals
2nd Rising : Read registers from RF
3rd Rising : ALU execution
4th Rising : MAR | MDR (address output | data output) [DM access] (if needed)
5th Rising : Write back to RF (if needed)
Program used for Testing the Pipeline:
```
```
      OPCODE       // INSTRUCTION
0b1101000000000000 // MVI 0 , A
0b1101000100000001 // MVI 1 , B
0b1101001000001010 // MVI 10 , C
0b0000000001010001 // ADD B , A
0b0101000100000001 // ADI 1 , B
0b1001001000000001 // SUBI 1 , C
0b1011001000000000 // CMPI 0 , C
0b0100000111000011 // JNZ 3
0b0000000000000000 // NOP
```

```NOP``` is used as HALT instructin to stop fetching further instructions. PC is not incremented and instruction queuing is stopped in controller.

The program is compiled and executed using ```sh compile.sh```.
The results are logged in ```log.txt```
