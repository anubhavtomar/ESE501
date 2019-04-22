/**
 * Author:    Anubhav Tomar
 * 
 * Library Definition
 **/

#ifndef LIB_H
#define LIB_H

#include<systemc.h>
#include <map>

struct point {
    double x;
	double y;
};

std::map<int , point> leftPointMap ();
std::map<int , point> rightPointMap ();
std::map<int , int> northMap ();
std::map<int , int> southtMap ();
std::map<int , int> eastMap ();
std::map<int , int> westMap ();
std::map<int , std::vector<int> > robotPathMap ();
std::map<int , int> robotStatusMap ();
std::map<int , std::vector<int> > obstaclePathMap ();
std::map<int , int> robotDir ();
std::map<int , int> robotLocGrid ();
std::map<int , point> robotLocPoint ();
std::map<int , int> obstacleDir ();
std::map<int , int> obstacleLocGrid ();
std::map<int , point> obstacleLocPoint ();
std::vector<double> gridPointsLocRobot ();
std::vector<double> gridPointsLocObstacle ();
void _printValue (int grid);
void _logGridRobot ();

std::map<int , point> _leftPoint = leftPointMap();
std::map<int , point> _rightPoint = rightPointMap();
std::map<int , int> _north = northMap();
std::map<int , int> _south = southtMap();
std::map<int , int> _east = eastMap();
std::map<int , int> _west = westMap();
std::map<int , std::vector<int> > _robotPath = robotPathMap();
std::map<int , int> _robotStatus = robotStatusMap();
std::map<int , std::vector<int> > _obstaclePath = obstaclePathMap();
std::map<int , int> _robotDir = robotDir();
std::map<int , int> _robotLocGrid = robotLocGrid();
std::map<int , point> _robotLocPoint = robotLocPoint();
std::map<int , int> _obstacleDir = obstacleDir();
std::map<int , int> _obstacleLocGrid = obstacleLocGrid();
std::map<int , point> _obstacleLocPoint = obstacleLocPoint();
std::vector<double> _gridPointsLocRobot = gridPointsLocRobot();
std::vector<double> _gridPointsLocObstacle = gridPointsLocObstacle();

std::map<int , point> leftPointMap () {
  	std::map<int , point> _map;
    int j=0;
    for(int i = 1 ; i <= 10 ; i++) {
        _map[i] = {j++*5 , 8*5};
    }
    _map[11] = {0*5 , 7*5};
    _map[12] = {9*5 , 7*5};
    j=0;
    for(int i = 13 ; i <= 22 ; i++) {
        _map[i] = {j++*5 , 6*5};
    }
    _map[23] = {0*5 , 5*5};
    _map[24] = {5*5 , 5*5};
    _map[25] = {9*5 , 5*5};
    j=0;
    for(int i = 26 ; i <= 35 ; i++) {
        _map[i] = {j++*5 , 4*5};
    }
    _map[36] = {0*5 , 3*5};
    _map[37] = {6*5 , 3*5};
    _map[38] = {9*5 , 3*5};
    j=0;
    for(int i = 39 ; i <= 48 ; i++) {
        _map[i] = {j++*5 , 2*5};
    }
    _map[49] = {0*5 , 1*5};
    _map[50] = {9*5 , 1*5};
    j=0;
    for(int i = 51 ; i <= 60 ; i++) {
        _map[i] = {j++*5 , 0*5};
    }
  	return _map;
}
std::map<int , point> rightPointMap () {
  	std::map<int , point> _map;
  	int j=0;
    for(int i = 1 ; i <= 10 ; i++) {
        _map[i] = {++j*5 , 9*5};
    }
    _map[11] = {0*5 , 8*5};
    _map[12] = {9*5 , 8*5};
    j=0;
    for(int i = 13 ; i <= 22 ; i++) {
        _map[i] = {++j*5 , 7*5};
    }
    _map[23] = {1*5 , 6*5};
    _map[24] = {6*5 , 6*5};
    _map[25] = {10*5 , 6*5};
    j=0;
    for(int i = 26 ; i <= 35 ; i++) {
        _map[i] = {++j*5 , 5*5};
    }
    _map[36] = {1*5 , 4*5};
    _map[37] = {7*5 , 4*5};
    _map[38] = {10*5 , 4*5};
    j=0;
    for(int i = 39 ; i <= 48 ; i++) {
        _map[i] = {++j*5 , 3*5};
    }
    _map[49] = {1*5 , 2*5};
    _map[50] = {10*5 , 2*5};
    j=0;
    for(int i = 51 ; i <= 60 ; i++) {
        _map[i] = {++j*5 , 1*5};
    }
  	return _map;
}
std::map<int , int> northMap () {
  	std::map<int , int> _map;
    for(int i = 1 ; i <= 10 ; i++) {
      	_map[i] = -1;
    }
    _map[11] = 1;
    _map[12] = 10;
    _map[13] = 11;
    for(int i = 14 ; i <= 21 ; i++) {
        _map[i] = -1;
    }
    _map[22] = 12;
    _map[23] = 13;
    _map[24] = 18;
    _map[25] = 22;
    _map[26] = 23;
    for(int i = 27 ; i <= 30 ; i++) {
        _map[i] = -1;
    }
    _map[31] = 24;
    for(int i = 32 ; i <= 34 ; i++) {
        _map[i] = -1;
    }
    _map[35] = 25;
    _map[36] = 26;
    _map[37] = 32;
    _map[38] = 35;
    _map[39] = 36;
    for(int i = 40 ; i <= 44 ; i++) {
        _map[i] = -1;
    }
    _map[45] = 37;
    for(int i = 46 ; i <= 47 ; i++) {
        _map[i] = -1;
    }
    _map[48] = 38;
    _map[49] = 39;
    _map[50] = 48;
    _map[51] = 49;
    for(int i = 52 ; i <= 59 ; i++) {
        _map[i] = -1;
    }
    _map[60] = 50;
  	return _map;
}
std::map<int , int> southtMap () {
  	std::map<int , int> _map;
    _map[1] = 11;
  	for(int i = 2 ; i <= 9 ; i++) {
        _map[i] = -1;
    }
    _map[10] = 12;
    _map[11] = 1;
    _map[12] = 10;
    _map[13] = 11;
    for(int i = 14 ; i <= 17 ; i++) {
        _map[i] = -1;
    }
    _map[18] = 24;
    for(int i = 19 ; i <= 21 ; i++) {
        _map[i] = -1;
    }
    _map[22] = 25;
    _map[23] = 26;
    _map[24] = 31;
    _map[25] = 35;
    _map[26] = 36;
    for(int i = 27 ; i <= 31 ; i++) {
        _map[i] = -1;
    }
    _map[32] = 37;
    for(int i = 33 ; i <= 34 ; i++) {
        _map[i] = -1;
    }
    _map[35] = 38;
    _map[36] = 39;
    _map[37] = 45;
    _map[38] = 48;
    _map[39] = 49;
    for(int i = 40 ; i <= 47 ; i++) {
        _map[i] = -1;
    }
    _map[48] = 50;
    _map[49] = 51;
    _map[50] = 60;
    for(int i = 51 ; i <= 60 ; i++) {
        _map[i] = -1;
    }
  	return _map;
}
std::map<int , int> eastMap () {
  	std::map<int , int> _map;
    _map[1] = -1;
  	for(int i = 2 ; i <= 10 ; i++) {
        _map[i] = i - 1;
    }
    _map[11] = -1;
    _map[12] = -1;
    _map[13] = -1;
    for(int i = 14 ; i <= 22 ; i++) {
        _map[i] = i - 1;
    }
    _map[23] = -1;
    _map[24] = -1;
    _map[25] = -1;
    _map[26] = -1;
    for(int i = 27 ; i <= 35 ; i++) {
        _map[i] = i - 1;
    }
    _map[36] = -1;
    _map[37] = -1;
    _map[38] = -1;
    _map[39] = -1;
    for(int i = 40 ; i <= 48 ; i++) {
        _map[i] = i - 1;
    }
    _map[49] = -1;
    _map[50] = -1;
    _map[51] = -1;
    for(int i = 52 ; i <= 60 ; i++) {
        _map[i] = i - 1;
    }
  	return _map;
}
std::map<int , int> westMap () {
  	std::map<int , int> _map;
    for(int i = 1 ; i <= 9 ; i++) {
        _map[i] = i + 1;
    }
    _map[10] = -1;
    _map[11] = -1;
    _map[12] = -1;
    for(int i = 13 ; i <= 21 ; i++) {
        _map[i] = i + 1;
    }
    _map[22] = -1;
    _map[23] = -1;
    _map[24] = -1;
    _map[25] = -1;
    for(int i = 26 ; i <= 34 ; i++) {
        _map[i] = i + 1;
    }
    _map[35] = -1;
    _map[36] = -1;
    _map[37] = -1;
    _map[38] = -1;
    for(int i = 39 ; i <= 47 ; i++) {
        _map[i] = i + 1;
    }
    _map[48] = -1;
    _map[49] = -1;
    _map[50] = -1;
    for(int i = 51 ; i <= 59 ; i++) {
        _map[i] = i + 1;
    }
    _map[60] = -1;
  	return _map;
}
std::map<int , std::vector<int> > robotPathMap () {
    std::map<int , std::vector<int> > _map;
    // Robot 1
    _map[1].push_back(0);
    _map[1].push_back(1);
    _map[1].push_back(11);
    _map[1].push_back(13);
    _map[1].push_back(14);
    _map[1].push_back(15);
    _map[1].push_back(16);
    _map[1].push_back(17);
    _map[1].push_back(18);
    _map[1].push_back(24);
    _map[1].push_back(31);
    _map[1].push_back(30);
    _map[1].push_back(29);
    _map[1].push_back(28);
    _map[1].push_back(27);
    _map[1].push_back(26);
    _map[1].push_back(36);
    _map[1].push_back(39);

    // Robot 2
    _map[2].push_back(0);
    _map[2].push_back(10);
    _map[2].push_back(12);
    _map[2].push_back(22);
    _map[2].push_back(21);
    _map[2].push_back(20);
    _map[2].push_back(19);
    _map[2].push_back(18);
    _map[2].push_back(24);
    _map[2].push_back(31);
    _map[2].push_back(32);
    _map[2].push_back(33);
    _map[2].push_back(34);
    _map[2].push_back(35);
    _map[2].push_back(25);

    // Robot 3
    _map[3].push_back(0);
    _map[3].push_back(49);
    _map[3].push_back(39);
    _map[3].push_back(36);
    _map[3].push_back(26);
    _map[3].push_back(27);
    _map[3].push_back(28);
    _map[3].push_back(29);
    _map[3].push_back(30);
    _map[3].push_back(31);
    _map[3].push_back(32);
    _map[3].push_back(37);
    _map[3].push_back(45);
    _map[3].push_back(46);
    _map[3].push_back(47);
    _map[3].push_back(48);
    _map[3].push_back(38);

    // Robot 4
    _map[4].push_back(0);
    _map[4].push_back(60);
    _map[4].push_back(50);
    _map[4].push_back(48);
    _map[4].push_back(47);
    _map[4].push_back(46);
    _map[4].push_back(45);
    _map[4].push_back(44);
    _map[4].push_back(43);
    _map[4].push_back(42);
    _map[4].push_back(41);
    _map[4].push_back(40);
    _map[4].push_back(39);
    _map[4].push_back(49);
    _map[4].push_back(51);
    _map[4].push_back(52);
    _map[4].push_back(53);
    _map[4].push_back(54);
    _map[4].push_back(55);
    return _map;
}
std::map<int , int> robotStatusMap () {
    std::map<int , int> _map;
    // Robot 1
    _map[1] = 1;

    // Robot 2
    _map[2] = 1;

    // Robot 3
    _map[3] = 1;

    // Robot 4
    _map[4] = 1;
    return _map;
}
std::map<int , std::vector<int> > obstaclePathMap () {
    std::map<int , std::vector<int> > _map;
    // Obstacle 1
    for(int i = 1 ; i <= 10 ; i++) {
        _map[1].push_back(i);
    }
    _map[1].push_back(12);
    for(int i = 22 ; i >= 13 ; i--) {
        _map[1].push_back(i);
    }
    _map[1].push_back(11);

    // Obstacle 2
    for(int i = 13 ; i <= 18 ; i++) {
        _map[2].push_back(i);
    }
    _map[2].push_back(24);
    for(int i = 31 ; i <= 35 ; i++) {
        _map[2].push_back(i);
    }
    _map[2].push_back(25);
    for(int i = 22 ; i >= 18 ; i--) {
        _map[2].push_back(i);
    }
    _map[2].push_back(24);
    for(int i = 31 ; i >= 26 ; i--) {
        _map[2].push_back(i);
    }
    _map[2].push_back(23);

    // Obstacle 3
    for(int i = 26 ; i <= 32 ; i++) {
        _map[3].push_back(i);
    }
    _map[3].push_back(37);
    for(int i = 45 ; i <= 48 ; i++) {
        _map[3].push_back(i);
    }
    _map[3].push_back(38);
    for(int i = 35 ; i >= 32 ; i--) {
        _map[3].push_back(i);
    }
    _map[3].push_back(37);
    for(int i = 45 ; i >= 39 ; i--) {
        _map[3].push_back(i);
    }
    _map[3].push_back(36);

    // Obstacle 4
    for(int i = 39 ; i <= 48 ; i++) {
        _map[4].push_back(i);
    }
    _map[4].push_back(50);
    for(int i = 60 ; i >= 51 ; i--) {
        _map[4].push_back(i);
    }
    _map[4].push_back(49);
    return _map;
}
std::map<int , int> robotDir () {
    std::map<int , int> _map;
    for(int i = 1 ; i <= 4 ; i++) {
        _map[i] = 0;
    }
    return _map;
}
std::map<int , int> robotLocGrid () {
    std::map<int , int> _map;
    for(int i = 1 ; i <= 4 ; i++) {
        _map[i] = 0;
    }
    return _map;
}
std::map<int , point> robotLocPoint () {
    std::map<int , point> _map;
    for(int i = 1 ; i <= 4 ; i++) {
        _map[i] = {0 , 0};
    }
    return _map;
}
std::map<int , int> obstacleDir () {
    std::map<int , int> _map;
    for(int i = 1 ; i <= 4 ; i++) {
        _map[i] = 0;
    }
    return _map;
}
std::map<int , int> obstacleLocGrid () {
    std::map<int , int> _map;
    for(int i = 1 ; i <= 4 ; i++) {
        _map[i] = 0;
    }
    return _map;
}
std::map<int , point> obstacleLocPoint () {
    std::map<int , point> _map;
    for(int i = 1 ; i <= 4 ; i++) {
        _map[i] = {0 , 0};
    }
    return _map;
}
std::vector<double> gridPointsLocRobot () {
    std::vector<double> _v;
    for(int i = 1 ; i <= 10 ; i++) {
        _v.push_back(0.5*i);
    }
    return _v;
}
std::vector<double> gridPointsLocObstacle () {
    std::vector<double> _v;
    for(int i = 1 ; i <= 2 ; i++) {
        _v.push_back(2.5*i);
    }
    return _v;
}
void _printValue (int grid) {
    for(int j = 1 ; j <= 4 ; j++) {
        if(_robotLocGrid[j] == grid) {
            cout<<"R"<<j<<" ";
            return;
        } else if(_obstacleLocGrid[j] == grid) {
            cout<<"O"<<j<<" ";
            return;
        }
    }
    cout<<" - ";
}
void _logGridRobot () {
    for(int i = 1 ; i <= 10 ; i++) {
        _printValue(i);
    }
    cout<<endl;
    _printValue(11);
    for(int i = 1 ; i <= 8 ; i++) {
        cout<<" X ";
    }
    _printValue(12);
    cout<<endl;
    for(int i = 13 ; i <= 22 ; i++) {
        _printValue(i);
    }
    cout<<endl;
    _printValue(23);
    for(int i = 1 ; i <= 4 ; i++) {
        cout<<" X ";
    }
    _printValue(24);
    for(int i = 1 ; i <= 3 ; i++) {
        cout<<" X ";
    }
    _printValue(25);
    cout<<endl;
    for(int i = 26 ; i <= 35 ; i++) {
        _printValue(i);
    }
    cout<<endl;
    _printValue(36);
    for(int i = 1 ; i <= 5 ; i++) {
        cout<<" X ";
    }
    _printValue(37);
    for(int i = 1 ; i <= 2 ; i++) {
        cout<<"X ";
    }
    _printValue(38);
    cout<<endl;
    for(int i = 39 ; i <= 48 ; i++) {
        _printValue(i);
    }
    cout<<endl;
    _printValue(49);
    for(int i = 1 ; i <= 8 ; i++) {
        cout<<" X ";
    }
    _printValue(50);
    cout<<endl;
    for(int i = 51 ; i <= 60 ; i++) {
        _printValue(i);
    }
    cout<<endl;
}

#endif