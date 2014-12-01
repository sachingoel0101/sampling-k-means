#include <bits/stdc++.h>
#include <random>
#include "Point.h"

using namespace std;

#ifndef SAMPLING_H
#define SAMPLING_H

/**
 *
 * The field file_name contains the name of file where all the data points are.
 * num_pts contains how many points are there in the file. This is found out when the constructor is called.
 * d2_sample(C,N) generates N points wrt the currently selected C centers.
 * uniform_sample(N) generates N points uniform randomly.
 * sample_indices(d,N) picks N indices according to the distribution d.
 * pick_points(I) picks all the point indices present in I.
 *
 * */

class Sampling{
public:
    Sampling(string);
	Sampling(vector<Point>);
	~Sampling();
	void reset_pools();
	Point next_point();
    vector<Point> d2_sample(vector<Point>,int);
	vector<Point> uniform_sample(int);
	vector<int> sample_indices(discrete_distribution<>,int);
	vector<Point> pick_points(vector<int>);
protected:
    string file_name;
	ifstream file;
	vector<Point> pool;
	int counter;
	int num_pts;
	bool mode;
};

#endif
