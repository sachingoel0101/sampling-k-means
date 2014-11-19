#include <bits/stdc++.h>
#include "Heuristic.h"

using namespace std;

Point Heuristic::h1_center(vector<Point> sampled_set, int m) {
	vector<Point> subset=h1_subset(sampled_set,m);
	int dim=sampled_set[0].get_dimension();
	vector<double> tmp;
	for(int i=0;i<dim;i++) tmp.push_back(0);
	Point ret(tmp,-1);
	for(int i=0;i<subset.size();i++)
		ret.add_point(subset[i]);
	ret.divide_int(subset.size());
	ret.print();
	cout<<'\n';
	return ret;
}


vector<Point> Heuristic::h1_subset(vector<Point> sampled_set, int m) {

    //choose top m candidates
	int n=sampled_set.size();
   // vector<vector<pair<double,int> > > matrix(n);

    /*for(int i=0; i<n; i++) {
        matrix[i].resize(n);
        for(int j=0; j<n; j++) {
            matrix[i][j] = make_pair(0,j);
        }
    }*/


	vector<pair<int,int> > ranks(n);
    for(int i=0; i<n; i++)
        ranks[i]=make_pair(0,i);

    for(int i=0; i<n; i++) {
		priority_queue<pair<double,int> > pq;
        for(int j=0; j<n; j++) {

			if(i!=j) {
            	double local_dist = sampled_set[i].dist(sampled_set[j]);
		       	pq.push(make_pair(local_dist,j));
				if(pq.size()>10) {
					pq.pop();
				}
			}
        }

		while(!pq.empty()) {
			pair<double,int> temp = pq.top();
			ranks[temp.second].first++;
			pq.pop();
		}
    }

   /* for(int i=0; i<n; i++) {
        sort(matrix[i].begin(), matrix[i].end());
    }
	*/
    

    //counting only top-10 ranks
    /*for(int i=0; i<n; i++) {
        int limit = min(10,n-1);

        for(int j=0; j<=limit; j++) {
            ranks[matrix[i][j].second].first++;
        }
    }*/

    sort(ranks.begin(),ranks.end());

	vector<Point> to_ret(m);

	for(int i=0;i<m;i++) {
		to_ret[i] = sampled_set[ranks[n-1-i].second];
	}	


	for(int i=0;i<200;i++){
		cout<<sampled_set[ranks[n-1-i].second].get_label()<<' ';
	}
	cout<<'\n';
	/*
    int dim = candidates[i].size();
    vector<int> ans(dim);
    for(int i=0; i<dim; i++)
        ans[i]=0;
	*/
	return to_ret;
}
