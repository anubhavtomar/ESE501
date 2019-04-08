/**
 * Author:    Anubhav Tomar
 * 
 * Environment Definition
 **/

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include<systemc.h>

// template < > 
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
			SC_THREAD(_movements);
			sensitive<<_clock.pos();
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
		bool isBoundaryTrue = true;

		bool _checkIfObstaclePassed () {
			cout<<"@ "<<sc_time_stamp()<<"------Start _checkIfObstaclePassed--------"<<endl;
			return (_obstacleX - _robotX) > 3 || _obstacleX < _robotX;
			cout<<"@ "<<sc_time_stamp()<<"------End _checkIfObstaclePassed--------"<<endl<<endl<<endl;
		}
		void _checkDeltaCrossing () {
			cout<<"@ "<<sc_time_stamp()<<"------Start _checkDeltaCrossing--------"<<endl;
			if(_robotX - (int)_robotX >= 0.5) {
				_isDeltaCrossingToRobot.write(true);
				isBoundaryTrue = false;
			} else if (!isBoundaryTrue && (int)_robotX != 0) {
				_isBoundaryToRobot.write(true);
				_isDeltaCrossingToRobot.write(false);
				isBoundaryTrue = true;
			} else {
				_isBoundaryToRobot.write(false);
				_isDeltaCrossingToRobot.write(false);
			}
			cout<<"@ "<<sc_time_stamp()<<"------End _checkDeltaCrossing--------"<<endl<<endl<<endl;
		}
		void _movements () {
			while(true) {
				wait();
				cout<<"@ "<<sc_time_stamp()<<"------Start _movements--------"<<endl;
				if(_obstacleX - _OBSTACLE_SPEED * 0.001 <= 0) {
					_obstacleX = 0;
				} else {
					_obstacleX = _obstacleX - _OBSTACLE_SPEED * 0.001;
				}
				if(_checkIfObstaclePassed()) {
					if(!_stopFromRobot.read()) {
						if(_robotX + _ROBOT_SPEED * 0.001 >= 6) {
							_robotX = 6;
						} else {
							_robotX = _robotX + _ROBOT_SPEED * 0.001;
						}
						_checkDeltaCrossing();
						_isObstacleToRobot.write(false);
					}
				} else {
					_isObstacleToRobot.write(true);
				}
				cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl;
				cout<<"       Obstacle Position X : "<<_obstacleX<<endl;
				cout<<"       Obstacle Position Y : "<<_obstacleY<<endl;
				cout<<"       Robot Position X : "<<_robotX<<endl;
				cout<<"       Robot Position Y : "<<_robotY<<endl;
				cout<<"       Is Delta (0.5m) before Crossing : "<<_isDeltaCrossingToRobot<<endl;
				cout<<"       Is Boundary : "<<_isBoundaryToRobot<<endl;
				cout<<"       Is Obstacle Ahead of Robot : "<<_isObstacleToRobot<<endl;
				cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl<<endl<<endl;

				cout<<"@ "<<sc_time_stamp()<<"------End _movements--------"<<endl<<endl<<endl;
			}
		}
};

#endif