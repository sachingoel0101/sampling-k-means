#include <bits/stdc++.h>
#include <random>
#include "Point.h"
#include "Resource.h"

using namespace std;

#ifndef SAMPLING_H
#define SAMPLING_H

/**
 *
 * d2_sample(C,N) generates N points wrt the currently selected C centers.
 * uniform_sample(N) generates N points uniform randomly.
 * sample_indices(d,N) picks N indices according to the distribution d.
 * pick_points(I) picks all the point indices present in I.
 *
 * */

class Sampling {
	public:
		Sampling (Resource *);
		vector<Point> d2_sample (const vector<Point> &, int);
		vector<Point> uniform_sample (int);
		vector<int> sample_indices (vector<double> &, int);
		vector<Point> pick_points (vector<int> &);
	protected:
		Resource *rsc;
		int num_pts;
};

#endif
