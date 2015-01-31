#include <bits/stdc++.h>
#include "Point.h"
#include "Resource.h"

using namespace std;

#ifndef CLUSTER_H
#define CLUSTER_H

/**
 *
 * scale_cluster divides all cluster means by the corresponding values in the argument vector
 * get_cost assigns the voronoi partitions and then calculates cost based on  distance measure
 *
 **/

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
