#include <bits/stdc++.h>
#include "Sampling.h"

using namespace std;

Sampling::Sampling(string __file_name) {
    file_name=__file_name;
	num_pts=0;
	ifstream file;
	file.open(file_name.c_str());
	string line;
	while(getline(file,line)){
		num_pts++;
	}
	file.close();
}

vector<Point> Sampling::d2_sample(vector<Point> centers,int N) {
    if(centers.size()==0) {
		return uniform_sample(N);
	}
	else {
		ifstream file;
		file.open(file_name.c_str());
        string line;
        vector<double> probabilities;
		for(int line_no=0;line_no<num_pts;line_no++){
        	getline(file,line);
            Point p1(line);
            double min_dist = p1.dist(centers[0]);
            for(int i=1; i< centers.size(); i++) {
                double local_dist = p1.dist(centers[i]);
                min_dist = min(min_dist,local_dist);
            }
            probabilities.push_back(min_dist*min_dist);
        }
		file.close();
        discrete_distribution<> d(probabilities.begin(), probabilities.end());
		return pick_points(sample_indices(d,N));
    }
}

vector<Point> Sampling::uniform_sample(int N) {
	vector<double> probabilities;
	for(int i=0;i<num_pts;i++){
		probabilities.push_back(1.0);
	}
	discrete_distribution<> d(probabilities.begin(), probabilities.end());
	return pick_points(sample_indices(d,N));
}

vector<int> Sampling:: sample_indices(discrete_distribution<> d,int N) {
	srand(time(NULL));
	random_device rd;
	mt19937 gen(rd());
	vector<int> ans;
	while(ans.size()<N) {
		int chosen = d(gen);
		ans.push_back(chosen);
	}
	sort(ans.begin(),ans.end());
	/**
	for(vector<int>::iterator it=ans.begin();it!=ans.end();++it){
		cout<<*it<<" "<<endl;
	}
	*/
	return ans;
}

vector<Point> Sampling::pick_points(vector<int> indices) {
	vector<Point> current_set;
	vector<int>::iterator it=indices.begin();
	int temp =0;
	ifstream file;
	file.open(file_name.c_str());
	string line;
	getline(file,line);
	while(true) {
		if(file.eof()) break;
		if(temp== (*it)) {
			Point p1(line);
			current_set.push_back(p1);
			it++;
			if(it==indices.end()) break;
		} else {
			getline(file,line);
			temp++;
		}
	}
	file.close();	
	/**
	for(vector<Point>::iterator it=current_set.begin();it!=current_set.end();++it){
		Point t=*it;
		t.print();
	}
	*/
	return current_set;
}
