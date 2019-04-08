# ESE501
Digital System Specification and Modeling Using SystemC

## Project 1
### Date: 3/13/19
Author: Anubhav Tomar

Description: RTL Level Modeling of a 16-bit MINI RISC Processor Architecture with 5-Stage Pipeline Execution

Language: SystemC

#### Files:
* [PM.h](https://github.com/anubhavtomar/ESE501/blob/master/Project1/PM.h) - Program Memory stores program

* [DM.h](https://github.com/anubhavtomar/ESE501/blob/master/Project1/DM.h) - Data Memory (For Load and Store Instructions)

* [RF.h](https://github.com/anubhavtomar/ESE501/blob/master/Project1/RF.h) - Register File (Register Bank)

* [ALU.h](https://github.com/anubhavtomar/ESE501/blob/master/Project1/ALU.h) - Execution module that handles executing all arithmetic and logical instructions

* [controller.cpp](https://github.com/anubhavtomar/ESE501/blob/master/Project1/controller.cpp) - FSM is used to determine what stage should be executed. Instruction queue is used to store and handle multiple instructions in different stages. Generate control signals for all the modules. Synchronization is done using ```wait``` in the main FSM thread.

* [testbench.cpp](https://github.com/anubhavtomar/ESE501/blob/master/Project1/testbench.cpp) - Initializes all modules, interconnect port of different modules using signals. Generate clock signals. Load the program memory and start the simulation.

* [log.txt](https://github.com/anubhavtomar/ESE501/blob/master/Project1/log.txt) - Log File logged at different stages in the pipeline while execution.


```
5-Stages 
1st Rising Edge : Read Instruction from PM using PC and store the instruction in IR(Instruction Register)
1st Falling Edge : Decode IR and generate required control signals
2nd Rising Edge : Read registers from RF
3rd Rising Edge : ALU execution
4th Rising Edge : MAR | MDR (address output | data output) [DM access] (if needed)
5th Rising Edge : Write back to RF (if needed)
```
#### Program used for Testing the Pipeline:
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

```NOP``` is used as HALT instruction to stop fetching further instructions. PC is not incremented and instruction queuing is stopped in the controller.

The program is compiled and executed using ```sh compile.sh```.
The results are logged in ```log.txt```


## Project 2
### Date: 4/7/19
Author: Anubhav Tomar

Description: Robot Navigation in Confined Areas 

Language: SystemC

#### Files:
* [PM.h](https://github.com/anubhavtomar/ESE501/blob/master/Project1/PM.h) - Program Memory stores program

* [DM.h](https://github.com/anubhavtomar/ESE501/blob/master/Project1/DM.h) - Data Memory (For Load and Store Instructions)

* [RF.h](https://github.com/anubhavtomar/ESE501/blob/master/Project1/RF.h) - Register File (Register Bank)

* [ALU.h](https://github.com/anubhavtomar/ESE501/blob/master/Project1/ALU.h) - Execution module that handles executing all arithmetic and logical instructions

* [controller.cpp](https://github.com/anubhavtomar/ESE501/blob/master/Project1/controller.cpp) - FSM is used to determine what stage should be executed. Instruction queue is used to store and handle multiple instructions in different stages. Generate control signals for all the modules. Synchronization is done using ```wait``` in the main FSM thread.

* [testbench.cpp](https://github.com/anubhavtomar/ESE501/blob/master/Project1/testbench.cpp) - Initializes all modules, interconnect port of different modules using signals. Generate clock signals. Load the program memory and start the simulation.

* [log.txt](https://github.com/anubhavtomar/ESE501/blob/master/Project1/log.txt) - Log File logged at different stages in the pipeline while execution.


```
5-Stages 
1st Rising Edge : Read Instruction from PM using PC and store the instruction in IR(Instruction Register)
1st Falling Edge : Decode IR and generate required control signals
2nd Rising Edge : Read registers from RF
3rd Rising Edge : ALU execution
4th Rising Edge : MAR | MDR (address output | data output) [DM access] (if needed)
5th Rising Edge : Write back to RF (if needed)
```
#### Program used for Testing the Pipeline:
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

```NOP``` is used as HALT instruction to stop fetching further instructions. PC is not incremented and instruction queuing is stopped in the controller.

The program is compiled and executed using ```sh compile.sh```.
The results are logged in ```log.txt```
