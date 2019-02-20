/*
	Problem 3 Design
*/

#include<systemc.h>

SC_MODULE(communicationInterface) {
	sc_in<sc_uint<12> > inData;
	sc_in<bool> clock , reset , clear;
	sc_out<sc_uint<4> > payloadOut;
	sc_out<sc_uint<8> > countOut , errorOut;

	void validateData();

	SC_CTOR(communicationInterface) {
		SC_METHOD(validateData);
		sensitive<<clock.pos();
	}
};

void communicationInterface::validateData() {
	cout<<"@ "<<sc_time_stamp()<<"----------Start validateData---------"<<endl;
	if(reset.read() == false || clear.read() == true) {
		payloadOut.write(0);
		countOut.write(0);
		errorOut.write(0);
		return;
	} 

	sc_uint<4> header = inData.read().range(11 , 8) , payload = inData.read().range(7 , 4);
	sc_uint<1> parity = inData.read().range(0 , 0);

	sc_uint<2> parityCheck = 0;
	if(header == 1) {
		payloadOut.write(payload);
		countOut.write(countOut.read() + 1);
	}

	while(payload) {
		parityCheck++;
		payload &= payload - 1;
	}

	errorOut.write(errorOut.read() + (parityCheck%2 && parity ? 1 : 0));
}