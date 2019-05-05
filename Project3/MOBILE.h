/**
 * Author:    Anubhav Tomar
 * 
 * Mobile Definition
 *
 * _inS1 -> network status (hight if busy | low if idle)
 * _inS2 -> ACK
 * _outM1 -> request network access
 * _outM2 -> packet begin/end indicator sc_signal
 **/

#ifndef MOBILE_H
#define MOBILE_H

#include<systemc.h>
#include<LIB.h>

// template < > 
class _mobileBlock : public sc_module {
	public:
		// Inputs
		sc_in<bool> _clock;
		sc_in<sc_uint<1> > _inS1; // From Server
		sc_in<sc_uint<1> > _inS2; // From Server
		
		// Outputs
		sc_out<sc_uint<1> > _outM1; // To Server
		sc_out<sc_uint<1> > _outM2; // To Server
		sc_out<int> _outRoi; // To Server
		sc_out<int> _outStart; // To Server
		sc_out<int> _outEnd; // To Server

		SC_HAS_PROCESS(_mobileBlock);
		_mobileBlock(sc_module_name name) : sc_module(name) {
			SC_THREAD(prcMobile);
			sensitive<<_clock.pos();
		};

	private:
		int txCounter = (TOTAL_TX_PACKETS);

		point prcSensor () {
			int x = rand() % 1023;
			int y = rand() % 1023;
			return point(x , y);
		}

		int prcConvert (struct point _p) {
			for(int i = 1 ; i <= 5 ; i++) {
				if(_p.x >= _roiMap[i][0].x && _p.x <= _roiMap[i][1].x && _p.y >= _roiMap[i][0].y && _p.y <= _roiMap[i][1].y) {
					return i;
				}
			}
			return 0;
		}

		std::vector<roiTuple> prcCompress (int _roi , std::vector<roiTuple> _res , int _c) {
			if(!_res.size()) {
				_res.push_back(roiTuple(_roi , 0 , 1));
				return _res;
			}
			if(_res.back().roi == _roi) {
				_res.back().endTime = _c;
			} else {
				_res.push_back(roiTuple(_roi , _c , _c));
			}
			return _res;
		}

		std::vector<roiTuple> prcPacketize () {
			std::vector<roiTuple> _res;
			int _count = 0;
			while(_res.size() < (TUPLES_PER_PACKETS)) {
				point _t = prcSensor();
				int _roi = prcConvert(_t);
				if(_roi) {
					_count++;
					_res = prcCompress(_roi , _res , _count);
				}
			}
			return _res;
		}

		void sendPacket (std::vector<roiTuple> _p) {
			for(int i = 0 ; i < 20 ; i++) {
				cout<<"/**===================================MOBILE LOG===================================**/"<<endl;
				cout<<"@ "<<sc_time_stamp()<<" ("<<_p[i].roi<<" , "<<_p[i].startTime<<" , "<<_p[i].endTime<<")"<<endl;
				cout<<"/**===================================MOBILE LOG===================================**/"<<endl<<endl<<endl;
				_outRoi.write(_p[i].roi); // To Server
				_outStart.write(_p[i].startTime); // To Server
				_outEnd.write(_p[i].endTime); // To Server
				wait(2 , SC_NS);
			}
		}

		void prcMobile () {
			while(true) {
				while(txCounter) {
					wait();
					if(_inS1.read() == 0b1) { // if network busy
						continue;
					}
					_outM1.write(0b1); // request network access
					wait(5 , SC_NS);
					if(_inS2.read() == 0b0) {
						continue;
					}
					std::vector<roiTuple> _p = prcPacketize();
					_outM2.write(0b1); // start of packet
					sendPacket(_p);
					// wait(); // till the full packet is transmitted
					_outM2.write(0b0); // end of packet
					_outM1.write(0b0);
					txCounter--;
				}

			}
		}
};

#endif