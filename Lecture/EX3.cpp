dont_initialize();
// for making sure that processes are not executed at the start of the simulation by initializing all the values to false/0

// if we use dont_initialize() we have to put sc_start(0) before starting the simulation (sc_start(0 , SC_NS)) in sc_main()


// Events

event_name.notify() // immediate notification

event_name.notify(SC_ZERT_TIME) //delta delay

event_name.notify(1 , SC_NS) 

event_name.cancel();

SC_MODULE(events) {
	sc_in<bool> block;
	sc_event e1 , e2;

	void do_test1(){
		while(){
			wait();
			cout<<sc_time_stamp()<<"STARTING test1()";
			wait();
			cout<<"triggering e1";
			e1.notify(5 , SC_NS);
			wait();

			...
			...
			...
		}
	}

	void do_test2(){
		while(){
			wait();
			wait(e1);
			cout<<sc_time_stamp()<<"git troggered e1";
			wait(3);
			cout<<"triggering e2";

			...
			...
			...
		}
	}


}

/*

1ns starting test1
2ns triggering e1
7ns got triggered e1

*/