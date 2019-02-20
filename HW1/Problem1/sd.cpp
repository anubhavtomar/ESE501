/*
	Problem 1 Design
*/

#include<systemc.h>

SC_MODULE(sequenceDetector) {
	sc_in<bool> clock , reset , clear;
	sc_in<bool> input;
	sc_out<bool> output;
	sc_out<bool> state;

	void detectorAlgo();
	void updateValues();

	enum states {S1,S2,S3,S4,S5};
	sc_signal<states> currentState , nextState;

	SC_CTOR(sequenceDetector) {
		currentState.write(S1);
		nextState.write(S1);

		SC_METHOD(detectorAlgo);
		sensitive<<clock.pos();

		SC_METHOD(updateValues);
		sensitive<<reset.neg()<<clear.pos()<<nextState;
	}
};

void sequenceDetector::updateValues() {
	cout<<"@ "<<sc_time_stamp()<<"------Start UpdateValues--------"<<endl;
	currentState.write(reset.read() == true ? nextState.read() : S1);
	output.write(nextState.read() == S5 && clear.read() == false ? true : false);
	state.write((reset.read() == false || nextState.read() == S1 || nextState.read() == S3) ? false : true);
	cout<<"@ "<<sc_time_stamp()<<"------End updateValues--------"<<endl;
}

void sequenceDetector::detectorAlgo() {
	cout<<"@ "<<sc_time_stamp()<<"------Start detectorAlgos--------"<<endl;
	bool inputVal = input.read();
	if(reset.read() == false) {
		nextState.write(S1);
		return;
	}
	switch(currentState.read()) {
		case S1 :
			if(inputVal == true) {
				nextState.write(S2);
			}
			break;
		case S2 : 
			if(inputVal == false) {
            	nextState.write(S3);
           	}	
       		break;
		case S3 : 
			if(inputVal == true) {
   	        	nextState.write(S4);
     		} else {
				nextState.write(S1);
			}
	        break;
		case S4 : 
			if(inputVal == true) {
               	nextState.write(S5);
            } else {
				nextState.write(S3);
			}
           	break;
		case S5 :
            if(inputVal == false) {
                nextState.write(S3);
            } else {
            	nextState.write(S2);
            }
    		break;		
		default:
			break;
	}
	cout<<"@ "<<sc_time_stamp()<<"------End detectorAlgos--------"<<endl;
}
