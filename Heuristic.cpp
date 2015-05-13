#include <bits/stdc++.h>
#include "Heuristic.h"
#include "omp.h"

using namespace std;

// this is the first step we do on the multiset. Perform a kmeans++ on it. The heuristic function is actually mean_d2_on_sample
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
    vector<int> counts(k); // number of points assigned to each kmeans++ center
    vector<Point> cluster_means(k); // for taking the centroid later on. We maintain a sum of all points assigned to a center here.
    vector<double> tmp(sampled_set[0].get_dimension());
    for (int i = 0; i < sampled_set[0].get_dimension(); i++) {
        tmp[i]=0;
    }
    Point tmp_point (tmp);
    for (int i = 0; i < k; i++) {
        counts[i]=0;
        cluster_means[i]=tmp_point;
    }
    // here the heuristic does things in a parallel fashion
    // maintain a local structure for each thread to keep track of cluster sums and counts
    vector<int *> local_tmp_counts (omp_get_max_threads() );
    vector<Point *> local_tmp_cluster_means (omp_get_max_threads() );
    #pragma omp parallel
    {
        int np = omp_get_num_threads();
        int tid = omp_get_thread_num();
        vector<int> local_counts (k);
        vector<Point> local_cluster_means(k);
        for (int i = 0; i < k; i++) {
            local_cluster_means[i]=tmp_point;
        }
        local_tmp_counts[tid] = local_counts.data();  // save the pointers to local data structures
        local_tmp_cluster_means[tid] = local_cluster_means.data();
        double min_dist, tmp_dist;
        int index;
        #pragma omp for schedule(static)
        for (int i = 0; i < sampled_set.size(); i++) {
            min_dist = level_2_sample[0].dist (sampled_set[i]);  // distance of each kmeans++ center from the points in sampled_set
            index = 0;
            for (int j = 1; j < k; j++) {
                tmp_dist = level_2_sample[j].dist (sampled_set[i]); // figure out the minimum and assign the point to that kmeans++ center
                if (tmp_dist < min_dist) {
                    min_dist = tmp_dist;
                    index = j;
                }
            }
            local_cluster_means[index].add_point (sampled_set[i]); // add to find centroid later
            local_counts[index]++;
        }
        #pragma omp for schedule(static)
        for (int i = 0; i < k; i++) {
            for (int p = 0; p < np; p++) {
                cluster_means[i].add_point (local_tmp_cluster_means[p][i]); // combine the results for each thread
                counts[i] += local_tmp_counts[p][i];
            }
        }
    }
    int max = counts[0];
    int index = 0;
    for (int i = 1; i < k; i++) {
        if (counts[i] > max) {
            max = counts[i];
            index = i; // largest cluster with maximum points from sampled_set assigned to it.
        }
    }
    cluster_means[index].divide_int (counts[index]); // scaling to find the centroid
    return cluster_means[index];
}
