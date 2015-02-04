#include <bits/stdc++.h>
#include "Cluster.h"
#include "omp.h"

using namespace std;

Cluster::Cluster (Resource *__rsc, const vector<Point> &__means) {
	int tmp = __means[0].get_dimension();
	for (vector<Point>::const_iterator it = __means.begin(); it != __means.end(); ++it) {
		Point tmp1 = *it;
		if (tmp1.get_dimension() != tmp) {
			cout << "Error: Dimension mismatch error while initializing cluster" << endl;
			exit (1);
		}
	}
	dimension = tmp;
	num_cluster = __means.size();
	for (int i = 0; i < num_cluster; i++) {
		means.push_back (__means[i]);
	}
	rsc = __rsc;
	num_pts = rsc->get_num_pts();
	for (int i = 0; i < num_pts; i++) {
		assignments.push_back (-1);
	}
	cost = DBL_MAX;
	find_assignments();
}

vector<Point> Cluster::get_means() const {
	return means;
}

int Cluster::get_dimension() const {
	return dimension;
}

int Cluster::get_num_cluster() const {
	return num_cluster;
}

vector<int> Cluster::get_assignments() const {
	return assignments;
}

double Cluster::get_cost () const {
	return cost;
}

double Cluster::get_cost_change () const {
	return cost_change;
}

int Cluster::get_assign_change () const {
	return assign_change;
}

void Cluster::print (ostream &writer) const {
	for (int i = 0; i < num_cluster; i++) {
		means[i].print (writer);
	}
	writer << "" << endl;
}

void Cluster::iterate () {
	vector<Point> tmp_means;
	vector<int> tmp_point_count;
	vector<double> tmp_point;
	for (int i = 0; i < dimension; i++) {
		tmp_point.push_back (0);
	}
	Point zero_point = Point (tmp_point);
	for (int i = 0; i < num_cluster; i++) {
		tmp_point_count.push_back (0);
		tmp_means.push_back (zero_point);
	}
	rsc->reset_pools();
	int index;
	for (int i = 0 ; i < num_pts; i++) {
		index = assignments[i];
		tmp_means[index].add_point (rsc->index_point (i) );
		tmp_point_count[index] += 1;
	}
	for (int i = 0; i < num_cluster; i++) {
		if (tmp_point_count[i] == 0) {
			cout << "Warning: Zero assignments during lloyd iteration" << endl;
			exit (1);
		}
	}
	means = tmp_means;
	scale_cluster (tmp_point_count);
	find_assignments();
}

void Cluster::find_assignments() {
	vector<int> prev_assign = assignments;
	double prev_cost = cost;
	cost = 0;
	assign_change = 0;
	rsc->reset_pools();
	vector<int> counts;
	counts.resize (num_cluster);
	int index = 0;
	double dist = 0;
	for (int i = 0; i < num_pts; i++) {
		index = belongs_to (rsc->index_point (i), dist);
		assignments[i] = index;
		counts[index]++;
		if (index != prev_assign[i]) {
			assign_change++;
		}
		cost += (dist * dist);
	}
	/**cout << "Point assignments: ";
	for (int i = 0; i < num_cluster; i++) {
		cout << counts[i] << " ";
	}
	cout << "" << endl;*/
	cost_change = cost - prev_cost;
}

int Cluster::belongs_to (const Point &p, double &dist) const {
	int index = 0;
	double min_dist = DBL_MAX;
	double tmp_dist;
	for (int i = 0; i < num_cluster; i++) {
		tmp_dist = means[i].dist (p);
		if (tmp_dist < min_dist) {
			index = i;
			min_dist = tmp_dist;
		}
	}
	dist = min_dist;
	return index;
}

void Cluster::scale_cluster (const vector<int> &point_count) {
	for (int i = 0; i < num_cluster; i++) {
		means[i].divide_int (point_count[i]);
	}
}
