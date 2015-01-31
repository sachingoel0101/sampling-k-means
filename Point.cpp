#include <bits/stdc++.h>
#include "Point.h"

using namespace std;

Point::Point (const vector<double> &__point) {
	dimension = __point.size();
	point.resize (dimension);
	for (int i = 0; i < dimension; i++) {
		point[i] = __point[i];
	}
}

Point::Point (string __line) {
	stringstream ss (__line);
	double tmp;
	dimension = 0;
	while (ss >> tmp) {
		point.push_back (tmp);
		dimension++;
	}
}

void Point::print (ostream &writer) const {
	writer << "Dimension:" << dimension << endl;
	writer << "Data:(";
	for (vector<double>::const_iterator it = point.begin(); it != point.end(); ++it) {
		writer  << *it << ' ';
	}
	writer << ")" << endl;
}

int Point::get_dimension() const {
	return dimension;
}

vector<double> Point::get_coordinates() const {
	return point;
}

double Point::dist (const Point &p) const {
	return dist (p, "Euclidean");
}

double Point::dist (const Point &p, string dist_fxn) const {
	if (p.get_dimension() != dimension) {
		cout << "Error: Dimension mismatch error while finding distance between:{" << endl;
		print (cout);
		p.print (cout);
		cout << "}" << endl;
		exit (1);
	} else {
		double temp = 0.0;
		vector<double> p_point = p.get_coordinates();
		if (dist_fxn.compare ("Euclidean") == 0) {
			for (int i = 0; i < dimension; i++) {
				temp += (p_point[i] - point[i]) * (p_point[i] - point[i]);
			}
			temp = sqrt (temp);
		} else {
			cout << "Error: Distance function '" << dist_fxn << "' doesn't exist" << endl;
			exit (1);
		}
		return temp;
	}
}

void Point::add_point (const Point &p) {
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

void Point::divide_int (int count) {
	if (count == 0) {
		cout << "Error: Division by zero" << endl;
		exit (1);
	} else {
		for (int i = 0; i < dimension; i++) {
			point[i] /= count;
		}
	}
}
