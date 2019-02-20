/*
	Problem 2 Testbench
*/

#include<systemc.h>
#include<counters.cpp>

SC_MODULE(countersTB) {
	sc_signal<sc_uint<8> > in1 , in2 , in3 , count1 , count2;
	sc_signal<bool> dec1 , dec2 , clock , load1 , load2 , ended;

	void clockSignal();
    void in1Signal();
    void in2Signal();
    void in3Signal();
    void dec1Signal();
    void dec2Signal();
    void load1Signal();
    void load2Signal();

	counters* ctrs;
	SC_CTOR(countersTB) {
		ctrs = new counters ("CTRS");
		ctrs->clock(clock);
        ctrs->in1(in1);
    	ctrs->in2(in2);
    	ctrs->in3(in3);
		ctrs->count1(count1);
		ctrs->count2(count2);
		ctrs->dec1(dec1);
		ctrs->dec2(dec2);
		ctrs->load1(load1);
		ctrs->load2(load2);
		ctrs->ended(ended);

		SC_THREAD(clockSignal);
    	SC_THREAD(in1Signal);
    	SC_THREAD(in2Signal);
    	SC_THREAD(in3Signal);
    	SC_THREAD(dec1Signal);
    	SC_THREAD(dec2Signal);
    	SC_THREAD(load1Signal);
    	SC_THREAD(load2Signal);
	}
};

void countersTB::clockSignal() {
	while (true){
        wait(20 , SC_NS);
    	clock = false;
        wait(20 , SC_NS);
    	clock = true;
	}
}

void countersTB::in1Signal() {
	while (true){
		wait(10 , SC_NS);
        in1 = 10;
		wait(90 , SC_NS);
		in1 = 100;
		wait(10 , SC_NS);
		in1 = 200;
		wait(100 , SC_NS);
	}
}

void countersTB::in2Signal() {
	while (true) {
		wait(10 , SC_NS);
        in2 = 10;
		wait(90 , SC_NS);
		in2 = 100;
		wait(10 , SC_NS);
		in2 = 200;
		wait(100 , SC_NS);
	}
}

void countersTB::in3Signal() {
	while (true) {
		wait(10 , SC_NS);
        in3 = 10;
        wait(10 , SC_NS);
    }
}

void countersTB::dec1Signal() {
	while (true) {
        wait(25 , SC_NS);
        dec1 = true;
        wait(65, SC_NS);
        dec1 = false;
        wait(30 , SC_NS);
        dec1 = true;
        wait(40 , SC_NS);
        dec1 = false;
    }
}

void countersTB::dec2Signal() {
	while (true) {
        wait(35 , SC_NS);
        dec2 = true;
        wait(55, SC_NS);
        dec2 = false;
        wait(40 , SC_NS);
        dec2 = true;
        wait(70 , SC_NS);
        dec2 = false;
    }
}

void countersTB::load1Signal() {
	while (true) {
        wait(40 , SC_NS);
        load1 =  true;
        wait(30, SC_NS);
        load1 = false;
        wait(100 , SC_NS);
    }
}

void countersTB::load2Signal() {
	while (true) {
        wait(50 , SC_NS);
        load2 = true;
        wait(50, SC_NS);
        load2 = false;
        wait(100 , SC_NS);
    }
}

int sc_main(int argc , char* argv[]) {
	cout<<"@ "<<sc_time_stamp()<<"----------Start---------"<<endl;
	countersTB* ctrsTB = new countersTB ("CTRSTB");
	cout<<"@ "<<sc_time_stamp()<<"----------Testbench Instance Created---------"<<endl;
	sc_trace_file* VCDFile;

	VCDFile = sc_create_vcd_trace_file("counters");
	cout<<"@ "<<sc_time_stamp()<<"----------VCD File Created---------"<<endl;
	sc_trace(VCDFile, ctrsTB->clock, "Clock");
	sc_trace(VCDFile, ctrsTB->in1, "In1");
	sc_trace(VCDFile, ctrsTB->in2, "In2");
	sc_trace(VCDFile, ctrsTB->in3, "In3");
	sc_trace(VCDFile, ctrsTB->count1, "Count1Out");
	sc_trace(VCDFile, ctrsTB->count2, "Count2Out");
	sc_trace(VCDFile, ctrsTB->dec1, "Dec1");
	sc_trace(VCDFile, ctrsTB->dec2, "Dec2");
	sc_trace(VCDFile, ctrsTB->load1, "Load1");
	sc_trace(VCDFile, ctrsTB->load2, "Load2");
	sc_trace(VCDFile, ctrsTB->load2, "Ended");

	cout<<"@ "<<sc_time_stamp()<<"----------Start Simulation---------"<<endl;
	sc_start(4000, SC_NS);
	cout<<"@ "<<sc_time_stamp()<<"----------End Simulation---------"<<endl;
	return 0;
}
