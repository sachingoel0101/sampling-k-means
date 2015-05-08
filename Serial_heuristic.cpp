#include <bits/stdc++.h>
#include "Heuristic.h"

using namespace std;

vector<Point> Heuristic::d2_on_sample (vector<Point> &sampled_set, int k) {
	// here, we have a memory resource, namely, sampled_set as the data set pool
	Resource rsc (&sampled_set);
	// initalize a sampler
	Sampling s (&rsc);
	vector<Point> ret_val;
	for (int i = 0; i < k; i++) {
		// sample k points one by one, to find a kmeans++ initialization
		Point tmp = s.d2_sample (ret_val, 1) [0];
		ret_val.push_back (tmp);
	}
	return ret_val;
}

Point Heuristic::mean_d2_on_sample (vector<Point> &sampled_set, int k) {
	// do a kmeans++ initialization on sampled_set
	vector<Point> level_2_sample = d2_on_sample (sampled_set, k);
	// now we assign each point in sampled_set to centers in level_2_sample and find the largest cluster
	vector<int> counts(k);
	vector<Point> cluster_means(k);
	vector<double> tmp(sampled_set[0].get_dimension());
	for (int i = 0; i < sampled_set[0].get_dimension(); i++) {
		tmp[i]=0;
	}
	Point tmp_point (tmp);
	for (int i = 0; i < k; i++) {
		counts[i]=0;
		cluster_means[i]=tmp_point;
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
		cluster_means[index].add_point (sampled_set[i]); // add each point to the cluster_means. We'll return the mean of largest cluster
		counts[index]++;
	}
	int max = counts[0];
	int index = 0;
	for (int i = 1; i < k; i++) {
		if (counts[i] > max) {
			max = counts[i]; // largest cluster with maximum points from sampled_set assigned to it.
			index = i;
		}
	}
	cluster_means[index].divide_int (counts[index]); // scale to take the centroid
	return cluster_means[index];
}
