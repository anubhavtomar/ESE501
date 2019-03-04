Phase 2 Project 1

5-stage pipeline => controller

program for adding 10 nummbes using for loop
	checking instructions (ADD , CMP , PSR , BRANCH)

	load the program 
	load the data

correct output

/*5-stage pipeline*/

IF | RD | EXE | MEM | WB

IF => instruction fetch
RD => sending control to RF
EXE => execution
MEM => memory access
WB => write to RF

1st rising edge : PC -> address generate -> access PM
2nd falling : IR (instruction register) -> generate control signals
3rd rising : RF access -> date flows through muxes in RF
4th rising : ALU input 
5th rising : MAR | MDR (address output | data output) [memory state]
6th rising : WR back (RF access)

/*Controller module*/

input : IR , PSR

output : control signals for every stage of the pipeline

/*Functional Modeling*/

- In early stages of a design proces, patitioning of functionality into hw/sw is not determined.
- At this point, avoid
	* timing
	* find-frain structure
	* low-level communication
- Goal is to create an executable speciincatino as efficient as possible

/*Untimed functional models*/
- the most general deterministic, untimed model of computation -> KPN (Kahn process n/w)
- FIFO is used to describe input to output mapping -> modules communication through FIFo

// Example of Data Flow Adder

			sc_fifo
Producer -------------> Consumer

template<class T> 
SC_MODULE(DR_ADDER) {
	sc_fifo_in<I> input1 , input2;
	sc_dido_out<T> output
	;
	void process() {
		while(1) {
			output.write(input1.read() + input2.read());			
		}
	}
}

/* sc_fifo pre defined methods*/
,write() -> blocking write if fifo is full
.nb_write() -> non blocking just return false

.read() -> wait if empty
.nb_read() -> if empty return false

.num_available() -> returns the number of data

.num_free() -> returns the number of slots

We use blocking methods only so that the process is *Deterministic.

Midterm

1. process event
2. fifo
3. homework related  (speed controller for your car in system c )

actual speed (status)
input
on
off
brake

output
speed

check extreme conditions (accelerate and brake at the same time etc)
