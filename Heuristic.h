#include <bits/stdc++.h>
#include "Point.h"
#include "Sampling.h"
#include "Cluster.h"
#include "Resource.h"

using namespace std;

#ifndef HEURISTIC_H
#define HEURISTIC_H

/**
The mean_d2_on_sample heuristic picks the center the largest cluster, a little bit different than the second one, in the sense that it takes avg
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
