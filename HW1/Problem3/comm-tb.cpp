/*
	Problem 3 Testbench
*/

#include<systemc.h>
#include<comm.cpp>

SC_MODULE(communicationInterfaceTB) {
	sc_signal<sc_uint<12> > inData;
	sc_signal<bool> clock , reset , clear;
	sc_signal<sc_uint<4> > payloadOut;
	sc_signal<sc_uint<8> > countOut , errorOut;

	void clockSignal();
	void clearSignal();
	void resetSignal();
    void inDataSignal();

	communicationInterface* cI;
	SC_CTOR(communicationInterfaceTB) {
		cI = new communicationInterface ("CI");
		cI->clock(clock);
        cI->inData(inData);
    	cI->reset(reset);
    	cI->clear(clear);
		cI->payloadOut(payloadOut);
		cI->countOut(countOut);
		cI->errorOut(errorOut);

		SC_THREAD(clockSignal);
    	SC_THREAD(clearSignal);
    	SC_THREAD(resetSignal);
    	SC_THREAD(inDataSignal);
	}
};

void communicationInterfaceTB::clockSignal() {
	while (true){
        wait(20 , SC_NS);
    	clock = false;
        wait(20 , SC_NS);
    	clock = true;
	}
}

void communicationInterfaceTB::clearSignal() {
	while (true){
		wait(10 , SC_NS);
        clear = false;
		wait(100 , SC_NS);
		clear = true;
		wait(10 , SC_NS);
		clear = false;
		wait(100 , SC_NS);
	}
}

void communicationInterfaceTB::resetSignal() {
	while (true) {
		wait(10 , SC_NS);
        reset = true;
		wait(70 , SC_NS);
		reset = false;
		wait(10 , SC_NS);
		reset = true;
		wait(1000 , SC_NS);
	}
}

void communicationInterfaceTB::inDataSignal() {
	while (true) {
		wait(10 , SC_NS);
        inData = 497;
        wait(30 , SC_NS);
        inData = 224;
        wait(50 , SC_NS);
        inData = 369;
        wait(30 , SC_NS);
        inData = 224;
        wait(30 , SC_NS);
    }
}

int sc_main(int argc , char* argv[]) {
	cout<<"@ "<<sc_time_stamp()<<"----------Start---------"<<endl;
	communicationInterfaceTB* cITB = new communicationInterfaceTB ("CITB");
	cout<<"@ "<<sc_time_stamp()<<"----------Testbench Instance Created---------"<<endl;
	sc_trace_file* VCDFile;

	VCDFile = sc_create_vcd_trace_file("communication-interface");
	cout<<"@ "<<sc_time_stamp()<<"----------VCD File Created---------"<<endl;
	sc_trace(VCDFile, cITB->clock, "Clock");
	sc_trace(VCDFile, cITB->inData, "inData");
	sc_trace(VCDFile, cITB->reset, "reset");
	sc_trace(VCDFile, cITB->clear, "clear");
	sc_trace(VCDFile, cITB->payloadOut, "payloadOut");
	sc_trace(VCDFile, cITB->countOut, "countOut");
	sc_trace(VCDFile, cITB->errorOut, "errorOut");

	cout<<"@ "<<sc_time_stamp()<<"----------Start Simulation---------"<<endl;
	sc_start(4000, SC_NS);
	cout<<"@ "<<sc_time_stamp()<<"----------End Simulation---------"<<endl;
	return 0;
}
