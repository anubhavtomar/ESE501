/**
 * Author:    Anubhav Tomar
 * 
 * Server Definition
 * _inM1_1 -> request network access
 * _inM1_2 -> request network access
 * _inM2_1 -> packet begin/end indicator
 * _inM2_2 -> packet begin/end indicator
 * _inTuple_1 -> input tuples
 * _inTuple_2 -> input tuples
 * _outS1 -> network status (hight if busy | low if idle)
 * _outS2_1 -> ok from server
 * _outS2_2 -> ok from server
 **/

#ifndef SERVER_H
#define SERVER_H

#include<systemc.h>
#include<LIB.h>

// template < > 
class _serverBlock : public sc_module {
	public:
		// Inputs
		sc_in<bool> _clock;
		sc_in<sc_uint<1> > _inM1_1; // From Mobile 1
		sc_in<sc_uint<1> > _inM2_1;	// From Mobile 1
		sc_in<int> _inRoi_1; // From Moblie 1
		sc_in<int> _inStart_1; // From Moblie 1
		sc_in<int> _inEnd_1; // From Moblie 1
		sc_in<sc_uint<1> > _inM1_2; // From Mobile 2
		sc_in<sc_uint<1> > _inM2_2;	// From Mobile 2
		sc_in<int> _inRoi_2; // From Moblie 2
		sc_in<int> _inStart_2; // From Moblie 2
		sc_in<int> _inEnd_2; // From Moblie 2
		
		// Outputs
		sc_out<sc_uint<1> > _outS1; // To Mobile 1
		sc_out<sc_uint<1> > _outS2_1; // To Mobile 1
		sc_out<sc_uint<1> > _outS2_2; // To Mobile 2

		SC_HAS_PROCESS(_serverBlock);
		_serverBlock(sc_module_name name) : sc_module(name) {
			SC_THREAD(prcServer);
			sensitive<<_clock.pos();
		};

	private:
		int numPacketRecieved_M1 = 0;
		int numPacketRecieved_M2 = 0;
		std::map<int , std::vector<roiTuple> > _roiMap;

		void recievePacket (int _mobileId) {
			roiTuple _t = roiTuple(0 , 0 , 0);
			int count = 0;
			switch(_mobileId) {
				case 1 : 
					_t.roi = _inRoi_1.read();
					_t.startTime = _inStart_1.read();
					_t.endTime = _inEnd_1.read();
					count = ++numPacketRecieved_M1;
					break;
				case 2 :
					_t.roi = _inRoi_2.read();
					_t.startTime = _inStart_2.read();
					_t.endTime = _inEnd_2.read();
					count = ++numPacketRecieved_M2;
					break;
				default :
					cout<<"INVALID ID";
					break;
			}
			cout<<"/**===================================SERVER LOG===================================**/"<<endl;
			cout<<"@ "<<sc_time_stamp()<<" From Mobile "<<_mobileId<<endl;
			cout<<"@ "<<sc_time_stamp()<<" ("<<_t.roi<<" , "<<_t.startTime<<" , "<<_t.endTime<<")"<<endl;
			cout<<"@ "<<sc_time_stamp()<<" Tuple Number : "<<count<<endl;
			cout<<"/**===================================SERVER LOG===================================**/"<<endl<<endl<<endl;
			_roiMap[_mobileId].push_back(_t);
		}

		void prcServer () {
			while(true) {
				wait();
				if(_inM1_1.read() == 0b0 && _inM2_2.read() == 0b0) { // if both
					continue;
				}
				if(_inM1_1.read() == 0b1) {
					_outS1.write(0b1);
					_outS2_1.write(0b1);
					wait(10 , SC_NS);
					if(_inM2_1.read() == 0b0) {
						wait(10 , SC_NS);
					}
					if(_inM2_1.read() == 0b1) {
						while(_inM2_1.read() == 0b1) {
							recievePacket(1);
							wait(1 , SC_NS);
						}
					}
					_outS1.write(0b0);
					_outS2_1.write(0b0);
				}
				if(_inM1_2.read() == 0b1) {
					_outS1.write(0b1);
					_outS2_2.write(0b1);
					wait(10 , SC_NS);
					if(_inM2_2.read() == 0b0) {
						wait(10 , SC_NS);
					}
					if(_inM2_2.read() == 0b1) {
						while(_inM2_2.read() == 0b1) {
							recievePacket(2);
							wait(1 , SC_NS);
						}
					}
					_outS1.write(0b0);
					_outS2_2.write(0b0);
				}
			}
		}
};

#endif