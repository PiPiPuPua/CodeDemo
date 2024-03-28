#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <set>
#include <limits.h>
#include "Graph.h"

using namespace std;

template<class Vertex, class EdgeT>
class Task2 {
public:
	~Task2() {};
	// template<class Vertex, class EdgeT>
	Task2(Graph<Vertex, EdgeT>* g) {
		Set(g);
	}


	//template <class Vertex, class EdgeT>
	Task2(const Graph<Vertex, EdgeT>& G)
	{
		if (G.CheckDense())
			this->data = new GraphMatrixForm<EdgeT>(*(G.data));
		else
			this->data = new GraphListForm<EdgeT>(*(G.data));
		this->directed = G.directed;
		this->dense = G.dense;
	}

	//template<class Vertex, class EdgeT>
	void Set(Graph<Vertex, EdgeT>* g) {
		this->g1 = g;
	}

	void Restart() {
		Result(CurV);
	}
	// template<class TVertex, class TEdge>
	void Result(int vertex)
	{
		CurV = vertex;

		for (int i = 0; i < g1->VertexSize(); i++)
			if (i != vertex)
				Deikstra(i);

		cout << endl;

	}

	void Deikstra(int vertex) {

		int n = g1->VertexSize();
		vector < vector < pair<int, int> > > g(n);
		int s = vertex;

		/*
		граф g хранитс€ в виде списков смежности : дл€ каждой вершины v список g[v] содержит список рЄбер,
		исход€щих из этой вершины, т.е.список пар pair<int, int>,
		где первый элемент пары Ч вершина, в которую ведЄт ребро, а второй элемент Ч вес ребра.
		*/

		for (int i = 0; i < g1->VertexSize(); i++) {
			for (int j = 0; j < g1->VertexSize(); j++) {

				if (g1->hasEdge(i, j)) {

					Vertex* v1 = g1->getVertex(i);
					Vertex* v2 = g1->getVertex(j);

					int wght = g1->ReadWeightEdge(v1->getName(), v2->getName());

					g[i].push_back(*(new pair<int, int>(j, wght)));
				}
			}
		}

		vector<int> d(n, INT_MAX), p(n);

		d[s] = 0;

		vector<char> u(n);
		for (int i = 0; i < n; ++i) {
			int v = -1;
			for (int j = 0; j < n; ++j)
				if (!u[j] && (v == -1 || d[j] < d[v]))
					v = j;
			if (d[v] == INT_MAX)
				break;
			u[v] = true;

			for (size_t j = 0; j < g[v].size(); ++j) {
				int to = g[v][j].first,
					len = g[v][j].second;
				if (d[v] + len < d[to]) {
					d[to] = d[v] + len;
					p[to] = v;
				}
			}
		}

		vector<int> path;
		for (int v = CurV; v != s; v = p[v])
			path.push_back(v);
		path.push_back(s);
		reverse(path.begin(), path.end());

		cout << endl << "–ассто€ние от вершины " << vertex << " составл€ет " << d[CurV] << endl;
		cout << "ѕуть: " << endl;
		for (int i = 0; i < path.size(); i++) {
			cout << path[i] << " ";
			if (i != path.size() - 1)
				cout << " -> ";
		}
	}

private:
	Graph<Vertex, EdgeT>* g1;
	int CurV;
};