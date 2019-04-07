/**
 * Author:    Anubhav Tomar
 * 
 * Robot Definition
 **/

#ifndef ROBOT_H
#define ROBOT_H

#include<systemc.h>

template < > 
class _robotBlock : public sc_module {
	public:
		// Inputs
		sc_in<bool> _robotEnable;
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
			SC_METHOD(_relayCrossingToServer);
			sensitive>>_isDeltaCrossingFromEnv.pos();

			SC_METHOD(_relayBoundaryToServer);
			sensitive>>_isBoundaryFromEnv.pos();

			SC_METHOD(_relayObstacleToServer);
			sensitive>>_isObstacleFromEnv.pos();

			SC_METHOD(_relayStartToEnv);
			sensitive>>_startFromServer.pos();

			SC_METHOD(_relayStopToEnv);
			sensitive>>_stopFromServer.pos();
		};

	private:
		void _relayCrossingToServer () {
			_isDeltaCrossingToServer.write(true);
		}
		void _relayBoundaryToServer () {
			_isBoundaryToServer.write(true);
		}
		void _relayObstacleToServer () {
			_isObstacleToServer.write(true);
		}
		void _relayStartToEnv () {
			_startToEnv.write(true);
		}
		void _relayStopToEnv () {
			_stopToEnv.write(true);
		}
		
};

#endif