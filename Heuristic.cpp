#include <bits/stdc++.h>
#include "Heuristic.h"
#include "omp.h"

using namespace std;

vector<Point> Heuristic::d2_on_sample (vector<Point> &sampled_set, int k) {
	Resource rsc (&sampled_set);
	Sampling s (&rsc);
	vector<Point> ret_val;
	for (int i = 0; i < k; i++) {
		Point tmp = s.d2_sample (ret_val, 1) [0];
		ret_val.push_back (tmp);
	}
	return ret_val;
}

Point Heuristic::mean_d2_on_sample (vector<Point> &sampled_set, int k) {
	vector<Point> level_2_sample = d2_on_sample (sampled_set, k);
	vector<int> counts;
	vector<Point> cluster_means;
	vector<double> tmp;
	for (int i = 0; i < sampled_set[0].get_dimension(); i++) {
		tmp.push_back (0);
	}
	Point tmp_point (tmp);
	for (int i = 0; i < k; i++) {
		counts.push_back (0);
		cluster_means.push_back (tmp_point);
	}
	vector<int *> local_tmp_counts (omp_get_max_threads() );
	vector<Point *> local_tmp_cluster_means (omp_get_max_threads() );
	#pragma omp parallel
	{
		int np = omp_get_num_threads();
		int tid = omp_get_thread_num();
		vector<int> local_counts (k);
		vector<Point> local_cluster_means;
		for (int i = 0; i < k; i++) {
			local_cluster_means.push_back (tmp_point);
		}
		local_tmp_counts[tid] = local_counts.data();
		local_tmp_cluster_means[tid] = local_cluster_means.data();
		double min_dist, tmp_dist;
		int index;
		#pragma omp for schedule(dynamic)
		for (int i = 0; i < sampled_set.size(); i++) {
			min_dist = level_2_sample[0].dist (sampled_set[i]);
			index = 0;
			for (int j = 1; j < k; j++) {
				tmp_dist = level_2_sample[j].dist (sampled_set[i]);
				if (tmp_dist < min_dist) {
					min_dist = tmp_dist;
					index = j;
				}
			}
			local_cluster_means[index].add_point (sampled_set[i]);
			local_counts[index]++;
		}
		#pragma omp for schedule(dynamic)
		for (int i = 0; i < k; i++) {
			for (int p = 0; p < np; p++) {
				cluster_means[i].add_point (local_tmp_cluster_means[p][i]);
				counts[i] += local_tmp_counts[p][i];
			}
		}
	}
	int max = counts[0];
	int index = 0;
	for (int i = 1; i < k; i++) {
		if (counts[i] > max) {
			max = counts[i];
			index = i;
		}
	}
	cluster_means[index].divide_int (counts[index]);
	return cluster_means[index];
}
