#include <bits/stdc++.h>
#include "Point.h"
#include "Sampling.h"
#include "Cluster.h"
#include "Resource.h"

using namespace std;

#ifndef HEURISTIC_H
#define HEURISTIC_H

/**
 *
 *The top_m heuristic picks the points based on which points have the most points nearest to them and share in the top lists.
 *The large_d2* heuristic picks points by performing d2 sampling based clustering on multiset and picking the point for largest cluster
 *The delta_d2* heuristic picks points as second, but by picking the cluster with largest delta value related to points already picked
 *The mean_d2* heuristic picks the center the largest cluster, a little bit different than the second one, in the sense that it takes avg
 *The lloyd_d2* heuristic performs lloyd's on the d2 sampling points on multi set and then picks the largest cluster
 *
 */

class Heuristic {
	public:
		//Point top_m (vector<Point> &, int);
		//Point large_d2_on_sample (vector<Point> &, int);
		//Point delta_large_d2_on_sample (vector<Point> &, int k, vector<Point> &);
		Point mean_d2_on_sample (vector<Point> &, int);
		//Point lloyd_d2_on_sample (vector<Point> &, int);
	private:
		//vector<Point> top_m_set (vector<Point> &, int);
		vector<Point> d2_on_sample (vector<Point> &, int);
};

#endif
