/**
 * Author:    Anubhav Tomar
 * 
 * Server Definition
 **/

#ifndef SERVER_H
#define SERVER_H

#include<systemc.h>

template < > 
class _serverBlock : public sc_module {
	public:
		// Inputs
		sc_in<bool> _serverEnableFromRobot;
		sc_in<bool> _isDeltaCrossingFromRobot;
		sc_in<bool> _isBoundaryFromRobot;
		sc_in<bool> _isObstacleFromRobot;

		// Outputs
		sc_out<bool> _startToRobot;
		sc_out<bool> _stopToRobot;
		sc_out<bool> _enableToRobot;

		SC_HAS_PROCESS(_serverBlock);
		_serverBlock(sc_module_name name) : sc_module(name) {
			SC_METHOD(_relayCrossing);
			sensitive>>_isDeltaCrossingFromRobot.pos();

			SC_METHOD(_relayBoundary);
			sensitive>>_isBoundaryFromRobot.pos();

			SC_METHOD(_relayObstacle);
			sensitive>>_isObstacleFromRobot.pos();	
		};

	private:
		sc_uint<1> const _START = 0b1;
		sc_uint<1> const _STOP = 0b0;
		vector<sc_uint<13> > _robots(1 , 0); // 6 -> 0 bits for Current Grid || 11 -> 7 bits for Next Grid || 12 -> 12 bit for state of Robot

		void _relayCrossing () {
			_robots[0].range(12 , 12) = 0b1;
			_startToRobot.write(true);
		}
		void _relayBoundary () {
			_robots[0].range(12 , 12) = 0b1;
			_startToRobot.write(true);
		}
		void _relayObstacle () {
			_robots[0].range(12 , 12) = 0b0;
		}
};

#endif