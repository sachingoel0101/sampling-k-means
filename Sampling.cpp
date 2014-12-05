#include <bits/stdc++.h>
#include "Sampling.h"

using namespace std;

Sampling::Sampling(string __file_name) {
    file_name=__file_name;
	num_pts=0;
	file.open(file_name.c_str());
	string line;
	while(getline(file,line)){
		num_pts++;
	}
	mode=true;
	reset_pools();
}

Sampling::Sampling(vector<Point> __pool){
	mode=false;
	counter=0;
	pool=__pool;
	num_pts=pool.size();
}
// we won't use the deconstructor as such. It causes problems
Sampling::~Sampling(){
	if(mode) file.close();
}
// instead we'll just close the file itself. Let the RAII handle the deconstruction of Sampling object itself.
void Sampling::close_file(){
	if(mode) file.close();
}
void Sampling::reset_pools(){
	if(mode){
		file.clear();
		file.seekg(0,ios_base::beg);
	}
	counter=0;
}

Point Sampling::next_point(){
	if(mode){
		string line;
		getline(file,line);
		return Point(line);
	} else{
		counter++;
		return pool[counter-1];
	}
}

vector<Point> Sampling::d2_sample(vector<Point> centers,int N) {
    if(centers.size()==0) {
		return uniform_sample(N);
	}
	else {
		reset_pools();
        vector<double> probabilities;
		for(int line_no=0;line_no<num_pts;line_no++){
        	Point p1=next_point();
            double min_dist = p1.dist(centers[0]);
            for(int i=1; i< centers.size(); i++) {
                double local_dist = p1.dist(centers[i]);
                min_dist = min(min_dist,local_dist);
            }
            probabilities.push_back(min_dist*min_dist);
        }
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
	reset_pools();
	Point p1=next_point();
	int tmp_cout=0;
	while(tmp_cout<num_pts) {
		if(temp== (*it)) {
			current_set.push_back(p1);
			it++;
			if(it==indices.end()) break;
		} else {
			p1=next_point();
			temp++;
			tmp_cout++;
		}
	}
	/**
	for(vector<Point>::iterator it=current_set.begin();it!=current_set.end();++it){
		Point t=*it;
		t.print();
	}
	*/
	return current_set;
}
/**
int main(void){
	vector<Point> tmp;
	tmp.push_back(Point("0 2.3 5.6"));
	tmp.push_back(Point("1 1.6 1.2"));
	tmp.push_back(Point("0 1.0 2.4"));
	tmp.push_back(Point("1 3.4 1.2"));
	tmp.push_back(Point("0 4.5 0.3"));
	tmp.push_back(Point("0 1.2 10.1"));
	Sampling s(tmp);
	s.uniform_sample(20);
}*/
