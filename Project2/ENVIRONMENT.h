/**
 * Author:    Anubhav Tomar
 * 
 * Environment Definition
 **/

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include<systemc.h>
#include<LIB.h>

// template < > 
class _environmentBlock : public sc_module {
	public:
		// Inputs
		sc_in<bool> _clock;
		sc_fifo_in<int> _robot1ToEnv , _robot2ToEnv , _robot3ToEnv , _robot4ToEnv;

		// Outputs
		sc_out<bool> _enableEnvToRobot1 , _enableEnvToRobot2 , _enableEnvToRobot3 , _enableEnvToRobot4;
		sc_fifo_out<int> _envToRobot1 , _envToRobot2 , _envToRobot3 , _envToRobot4;

		SC_HAS_PROCESS(_environmentBlock);
		_environmentBlock(sc_module_name name) : sc_module(name) {
			SC_THREAD(_envProcess);
			sensitive<<_clock.pos();

			_init();
		};

	private:
		int _pointMapPosObstacle = 0;
		int _pointMapPosRobot1 = 0;
		int _pointMapPosRobot2 = 0;
		int _pointMapPosRobot3 = 0;
		int _pointMapPosRobot4 = 0;
		int _obstaclePathMapPos = 0;

		void _init () {
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl;
			cout<<"@ "<<sc_time_stamp()<<"------Start _init--------"<<endl;
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl<<endl<<endl;

			_obstacleLocGrid[1] = 1;
			_obstacleLocPoint[1] = {0 , 8.5*5};
			_obstacleDir[1] = 3;
			_obstacleLocGrid[2] = 13;
			_obstacleLocPoint[2] = {0 , 6.5*5};
			_obstacleDir[2] = 3;
			_obstacleLocGrid[3] = 26;
			_obstacleLocPoint[1] = {0 , 4.5*5};
			_obstacleDir[3] = 3;
			_obstacleLocGrid[4] = 39;
			_obstacleLocPoint[1] = {0 , 2.5*5};
			_obstacleDir[4] = 3;
		}
		void _updateRobotPath (int _id , int data) {
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl;
			cout<<"@ "<<sc_time_stamp()<<"------Start _updateRobotPath--------"<<endl;
			cout<<"       id :  "<<_id<<endl;
			cout<<"       data :  "<<data<<endl;
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl<<endl<<endl;

			switch(_id) {
				case 1 :
					_pointMapPosRobot1 = 0;
					break;
				case 2 :
					_pointMapPosRobot2 = 0;
					break;
				case 3 :
					_pointMapPosRobot3 = 0;
					break;
				case 4 :
					_pointMapPosRobot4 = 0;
					break;
				default :
					cout<<"WRONG TYPE INPUT ON ROBOT"<<endl;
					break;
			}
			// cout<<"Current Dir "<<_robotDir[_id]<<endl;
			if(_robotDir[_id] == 0 || _north[_robotLocGrid[_id]] == data) {
				_robotDir[_id] = 1; // North
				// cout<<"_leftPoint "<<_leftPoint[data].x<<" , "<<_leftPoint[data].y<<endl;
				_robotLocPoint[_id] = {_leftPoint[data].x + 2.5 , _leftPoint[data].y};
			} else if (_south[_robotLocGrid[_id]] == data) {
				_robotDir[_id] = 2; // South
				_robotLocPoint[_id] = {_rightPoint[data].x - 2.5 , _rightPoint[data].y};
			} else if (_east[_robotLocGrid[_id]] == data) {
				_robotDir[_id] = 3; // East
				_robotLocPoint[_id] = {_leftPoint[data].x , _leftPoint[data].y + 2.5};
			} else if (_west[_robotLocGrid[_id]] == data) {
				_robotDir[_id] = 4; // West
				_robotLocPoint[_id] = {_rightPoint[data].x, _rightPoint[data].y - 2.5};
			}
			_robotLocGrid[_id] = data;
		}
		void _updateRobot (int _id , int type , int data) {
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl;
			cout<<"@ "<<sc_time_stamp()<<"------Start _updateRobot--------"<<endl;
			cout<<"       id :  "<<_id<<endl;
			cout<<"       type :  "<<type<<endl;
			cout<<"       data :  "<<data<<endl;
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl<<endl<<endl;

			switch(type) {
				case 0 : // PATH
					_updateRobotPath(_id , data);
					break;
				case 1 : // STATUS
					_robotStatus[_id] = data;
					break;
				default :
					cout<<"WRONG TYPE INPUT ON ROBOT"<<endl;
					break;
			}
		}
		void _updateObstacle (int _id , int data) {
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl;
			cout<<"@ "<<sc_time_stamp()<<"------Start _updateObstacle--------"<<endl;
			cout<<"       id :  "<<_id<<endl;
			cout<<"       data :  "<<data<<endl;
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl<<endl<<endl;

			if(_north[_obstacleLocGrid[_id]] == data) {
				_obstacleDir[_id] = 1; // North
				_obstacleLocPoint[_id] = {_leftPoint[data].x + 2.5 , _leftPoint[data].y};
			} else if (_south[_obstacleLocGrid[_id]] == data) {
				_obstacleDir[_id] = 2; // South
				_obstacleLocPoint[_id] = {_rightPoint[data].x - 2.5 , _rightPoint[data].y};
			} else if (_east[_obstacleLocGrid[_id]] == data) {
				_obstacleDir[_id] = 3; // East
				_obstacleLocPoint[_id] = {_leftPoint[data].x , _leftPoint[data].y + 2.5};
			} else if (_west[_obstacleLocGrid[_id]] == data) {
				_obstacleDir[_id] = 4; // West
				_obstacleLocPoint[_id] = {_rightPoint[data].x, _rightPoint[data].y - 2.5};
			}
			_obstacleLocGrid[_id] = data;
		}
		bool _moveObstacle (int _id) {
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl;
			cout<<"@ "<<sc_time_stamp()<<"------Start _moveObstacle--------"<<endl;
			cout<<"       id :  "<<_id<<endl;
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl<<endl<<endl;

			bool _isGridCrossing = false;
			switch(_obstacleDir[_id]) {
				case 1 : // North
					_obstacleLocPoint[_id].y += 2.5;//_gridPointsLocObstacle[_pointMapPosObstacle];
					if(_obstacleLocPoint[_id].y >= _rightPoint[_obstacleLocGrid[_id]].y) {
						_updateObstacle(_id , _obstaclePath[_id][_obstaclePathMapPos%(_obstaclePath[_id].size())]);
						_isGridCrossing = true;
					}
					break;
				case 2 : // South
					_obstacleLocPoint[_id].y -= 2.5;//_gridPointsLocObstacle[_pointMapPosObstacle];
					if(_obstacleLocPoint[_id].y <= _leftPoint[_obstacleLocGrid[_id]].y) {
						_updateObstacle(_id , _obstaclePath[_id][(_obstaclePathMapPos)%(_obstaclePath[_id].size())]);
						_isGridCrossing = true;
					}
					break;
				case 3 : // East
					_obstacleLocPoint[_id].x += 2.5;//_gridPointsLocObstacle[_pointMapPosObstacle];
					if(_obstacleLocPoint[_id].x >= _rightPoint[_obstacleLocGrid[_id]].x) {
						_updateObstacle(_id , _obstaclePath[_id][(_obstaclePathMapPos)%(_obstaclePath[_id].size())]);
						_isGridCrossing = true;
					}
					break;
				case 4 : // West
					_obstacleLocPoint[_id].x -= 2.5;//_gridPointsLocObstacle[_pointMapPosObstacle];
					if(_obstacleLocPoint[_id].x <= _leftPoint[_obstacleLocGrid[_id]].x) {
						_updateObstacle(_id , _obstaclePath[_id][(_obstaclePathMapPos)%(_obstaclePath[_id].size())]);
						_isGridCrossing = true;
					}
					break;
				default : 
					break;
			}
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl;
			cout<<"@ "<<sc_time_stamp()<<"------End _moveObstacle--------"<<endl;
			cout<<"       id :  "<<_id<<endl;
			cout<<"       grid :  "<<_obstacleLocGrid[_id]<<endl;
			cout<<"       x :  "<<_obstacleLocPoint[_id].x<<endl;
			cout<<"       y :  "<<_obstacleLocPoint[_id].y<<endl;
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl<<endl<<endl;
			return _isGridCrossing;
		}
		bool _checkBorder (int _id) {
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl;
			cout<<"@ "<<sc_time_stamp()<<"------Start _checkBorder--------"<<endl;
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl<<endl<<endl;

			return _robotLocPoint[_id].y >= _rightPoint[_robotLocGrid[_id]].y || _robotLocPoint[_id].y <= _leftPoint[_robotLocGrid[_id]].y
				|| _robotLocPoint[_id].x >= _rightPoint[_robotLocGrid[_id]].x || _robotLocPoint[_id].x <= _leftPoint[_robotLocGrid[_id]].x;
		}
		bool _checkObstacleProximity(int _id) {
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl;
			cout<<"@ "<<sc_time_stamp()<<"------Start _checkObstacleProximity--------"<<endl;
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl<<endl<<endl;

			for(int i = 1 ; i <= 4 ; i++) {
				if(_robotDir[_id] == 1 && _obstacleDir[i] == 2) {
					return _robotLocPoint[_id].y < _obstacleLocPoint[i].y && _obstacleLocPoint[i].y - _robotLocPoint[_id].y <= 3.0;
				} else if(_robotDir[_id] == 2 && _obstacleDir[i] == 1) {
					return _robotLocPoint[_id].y > _obstacleLocPoint[i].y && _robotLocPoint[_id].y - _obstacleLocPoint[i].y <= 3.0;
				}  else if(_robotDir[_id] == 3 && _obstacleDir[i] == 4) {
					return _robotLocPoint[_id].x < _obstacleLocPoint[i].x && _obstacleLocPoint[i].x - _robotLocPoint[_id].x <= 3.0;
				}  else if(_robotDir[_id] == 4 && _obstacleDir[i] == 3) {
					return _robotLocPoint[_id].x > _obstacleLocPoint[i].x && _robotLocPoint[_id].x - _obstacleLocPoint[i].x <= 3.0;
				} 
			}
			return false;
		}
		bool _moveRobot (int _id) {
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl;
			cout<<"@ "<<sc_time_stamp()<<"------Start _moveRobot--------"<<endl;
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl<<endl<<endl;

			int _isGridCrossing = false;
			if(_robotLocGrid[_id] == 0 || (_robotStatus[_id] == 0 && _checkBorder(_id))) {
				return false;
			}
			if(_checkObstacleProximity(_id)) {
				_sendSignalToServer(_id , 0);
				return false;	
			}
			int _pointMapPos;
			switch(_id) {
				case 1 :
					_pointMapPos = _pointMapPosRobot1;
					_pointMapPosRobot1++;
					break;
				case 2 :
					_pointMapPos = _pointMapPosRobot2;
					_pointMapPosRobot2++;
					break;
				case 3 :
					_pointMapPos = _pointMapPosRobot3;
					_pointMapPosRobot3++;
					break;
				case 4 :
					_pointMapPos = _pointMapPosRobot4;
					_pointMapPosRobot4++;
					break;
				default :
					cout<<"WRONG TYPE INPUT ON ROBOT"<<endl;
					break;
			}
			switch(_robotDir[_id]) {
				case 1 : // North
					_robotLocPoint[_id].y += 0.5;//_gridPointsLocRobot[_pointMapPos];
					if(_robotLocPoint[_id].y >= _rightPoint[_robotLocGrid[_id]].y) {
						_isGridCrossing = true;
					}
					break;
				case 2 : // South
					_robotLocPoint[_id].y -= 0.5;//_gridPointsLocRobot[_pointMapPos];
					if(_robotLocPoint[_id].y <= _leftPoint[_robotLocGrid[_id]].y) {
						_isGridCrossing = true;
					}
					break;
				case 3 : // East
					_robotLocPoint[_id].x += 0.5;//_gridPointsLocRobot[_pointMapPos];
					if(_robotLocPoint[_id].x >= _rightPoint[_robotLocGrid[_id]].x) {
						_isGridCrossing = true;
					}
					break;
				case 4 : // West
					_robotLocPoint[_id].x -= 0.5;//_gridPointsLocRobot[_pointMapPos];
					if(_robotLocPoint[_id].x <= _leftPoint[_robotLocGrid[_id]].x) {
						_isGridCrossing = true;
					}
					break;
				default : 
					break;
			}
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl;
			cout<<"       id :  "<<_id<<endl;
			cout<<"       grid :  "<<_robotLocGrid[_id]<<endl;
			cout<<"       x :  "<<_robotLocPoint[_id].x<<endl;
			cout<<"       y :  "<<_robotLocPoint[_id].y<<endl;
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl<<endl<<endl;
			return _isGridCrossing;
		}
		void _enableRobot (int _id) {
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl;
			cout<<"@ "<<sc_time_stamp()<<"------Start _enableRobot--------"<<endl;
			cout<<"       id :  "<<_id<<endl;
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl<<endl<<endl;

			_enableEnvToRobot1.write(false);
			_enableEnvToRobot2.write(false);
			_enableEnvToRobot3.write(false);
			_enableEnvToRobot4.write(false);
			// wait(10 , SC_NS);
			switch(_id) {
				case 1 : 
					_enableEnvToRobot1.write(true);
					break;
				case 2 : 
					_enableEnvToRobot2.write(true);
					break;
				case 3 : 
					_enableEnvToRobot3.write(true);
					break;
				case 4 : 
					_enableEnvToRobot4.write(true);
					break;
				default:
					cout<<"WRONG ROBOT ID"<<endl;
					break;
			}
			wait(10 , SC_NS);
			_enableEnvToRobot1.write(false);
			_enableEnvToRobot2.write(false);
			_enableEnvToRobot3.write(false);
			_enableEnvToRobot4.write(false);
		}
		void _sendSignalToServer (int _id , int data) {
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl;
			cout<<"@ "<<sc_time_stamp()<<"------Start _sendSignalToServer--------"<<endl;
			cout<<"       id :  "<<_id<<endl;
			cout<<"       data :  "<<data<<endl;
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl<<endl<<endl;

			switch(_id) {
				case 1 :
					_envToRobot1.write(_id);
					_envToRobot1.write(1); // count
					_envToRobot1.write(0); // status signal
					_envToRobot1.write(data);
					break;
				case 2 :
					_envToRobot2.write(_id);
					_envToRobot2.write(1); // count
					_envToRobot2.write(0); // status signal
					_envToRobot2.write(data);
					break;
				case 3 :
					_envToRobot3.write(_id);
					_envToRobot3.write(1); // count
					_envToRobot3.write(0); // status signal
					_envToRobot3.write(data);
					break;
				case 4 :
					_envToRobot4.write(_id);
					_envToRobot4.write(1); // count
					_envToRobot4.write(0); // status signal
					_envToRobot4.write(data);
					break;
				default :
					cout<<"WRONG ROBOT ID"<<endl;
					break;
			}
			_enableRobot(_id);
		}
		void _readRobotFifo (int _id) {
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl;
			cout<<"@ "<<sc_time_stamp()<<"------Start _readRobotFifo--------"<<endl;
			cout<<"       id :  "<<_id<<endl;
			cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl<<endl<<endl;
			
			int _count;
			switch(_id) {
				case 1 :
					_robot1ToEnv.read(_count);
					break;
				case 2 :
					_robot2ToEnv.read(_count);
					break;
				case 3 :
					_robot3ToEnv.read(_count);
					break;
				case 4 :
					_robot4ToEnv.read(_count);
					break;
				default :
					cout<<"WRONG ROBOT ID"<<endl;
					break;
			}
			while(_count--) {
				int _type , _data;
				switch(_id) {
					case 1 :
						_robot1ToEnv.read(_type);
						_robot1ToEnv.read(_data);
						break;
					case 2 :
						_robot2ToEnv.read(_type);
						_robot2ToEnv.read(_data);
						break;
					case 3 :
						_robot3ToEnv.read(_type);
						_robot3ToEnv.read(_data);
						break;
					case 4 :
						_robot4ToEnv.read(_type);
						_robot4ToEnv.read(_data);
						break;
					default :
						cout<<"WRONG ROBOT ID"<<endl;
						break;
				}
				_updateRobot(_id , _type , _data);
			}
		}
		void _envProcess () {
			while(true) {
				wait();
				cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl;
				cout<<"@ "<<sc_time_stamp()<<"------Start _envProcess--------"<<endl;
				cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl<<endl<<endl;

				int _robotId;
				if(_robot1ToEnv.nb_read(_robotId)) {
					_readRobotFifo(_robotId);
				}
				if(_robot2ToEnv.nb_read(_robotId)) {
					_readRobotFifo(_robotId);
				}
				if(_robot3ToEnv.nb_read(_robotId)) {
					_readRobotFifo(_robotId);
				}
				if(_robot4ToEnv.nb_read(_robotId)) {
					_readRobotFifo(_robotId);
				}
				bool _isGridCrossing;
				for(int i = 1 ; i <= 4 ; i++) {
					_isGridCrossing = _moveObstacle(i);
				}
				if(_isGridCrossing) {
					_obstaclePathMapPos++;
				}
				_pointMapPosObstacle++;
				for(int i = 1 ; i <= 4 ; i++) {
					_isGridCrossing = _moveRobot(i);
					if(_isGridCrossing) {
						_sendSignalToServer(i , 2);
					}
				}

				cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl;
				_logGridRobot();
				cout<<"/**===================================ENVIRONMENT LOG===================================**/"<<endl<<endl<<endl;
			}
		}
};

#endif