--------------------
Project 3 
--------------------
Gaze tracking system
--------------------

Server  transmitting a movie  	-> 	Mobile1
								-> 	Mobile2
								-> 	Mobile3

1024x1024 image (mpeg) -> has some ROI (region of interest) e.g shoe, car, person
we have x1,y1 and x2,y2 rectangular box of ROIs  => stored on mobile

For phase 1 we have 2 mobiles

------
Mobile
------

[1]	sensor module -> camera sampling at 200samples/sec
	it generates random (x,y) every 1/200 sec

[2]	conversion module -> (x,y) -> converted to ROI

[3]	compression -> (ROI_numeber , ts , te)
	0 1 2 3 4 5 6 7 8 
	1 1 1 2 2 2 3 3 5

	compressed packet => (1 , 0 , 2) , (2 , 3 , 5) , .........

[4]	packetizatizing -> Tx Process
	counter : numTxPacket
	counter: numTuple
	once numTuple == 20

	TxPacket = 20 tuples

----------------
Network Protocol
----------------

[1]	Duplex : only one mobile or server can use the Network

[2]	Bandwidth = 1Mbps (mega bits per second)

[3]	Tuple Size  (3)(64 bits) => packet = 20*3*64 bits

------
Mobile 	
------
		-> M1_out
		-> M2_out
		<- S1_out
		<- S2_out
					------
					Server
					------



M1_out -> request network access
M2_out -> packet begin/end indicator sc_signal
S1_out -> network status (hight if busy | low if idle)
S2_out -> ACK

---------------------
Packetization Process
---------------------

Assume TxCounter != 0

[1]	check S1_out
	if(S1_out == HIGH) {
		wait(random time)
		go back to [1]
	}

	if(S1_out == LOW) {
		send M1_out = HIGH
		if(not ok from server) {
			wait(random time)
			go back to [1]
		} 
		if(OK from server) {
			M2_out is raised to HIGH
			wait(transmit time calculated using packet size and bandwidth) 
			M2_out is lowered to LOW
			TxCounter--
			go back to [1]	
		}
	}

useful methods on the server

valueChangedEvent()
posEdgeEvent()
negEdgeEvent()

wait(signal_name->posEdgeEvent())

--------------------------
Server
input -> S1_in_1 , S1_in_2
--------------------------
[1]	wait(S1_in_1->posEdgeEvent() , S1_in_2->posEdgeEvent())

	if(S1_in_1.read() == HIGH) {
		tell mobile to send (send OK)
		SET network busy
		wait(for raising edge) -> *enclosed in a timeout*
			wait(for falling edge)
			SET network is free
			go back to [1]
	}// same as for S1_in_2


