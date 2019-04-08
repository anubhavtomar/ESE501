/**
 * Author:    Anubhav Tomar
 * 
 * Server Definition
 **/

#ifndef SERVER_H
#define SERVER_H

#include<systemc.h>

// template < > 
class _serverBlock : public sc_module {
	public:
		// Inputs
		sc_in<bool> _serverEnableFromRobot;
		sc_in<bool> _isDeltaCrossingFromRobot;
		sc_in<bool> _isBoundaryFromRobot;
		sc_in<bool> _isObstacleFromRobot;

		// Outputs
		sc_out<bool> _enableToRobot;
		sc_out<bool> _startToRobot;
		sc_out<bool> _stopToRobot;

		SC_HAS_PROCESS(_serverBlock);
		_serverBlock(sc_module_name name) : sc_module(name) {
			SC_METHOD(_relayServer);
			sensitive<<_serverEnableFromRobot.pos();

			SC_METHOD(_relayObstacle);
			sensitive<<_isObstacleFromRobot.pos();

			for(int i = 0 ; i < 1 ; i++) {
				_robots.push_back(0b000001000000);
			}
		};

	private:
		sc_uint<1> const _START = 0b1;
		sc_uint<1> const _STOP = 0b0;
		/*6 -> 0 bits for Current Grid || 11 -> 7 bits for Next Grid || 12 -> 12 bit for state of Robot*/
		std::vector<sc_uint<13> > _robots;
		sc_uint<3> _robotStopPos = 0b110;

		void _relayServer () {
			cout<<"@ "<<sc_time_stamp()<<"------Start _relayServer--------"<<endl;
			_robots[0].range(12 , 12) = 0b1;
			bool toStart = true;
			if(_isBoundaryFromRobot.read()) {
				sc_uint<6> _nextGrid = _robots[0].range(11 , 6);
				if(_nextGrid == _robotStopPos) {
					_enableToRobot.write(true);
					_robots[0].range(12 , 12) = 0b0;
					_stopToRobot.write(true);
					toStart = false;
				}
				_robots[0].range(5 , 0) = _robots[0].range(11 , 6);
				_robots[0].range(11 , 6) = _nextGrid + 0b000001;
			}
			_startToRobot.write(toStart);

			cout<<"/**===================================SERVER LOG===================================**/"<<endl;
			cout<<"       Start Signal To Robot : "<<toStart<<endl;
			cout<<"       Current Grid : "<<_robots[0].range(5 , 0)<<endl;
			cout<<"       Next Grid : "<<_robots[0].range(11 , 6)<<endl;
			cout<<"       State : "<<_robots[0].range(12 , 12)<<endl;
			cout<<"/**===================================SERVER LOG===================================**/"<<endl<<endl<<endl;
			
			cout<<"@ "<<sc_time_stamp()<<"------End _relayServer--------"<<endl<<endl<<endl;
		}
		void _relayObstacle () {
			cout<<"@ "<<sc_time_stamp()<<"------Start _relayObstacle--------"<<endl;
			_robots[0].range(12 , 12) = 0b0;

			cout<<"/**===================================SERVER LOG===================================**/"<<endl;
			cout<<"       Current Grid : "<<_robots[0].range(5 , 0)<<endl;
			cout<<"       Next Grid : "<<_robots[0].range(11 , 6)<<endl;
			cout<<"       State : "<<_robots[0].range(12 , 12)<<endl;
			cout<<"/**===================================SERVER LOG===================================**/"<<endl<<endl<<endl;

			cout<<"@ "<<sc_time_stamp()<<"------End _relayObstacle--------"<<endl<<endl<<endl;
		}
};

#endif