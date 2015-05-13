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

// This function samples the N indices from resource based on its probability. Here we use already generated random numbers
// stored in rand array. start_index indicates where to start taking random numbers from rand array. Random numbers are between 0 to 1

// READ THIS FUNCTION AFTER YOU'VE GONE THROUGH THE d2_sample function. It's a helper to that one.
vector<int> sample_indices_1 (vector<double> &probabilities, int N, vector<double> & rand, int start_index) {
    int length = probabilities.size();

    for(int i=1; i<length; i++) {
        probabilities[i] += probabilities[i-1]; //making probabilities cumulative but not normalized(between 0 to 1)
    }
    vector<int> ans(N); // array to store indices picked
    int i=0;

    //pick N indices
    while (i<N) {
        double temp_num =  ( rand[start_index+i] )*(probabilities[length-1]);	// generate random number between 0 to final value in cumulative array
        int start=0,end=length-1;
        int mid;

        //doing binary search to locate the index with probability <= temp_num
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

// sample N points from the resource with respect to centers
// for usual D2-sampling in kmeans++, use N=1
vector<Point> Sampling::d2_sample (const vector<Point> &centers, int N) {
    if (centers.size() == 0) {
        return uniform_sample (N); // if no center has been picked uniformly sample first center
    }
    else {
        rsc->reset_pools(); //reset the resource to the beginning. It's a good idea to do this before performing any access on the resource always.
        vector<double> distances(num_pts); // we'll store d^2 of all points here. Proportional to probabilities.
        double sum=0;

        //calculate minimum distances of each point parllely with reduction on sum across all threads so to calculate total cost
        #pragma omp parallel reduction(+:sum)
        {
            double min_dist, local_dist;
            Point p1 = rsc->index_point (0); // useless statement. Could've just used Point p1.s
            // following is the loop that is executed parallely
            #pragma omp for schedule(static)
            for (int line_no = 0; line_no < num_pts; line_no++) {
                p1 = rsc->index_point (line_no);   // make sure you're using a memory based system if you use index_point. If you have a file, better replace this with next_point. It'll work just fine.
                min_dist = p1.dist (centers[0]);
                for (int i = 1; i < centers.size(); i++) {
                    local_dist = p1.dist (centers[i]);
                    min_dist = min (min_dist, local_dist); // calculating minimum distances
                }
                distances[line_no] = (min_dist * min_dist); // this is done without any race conditions. All memory locations are distinct. No two threads are updating the same memory location.
                sum+=min_dist*min_dist; // the total cost of all points. To be used in denominator.
            }
        }
        //simple d2 sampling
        if(N==1) {
            vector<int> indices=sample_indices(distances,1);  // if we have just one point to pick, just use the sample_indices
            return pick_points(indices); // and then, pick that points from the resources.
        }

        // however, if we have lots of points to sample, we need to do this parallely.
        // we're going to maintain local vectors of what indices to pick from the resource
        // this maintains a pointer to all those local vectors
        vector<vector<int>> result(omp_get_max_threads());
        vector<int> lower_vals(omp_get_max_threads()); // every thread is given responsibility to sample from some contiguous chunk of points from the resource. This maintains a list of which thread starts from where.

        // we won't be able to sample exactly N points.
        int total_samples=0;
        // every thread can sample at most 2N/p points. [1 is just taking a ceil]
        int per_thread=2*((N/omp_get_max_threads())+1);
        // next we generate 2N random numbers.[perhaps a bit more because some threads might sample more than what they should because of the 1 we added]
        vector<double> random_nos(per_thread*omp_get_max_threads()); // array to store random numbers
        random_device rd;
        mt19937 gen(rd());  // this is a Mersenne twister rng. Works better than the usual congruence type rng.
        unsigned int max_val=gen.max(); // the maximum value of the rng to normaize to [0,1]
        struct timeval before_ran, after_ran; // just for time measurement for random no generation.
        gettimeofday(&before_ran,NULL);
        // generate random numbers
        for(int i=0; i<2*N; i++) {
            random_nos[i]=((double) gen())/max_val;
        }
        gettimeofday(&after_ran,NULL);

        #pragma omp parallel reduction(+:total_samples)
        {
            int np=omp_get_num_threads();
            int tid=omp_get_thread_num();
            int batch_size=num_pts/np;        // every thread is assigned this many no of points
            int lower=tid*batch_size; // calculating the lower inde for this thread ( we will send this to sample_index so it starts using random numbers from this value)
            lower_vals[tid]=lower;

            int higher=(tid+1)*batch_size;
            if(higher>num_pts) higher=num_pts; // the maximum index a thread has assigned to it. Just to take care of the thread which gets the last chunk assigned to it
            double tmp_sum=0;
            vector<double> local_distances; // distance of these points from the centers
            local_distances.resize(higher-lower);
            for(int i=lower; i<higher; i++) {
                tmp_sum+=distances[i];  // maintain the cost of this set of points
                local_distances[i-lower]=distances[i]; // we already calculated this parallely.
            }
            int points_to_sample=(int) ceil(N * (tmp_sum/sum)); //number of points to sample in this thread.
            int actual_samples=min(points_to_sample,per_thread); // can't be more than 2N/p. We only have that many random nos for each thread

            result[tid]=sample_indices_1(local_distances,actual_samples,random_nos,tid*per_thread); // pick samples for this thread
            total_samples+=actual_samples; // update the total samples we actually get. This is more than N.
        }


        vector<int> points_to_pick(total_samples);
        int counter=0;

        // make a single array of indices to be picked
        // just merge all the indices every thread picked
        for(int i=0; i<result.size(); i++) {
            vector<int> tmp=result[i];
            int length=tmp.size();
            for(int j=0; j<length; j++) {
                points_to_pick[counter]=tmp[j]+lower_vals[i];
                counter++;
            }
        }
        // and pick those indices from the resource
        return pick_points(points_to_pick);
    }
}

//uniformly sample N indices
vector<Point> Sampling::uniform_sample (int N) {
    vector<double> probabilities(num_pts);
    for (int i = 0; i < num_pts; i++) {
        probabilities[i] = (1.0);
    }
    vector<int> indices=sample_indices(probabilities,N);
    return pick_points (indices );
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
