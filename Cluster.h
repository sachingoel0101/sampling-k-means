#include <bits/stdc++.h>
#include "Point.h"
#include "Resource.h"

using namespace std;

#ifndef CLUSTER_H
#define CLUSTER_H

/**
Resource holds the file/memory pool of data set
cost is the current cost of means
means contains the current centers and assignments is an array containing which point is assigned to which center
assign_change is the number of points which change their assignment during a lloyd iteration
cost_change is the cost change during a lloyd iteration
scale_cluster divides each center by the corresponding value in the argument array. This is used while updating the mean during a lloyd iteration. We just sum all points belonging to a particular center and divide to get the centroid
belongs_to finds out what center the point belongs to
iterate does a lloyd iteration
*/

class Cluster {
protected:
    Resource *rsc;
    int num_pts;
    int dimension;
    int num_cluster;
    double cost;
    vector<Point> means;
    vector<int> assignments;
    int assign_change;
    double cost_change;
    void scale_cluster (const vector<int> &);
    int belongs_to (const Point &, double &) const;
    void find_assignments();
public:
    Cluster (Resource *, const vector<Point> &);
    vector<Point> get_means() const;
    int get_dimension() const;
    int get_num_cluster() const;
    vector<int> get_assignments() const;
    double get_cost() const;
    int get_assign_change() const;
    double get_cost_change() const;
    void print (ostream &) const;
    void iterate ();
};

#endif
