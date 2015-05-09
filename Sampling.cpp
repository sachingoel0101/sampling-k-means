#include <bits/stdc++.h>
#include "Sampling.h"
#include "omp.h"
#include <sys/time.h>

using namespace std;

//Initialize the sampling object with the given resource
Sampling::Sampling (Resource *__rsc) {
	rsc = __rsc;
	rsc->reset_pools();
	num_pts = rsc->get_num_pts();
}

// This function samples the N indices from resource based on its probability. Here we use already generated random numbers 
// stored in rand array. start_index indicates where to start taking random numbers from rand array. Random numbers are between 0 to 1

vector<int> sample_indices_1 (vector<double> &probabilities, int N, vector<double> & rand, int start_index) {
	int length = probabilities.size();

	for(int i=1;i<length;i++) {
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
			} else if(temp_num > probabilities [mid]){
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


// return the next center using Heuristic with N points multiset to be picked.
// Note that for simple D2 sampling just give N=1

vector<Point> Sampling::d2_sample (const vector<Point> &centers, int N) {
	if (centers.size() == 0) {
		return uniform_sample (N); // if no center has been picked uniformly sample first center
	}
	else {
		rsc->reset_pools(); //reset the resource
		vector<double> distances(num_pts);
		double sum=0;

		//calculate minimum distances of each point parllely with reduction on sum across all threads so to calculate total cost
		#pragma omp parallel reduction(+:sum)
		{
			double min_dist, local_dist;
			Point p1 = rsc->index_point (0);
			// following is the loop that is executed parallely
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
		//simple d2 sampling
		if(N==1){
			vector<int> indices=sample_indices(distances,1);
			return pick_points(indices);
		}

		
		vector<vector<int>> result(omp_get_max_threads());
		vector<int> lower_vals(omp_get_max_threads()); // store lower index for each thread frow where it will start using random numbers from rand array

		
		int total_samples=0;
		
		int per_thread=2*((N/omp_get_max_threads())+1);
		vector<double> random_nos(per_thread*omp_get_max_threads()); // array to store random numbers
		random_device rd;
		mt19937 gen(rd());
		unsigned int max_val=gen.max();
		struct timeval before_ran, after_ran;
		gettimeofday(&before_ran,NULL);

		// generate random numbers
		for(int i=0;i<2*N;i++){
			random_nos[i]=((double) gen())/max_val;
		}
		gettimeofday(&after_ran,NULL);

		// parallely sample indices with reduction on total_samples picked
		#pragma omp parallel reduction(+:total_samples)
		{
			int np=omp_get_num_threads();
			int tid=omp_get_thread_num();
			int batch_size=num_pts/np;
			int lower=tid*batch_size; // calculating the lower inde for this thread ( we will send this to sample_index so it starts using random numbers from this value)
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
			int points_to_sample=(int) ceil(N * (tmp_sum/sum)); //number of points to sample in this thread
			int actual_samples=min(points_to_sample,per_thread);
			
			//cout<<"Sampling needed:"<<points_to_sample<<" Sampling actual:"<<per_thread<<endl;
			result[tid]=sample_indices_1(local_distances,actual_samples,random_nos,tid*per_thread); // pick samples for this thread
			total_samples+=actual_samples;
		}

		
		vector<int> points_to_pick(total_samples);
		int counter=0;

		// make a single array of indices to be picked
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

//uniformly sample N indices
vector<Point> Sampling::uniform_sample (int N) {
	vector<double> probabilities(num_pts);
	for (int i = 0; i < num_pts; i++) {
		probabilities[i] = (1.0);
	}
	vector<int> indices=sample_indices(probabilities,N);
	return pick_points (indices );
}

// This function is similar to sample_indices_1. The only difference is that it doesn't use pre generated random numbers.
vector<int> Sampling:: sample_indices (vector<double> &probabilities, int N) {
	random_device rd;
	mt19937 gen(rd());
	unsigned int max_val=gen.max();
	int length = probabilities.size();
	for(int i=1;i<length;i++) {
		probabilities[i] += probabilities[i-1];
	}
	vector<int> ans(N);
	int i=0;
	while (i<N) {
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
		ans[i]=mid;
		i++;
	}
	return ans;
}

// Given a set of indices, this function returns the point set of those indices.
vector<Point> Sampling:: pick_points (vector<int> & indices) {
	vector<Point> result(indices.size());
	for(int i=0;i<indices.size();i++){
		result[i]=rsc->index_point(indices[i]);
	}
	return result;
}
