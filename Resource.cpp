#include <bits/stdc++.h>
#include "Resource.h"

using namespace std;

// initialize the resource with data from a file. Not recommended unless you have a file that is too big to fit in a memory. Otherwise, just parse the file to a vector of Points, and use the second constructor
Resource::Resource (string __file_name) {
    file_name = __file_name;
    num_pts = 0;
    file.open (file_name.c_str() );
    string line;
    // not actually reading the data. Just counting the number of points for later use.
    while (getline (file, line) ) {
        num_pts++;
    }
    // determines that this resource is based on data from a file
    mode = true;
    // reset the file pointer. We're at beginning of the file again
    reset_pools();
}

// create a resource from a vector of Points in memory
Resource::Resource (const vector<Point> *__pool) {
    // now, we have a vector of Points
    mode = false;
    // right now, we have accessed none of the elements of this vector
    counter = 0;
    // store the pointer to the data pool
    pool = __pool;
    // number of points for later use
    num_pts = pool->size();
}

// close the file if the resource was based on a file
void Resource::close_file() {
    if (mode) {
        file.close();
    }
}

// reset all pointers. If file based access, go to the beginning. Else, make counter=0 so we know that we have to access the 0th element of the point pool next time someone calls next_point
void Resource::reset_pools() {
    if (mode) {
        file.clear();
        file.seekg (0, ios_base::beg);
    }
    counter = 0;
}

// returns the number of points in the resource
int Resource::get_num_pts () const {
    return num_pts;
}

// access the next point in the resource. For files, easy. Just do a getline and parse. For memory based, increment counter.
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

// access a point at a particular location in the pool. Works only with a memory based resource
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
