/**
 * Author:    Anubhav Tomar
 * 
 * Robot Definition
 **/

#ifndef ROBOT_H
#define ROBOT_H

#include<systemc.h>

// template < > 
class _robotBlock : public sc_module {
	public:
		// Inputs
		sc_in<bool> _robotEnableFromServer;
		sc_in<bool> _startFromServer;
		sc_in<bool> _stopFromServer;
		sc_in<bool> _isDeltaCrossingFromEnv;
		sc_in<bool> _isBoundaryFromEnv;
		sc_in<bool> _isObstacleFromEnv;
		
		// Outputs
		sc_out<bool> _startToEnv;
		sc_out<bool> _stopToEnv;
		sc_out<bool> _enableToServer;
		sc_out<bool> _isDeltaCrossingToServer;
		sc_out<bool> _isBoundaryToServer;
		sc_out<bool> _isObstacleToServer;

		SC_HAS_PROCESS(_robotBlock);
		_robotBlock(sc_module_name name) : sc_module(name) {
			SC_THREAD(_relayToServer);
			sensitive<<_isDeltaCrossingFromEnv.pos();
			sensitive<<_isBoundaryFromEnv.pos();
			sensitive<<_isObstacleFromEnv;

			SC_METHOD(_relayToEnv);
			sensitive<<_robotEnableFromServer.pos();
		};

	private:
		void _relayToServer () {
			while(true) {
				wait();

				cout<<"@ "<<sc_time_stamp()<<"------Start _relayToServer--------"<<endl;
				_isObstacleToServer.write(_isObstacleFromEnv.read());
				_isBoundaryToServer.write(_isBoundaryFromEnv.read());
				_isDeltaCrossingToServer.write(_isDeltaCrossingFromEnv.read());
				_enableToServer.write(true);
				wait(10 , SC_NS);
				_enableToServer.write(false);

				cout<<"/**===================================ROBOT LOG===================================**/"<<endl;
				cout<<"       Obstacle Signal To Server : "<<_isObstacleToServer<<endl;
				cout<<"       Boundary Signal To Server : "<<_isBoundaryToServer<<endl;
				cout<<"       Delta Crossing Signal To Server : "<<_isDeltaCrossingToServer<<endl;
				cout<<"/**===================================ROBOT LOG===================================**/"<<endl<<endl<<endl;

				cout<<"@ "<<sc_time_stamp()<<"------End _relayToServer--------"<<endl<<endl<<endl;
			}
		}
		void _relayToEnv () {
			cout<<"@ "<<sc_time_stamp()<<"------Start _relayEnv--------"<<endl;
			_startToEnv.write(_startFromServer.read());
			_stopToEnv.write(_stopFromServer.read());

			cout<<"/**===================================ROBOT LOG===================================**/"<<endl;
			cout<<"       Start Signal To Environment : "<<_startFromServer.read()<<endl;
			cout<<"       Stop Signal To Environment : "<<_stopFromServer.read()<<endl;
			cout<<"/**===================================ROBOT LOG===================================**/"<<endl<<endl<<endl;

			cout<<"@ "<<sc_time_stamp()<<"------End _relayEnv--------"<<endl<<endl<<endl;
		}
};

#endif