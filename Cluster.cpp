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
	means.resize(num_cluster);
	for (int i = 0; i < num_cluster; i++) {
		means[i]=__means[i];
	}
	rsc = __rsc;
	num_pts = rsc->get_num_pts();
	assignments.resize(num_pts);
	for (int i = 0; i < num_pts; i++) {
		assignments[i]=-1;
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
	vector<Point> tmp_means(num_cluster);
	vector<int> tmp_point_count(num_cluster);
	vector<double> tmp_point(dimension);
	for (int i = 0; i < dimension; i++) {
		tmp_point[i]=0;
	}
	Point zero_point = Point (tmp_point);
	for (int i = 0; i < num_cluster; i++) {
		tmp_point_count[i]=0;
		tmp_means[i]=zero_point;
	}
	rsc->reset_pools();
	int index;
	// maintains sum of points and point counts to each center locally.
	// these two are arrays of pointers to each local structure. After all, we need to combine them later.
	vector<int *> local_tmp_point_count (omp_get_max_threads() );
	vector<Point *> local_tmp_means (omp_get_max_threads() );
	#pragma omp parallel private(index)
	{
		int np = omp_get_num_threads();
		vector<int> local_point_count(num_cluster); // for each thread, init local point counters and summers.
		vector<Point> local_means(num_cluster);
		for (int i = 0; i < num_cluster; i++) {
			local_point_count[i]=0;
			local_means[i]=zero_point;
		}
		int tid = omp_get_thread_num();
		local_tmp_point_count[tid] = local_point_count.data(); // save the pointers to these local structures
		local_tmp_means[tid] = local_means.data();
		#pragma omp for schedule(static)
		for (int i = 0 ; i < num_pts; i++) {
			index = assignments[i];
			local_means[index].add_point (rsc->index_point (i) );
			local_point_count[index] += 1;
		}
		// a for loop is an automatic barrier for openmp. We can be sure that all threads are done doing their work.
		#pragma omp for schedule(static)
		for (int i = 0; i < num_cluster; i++) { // here we do the combining. Note that there are no race conditions here to same memory locations
			for (int p = 0; p < np; p++) {
				tmp_point_count[i] += local_tmp_point_count[p][i];
				tmp_means[i].add_point (local_tmp_means[p][i]);
			}
		}
	}
	for (int i = 0; i < num_cluster; i++) {
		if (tmp_point_count[i] == 0) {
			cout << "Warning: Zero assignments during lloyd iteration" << endl;
			tmp_means[i]=tmp_point; // set to zero
			tmp_point_count[i]=1;
			//exit (1);
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
	vector<int> counts(num_cluster);
	int index = 0;
	double dist = 0;
	double tmp_cost = 0;
	int tmp_assign_change = 0;
	// this is done parallely here. Every core maintains its local data structures and then we merge them all.
	vector<int *> local_tmp_counts (omp_get_max_threads() );
	#pragma omp parallel private(index,dist) reduction(+:tmp_cost,tmp_assign_change)
	{
		// some primitive operations are left to openmp to handle. For example additions, for tmp_cost, tmp_assign_change.
		int np = omp_get_num_threads();
		vector<int> local_counts (num_cluster); // but, the counts for each mean need to be handled in local arrays
		local_tmp_counts[omp_get_thread_num()] = local_counts.data();
		#pragma omp for schedule(static)
		for (int i = 0; i < num_pts; i++) {
			index = belongs_to (rsc->index_point (i), dist);
			assignments[i] = index;
			local_counts[index]++;
			if (index != prev_assign[i]) {
				tmp_assign_change++;
			}
			tmp_cost += (dist * dist);
		}
		#pragma omp for schedule(static)
		// here we combine the results for counts. We're not really using them here. The counts array is for debugging.
		for (int i = 0; i < num_cluster; i++) {
			for (int p = 0; p < np; p++) {
				counts[i] += local_tmp_counts[p][i];
			}
		}
	}
	cost = tmp_cost;
	assign_change = tmp_assign_change;
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
