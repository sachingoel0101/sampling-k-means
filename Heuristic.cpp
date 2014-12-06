#include <bits/stdc++.h>
#include "Heuristic.h"
#include "Sampling.h"

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

    sort(ranks.begin(),ranks.end());

	vector<Point> to_ret(m);

	for(int i=0;i<m;i++) {
		to_ret[i] = sampled_set[ranks[n-1-i].second];
	}	


	for(int i=0;i<200;i++){
		cout<<sampled_set[ranks[n-1-i].second].get_label()<<' ';
	}
	cout<<'\n';
	return to_ret;
}

Point Heuristic::h2_center(vector<Point> sampled_set,int k){
	vector<Point> level_2_sample=h2_subset(sampled_set,k);
	//cout<<"Heuristic centers chosen:";
	vector<int> labels_picked;
	for(vector<Point>::iterator it=level_2_sample.begin();it!=level_2_sample.end();++it){
		Point tmp=*it;
		labels_picked.push_back(tmp.get_label());
		//tmp.print();
	}
	cout<<"Labels picked"<<endl;
	for(vector<int>::iterator it=labels_picked.begin();it!=labels_picked.end();++it){
		cout<<*it<<' ';
	}
	cout<<'\n';
	// after picking k points from the multiset using d2_sample
	// determine which of these points has the most points associated to it out of the multiset
	vector<int> counts;
	vector<int> mis_match;
	for(int i=0;i<k;i++){	
		counts.push_back(0);
		mis_match.push_back(0);
	}

	for(int i=0;i<sampled_set.size();i++){
		double min_dist=level_2_sample[0].dist(sampled_set[i]);
		int index=0;
		for(int j=1;j<k;j++){
			double tmp_dist=level_2_sample[j].dist(sampled_set[i]);
			if(tmp_dist<min_dist){
				min_dist=tmp_dist;
				index=j;
			}
		}
		if(sampled_set[i].get_label()!=level_2_sample[index].get_label()) mis_match[index]++;
		counts[index]++;
	}
	int max=counts[0];
	int index=0;
	cout<<"Counts: "<<counts[0]<<' '<<mis_match[0]<<endl;
	for(int i=1;i<k;i++){
		cout<<counts[i]<<' '<<mis_match[i]<<endl;
		if(counts[i]>max){
			max=counts[i];
			index=i;
		}
	}
	cout<<'\n';
	return level_2_sample[index];
}

vector<Point> Heuristic::h2_subset(vector<Point> sampled_set,int k){
	Sampling s(sampled_set);
	vector<Point> ret_val;
	for(int i=0;i<k;i++){
		Point tmp=s.d2_sample(ret_val,1)[0];
		ret_val.push_back(tmp);
	}
	s.close_file();
	return ret_val;
}
