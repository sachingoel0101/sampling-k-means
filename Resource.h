#include <bits/stdc++.h>
#include "Point.h"

using namespace std;

#ifndef RESOURCE_H
#define RESOURCE_H

class Resource {
	public:
		Resource (string);
		Resource (const vector<Point> *);
		void close_file();
		void reset_pools();
		int get_num_pts() const;
		Point next_point();
		Point index_point (int) const;
	protected:
		string file_name;
		ifstream file;
		const vector<Point>  *pool;
		int counter;
		int num_pts;
		bool mode;
};

#endif
