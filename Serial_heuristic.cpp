#include <bits/stdc++.h>
#include "Heuristic.h"

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
	for (int i = 0; i < sampled_set.size(); i++) {
		double min_dist = level_2_sample[0].dist (sampled_set[i]);
		int index = 0;
		for (int j = 1; j < k; j++) {
			double tmp_dist = level_2_sample[j].dist (sampled_set[i]);
			if (tmp_dist < min_dist) {
				min_dist = tmp_dist;
				index = j;
			}
		}
		cluster_means[index].add_point (sampled_set[i]);
		counts[index]++;
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
