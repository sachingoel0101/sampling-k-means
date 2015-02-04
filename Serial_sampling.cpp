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
		discrete_distribution<> d (probabilities.begin(), probabilities.end() );
		return pick_points (sample_indices (d, N) );
	}
}

vector<Point> Sampling::uniform_sample (int N) {
	vector<double> probabilities;
	for (int i = 0; i < num_pts; i++) {
		probabilities.push_back (1.0);
	}
	discrete_distribution<> d (probabilities.begin(), probabilities.end() );
	return pick_points (sample_indices (d, N) );
}

vector<int> Sampling:: sample_indices (discrete_distribution<> d, int N) {
	srand (time (NULL) );
	random_device rd;
	mt19937 gen (rd() );
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
