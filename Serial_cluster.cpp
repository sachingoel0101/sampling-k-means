#include <bits/stdc++.h>
#include "Cluster.h"
#include "omp.h"

using namespace std;

Cluster::Cluster (Resource *__rsc, const vector<Point> &__means) {
    // just copying stuff and initializaing the cluster properly, sanity checks for dimensionality, number of clusters
    int tmp = __means[0].get_dimension();
    for (vector<Point>::const_iterator it = __means.begin(); it != __means.end(); ++it) {
        Point tmp1 = *it;
        if (tmp1.get_dimension() != tmp) {
            cout << "Error: Dimension mismatch error while initializing cluster" << endl;
            exit (1);
        }
    }
    dimension = tmp;
    num_cluster = __means.size();
    means.resize(num_cluster);
    for (int i = 0; i < num_cluster; i++) {
        means[i]=__means[i];
    }
    rsc = __rsc;
    num_pts = rsc->get_num_pts();
    assignments.resize(num_pts);
    // initially every point is assigned to -1, i.e., null
    for (int i = 0; i < num_pts; i++) {
        assignments[i]=-1;
    }
    cost = DBL_MAX;
    // and we do an initial assignment of each point. This might seem counter-intuitive, since in the lloyd iteration, we will
    // do a mean update first and then assign points. But, doesn't matter. We're eventually getting what we want. A reduction in
    // cost
    find_assignments();
}

vector<Point> Cluster::get_means() const {
    return means;
}

int Cluster::get_dimension() const {
    return dimension;
}

int Cluster::get_num_cluster() const {
    return num_cluster;
}

vector<int> Cluster::get_assignments() const {
    return assignments;
}

double Cluster::get_cost () const {
    return cost;
}

double Cluster::get_cost_change () const {
    return cost_change;
}

int Cluster::get_assign_change () const {
    return assign_change;
}

void Cluster::print (ostream &writer) const {
    for (int i = 0; i < num_cluster; i++) {
        means[i].print (writer);
    }
    writer << "" << endl;
}

void Cluster::iterate () {
    // initialize new means for the cluster. Just start from zero
    vector<Point> tmp_means(num_cluster);
    vector<int> tmp_point_count(num_cluster);
    vector<double> tmp_point(dimension);
    for (int i = 0; i < dimension; i++) {
        tmp_point[i]=0;
    }
    Point zero_point = Point (tmp_point);
    for (int i = 0; i < num_cluster; i++) {
        tmp_point_count[i]=0;
        tmp_means[i]=zero_point;
    }
    // now, start from the first point and add every point to whatever cluster center it belongs to. We already
    // have the assignments from the previous iteration
    rsc->reset_pools();
    int index;
    for (int i = 0 ; i < num_pts; i++) {
        index = assignments[i];
        tmp_means[index].add_point (rsc->index_point (i) );
        tmp_point_count[index] += 1;
    }
    for (int i = 0; i < num_cluster; i++) {
        if (tmp_point_count[i] == 0) {
            // this is an issue. If this is happening, you've clearly done a bad initialization.
            // but no reason to start afresh. Just start from zero again. It's a bad solution anyway.
            cout << "Warning: Zero assignments during lloyd iteration" << endl;
            tmp_means[i]=tmp_point; // set zero
            tmp_point_count[i]=1;
            //exit (1);
        }
    }
    means = tmp_means;
    // now, just take the centroid
    scale_cluster (tmp_point_count);
    // find the new assignments for each point
    find_assignments();
}

void Cluster::find_assignments() {
    vector<int> prev_assign = assignments;
    double prev_cost = cost;
    cost = 0;
    assign_change = 0;
    // remember, always reset the pools before starting an iteration over all points in the data set
    rsc->reset_pools();
    vector<int> counts(num_cluster);
    int index = 0;
    double dist = 0;
    for (int i = 0; i < num_pts; i++) {
        // note that we have already updated the means in the iterate function. So these assignments are wrt new means
        index = belongs_to (rsc->index_point (i), dist);
        assignments[i] = index;
        counts[index]++;
        // this is to have a different convergence criteria than the usual cost change. You can check how many points
        // have changed their assignments
        if (index != prev_assign[i]) {
            assign_change++;
        }
        cost += (dist * dist);
    }
    cost_change = cost - prev_cost;
}

int Cluster::belongs_to (const Point &p, double &dist) const {
    // just figure out the center nearest to the point p. We also return the minimum distance we find in dist.
    int index = 0;
    double min_dist = DBL_MAX;
    double tmp_dist;
    for (int i = 0; i < num_cluster; i++) {
        tmp_dist = means[i].dist (p);
        if (tmp_dist < min_dist) {
            index = i;
            min_dist = tmp_dist;
        }
    }
    dist = min_dist;
    return index;
}

void Cluster::scale_cluster (const vector<int> &point_count) {
    // for each mean, divide by the corresponding entry in point count. This is essentially taking a centroid
    for (int i = 0; i < num_cluster; i++) {
        means[i].divide_int (point_count[i]);
    }
}