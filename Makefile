all:
	g++ -o cluster main.cpp Heuristic.cpp Cluster.cpp Sampling.cpp Resource.cpp Point.cpp -std=c++11
clean:
	rm -rf cluster
