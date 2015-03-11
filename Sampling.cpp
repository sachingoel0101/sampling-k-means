#include <bits/stdc++.h>
#include "Sampling.h"
#include "omp.h"

using namespace std;

Sampling::Sampling (Resource *__rsc) {
	rsc = __rsc;
	rsc->reset_pools();
	num_pts = rsc->get_num_pts();
}

vector<Point> Sampling::d2_sample (const vector<Point> &centers, int N) {
	if (centers.size() == 0) {
		return uniform_sample (N);
	}
	else {
		rsc->reset_pools();
		vector<double> distances;
		distances.resize (num_pts);
		double sum=0;
		#pragma omp parallel reduction(+:sum)
		{
			double min_dist, local_dist;
			Point p1 = rsc->index_point (0);
			#pragma omp for schedule(static)
			for (int line_no = 0; line_no < num_pts; line_no++) {
				p1 = rsc->index_point (line_no);
				min_dist = p1.dist (centers[0]);
				for (int i = 1; i < centers.size(); i++) {
					local_dist = p1.dist (centers[i]);
					min_dist = min (min_dist, local_dist);
				}
				distances[line_no] = (min_dist * min_dist);
				sum+=min_dist*min_dist;
			}
		}
		if(N==1){
			vector<int> indices=sample_indices(distances,1);
			return pick_points(indices);
		}
		vector<vector<int>> result;
		result.resize(omp_get_max_threads());
		vector<int> lower_vals;
		lower_vals.resize(omp_get_max_threads());
		int total_samples=0;
		#pragma omp parallel reduction(+:total_samples)
		{
			int np=omp_get_num_threads();
			int tid=omp_get_thread_num();
			int batch_size=num_pts/np;
			int lower=tid*batch_size;
			lower_vals[tid]=lower;
			int higher=(tid+1)*batch_size;
			if(higher>num_pts) higher=num_pts;
			double tmp_sum=0;
			vector<double> local_distances;
			local_distances.resize(higher-lower);
			for(int i=lower;i<higher;i++){
				tmp_sum+=distances[i];
				local_distances[i-lower]=distances[i];
			}
			int points_to_sample=(int) ceil(N * (tmp_sum/sum));
			total_samples+=points_to_sample;
			result[tid]=sample_indices(local_distances,points_to_sample);
		}
		vector<int> points_to_pick;
		points_to_pick.resize(total_samples);
		int counter=0;
		for(int i=0;i<result.size();i++){
			vector<int> tmp=result[i];
			int length=tmp.size();
			for(int j=0;j<length;j++){
				points_to_pick[counter]=tmp[j]+lower_vals[i];
				counter++;
			}
		}
		return pick_points(points_to_pick);
	}
}

vector<Point> Sampling::uniform_sample (int N) {
	vector<double> probabilities;
	probabilities.resize (num_pts);
	for (int i = 0; i < num_pts; i++) {
		probabilities[i] = (1.0);
	}
	vector<int> indices=sample_indices(probabilities,N);
	return pick_points (indices );
}

vector<int> Sampling:: sample_indices (vector<double> &probabilities, int N) {
	random_device rd;
	mt19937 gen(rd());
	unsigned int max_val=gen.max();
	int length = probabilities.size();
	for(int i=1;i<length;i++) {
		probabilities[i] += probabilities[i-1];
	}
	vector<int> ans;
	while (ans.size() < N) {
		double temp_num =  ( ((double) gen())/max_val )*(probabilities[length-1]);		
		int start=0,end=length-1;
		int mid;
		while(start<=end) {
			mid = (start+end)/2;
			if(temp_num<probabilities[mid-1]) {
				end = mid-1;
			} else if(temp_num > probabilities [mid]){
				start = mid+1;
			} else {
				break;
			}
		}
		ans.push_back (mid);
	}
	return ans;
}

vector<Point> Sampling:: pick_points (vector<int> & indices) {
	vector<Point> result;
	for(vector<int>::iterator it=indices.begin();it!=indices.end();++it){
		result.push_back(rsc->index_point(*it));
	}
	return result;
}
