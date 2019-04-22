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
* [ENVIRONMENT.h](https://github.com/anubhavtomar/ESE501/blob/master/Project2/ENVIRONMENT.h) - Environment Module. Read the path from the robot input FIFO and move all the robots in the specified direction. Move all the obstacles in the desired path. Stop the robot if any obstacle is <= 3 meter away in front of robot and resumes after it passes the robot. Send grid crossing signal or stop signal to robot.

* [ROBOT.h](https://github.com/anubhavtomar/ESE501/blob/master/Project2/ROBOT.hh) - Robot Module. Read input FIFO on enable from environment and relay the data to output FIFO to server. Read input FIFO on enable from server and relay the data to output FIFO to environment.

* [SERVER.h](https://github.com/anubhavtomar/ESE501/blob/master/Project2/SERVER.h) - Server Module. Read the input from the robot input FIFO. If crossing signal is received, it checks for any collision with other robots and send new grid if there is no collision otherwise send stop signal to output FIFO to robot. Save the stop status is stop signal is received.

* [LIB.h](https://github.com/anubhavtomar/ESE501/blob/master/Project2/LIB.h) - A library file containing 2D Grid maps, Robot paths map, obstacles path map, robot status map, robots location map, obstacles location map

* [testbench.cpp](https://github.com/anubhavtomar/ESE501/blob/master/Project2/testbench.cpp) - Initializes all modules, interconnect port of different modules using signals. Generate clock signals and start the simulation.

* [log.txt](https://github.com/anubhavtomar/ESE501/blob/master/Project2/log.txt) - Log File logged in different modules while execution.

#### Phase 1
```
1-D Grid : [0 to 6]
Size of each block in the Grid : 1 meter
Robot movement : [0 to 6]
Obstacle movement : [6 to 0]
Robot Speed : 2 meter/sec
Obstacle Speed : 5 meter/sec
Clock cycle : 1 milisec
```

#### Phase 2
```
2-D Grid : [1 to 60]
X : wall

1   2   3   4   5   6   7   8   9   10
11  X   X   X   X   X   X   X   X   12
13  14  15  16  17  18  19  20  21  22
23  X   X   X   X   24  X   X   X   25
26  27  28  29  30  31  32  33  34  35
36  X   X   X   X   X   37  X   X   38
39  40  41  42  43  44  45  46  47  48
49  X   X   X   X   X   X   X   X   50
51  52  53  54  55  56  57  58  59  60

Size of each block in the Grid : 5 meter
Number of Robots : 4
Number of Obstacles : 4
Robot1 path : [1 , 11 , 13 , 14 , 15 , 16 , 17 , 18 , 24 , 31 , 30 , 29 , 28 , 27 , 26 , 36 , 39]
Robot2 path : [10 , 12 , 22 , 21 , 20 , 19 , 18 , 24 , 31 , 32 , 33 , 34 , 35 , 25]
Robot3 path : [49 , 39 , 36 , 26 , 27 , 28 , 29 , 30 , 31 , 32 , 37 , 45 , 46 , 47 , 48 , 38]
Robot4 path : [60 , 50 , 48 , 47 , 46 , 45 , 44 , 43 , 42 , 41 , 40 , 39 , 49 , 51 , 52 , 53 , 54 , 55]
Obstacle1 path : [1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 12 , 13 , 22 , 21 , 20 , 19 , 18 , 17 , 16 , 15 , 14 , 13 , 11 , 1 , ...]
Obstacle2 path : [13 , 14 , 15 , 16 , 17 , 18 , 24 , 31 , 32 , 33 , 34 , 35 , 25 , 22 , 21 , 20 , 19 , 18 , 24 , 31 , 30 , 29 , 28 , 27 , 26 , 26 , 23 , 13 , ...]
Obstacle3 path : [26 , 27 , 28 , 29 , 30 , 31 , 32 , 37 , 45 , 46 , 47 , 48 , 38 , 35 , 34 , 33 , 32 , 37 , 45 , 44 , 43 , 42 , 41 , 40 , 39 , 36 , 26 , ...]
Obstacle4 path : [39 , 40 , 41 , 42 , 43 , 44 , 45 , 46 , 47 , 48 , 50 , 60 , 59 , 58 , 57 , 56 , 55 , 54 , 53 , 52 , 51 , 49 , 39 , ...]
Robots Speed : 2 meter/sec
Obstacles Speed : 5 meter/sec
Clock cycle : 1 milisec
```

The program is compiled and executed using ```sh compile.sh```.
The results are logged in ```log.txt```
