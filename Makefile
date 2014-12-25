random_lloyd:
	g++ -o random_lloyd random_lloyd.cpp Heuristic.cpp Cluster.cpp Point.cpp Sampling.cpp -std=c++11

kmp_lloyd:
	g++ -o kmp_lloyd kmp_lloyd.cpp Heuristic.cpp Cluster.cpp Point.cpp Sampling.cpp -std=c++11

top_m_lloyd:
	g++ -o top_m_lloyd top_m_lloyd.cpp Heuristic.cpp Cluster.cpp Point.cpp Sampling.cpp -std=c++11

top_m_mixed_lloyd:
	g++ -o top_m_mixed_lloyd top_m_mixed_lloyd.cpp Heuristic.cpp Cluster.cpp Point.cpp Sampling.cpp -std=c++11

level2_d2_lloyd:
	g++ -o level2_d2_lloyd level2_d2_lloyd.cpp Heuristic.cpp Cluster.cpp Point.cpp Sampling.cpp -std=c++11

another_level2_d2_lloyd:
	g++ -o another_level2_d2_lloyd another_level2_d2_lloyd.cpp Heuristic.cpp Cluster.cpp Point.cpp Sampling.cpp -std=c++11

mean_level2_d2_lloyd:
	g++ -o mean_level2_d2_lloyd another_level2_d2_lloyd.cpp Heuristic.cpp Cluster.cpp Point.cpp Sampling.cpp -std=c++11
clean:
	rm -rf random_lloyd kmp_lloyd top_m_lloyd top_m_mixed_lloyd level2_d2_lloyd another_level2_d2_lloyd mean_level2_d2_lloyd

all: random_lloyd kmp_lloyd top_m_lloyd top_m_mixed_lloyd level2_d2_lloyd another_level2_d2_lloyd mean_level2_d2_lloyd
