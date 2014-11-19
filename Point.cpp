#include <bits/stdc++.h>
#include "Point.h"

using namespace std;

Point::Point(vector<double> __point,int  __label) {
	dimension = __point.size();
	point=__point;
	label=__label;
}

Point::Point(string __line){
	stringstream ss(__line);
	double tmp;
	dimension=0;
	int l;
	ss>>l;
	label=l;
	while(ss>>tmp){
		point.push_back(tmp);
		dimension++;
	}
}

Point::Point(void){
	dimension=0;
}
void Point::print() {
	cout << "Dimension:"<<dimension<<" Label:"<<label;
	cout<<" Data:( ";
	for (vector<double>::iterator it=point.begin();it!=point.end();++it){
		cout<< *it<< ' ';
	}
	cout<<")\n";
}

int Point::get_dimension() {
	return dimension;
}

vector<double> Point::get_coordinates(){
	return point;
}

int Point:: get_label(){
	return label;
}
double Point::dist(Point p) {
	if(p.get_dimension() != dimension) {
		cout<<"Dimension mismatch error"<<endl;
		throw 1;
	} else {
		double temp =0.0;
		vector<double> p_point=p.get_coordinates();
		for(int i=0; i<dimension; i++) 
			temp += (p_point[i]-point[i])*(p_point[i]-point[i]);
		temp = sqrt(temp);
		return temp;
	}
}

void Point::add_point(Point p){
	if(p.get_dimension()!=dimension){
		cout<<"Dimension mismatch error"<<endl;
		throw 1;
	}
	else{
		vector<double> tmp=p.get_coordinates();
		for(int i=0;i<dimension;i++){
			point[i]+=tmp[i];
		}
		label=-1;
	}
}

void Point::divide_int(int count){
	if(count==0){
		cout<<"Division by zero error"<<endl;
		throw 0;
	} else{
		for(int i=0;i<dimension;i++){
			point[i]/=count;
		}
	}
}

/**
int main() {
	vector<double> temp1,temp2;
	temp1.push_back(10);
	temp1.push_back(12.5);
	temp2.push_back(3);
	temp2.push_back(1.5);
	Point p1(temp1);
	Point p2(temp2);
	string line="2.3 5.6 7.8";
	Point p3(line);
	Point p4;
	Point p5;
	p1.print();p2.print();p3.print();p4.print();p5.print();
	cout<<p1.dist(p2)<<endl;
	p1.add_point(p2);
	p1.print();
	try{
		p1.add_point(p3);
	} catch(...){
		// do nothing
	}
	cout<<p4.dist(p5)<<endl;
	p4.add_point(p4);
	p4.print();
	try{
		p3.divide_int(0);
	} catch(...){
		// do nothing
	}
	return 0;
}
*/
