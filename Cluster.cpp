#include <bits/stdc++.h>
#include "Cluster.h"

using namespace std;

Cluster::Cluster(string __filename,vector<Point> __means) {
	int tmp=__means[0].get_dimension();
	for(vector<Point>::iterator it=__means.begin();it!=__means.end();++it){
		Point tmp1=*it;
		if(tmp1.get_dimension()!=tmp){
			cout<<"Dimension mismatch error"<<endl;
			throw 1;
		}
	}
    filename=__filename;
    means=__means;
	dimension=tmp;
	num_cluster=__means.size();
	vector<int> assignments;
	for(int i=0;i<num_cluster;i++) assignments.push_back(0);
}

Cluster::Cluster(void){
	dimension=0;
	num_cluster=0;
}

vector<Point> Cluster::get_means(){
	return means;
}

int Cluster::get_dimension(){
	return dimension;
}

int Cluster::get_num_cluster(){
	return num_cluster;
}

vector<int> Cluster::get_assignments(){
	return assignments;
}

void Cluster::set_assignments(vector<int> __assignments){
	assignments=__assignments;
}

void Cluster::print() {
    for(int i=0; i<num_cluster; i++){
        means[i].print();
	}
	cout<<'\n';
}

double Cluster::get_cost(){
	ifstream file;
	file.open(filename.c_str());
	string line;
	double ans=0;
	while(getline(file,line)){
		Point tmp(line);
		ans+=tmp.dist(means[belongs_to(tmp)]);
	}
	return ans;
}

Cluster Cluster::iterate(int &change, vector<int> &prev_assign) {
	change=0;
	vector<int> new_assign;
	ifstream data;
	data.open(filename.c_str());
	vector<Point> tmp_means;
	vector<int> tmp_point_count;
	vector<double> tmp_point;
	for(int i=0;i<dimension;i++){
		tmp_point.push_back(0);
	}
	Point zero_point=Point(tmp_point);
	for(int i=0;i<num_cluster;i++){
		tmp_point_count.push_back(0);
		tmp_means.push_back(zero_point);
	}
	string line;
	getline(data,line);
	int i=0;
	for(line;!data.eof();getline(data,line)){
		Point tmp(line);
		int index=belongs_to(tmp);
		if(index!=prev_assign[i]) change++;
		new_assign.push_back(index);
		tmp_means[index].add_point(tmp);
		tmp_point_count[index]+=1;
		i++;
	}
	data.close();
	Cluster new_cluster(filename,tmp_means);
	new_cluster.set_assignments(tmp_point_count);
	cout<<"Point assignments: ";
	for(int i=0;i<num_cluster;i++){
		cout<<tmp_point_count[i]<<' ';
		if(tmp_point_count[i]==0) tmp_point_count[i]=1;
	}
	cout<<'\n';
	new_cluster.scale_cluster(tmp_point_count);	
	prev_assign.clear();
	prev_assign=new_assign;
	return new_cluster;
}

int Cluster::belongs_to(Point p){
	int index=0;
	double min_dist=DBL_MAX;
	for(int i=0;i<num_cluster;i++){
		if(means[i].dist(p)<min_dist){
			index=i; min_dist=means[i].dist(p);
		}
	}
	return index;
}

void Cluster::scale_cluster(vector<int> point_count){
	for(int i=0;i<num_cluster;i++){
		means[i].divide_int(point_count[i]);
	}
}
