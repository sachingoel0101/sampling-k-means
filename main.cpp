#include <bits/stdc++.h>
#include "Point.h"
#include "Cluster.h"
#include "Sampling.h"
#include "Resource.h"
#include "Heuristic.h"
#include "sys/time.h"

using namespace std;

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
	int mode = 0;
	string program (argv[1]);
	if (program.compare ("random") == 0 ) {
		mode = 0;
	}
	else if (program.compare ("kmeans++") == 0 ) {
		mode = 1;
	}
	else if (program.compare ("mean") == 0 ) {
		mode = 2;
	}
	else {
		cout << "Error: Mode " << program << " not available";
		exit (1);
	}
	vector<Point> data = parse_file (argv[2]);
	struct timeval start, end;
	gettimeofday (&start, NULL);
	Resource rsc (&data);
	Sampling sampler (&rsc);
	int num_cluster = stoi (argv[3]);
	vector<Point> p;
	if (mode == 0) {
		p = sampler.uniform_sample (num_cluster);
	}
	else if (mode == 1) {
		vector<Point> tmp;
		for (int i = 0; i < num_cluster; i++) {
			tmp = sampler.d2_sample (p, 1);
			p.push_back (tmp[0]);
		}
	} else {
		Heuristic h;
		int N = stoi (argv[4]);
		vector<Point> tmp;
		for (int i = 0; i < num_cluster; i++) {
			tmp = sampler.d2_sample (p, N);
			p.push_back (h.mean_d2_on_sample (tmp, num_cluster) );
		}
	}
	gettimeofday (&end, NULL);
	cout << "Initialization time: " << (end.tv_sec - start.tv_sec + 1e-6 * (end.tv_usec - start.tv_usec) ) << endl;
	Cluster c (&rsc, p);
	cout << "Iteration 0:" << sqrt (c.get_cost() ) << " Assignment change:" << c.get_assign_change() << endl;
	int iter = 1;
	int totalPts = rsc.get_num_pts();
	while ( ( (double) c.get_assign_change() ) / totalPts > 1e-3) {
		c.iterate();
		cout << "Iteration " << iter << ":" << sqrt (c.get_cost() ) << " Assignment change:" << c.get_assign_change() << endl;
		iter++;
	}
	gettimeofday (&end, NULL);
	cout << "Total time: " << (end.tv_sec - start.tv_sec + 1e-6 * (end.tv_usec - start.tv_usec) ) << endl;
	cout << "Number of iterations:" << iter << endl;
	cout << "Final cost:" << sqrt (c.get_cost() ) << endl;
	return 0;
}