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
			discrete_distribution<> d (distances.begin(),distances.end());
			return pick_points(sample_indices(d,1));
		}
		vector<vector<int>> result;
		result.resize(omp_get_max_threads());
		vector<int> batch_sizes;
		batch_sizes.resize(omp_get_max_threads());
		int total_samples=0;
		#pragma omp parallel reduction(+:total_samples)
		{
			int np=omp_get_num_threads();
			int tid=omp_get_thread_num();
			int batch_size=num_pts/np;
			batch_sizes[tid]=batch_size;
			int lower=tid*batch_size;
			int higher=(tid+1)*batch_size;
			if(higher>num_pts) higher=num_pts;
			double tmp_sum=0;
			for(int i=lower;i<higher;i++){
				tmp_sum+=distances[i];
			}
			discrete_distribution<> d(distances.begin()+lower,distances.begin()+higher);
			int points_to_sample=(int) ceil(N * (tmp_sum/sum));
			total_samples+=points_to_sample;
			result[tid]=sample_indices(d,points_to_sample);
		}
		int batch_size=batch_sizes[0];
		vector<int> points_to_pick;
		points_to_pick.resize(total_samples);
		int counter=0;
		for(int i=0;i<result.size();i++){
			vector<int> tmp=result[i];
			int length=tmp.size();
			for(int j=0;j<length;j++){
				points_to_pick[counter]=tmp[j]+i*batch_size;
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
	discrete_distribution<> d (probabilities.begin(), probabilities.end() );
	return pick_points (sample_indices (d, N) );
}

vector<int> Sampling:: sample_indices (discrete_distribution<> d, int N) {
	srand (time (NULL) );
	random_device rd;
	mt19937 gen(rd());
	vector<int> ans;
	while (ans.size() < N) {
		int chosen = d (gen);
		ans.push_back (chosen);
	}
	sort (ans.begin(), ans.end() );
	return ans;
}

vector<Point> Sampling::pick_points (const vector<int> &indices) {
	vector<Point> current_set;
	vector<int>::const_iterator it = indices.begin();
	int temp = 0;
	rsc->reset_pools();
	Point p1 = rsc->next_point();
	int tmp_cout = 0;
	while (tmp_cout < num_pts) {
		if (temp == (*it) ) {
			current_set.push_back (p1);
			it++;
			if (it == indices.end() ) {
				break;
			}
		} else {
			p1 = rsc->next_point();
			temp++;
			tmp_cout++;
		}
	}
	return current_set;
}
