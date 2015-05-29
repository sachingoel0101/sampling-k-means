#include <bits/stdc++.h>
#include "Sampling.h"
#include "omp.h"
#include <sys/time.h>

using namespace std;

//Initialize the sampling object with the given resource. We'll use this resource to sample the points
Sampling::Sampling (Resource *__rsc) {
    rsc = __rsc;
    rsc->reset_pools();
    num_pts = rsc->get_num_pts();
}

// find where the value p lies in the cumulative distribution. Only consider the range [startIndex,endIndex)
int sample_from_distribution (vector<double> &probabilities, int startIndex, int endIndex, double p) {
    int start=startIndex,end=endIndex-1;
    int mid;
    while(start<=end) {
        mid = (start+end)/2;
        if(p<probabilities[mid-1]) {
            end = mid-1;
        } else if(p > probabilities [mid]) {
            start = mid+1;
        } else {
            break;
        }
    }
    return mid;
}


// sample N points from the resource with respect to centers
// for usual D2-sampling in kmeans++, use N=1

vector<Point> Sampling::d2_sample (const vector<Point> &centers, int N) {
    random_device rd;
    mt19937 gen(rd());
    unsigned int max_val=gen.max();
    vector<double> rand_nos(2*N);
    // two random number for each point to pick. We have two distribution to sample from.
    for(int i=0;i<2*N;i++){
        rand_nos[i] = ((double)gen())/max_val;
    }
    rsc->reset_pools(); //reset the resource to the beginning. It's a good idea to do this before performing any access on the resource always.
    int max_thread = omp_get_max_threads();
    vector<double> distances(num_pts); // cumulative probability for each group of points
    vector<double> local_sums(max_thread);   // local sums. first is [0...num_pts/max_thread-1], and so on.
    vector<Point> result(N);
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int num_thread = omp_get_num_threads();
        int per_thread = num_pts/num_thread;
        int lower = tid*per_thread;
        int higher = (tid+1)*per_thread;
        if(tid == num_thread - 1) higher = num_pts;
        int block_size = higher-lower;
        double min_dist, local_dist;
        Point p;
        double prev_val = 0;
        double local_sum = 0;
        int center_size = centers.size();
        for(int i=0;i<block_size;i++){
            if(center_size == 0){
                local_sum += 1.0;
                distances[lower+i] = i+1.0;  // for uniform sample, just keep adding one.
            } else{
                p = rsc->index_point (lower+i);
                min_dist = p.dist(centers[0]);
                for (int j = 1; j < centers.size(); j++) {
                    local_dist = p.dist (centers[j]);
                    min_dist = min (min_dist, local_dist); // calculating minimum distances
                }
                local_sum += min_dist * min_dist; // sum of this group
                distances[lower+i] = min_dist*min_dist + prev_val; // cumulative distribution in this group
                prev_val = distances[lower+i];
            }
        }
        local_sums[tid] = local_sum;
        #pragma omp barrier // everyone is here now
        #pragma omp master
        {
            for(int i=1;i<num_thread;i++){
                local_sums[i] = local_sums[i] + local_sums[i-1];  // build cumulative for each group
            }
        }
        #pragma omp barrier
        #pragma omp for
        for(int i=0;i<N;i++){
            // first sample from the groups
            int groupNo = sample_from_distribution(local_sums, 0, num_thread-1, rand_nos[i*2]*local_sums[num_thread-1]);
            int startIndex = groupNo*per_thread;
            int endIndex = (groupNo+1)*per_thread;
            if(groupNo == num_thread - 1) endIndex = num_pts;
            // then from within the group
            result[i] = rsc->index_point(sample_from_distribution(distances, startIndex, endIndex, rand_nos[2*i+1]*distances[endIndex-1]));
        }
    }
    return result;
}

//uniformly sample N indices
vector<Point> Sampling::uniform_sample (int N) {
    vector<Point> centers(0);
    return d2_sample(centers,N);
}

// generate a random number and pick the index corresponding to this from the probability distribution
vector<int> Sampling:: sample_indices (vector<double> &probabilities, int N) {
    random_device rd;
    mt19937 gen(rd());
    unsigned int max_val=gen.max();
    int length = probabilities.size();
    // first build a cumulative distribution
    for(int i=1; i<length; i++) {
        probabilities[i] += probabilities[i-1];
    }
    vector<int> ans(N);
    int i=0;
    while (i<N) {
        double temp_num =  ( ((double) gen())/max_val )*(probabilities[length-1]); // the division is necessary as the vector probabilities isn't normalized
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
        ans[i]=mid;
        i++;
    }
    return ans;
}

// Given a set of indices, this function returns the point set of those indices.
vector<Point> Sampling:: pick_points (vector<int> & indices) {
    vector<Point> result(indices.size());
    for(int i=0; i<indices.size(); i++) {
        result[i]=rsc->index_point(indices[i]); // clearly can't use this with a file based resource.
        // if you have a file based resource, you'll have to write your own function for this. You could perhaps look at an earlier revision of our code. We did write it. I'll update which exact revision later.
    }
    return result;
}
