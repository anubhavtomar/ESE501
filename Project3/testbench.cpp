/**
 * Author:    Anubhav Tomar
 * 
 * Gaze Behavior Monitoring System Testbench
 **/

#include<systemc.h>
#include<SERVER.h>
#include<MOBILE.h>

// template < >
class _gazeMonitor : public sc_module {
	public:
		/*Signals for Module Interconnections*/
		sc_signal<bool> _clock;
		sc_signal<sc_uint<1> > _inM1_1 , _inM2_1 , _inM1_2 , _inM2_2;
		sc_signal<int> _inRoi_1 , _inStart_1 , _inEnd_1 , _inRoi_2 , _inStart_2 , _inEnd_2;
		
		// Outputs
		sc_signal<sc_uint<1> > _outS1 , _outS2_1 , _outS2_2;

		/*Module Instantiation*/
		_serverBlock* _s;
		_mobileBlock* _m1;
		_mobileBlock* _m2;

		SC_HAS_PROCESS(_gazeMonitor);
		_gazeMonitor(sc_module_name name) : sc_module(name) {
			/**
			 * Module Instances Created
			 **/
			_s = new _serverBlock ("_server_");
			_m1 = new _mobileBlock ("_mobile1_");
			_m2 = new _mobileBlock ("_mobile2_");

			/**
			 * Server -> Mobile
			 **/
			_s->_outS1(_outS1);
			_m1->_inS1(_outS1);
			_m2->_inS1(_outS1);
			_s->_outS2_1(_outS2_1);
			_m1->_inS2(_outS2_1);
			_s->_outS2_2(_outS2_2);
			_m2->_inS2(_outS2_2);

			/**
			 * Mobile -> Server
			 **/
			_m1->_outM1(_inM1_1);
			_s->_inM1_1(_inM1_1);
			_m1->_outM2(_inM2_1);
			_s->_inM2_1(_inM2_1);
			_m1->_outRoi(_inRoi_1);
			_s->_inRoi_1(_inRoi_1);
			_m1->_outStart(_inStart_1);
			_s->_inStart_1(_inStart_1);
			_m1->_outEnd(_inEnd_1);
			_s->_inEnd_1(_inEnd_1);

			_m2->_outM1(_inM1_2);
			_s->_inM1_2(_inM1_2);
			_m2->_outM2(_inM2_2);
			_s->_inM2_2(_inM2_2);
			_m2->_outRoi(_inRoi_2);
			_s->_inRoi_2(_inRoi_2);
			_m2->_outStart(_inStart_2);
			_s->_inStart_2(_inStart_2);
			_m2->_outEnd(_inEnd_2);
			_s->_inEnd_2(_inEnd_2);

			/**
			 * Clock -> Environment & Server
			 **/
			_s->_clock(_clock);
			_m1->_clock(_clock);
			_m2->_clock(_clock);
			
			SC_THREAD(clockSignal);
			_outS1.write(0b0);

		};

		void clockSignal() {
			while (true){
		        wait(0.5 , SC_MS);
		    	_clock = false;
		        wait(0.5 , SC_MS);
		    	_clock = true;
			}
		};

};

int sc_main(int argc , char* argv[]) {
	cout<<"@ "<<sc_time_stamp()<<"----------Start---------"<<endl<<endl<<endl;
	_gazeMonitor _gaze("_gazeMonitor_");

	cout<<"@ "<<sc_time_stamp()<<"----------Start Simulation---------"<<endl<<endl<<endl;
	sc_start(10000 , SC_MS);
	cout<<"@ "<<sc_time_stamp()<<"----------End Simulation---------"<<endl<<endl<<endl;
	return 0;
}