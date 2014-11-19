#include <bits/stdc++.h>
#include "Point.h"

using namespace std;

#ifndef HEURISTIC_H
#define HEURISTIC_H

class Heuristic{
public:
    Point h1_center(vector<Point> sampled_set,int m);
private:
	vector<Point> h1_subset(vector<Point> sampled_set, int m);
};

#endif
