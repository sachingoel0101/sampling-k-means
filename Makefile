random_lloyd:
	g++ -o random_lloyd random_lloyd.cpp Heuristic.cpp Cluster.cpp Point.cpp Sampling.cpp -std=c++11

kmp_lloyd:
	g++ -o kmp_lloyd kmp_lloyd.cpp Heuristic.cpp Cluster.cpp Point.cpp Sampling.cpp -std=c++11

hst_lloyd:
	g++ -o hst_lloyd hst_lloyd.cpp Heuristic.cpp Cluster.cpp Point.cpp Sampling.cpp -std=c++11

clean:
	rm -rf random_lloyd kmp_lloyd hst_lloyd

all: random_lloyd kmp_lloyd hst_lloyd
