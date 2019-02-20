/*
	Problem 1 Testbench
*/

#include<systemc.h>
#include<sd.cpp>

SC_MODULE(sequenceDetectorTB) {
	sc_signal<bool> clock , reset , clear , input , output , state;

	void clockSignal();
    void resetSignal();
    void clearSignal();
    void inputSignal();

	sequenceDetector* sd;
	SC_CTOR(sequenceDetectorTB) {
		sd = new sequenceDetector ("SD");
        sd->clock(clock);
    	sd->reset(reset);
        sd->clear(clear);
    	sd->input(input);
        sd->output(output);
		sd->state(state);

    	SC_THREAD(clockSignal);
    	SC_THREAD(resetSignal);
    	SC_THREAD(clearSignal);
		SC_THREAD(inputSignal);
	}
};

void sequenceDetectorTB::clockSignal() {
	while (true){
        wait(20 , SC_NS);
    	clock = false;
        wait(20 , SC_NS);
    	clock = true;
	}
}

void sequenceDetectorTB::resetSignal() {
	while (true){
		wait(10 , SC_NS);
        reset = true;
		wait(90 , SC_NS);
		reset = false;
		wait(10 , SC_NS);
		reset = true;
		wait(1040 , SC_NS);
	}
}

void sequenceDetectorTB::clearSignal() {
	while (true) {
		wait(50 , SC_NS);
        clear = false;
		wait(90 , SC_NS);
		clear = true;
		wait(10 , SC_NS);
		clear = false;
		wait(760 , SC_NS);
	}
}

void sequenceDetectorTB::inputSignal() {
	while (true) {
        wait(25 , SC_NS);
        input = true;
        wait(65, SC_NS);
        input = false;
        wait(30 , SC_NS);
        input = true;
        wait(40 , SC_NS);
        input = false;
    }
}

int sc_main(int argc , char* argv[]) {
	cout<<"@ "<<sc_time_stamp()<<"----------Start---------"<<endl;
	sequenceDetectorTB* sdTB = new sequenceDetectorTB ("SDTB");
	cout<<"@ "<<sc_time_stamp()<<"----------Testbench Instance Created---------"<<endl;
	sc_trace_file* VCDFile;

	VCDFile = sc_create_vcd_trace_file("sequence-detector");
	cout<<"@ "<<sc_time_stamp()<<"----------VCD File Created---------"<<endl;
	sc_trace(VCDFile, sdTB->clock, "Clock");
	sc_trace(VCDFile, sdTB->reset, "Reset");
	sc_trace(VCDFile, sdTB->clear, "Clear");
	sc_trace(VCDFile, sdTB->input, "Input");
	sc_trace(VCDFile, sdTB->state, "State");
	sc_trace(VCDFile, sdTB->output, "Output");

	cout<<"@ "<<sc_time_stamp()<<"----------Start Simulation---------"<<endl;
	sc_start(4000, SC_NS);
	cout<<"@ "<<sc_time_stamp()<<"----------End Simulation---------"<<endl;
	return 0;
}
