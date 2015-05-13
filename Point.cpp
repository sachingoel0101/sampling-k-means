#include <bits/stdc++.h>
#include "Point.h"

using namespace std;

// null point constructor
Point::Point () {
    dimension=0;
    point.resize(0);
}

// point from a vector of coordinates
Point::Point (const vector<double> &__point) {
    dimension = __point.size();
    point.resize (dimension);
    for (int i = 0; i < dimension; i++) {
        point[i] = __point[i];
    }
}

// point from a string of numbers. We aren't checking for sanity. Make sure the string is in proper format
Point::Point (string __line) {
    stringstream ss (__line);
    double tmp;
    dimension = 0;
    while (ss >> tmp) {
        point.push_back (tmp);
        dimension++;
    }
}

// printing a point for debugging purpose. writer can be any output stream. Usually, you might wanna use cout
void Point::print (ostream &writer) const {
    writer << "Dimension:" << dimension << endl;
    writer << "Data:(";
    for (vector<double>::const_iterator it = point.begin(); it != point.end(); ++it) {
        writer  << *it << ' ';
    }
    writer << ")" << endl;
}

// dimension of the point. Zero in case of null point
int Point::get_dimension() const {
    return dimension;
}

// coordinates returned as a vector
vector<double> Point::get_coordinates() const {
    return point;
}

// Euclidean distance calculation. Default distance measure
double Point::dist (const Point &p) const {
    return dist (p, "Euclidean");
}

// distance calculation for any distance measure. Right now, only Euclidean distance implemented
double Point::dist (const Point &p, string dist_fxn) const {
    // sanity checks. Dimensions must be the same
    if (p.get_dimension() != dimension) {
        cout << "Error: Dimension mismatch error while finding distance between:{" << endl;
        print (cout);
        p.print (cout);
        cout << "}" << endl;
        exit (1);
    } else {
        double temp = 0.0;
        // get the coordinates. Just because we're passing by reference doesn't mean you just go ahead and do pointer access. Stick to getter and setter methods. Always a good idea.
        vector<double> p_point = p.get_coordinates();
        if (dist_fxn.compare ("Euclidean") == 0) {
            for (int i = 0; i < dimension; i++) {
                temp += (p_point[i] - point[i]) * (p_point[i] - point[i]);
            }
            temp = sqrt (temp); // just a norm calculation. Nothing special.
        } else {
            cout << "Error: Distance function '" << dist_fxn << "' doesn't exist" << endl;
            exit (1);
        }
        return temp;
    }
}

// add the point p to this
void Point::add_point (const Point &p) {
    // again, sanity checks.
    if (p.get_dimension() != dimension) {
        cout << "Error: Dimension mismatch error while adding:{" << endl;
        print (cout);
        p.print (cout);
        cout << "}" << endl;
        exit (1);
    }
    else {
        vector<double> tmp = p.get_coordinates();
        for (int i = 0; i < dimension; i++) {
            point[i] += tmp[i];
        }
    }
}

// scale this by count.
void Point::divide_int (int count) {
    // can't scale by zero.
    if (count == 0) {
        cout << "Error: Division by zero" << endl;
        exit (1);
    } else {
        for (int i = 0; i < dimension; i++) {
            point[i] /= count; // divide every entry in vector by count
        }
    }
}
