Project 2

3 Modules:
----------
1. Robot Modules
2. Server Modules
3. Environment Modules

Sensors : // interacting with environment module 
GPS
Proximity 
Motor 

		Physical communication
Robot	-><-	Server

		Env

Robot  -><- environment
server -><- environment

Phase 1  : 1D Grid based synchronization


Robot Module:
--------------
Robot moves through the grid 1->6

1 when the robot reaches delta x from a boundary of the next grid. it tell the server that "I am crossing"
	2 Cases(signals received from the server):	OK
												STOP (some other robot is in grid 2)

2 if ok or STOP is not received at the boundary. robot stops and tell server "I stopped"

Environment Module:
-------------------

SC_METHOD

1. Every clock cycle(mili sec) (GPS) (if not STOP) (for robot and obstacle)
	x = vx * T + x; 
	y = vy * T + y;

2. Check if (x,y) is within delta x => env tell robot "you are crossing" // input signal to robot

3. if (x,y) is at boundary => tell robot to stop // input signal to robot

4. if STOP is received  // from robot

5. check for Obstacles  // input to robot (Proximity)
	within 3m

	if obstacle cleared  // input to robot (Proximity)


all the functions are runnig on N number of robots. => for loop
all the functinos except Proximity is O(N). Proximity is O(N2)

/*
	Signals
*/

Robot >> ENABLE  >> server
		 data

Server >> ENABLE  >> robot
		 data

Server >> Robot
	START , STOP , RESUME
	Pass these signals to ENV(relay signals to env)

Robot >> Server
	Receive from ENV
	Pass these signals to Serve(relay signals to server)

/*Specification*/
Obstacle >> 2m/sec
Robot >> 2m/sec
Robot moves when obstacle is passed.

Server Module :
---------------
array
Robot Index
Current grid
Next grid
states