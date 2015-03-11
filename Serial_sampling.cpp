#include <bits/stdc++.h>
#include "Sampling.h"

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
		vector<double> probabilities;
		for (int line_no = 0; line_no < num_pts; line_no++) {
			Point p1 = rsc->next_point();
			double min_dist = p1.dist (centers[0]);
			for (int i = 1; i < centers.size(); i++) {
				double local_dist = p1.dist (centers[i]);
				min_dist = min (min_dist, local_dist);
			}
			probabilities.push_back (min_dist * min_dist);
		}
		vector<int> indices=sample_indices (probabilities, N);
		return pick_points(indices);
	}
}

vector<Point> Sampling::uniform_sample (int N) {
	vector<double> probabilities;
	for (int i = 0; i < num_pts; i++) {
		probabilities.push_back (1.0);
	}
	vector<int> indices=sample_indices(probabilities,N);
	return pick_points(indices);
}

vector<int> Sampling:: sample_indices (vector<double>& probabilities, int N) {
	random_device rd;
	mt19937 gen(rd());
	unsigned int max_val=gen.max();
	int length = probabilities.size();
	for(int i=1;i<length;i++) {
		probabilities[i] += probabilities[i-1];
	}

	vector<int> ans;
	while (ans.size() < N) {
		double temp_num = (((double) gen())/max_val) *(probabilities[length-1]);		
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
