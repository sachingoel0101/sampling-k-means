#include <bits/stdc++.h>
#include "Point.h"

using namespace std;

#ifndef HEURISTIC_H
#define HEURISTIC_H

/**
 *
 *The first heuristic picks the points based on which points have the most points nearest to them and share in the top lists.
 *The second heuristic picks points by performing d2 sampling based clustering on multiset and picking the point for largest cluster
 *The third heuristic picks points as second, but by picking the cluster with largest delta value related to points already picked
 *The fourth heuristic picks the center the largest cluster, a little bit different than the second one, in the sense that it takes avg
 *The fifth heuristic performs lloyd's on the d2 sampling points on multi set and then picks the largest cluster
 *
 */

class Heuristic{
public:
    Point h1_center(vector<Point> sampled_set,int m);
	Point h2_center(vector<Point> sampled_set,int k);
	Point h3_center(vector<Point> sampled_set,int k,vector<Point> already_chosen);
	Point h4_center(vector<Point> sampled_set,int k);
	Point h5_center(vector<Point> sampled_set,int k);
private:
	vector<Point> h1_subset(vector<Point> sampled_set,int m);
	vector<Point> h2_subset(vector<Point> sampled_set,int k);
	vector<Point> h3_subset(vector<Point> sampled_set,int k);
	vector<Point> h4_subset(vector<Point> sampled_set,int k);
	vector<Point> h5_subset(vector<Point> sampled_set,int k);
};

#endif
