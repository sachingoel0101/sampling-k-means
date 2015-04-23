#include <bits/stdc++.h>

using namespace std;

#ifndef POINT_H
#define POINT_H

/**
 *
 * the vector point stores all the coordinates
 * dist returns euclidean distance for string 'euclidean'. More to be added
 *later.
 * add_point adds the coordinates to this
 * divide_int scales coordinates of this
 *
 * */

class Point {
	public:
		Point ();
		Point (const vector<double> &);
		Point (string);
		void print (ostream &) const;
		int get_dimension() const;
		vector<double> get_coordinates() const;
		double dist (const Point &) const; // default Euclidean distance returned
		double dist (const Point &, string) const;
		void add_point (const Point &);
		void divide_int (int);
	protected:
		vector<double> point;
		int dimension;
};

#endif
