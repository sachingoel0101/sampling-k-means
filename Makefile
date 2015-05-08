all:
	g++ -o cluster_parallel main.cpp Cluster.cpp Heuristic.cpp Sampling.cpp Resource.cpp Point.cpp -std=c++11 -fopenmp 
	g++ -o cluster_serial main.cpp Serial_cluster.cpp Serial_heuristic.cpp Serial_sampling.cpp Resource.cpp Point.cpp -std=c++11
	g++ -o parallel run.cpp Cluster.cpp Heuristic.cpp Sampling.cpp Resource.cpp Point.cpp -std=c++11 -fopenmp 
	g++ -o serial run.cpp Serial_cluster.cpp Serial_heuristic.cpp Serial_sampling.cpp Resource.cpp Point.cpp -std=c++11

clean:
	rm -rf cluster*
	rm -rf parallel
	rm -rf serial
