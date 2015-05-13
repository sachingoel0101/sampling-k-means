#include <bits/stdc++.h>
#include "Point.h"

using namespace std;

#ifndef RESOURCE_H
#define RESOURCE_H

/**
This class provides a Point pool. You can either provide the pool as a file or as a memory location, which is an array of Points.
If you have a file resource, you can only read the next point, where the appropriate pointers are managed by the ifstream object
If you have a memory pool, you can access whatever you want.
If you want a next_point, we maintain a counter to return that
To go to the beginning of the resource, just call reset_pools()
*/
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
