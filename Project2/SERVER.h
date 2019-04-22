/**
 * Author:    Anubhav Tomar
 * 
 * Server Definition
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
		sc_fifo_in<int> _robot1ToServer , _robot2ToServer , _robot3ToServer , _robot4ToServer;

		// Outputs
		sc_out<bool> _enableServerToRobot1 , _enableServerToRobot2 , _enableServerToRobot3 , _enableServerToRobot4;
		sc_fifo_out<int> _serverToRobot1 , _serverToRobot2 , _serverToRobot3 , _serverToRobot4;

		SC_HAS_PROCESS(_serverBlock);
		_serverBlock(sc_module_name name) : sc_module(name) {
			SC_THREAD(_serverProcess);
			sensitive<<_clock.pos();

			// SC_THREAD(_robotInitProcess);

			sc_time t1(1 , SC_SEC);
			sc_time t2(5 , SC_SEC);
			sc_time t3(7 , SC_SEC);
			sc_time t4(2 , SC_SEC);
		};

	private:
		sc_time t1;
		sc_time t2;
		sc_time t3;
		sc_time t4;
		sc_event _robotInitEvent;

		bool _isLowerPriority (int r1 , int r2) {
			cout<<"/**===================================SERVER LOG===================================**/"<<endl;
			cout<<"@ "<<sc_time_stamp()<<"------Start _isLowerPriority--------"<<endl;
			cout<<"/**===================================SERVER LOG===================================**/"<<endl<<endl<<endl;

			if(r1 > r2) {
				_sendStopTo(r2); // r2 will stop at the crossing
				return false;
			}
			return true;
		}
		bool _checkCrossingOf (int _id) {
			cout<<"/**===================================SERVER LOG===================================**/"<<endl;
			cout<<"@ "<<sc_time_stamp()<<"------Start _checkCrossingOf--------"<<endl;
			cout<<"/**===================================SERVER LOG===================================**/"<<endl<<endl<<endl;

			if(_robotPath[_id].size() <= 1) {
				if(_robotPath[_id].size()) {
					_robotPath[_id].erase(_robotPath[_id].begin());
				}
				cout<<"ROBOT REACHED DESTINATION";
				return true;
			}
			int currentGrid = _robotPath[_id].front();
			int nextGrid = _robotPath[_id][1];
			for(int i = 1 ; i <= 4 ; i++) {
				if(i != _id && _robotPath[i].size() && _robotPath[i][0] == nextGrid && _isLowerPriority(_id , i)) {
					return true;
				}
			}
			for(int i = 1 ; i <= 4 ; i++) {
				if(i != _id && _robotStatus[i] == 0 && _robotPath[i].size() > 1 && _robotPath[i][1] == currentGrid) {
					_robotPath[i].erase(_robotPath[i].begin());
					_sendNextGridTo(i);
					break;
				}
			}
			return false;
		}
		void _enableRobot (int _id) {
			cout<<"/**===================================SERVER LOG===================================**/"<<endl;
			cout<<"@ "<<sc_time_stamp()<<"------Start _enableRobot--------"<<endl;
			cout<<"/**===================================SERVER LOG===================================**/"<<endl<<endl<<endl;
			
			_enableServerToRobot1.write(false);
			_enableServerToRobot2.write(false);
			_enableServerToRobot3.write(false);
			_enableServerToRobot4.write(false);
			// wait(10 , SC_NS);
			switch(_id) {
				case 1 : 
					_enableServerToRobot1.write(true);
					break;
				case 2 : 
					_enableServerToRobot2.write(true);
					break;
				case 3 : 
					_enableServerToRobot3.write(true);
					break;
				case 4 : 
					_enableServerToRobot4.write(true);
					break;
				default:
					cout<<"WRONG ROBOT ID"<<endl;
					break;
			}
			wait(10 , SC_NS);
			_enableServerToRobot1.write(false);
			_enableServerToRobot2.write(false);
			_enableServerToRobot3.write(false);
			_enableServerToRobot4.write(false);
		}
		void _sendStopTo (int _id) {
			cout<<"/**===================================SERVER LOG===================================**/"<<endl;
			cout<<"@ "<<sc_time_stamp()<<"------Start _sendStopTo--------"<<endl;
			cout<<"/**===================================SERVER LOG===================================**/"<<endl<<endl<<endl;

			switch(_id) {
				case 1 :
					_serverToRobot1.write(_id);
					_serverToRobot1.write(1); // count
					_serverToRobot1.write(1); // status signal
					_serverToRobot1.write(0); // STOP
					break;
				case 2 :
					_serverToRobot2.write(_id);
					_serverToRobot2.write(1); // count
					_serverToRobot2.write(1); // status signal
					_serverToRobot2.write(0); // STOP
					break;
				case 3 :
					_serverToRobot3.write(_id);
					_serverToRobot3.write(1); // count
					_serverToRobot3.write(1); // status signal
					_serverToRobot3.write(0); // STOP
					break;
				case 4 :
					_serverToRobot4.write(_id);
					_serverToRobot4.write(1); // count
					_serverToRobot4.write(1); // status signal
					_serverToRobot4.write(0); // STOP
					break;
				default :
					cout<<"WRONG ROBOT ID"<<endl;
					break;
			}
			_enableRobot(_id);
			_saveStatusOf(_id , 0);
		}
		void _sendNextGridTo (int _id) {
			cout<<"/**===================================SERVER LOG===================================**/"<<endl;
			cout<<"@ "<<sc_time_stamp()<<"------Start _sendNextGridTo--------"<<endl;
			cout<<"/**===================================SERVER LOG===================================**/"<<endl<<endl<<endl;

			cout<<"ID : "<<_id<<endl;
			_robotPath[_id].erase(_robotPath[_id].begin());
			switch(_id) {
				case 1 :
					_serverToRobot1.write(_id);
					_serverToRobot1.write(2); // count
					_serverToRobot1.write(1); // status signal
					_serverToRobot1.write(1); // START
					_serverToRobot1.write(0); // path signal
					_serverToRobot1.write(_robotPath[_id].front());
					break;
				case 2 :
					_serverToRobot2.write(_id);
					_serverToRobot2.write(2); // count
					_serverToRobot2.write(1); // status signal
					_serverToRobot2.write(1); // START
					_serverToRobot2.write(0); // path signal
					_serverToRobot2.write(_robotPath[_id].front());
					break;
				case 3 :
					_serverToRobot3.write(_id);
					_serverToRobot3.write(2); // count
					_serverToRobot3.write(1); // status signal
					_serverToRobot3.write(1); // START
					_serverToRobot3.write(0); // path signal
					_serverToRobot3.write(_robotPath[_id].front());
					break;
				case 4 :
					_serverToRobot4.write(_id);
					_serverToRobot4.write(2); // count
					_serverToRobot4.write(1); // status signal
					_serverToRobot4.write(1); // START
					_serverToRobot4.write(0); // path signal
					_serverToRobot4.write(_robotPath[_id].front());
					break;
				default :
					cout<<"WRONG ROBOT ID"<<endl;
					break;
			}
			_enableRobot(_id);
			_saveStatusOf(_id , 1);
		}
		void _saveStatusOf (int _id , int _status) {
			_robotStatus[_id] = _status;
		}
		void _robotInitProcess () {
			while(true) {
				wait(_robotInitEvent);
				cout<<"/**===================================SERVER LOG===================================**/"<<endl;
				cout<<"@ "<<sc_time_stamp()<<"------Start _robotInitProcess--------"<<endl;
				cout<<"/**===================================SERVER LOG===================================**/"<<endl<<endl<<endl;

				_sendNextGridTo(1); // R1 start at 1sec
				wait(_robotInitEvent);
				cout<<"/**===================================SERVER LOG===================================**/"<<endl;
				cout<<"@ "<<sc_time_stamp()<<"------Start _robotInitProcess--------"<<endl;
				cout<<"/**===================================SERVER LOG===================================**/"<<endl<<endl<<endl;

				_sendNextGridTo(4); // R4 starts at 2sec
				wait(_robotInitEvent);
				cout<<"/**===================================SERVER LOG===================================**/"<<endl;
				cout<<"@ "<<sc_time_stamp()<<"------Start _robotInitProcess--------"<<endl;
				cout<<"/**===================================SERVER LOG===================================**/"<<endl<<endl<<endl;

				_sendNextGridTo(3); // R3 starts at 5sec
				wait(_robotInitEvent);
				cout<<"/**===================================SERVER LOG===================================**/"<<endl;
				cout<<"@ "<<sc_time_stamp()<<"------Start _robotInitProcess--------"<<endl;
				cout<<"/**===================================SERVER LOG===================================**/"<<endl<<endl<<endl;

				_sendNextGridTo(2); // R2 starts at 7sec
				wait(_robotInitEvent);
			}
		}
		void _readRobotFifo (int _id) {
			cout<<"/**===================================SERVER LOG===================================**/"<<endl;
			cout<<"@ "<<sc_time_stamp()<<"------Start _readRobotFifo--------"<<endl;
			cout<<"/**===================================SERVER LOG===================================**/"<<endl<<endl<<endl;

			int _count , _type , _data;
			switch(_id) {
				case 1 :
					_robot1ToServer.read(_count);
					break;
				case 2 :
					_robot2ToServer.read(_count);
					break;
				case 3 :
					_robot3ToServer.read(_count);
					break;
				case 4 :
					_robot4ToServer.read(_count);
					break;
				default :
					cout<<"WRONG ROBOT ID"<<endl;
					break;
			}
			while(_count--) {
				int _type , _data;
				switch(_id) {
					case 1 :
						_robot1ToServer.read(_type);
						_robot1ToServer.read(_data);
						break;
					case 2 :
						_robot2ToServer.read(_type);
						_robot2ToServer.read(_data);
						break;
					case 3 :
						_robot3ToServer.read(_type);
						_robot3ToServer.read(_data);
						break;
					case 4 :
						_robot4ToServer.read(_type);
						_robot4ToServer.read(_data);
						break;
					default :
						cout<<"WRONG ROBOT ID"<<endl;
						break;
				}
				switch(_data) {
					case 0 : // STOP
						_saveStatusOf(_id , 0);
						break;
					case 1 : // START
						_saveStatusOf(_id , 1);
						break;
					case 2 : // CROSSING
						if(_checkCrossingOf(_id)) {
							_sendStopTo(_id);
						} else {
							_sendNextGridTo(_id);
						}
						break;
					default:
						cout<<"WRONG DATA INPUT ON SERVER"<<endl;
						break;
				}
			}
		}
		void _serverProcess () {
			while(true) {
				cout<<"/**===================================SERVER LOG===================================**/"<<endl;
				cout<<"@ "<<sc_time_stamp()<<"------Start _serverProcess--------"<<endl;
				cout<<"/**===================================SERVER LOG===================================**/"<<endl<<endl<<endl;

				if(sc_time_stamp().to_string().compare("1 s") == 0) {
					cout<<"/**===================================SERVER LOG===================================**/"<<endl;
					cout<<"@ "<<sc_time_stamp()<<"------Start _robotInitProcess--------"<<endl;
					cout<<"/**===================================SERVER LOG===================================**/"<<endl<<endl<<endl;

					_sendNextGridTo(1); // R1 start at 1sec
				}
				if(sc_time_stamp().to_string().compare("5 s") == 0) {
					cout<<"/**===================================SERVER LOG===================================**/"<<endl;
					cout<<"@ "<<sc_time_stamp()<<"------Start _robotInitProcess--------"<<endl;
					cout<<"/**===================================SERVER LOG===================================**/"<<endl<<endl<<endl;

					_sendNextGridTo(3); // R3 starts at 5sec
				}
				if(sc_time_stamp().to_string().compare("7 s") == 0) {
					cout<<"/**===================================SERVER LOG===================================**/"<<endl;
					cout<<"@ "<<sc_time_stamp()<<"------Start _robotInitProcess--------"<<endl;
					cout<<"/**===================================SERVER LOG===================================**/"<<endl<<endl<<endl;

					_sendNextGridTo(2); // R2 starts at 7sec
				}
				if(sc_time_stamp().to_string().compare("2 s") == 0) {
					cout<<"/**===================================SERVER LOG===================================**/"<<endl;
					cout<<"@ "<<sc_time_stamp()<<"------Start _robotInitProcess--------"<<endl;
					cout<<"/**===================================SERVER LOG===================================**/"<<endl<<endl<<endl;
					_sendNextGridTo(4); // R4 starts at 2sec
				}
				int _robotId;
				if(_robot1ToServer.nb_read(_robotId)) {
					_readRobotFifo(_robotId);
				}
				if(_robot2ToServer.nb_read(_robotId)) {
					_readRobotFifo(_robotId);
				}
				if(_robot3ToServer.nb_read(_robotId)) {
					_readRobotFifo(_robotId);
				}
				if(_robot4ToServer.nb_read(_robotId)) {
					_readRobotFifo(_robotId);
				}
				wait();
			}
		}
};

#endif