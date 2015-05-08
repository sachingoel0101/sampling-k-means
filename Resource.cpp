#include <bits/stdc++.h>
#include "Resource.h"

using namespace std;

Resource::Resource (string __file_name) {  // we don't actually use this type of resource. We switched over to a complete memory based data set
	file_name = __file_name;
	num_pts = 0;
	file.open (file_name.c_str() );
	string line;
	while (getline (file, line) ) {
		num_pts++;
	}
	mode = true;
	reset_pools();
}

Resource::Resource (const vector<Point> *__pool) {
	mode = false;
	counter = 0;
	pool = __pool;
	num_pts = pool->size();
}

void Resource::close_file() {
	if (mode) {
		file.close();
	}
}

void Resource::reset_pools() {
	if (mode) {
		file.clear();
		file.seekg (0, ios_base::beg);
	}
	counter = 0;
}

int Resource::get_num_pts () const {
	return num_pts;
}


/**
*
* Access the next point. Acts as an iterator
*
*/
Point Resource::next_point() {
	if (mode && !file.eof() ) {
		string line;
		getline (file, line);
		return Point (line);
	}
	else if (counter < num_pts) {
		counter++;
		return pool->at (counter - 1);
	}
	else {
		cout << "Error: Resource exhausted" << endl;
		exit (1);
	}
}

/**
*
* Access a point at a specific index
*
*/
Point Resource::index_point (int index) const {
	if (mode) {
		cout << "Error: Invalid access from within the resource file" << endl;
		exit (1);
	} else if (counter > num_pts) {
		cout << "Error: Index out of bounds. Resource access failed" << endl;
		exit (1);
	}
	else {
		return pool->at (index);
	}
}
