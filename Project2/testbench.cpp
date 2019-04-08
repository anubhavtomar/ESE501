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

		// Server
		sc_signal<bool> _serverEnableFromRobot , _isDeltaCrossingFromRobot , _isBoundaryFromRobot , _isObstacleFromRobot , _startToRobot , _stopToRobot , _enableToRobot;

		// Robot
		sc_signal<bool> _robotEnableFromServer , _startFromServer , _stopFromServer , _isDeltaCrossingFromEnv , _isBoundaryFromEnv , _isObstacleFromEnv , _startToEnv , _stopToEnv , _enableToServer , _isDeltaCrossingToServer , _isBoundaryToServer , _isObstacleToServer;

		// Environment
		sc_signal<bool> _clock , _startFromRobot , _stopFromRobot , _isDeltaCrossingToRobot , _isBoundaryToRobot, _isObstacleToRobot;

		/*Module Instantiation*/
		_serverBlock* _server;
		_robotBlock* _robot;
		_environmentBlock* _env;

		SC_HAS_PROCESS(_robotNavigation);
		_robotNavigation(sc_module_name name) : sc_module(name) {

			/**
			 * Module Instances Created
			 **/
			_server = new _serverBlock ("Server");
			_robot = new _robotBlock ("Robot");
			_env = new _environmentBlock ("Environment");

			/**
			 * Server -> Robot
			 **/
			_server->_enableToRobot(_enableToRobot);
			_robot->_robotEnableFromServer(_enableToRobot);
			_server->_startToRobot(_startToRobot);
			_robot->_startFromServer(_startToRobot);
			_server->_stopToRobot(_stopToRobot);
			_robot->_stopFromServer(_stopToRobot);

		 	/**
			 * Robot -> Server
			 **/
			_robot->_enableToServer(_enableToServer);
			_server->_serverEnableFromRobot(_enableToServer);
			_robot->_isDeltaCrossingToServer(_isDeltaCrossingToServer);
			_server->_isDeltaCrossingFromRobot(_isDeltaCrossingToServer);
			_robot->_isBoundaryToServer(_isBoundaryToServer);
			_server->_isBoundaryFromRobot(_isBoundaryToServer);
			_robot->_isObstacleToServer(_isObstacleToServer);
			_server->_isObstacleFromRobot(_isObstacleToServer);

		 	/**
			 * Environment -> Robot
			 **/
			_env->_isDeltaCrossingToRobot(_isDeltaCrossingToRobot);
			_robot->_isDeltaCrossingFromEnv(_isDeltaCrossingToRobot);
			_env->_isBoundaryToRobot(_isBoundaryToRobot);
			_robot->_isBoundaryFromEnv(_isBoundaryToRobot);
			_env->_isObstacleToRobot(_isObstacleToRobot);
			_robot->_isObstacleFromEnv(_isObstacleToRobot);

		 	/**
			 * Robot -> Environment
			 **/
			_robot->_startToEnv(_startToEnv);
			_env->_startFromRobot(_startToEnv);
			_robot->_stopToEnv(_stopToEnv);
			_env->_stopFromRobot(_stopToEnv);

			/**
			 * Clock -> Environment
			 **/
			_env->_clock(_clock);
			
			SC_THREAD(clockSignal);
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
	_robotNavigation _robotNav("Robot Navigation");

	cout<<"@ "<<sc_time_stamp()<<"----------Start Simulation---------"<<endl<<endl<<endl;
	sc_start(5000 , SC_MS);
	cout<<"@ "<<sc_time_stamp()<<"----------End Simulation---------"<<endl<<endl<<endl;
	return 0;
}