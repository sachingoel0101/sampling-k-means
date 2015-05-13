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
        vector<double> probabilities(num_pts);
        // find the distance square values for each point in the data set from the current means in centers
        for (int line_no = 0; line_no < num_pts; line_no++) {
            Point p1 = rsc->next_point(); // read next and next point
            double min_dist = p1.dist (centers[0]);
            for (int i = 1; i < centers.size(); i++) {
                double local_dist = p1.dist (centers[i]);
                min_dist = min (min_dist, local_dist); // minimum distance
            }
            probabilities[line_no]=(min_dist * min_dist); // remember! distance squared. Not distance.
        }
        vector<int> indices=sample_indices (probabilities, N); // probabilities will actually be proportional to these distances.
        return pick_points(indices); // pick the corresponding indices from the data set
    }
}

vector<Point> Sampling::uniform_sample (int N) {
    vector<double> probabilities(num_pts);
    for (int i = 0; i < num_pts; i++) {
        probabilities[i]=1.0; // every one has an equal chance to be picked up
    }
    vector<int> indices=sample_indices(probabilities,N);
    return pick_points(indices);
}

vector<int> Sampling:: sample_indices (vector<double>& probabilities, int N) {
    random_device rd;
    mt19937 gen(rd()); // this is a Mersenne Twister generator. Provides better performance than simple congruence based rng
    unsigned int max_val=gen.max(); // to convert to value between 0 and 1
    int length = probabilities.size();
    for(int i=1; i<length; i++) {
        probabilities[i] += probabilities[i-1]; // build a cumulative probability distribution
    }

    vector<int> ans(N);
    int i=0;
    while (i < N) {
        double temp_num = (((double) gen())/max_val) *(probabilities[length-1]);
        // do a binary search for the required probability value in the cumulative probability distribution
        int start=0,end=length-1;
        int mid;
        while(start<=end) {
            mid = (start+end)/2;
            if(temp_num<probabilities[mid-1]) {
                end = mid-1;
            } else if(temp_num > probabilities [mid]) {
                start = mid+1;
            } else {
                break;
            }
        }
        ans[i]=(mid);
        i++;
    }
    return ans;
}

vector<Point> Sampling:: pick_points (vector<int> & indices) {
    // just select the respective points from the resource
    vector<Point> result(indices.size());
    for(int i=0; i<indices.size(); i++) {
        result[i]=rsc->index_point(indices[i]);
    }
    return result;
}
