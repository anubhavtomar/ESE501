/**
 * Author:    Anubhav Tomar
 * 
 * Environment Definition
 **/

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include<systemc.h>

template < > 
class _environmentBlock : public sc_module {
	public:
		// Inputs
		sc_in<bool> _clock;
		sc_in<bool> _startFromRobot;
		sc_in<bool> _stopFromRobot;

		// Outputs
		sc_out<bool> _isDeltaCrossingToRobot;
		sc_out<bool> _isBoundaryToRobot;
		sc_out<bool> _isObstacleToRobot;

		SC_HAS_PROCESS(_environmentBlock);
		_environmentBlock(sc_module_name name) : sc_module(name) {
			SC_METHOD(_movements);
			sensitive>>_clock.pos();
		};

	private:
		/*
			Robot : 0 -> 6
			Obstacle : 6 -> 0
		*/
		float _robotX = 0;
		float _robotY = 0;
		float _obstacleX = 6;
		float _obstacleY = 0;
		sc_uint<2> _ROBOT_SPEED = 0b10; // 2 m/sec
		sc_uint<3> _OBSTACLE_SPEED = 0b101; // 5m/sec

		bool _checkIfObstaclePassed () {
			return _obstacleX - _robotX > 3 || _obstacleX < _robotX;
		}

		void _checkDeltaCrossing () {
			if(_robotX - (int)_robotX > 0.5) {
				_isDeltaCrossingToRobot.write(true);
			}
		}

		void _movements () {
			_obstacleX = _obstacleX - _OBSTACLE_SPEED * 0.0001;

			if(_stopFromRobot.read()) {
				if(_checkIfObstaclePassed) {
					_robotX = _ROBOT_SPEED * 0.0001 + _robotX;
					if(!_isDeltaCrossingToRobot.read()) {
						_checkDeltaCrossing();
					}
				} else {
					_isObstacleToRobot.write(true);
				}
			}
		}
};

#endif