/*
	Problem 2 Design
*/

#include<systemc.h>

SC_MODULE(counters) {
	sc_in<sc_uint<8> > in1 , in2 , in3;
	sc_in<bool> dec1 , dec2 , clock , load1 , load2;
	sc_out<sc_uint<8> > count1 , count2;
	sc_out<bool> ended;

	sc_signal<bool> isOverflow1 , isOverflow2;

	void handleCount1();
	void handleCount2();
	void updateEnded();

	SC_CTOR(counters) {
		isOverflow1.write(false);
		isOverflow2.write(false);
		
		SC_METHOD(handleCount1);
		sensitive<<clock.pos();

		SC_METHOD(handleCount2);
		sensitive<<clock.pos();

		SC_METHOD(updateEnded);
		sensitive<<clock.pos();
	}
};

void counters::handleCount1() {
	if(load1.read() == true) {
		cout<<"@ "<<sc_time_stamp()<<"----------Start registerCount1---------"<<endl;
		count1.write(in1.read());
		cout<<"@ "<<sc_time_stamp()<<"----------End registerCount1---------"<<endl;
	} else if(dec1.read() == true) {
		cout<<"@ "<<sc_time_stamp()<<"----------Start decCount1---------"<<endl;
		if(count1.read() == 0) {
			isOverflow1.write(true);
			return;
		}
		isOverflow1.write(false);
		count1.write(count1.read() - 1);
		cout<<"@ "<<sc_time_stamp()<<"----------End decCount1---------"<<endl;
	}
}

void counters::handleCount2() {
	if(load2.read() == true) {
		cout<<"@ "<<sc_time_stamp()<<"----------Start registerCount2---------"<<endl;
		count2.write(in2.read());
		cout<<"@ "<<sc_time_stamp()<<"----------End registerCount2---------"<<endl;
	} else if(dec2.read() == true) {
		cout<<"@ "<<sc_time_stamp()<<"----------Start decCount2---------"<<endl;
		if(count2.read() <= in3.read()) {
			isOverflow2.write(true);
			return;
		}
		isOverflow2.write(false);
		count2.write(count2.read() - in3.read());
		cout<<"@ "<<sc_time_stamp()<<"----------End decCount2---------"<<endl;
	}
}

void counters::updateEnded() {
	cout<<"@ "<<sc_time_stamp()<<"----------Start updateEnded---------"<<endl;
	if(count1.read() == count2.read() || isOverflow1.read() || isOverflow2.read()) {
		ended.write(true);
	} else {
		ended.write(false);
	}
	cout<<"@ "<<sc_time_stamp()<<"----------End updateEnded---------"<<endl;
}