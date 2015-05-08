#include <bits/stdc++.h>
#include "Point.h"
#include "Cluster.h"
#include "Sampling.h"
#include "Resource.h"
#include "Heuristic.h"
#include "sys/time.h"

using namespace std;

/**
* This is a modification over the main.cpp
* Here you get to run every mode we have, by loading the data only once, as many times as you want.
* Helpful for doing a variance analysis
* This also works with any implementation you might have for Sampler,  Heuristic and Cluster.
*/

vector<Point> parse_file (string file_name) {
	ifstream file;
	file.open (file_name.c_str() );
	string line;
	vector<Point> tmp;
	while (getline (file, line) ) {
		tmp.push_back (Point (line) );
	}
	file.close();
	return tmp;
}
int main (int argc, char *argv[]) {
	cout << "*************************************************************\nRunning: ";
	for (int i = 0; i < argc; i++) {
		cout << argv[i] << " ";
	}
	cout << endl;
	string method="parallel";
	if(string(argv[0]).find("parallel")==string::npos) method="serial"; // we're going to write logs to a file prefixed with parallel or serial accordingly
	string data_name=string(argv[1]);
	vector<Point> data = parse_file ("../data/"+data_name);
	Resource rsc (&data);
	Sampling sampler (&rsc);
	int num_cluster = stoi (argv[2]);
	struct timeval init_start, init_end, iter_end;
	string log;
	ofstream logger;
	for(int counter=0;counter<5;counter++){ // specify how many times you want to run every method
		cout << counter << endl;
		for (int mode = 0; mode < 5; mode++) {
			logger.open("../logs/"+method+"_"+data_name+"_"+to_string(num_cluster)+"_"+to_string(mode)+"_"+to_string(counter)+".txt");
			// the output file follows the format: method<parallel/serial>_data file name_num clusters_mode<0 for random, so on>_run_number
			// rest is the same as in the main.cpp file. Just initialize properly and iterate
			gettimeofday (&init_start, NULL);
			vector<Point> p;
			string log;
			if (mode == 0) {
				log = "Random\n";
				cout <<log;
				p = sampler.uniform_sample (num_cluster);
			}
			else if (mode == 1) {
				vector<Point> tmp;
				log = "Kmeans++\n";
				cout<< log;
				for (int i = 0; i < num_cluster; i++) {
					tmp = sampler.d2_sample (p, 1);
					p.push_back (tmp[0]);
				}
			} else {
				int N;
				if (mode == 2) {
					N = 500;
				}
				else if (mode == 3) {
					N = 1000;
				}
				else {
					N = 1500;
				}
				Heuristic h;
				vector<Point> tmp;
				log = "Mean for N=" + to_string (N) + "\n";
				cout<<log;
				for (int i = 0; i < num_cluster; i++) {
					tmp = sampler.d2_sample (p, N);
					p.push_back (h.mean_d2_on_sample (tmp, num_cluster) );
				}
			}
			gettimeofday (&init_end, NULL);
			Cluster c (&rsc, p);
			log = log + "Iteration 0:" + to_string (sqrt (c.get_cost() ) ) + " Assignment change:" + to_string (c.get_assign_change() ) + "\n";
			int iter = 1;
			int totalPts = rsc.get_num_pts();
			while ( ( (double) c.get_assign_change() ) / totalPts > 1e-3) {
				c.iterate();
				log = log + "Iteration " + to_string ( iter) + ":" + to_string (sqrt (c.get_cost() ) ) + " Assignment change:" + to_string (c.get_assign_change() ) + "\n";
				iter++;
			}
			gettimeofday (&iter_end, NULL);
			logger << log;
			logger << "Total time: " << (iter_end.tv_sec - init_start.tv_sec + 1e-6 * (iter_end.tv_usec - init_start.tv_usec) ) << endl;
			logger << "Initialization time: " << (init_end.tv_sec - init_start.tv_sec + 1e-6 * (init_end.tv_usec - init_start.tv_usec) ) << endl;
			logger << "Number of iterations:" << (iter - 1 ) << endl;
			logger << "Time per iteration:" << ( (iter_end.tv_sec - init_end.tv_sec + 1e-6 * (iter_end.tv_usec - init_end.tv_usec) ) / (iter - 1) ) << endl;
			logger << "Final cost:" << sqrt (c.get_cost() ) << endl;
			logger << "****************************************************\n";
			logger.close();
		}
	}
	return 0;
}
