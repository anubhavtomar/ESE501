/**
 * Author:    Anubhav Tomar
 * 
 * Robot Definition
 **/

#ifndef ROBOT_H
#define ROBOT_H

#include<systemc.h>
#include<LIB.h>

// template < > 
class _robotBlock : public sc_module {
	public:
		// Inputs
		sc_in<bool> _enableFromServer , _enableFromEnv;
		sc_fifo_in<int> _serverToRobot , _envToRobot;
		
		// Outputs
		sc_fifo_out<int> _robotToServer , _robotToEnv;

		SC_HAS_PROCESS(_robotBlock);
		_robotBlock(sc_module_name name , int _robotId) : sc_module(name) {
			SC_THREAD(_robotProcess);
			sensitive<<_enableFromServer.pos();
			sensitive<<_enableFromEnv.pos();

			_id = _robotId;
		};

	private:
		int _id;

		void _relayToServer (int type , int data) {
			cout<<"/**===================================ROBOT LOG===================================**/"<<endl;
			cout<<"@ "<<sc_time_stamp()<<"------Start _relayToServer--------"<<endl;
			cout<<"/**===================================ROBOT LOG===================================**/"<<endl<<endl<<endl;

			// wait(10 , SC_NS);
			_robotToServer.write(type);
			_robotToServer.write(data);
			// wait(10 , SC_NS);
		}

		void _relayToEnv (int type , int data) {
			cout<<"/**===================================ROBOT LOG===================================**/"<<endl;
			cout<<"@ "<<sc_time_stamp()<<"------Start _relayToEnv--------"<<endl;
			cout<<"/**===================================ROBOT LOG===================================**/"<<endl<<endl<<endl;

			// wait(10 , SC_NS);
			_robotToEnv.write(type);
			_robotToEnv.write(data);
			// wait(10 , SC_NS);
		}

		void _robotProcess () {
			while(true) {
				wait();
				cout<<"/**===================================ROBOT LOG===================================**/"<<endl;
				cout<<"@ "<<sc_time_stamp()<<"------Start _robotProcess--------"<<endl;
				cout<<"/**===================================ROBOT LOG===================================**/"<<endl<<endl<<endl;

				int _robotId;
				if(_serverToRobot.nb_read(_robotId)) {
					cout<<"ID "<<_robotId<<endl;
					if(_robotId != _id) {
						// not this robot
						continue;
					}
					int _count;
					_serverToRobot.read(_count);
					_robotToEnv.write(_id);
					_robotToEnv.write(_count);
					while(_count--) {
						int _type , _data;
						_serverToRobot.read(_type);
						_serverToRobot.read(_data);
						_relayToEnv(_type , _data);
					}
				}
				if(_envToRobot.nb_read(_robotId)) {
					cout<<"ID "<<_robotId<<endl;
					if(_robotId != _id) {
						// not this robot
						continue;
					}
					int _count;
					_envToRobot.read(_count);
					_robotToServer.write(_id);
					_robotToServer.write(_count);
					while(_count--) {
						int _type , _data;
						_envToRobot.read(_type);
						_envToRobot.read(_data);
						_relayToServer(_type , _data);
					}
				}
			}
		}
};

#endif