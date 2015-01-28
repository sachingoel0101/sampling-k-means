#include <bits/stdc++.h>
#include "Cluster.h"
#include "Sampling.h"
#include "Heuristic.h"
#include "sys/time.h"

//#define abs(x) x>0?x:-x

using namespace std;

/**bool check_diff(vector<int> a,vector<int> b){
	int total=0;
	for(vector<int>::ietrator it=a.begin();it!=a.end();++it){
		total=total+(*it);
	}
	int total_diff=0;
	for(int i=0;i<a.size();i++){
		total_diff=abs(a[i]-b[i]);
	}
	if()
}
*/
int main(int argc,char* argv[]) {
	struct timeval start,end;
	gettimeofday(&start,NULL);
	Sampling sampler(argv[1]);
	int numcluster=stoi(argv[2]);
	int n_val=stoi(argv[3]);
	vector<Point> p;
	Heuristic h;
	for(int i=0;i<numcluster;i++){
		p.push_back(h.h3_center(sampler.d2_sample(p,n_val),numcluster,p));
	}
	gettimeofday(&end,NULL);
	sampler.close_file();
	cout<<"Initialization time: "<<(end.tv_sec-start.tv_sec+1e-6*(end.tv_usec-start.tv_usec))<<"\n";
	vector<double> costs;
	Cluster c1(argv[1],p);
	int totalPts=stoi(argv[4]);
	vector<int> tmp_assign;
	for(int i=0;i<totalPts;i++) tmp_assign.push_back(-1);
	//costs.push_back(c1.get_cost());
	c1.print();
	cout<<"0:"<<c1.get_cost()<<endl;
	int change;
	Cluster c2=c1.iterate(change,tmp_assign);
	//costs.push_back(c2.get_cost());
	//c2.print();
	cout<<"1:"<<c2.get_cost()<<" "<<change<<endl;
	int iter=1;
	while(((double)change)/totalPts>1e-3){
		//c2.print();
		c1=c2;
		c2=c1.iterate(change,tmp_assign);
		//costs.push_back(c2.get_cost());
		cout<<(iter+1)<<":"<<c2.get_cost()<<" "<<change<<endl;
		iter++;
	}
	//cout<<iter<<endl;
	/**for(vector<double>::iterator it=costs.begin();it!=costs.end();++it)
		cout<<*it<<',';
	cout<<'\n';*/
	gettimeofday(&end,NULL);
	cout<<"Total time: "<<(end.tv_sec-start.tv_sec+1e-6*(end.tv_usec-start.tv_usec))<<"\n";
	return 0;
}
