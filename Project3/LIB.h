/**
 * Author:    Anubhav Tomar
 * 
 * Library Definition
 **/

#ifndef LIB_H
#define LIB_H

#include<systemc.h>
#include <map>

#define TOTAL_TX_PACKETS 30
#define TUPLES_PER_PACKETS 20

struct roiTuple {
    int roi;
	int startTime;
    int endTime;
    roiTuple();
    roiTuple(int _r , int _s , int _e) : roi(_r) , startTime(_s) , endTime(_e) {};
    roiTuple(const roiTuple &_r) : roi(_r.roi) , startTime(_r.startTime) , endTime(_r.endTime) {};

};

struct point {
    int x;
    int y;
    point(int _x , int _y) : x(_x) , y(_y) {};
};

std::map<int , std::vector<point> > roiMap ();

std::map<int , std::vector<point> > _roiMap = roiMap();

std::map<int , std::vector<point> > roiMap () {
  	std::map<int , std::vector<point> > _map;
    // ROI 1
    _map[1].push_back(point(50 , 20));
    _map[1].push_back(point(400 , 320));
    // ROI 2
    _map[2].push_back(point(50 , 370));
    _map[2].push_back(point(450 , 1000));
    // ROI 3
    _map[3].push_back(point(470 , 20));
    _map[3].push_back(point(600 , 900));
    // ROI 4
    _map[4].push_back(point(670 , 40));
    _map[4].push_back(point(950 , 550));
    // ROI 5
    _map[5].push_back(point(680 , 700));
    _map[5].push_back(point(1000 , 1000));
  	return _map;
}

#endif