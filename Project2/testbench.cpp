/**
 * Author:    Anubhav Tomar
 * 
 * Robot Navigation Testbench
 **/

#include<systemc.h>
#include<SERVER.h>
#include<ROBOT.h>
#include<ENVIRONMENT.h>

// template < >
class _robotNavigation : public sc_module {
	public:
		/*Signals for Module Interconnections*/

		// FIFO
		sc_fifo<int> _serverToRobot1 , _envToRobot1 , _robot1ToServer , _robotToEnv1;
		sc_fifo<int> _serverToRobot2 , _envToRobot2 , _robot2ToServer , _robotToEnv2;
		sc_fifo<int> _serverToRobot3 , _envToRobot3 , _robot3ToServer , _robotToEnv3;
		sc_fifo<int> _serverToRobot4 , _envToRobot4 , _robot4ToServer , _robotToEnv4;

		// Server
		sc_signal<bool> _enableServerToRobot1 , _enableServerToRobot2 , _enableServerToRobot3 , _enableServerToRobot4;

		// Robot
		sc_signal<bool> _enableFromServer , _enableFromEnv;

		// Environment
		sc_signal<bool> _clock , _enableEnvToRobot1 , _enableEnvToRobot2 , _enableEnvToRobot3 , _enableEnvToRobot4;

		/*Module Instantiation*/
		_serverBlock* _s;
		_environmentBlock* _e;

		_robotBlock* _r1;
		_robotBlock* _r2;
		_robotBlock* _r3;
		_robotBlock* _r4;

		SC_HAS_PROCESS(_robotNavigation);
		_robotNavigation(sc_module_name name) : sc_module(name) {
			/**
			 * Module Instances Created
			 **/
			_s = new _serverBlock ("_server_");
			_e = new _environmentBlock ("_environment_");
			_r1 = new _robotBlock ("_robot1_" , 1);
			_r2 = new _robotBlock ("_robot2_" , 2);
			_r3 = new _robotBlock ("_robot3_" , 3);
			_r4 = new _robotBlock ("_robot4_" , 4);

			/**
			 * Server -> Robots
			 **/
			_s->_serverToRobot1(_serverToRobot1);
			_r1->_serverToRobot(_serverToRobot1);
			_s->_serverToRobot2(_serverToRobot2);
			_r2->_serverToRobot(_serverToRobot2);
			_s->_serverToRobot3(_serverToRobot3);
			_r3->_serverToRobot(_serverToRobot3);
			_s->_serverToRobot4(_serverToRobot4);
			_r4->_serverToRobot(_serverToRobot4);
			_s->_enableServerToRobot1(_enableServerToRobot1);
			_r1->_enableFromServer(_enableServerToRobot1);
			_s->_enableServerToRobot2(_enableServerToRobot2);
			_r2->_enableFromServer(_enableServerToRobot2);
			_s->_enableServerToRobot3(_enableServerToRobot3);
			_r3->_enableFromServer(_enableServerToRobot3);
			_s->_enableServerToRobot4(_enableServerToRobot4);
			_r4->_enableFromServer(_enableServerToRobot4);

		 	/**
			 * Robot -> Server
			 **/
			_r1->_robotToServer(_robot1ToServer);
			_s->_robot1ToServer(_robot1ToServer);
			_r2->_robotToServer(_robot2ToServer);
			_s->_robot2ToServer(_robot2ToServer);
			_r3->_robotToServer(_robot3ToServer);
			_s->_robot3ToServer(_robot3ToServer);
			_r4->_robotToServer(_robot4ToServer);
			_s->_robot4ToServer(_robot4ToServer);

		 	/**
			 * Environment -> Robots
			 **/
			_e->_envToRobot1(_envToRobot1);
			_r1->_envToRobot(_envToRobot1);
			_e->_envToRobot2(_envToRobot2);
			_r2->_envToRobot(_envToRobot2);
			_e->_envToRobot3(_envToRobot3);
			_r3->_envToRobot(_envToRobot3);
			_e->_envToRobot4(_envToRobot4);
			_r4->_envToRobot(_envToRobot4);
			_e->_enableEnvToRobot1(_enableEnvToRobot1);
			_r1->_enableFromEnv(_enableEnvToRobot1);
			_e->_enableEnvToRobot2(_enableEnvToRobot2);
			_r2->_enableFromEnv(_enableEnvToRobot2);
			_e->_enableEnvToRobot3(_enableEnvToRobot3);
			_r3->_enableFromEnv(_enableEnvToRobot3);
			_e->_enableEnvToRobot4(_enableEnvToRobot4);
			_r4->_enableFromEnv(_enableEnvToRobot4);

		 	/**
			 * Robots -> Environment
			 **/
			_r1->_robotToEnv(_robotToEnv1);
			_e->_robot1ToEnv(_robotToEnv1);
			_r2->_robotToEnv(_robotToEnv2);
			_e->_robot2ToEnv(_robotToEnv2);
			_r3->_robotToEnv(_robotToEnv3);
			_e->_robot3ToEnv(_robotToEnv3);
			_r4->_robotToEnv(_robotToEnv4);
			_e->_robot4ToEnv(_robotToEnv4);

			/**
			 * Clock -> Environment & Server
			 **/
			_e->_clock(_clock);
			_s->_clock(_clock);
			
			SC_THREAD(clockSignal);

			sc_fifo<int> _serverToRobot1 (50) , _envToRobot1 (50) , _robotToServer1 (50) , _robotToEnv1 (50);
			sc_fifo<int> _serverToRobot2 (50) , _envToRobot2 (50) , _robotToServer2 (50) , _robotToEnv2 (50);
			sc_fifo<int> _serverToRobot3 (50) , _envToRobot3 (50) , _robotToServer3 (50) , _robotToEnv3 (50);
			sc_fifo<int> _serverToRobot4 (50) , _envToRobot4 (50) , _robotToServer4 (50) , _robotToEnv4 (50);
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
	_robotNavigation _robotNav("_robotNavigation_");

	cout<<"@ "<<sc_time_stamp()<<"----------Start Simulation---------"<<endl<<endl<<endl;
	sc_start(10000 , SC_MS);
	cout<<"@ "<<sc_time_stamp()<<"----------End Simulation---------"<<endl<<endl<<endl;
	return 0;
}