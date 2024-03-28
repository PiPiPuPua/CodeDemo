#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <set>
#include "Graph.h"

using namespace std;

template<class Vertex, class EdgeT>
class Task1 {
public:
	~Task1() {};

	// template<class Vertex, class EdgeT>
	Task1(Graph<Vertex, EdgeT>* g) {
		Set(g);
	}

	Task1(const Graph<Vertex, EdgeT>& G)
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

		if (!g->Directed()) cout << "Graph should be directed " << endl;
		this->g1 = g;
		c = new int[g1->VertexSize()];

		for (int j = 0; j < g1->VertexSize(); j++)
			c[j] = -1;

		path = new int[g1->VertexSize()];
		path[0] = v0;
		c[v0] = v0;
	}

	void printGam(void)
	{
		cout << "Гамильтонов цикл существует, путь:" << endl;
		int p;
		for (p = 0; p < g1->VertexSize(); p++) {

			Vertex* a = g1->getVertex(path[p]);
			cout << a->getName() << " ";
		}
		cout << path[0];
		cout << endl;
	}

	int Gamilton(int k) {

		int v;          //индекс вершины
		int q1 = 0;     //признак нахождения пути, сначала - не найдено
		for (v = 0; v < g1->VertexSize() && !q1; v++) // обход матрицы по всем вершинам и пока не найден путь
		{
			//есть ли ребро между текущей вершиной и вершиной, найденной при предыдущем вызове
			//если граф неориентированный, то оба используемых элемента матрицы должны быть равны
			//если ориентированный, то, по идее, надо смотреть только a[path[k - 1]][v]
			if (v!= path[k - 1] && g1->hasEdge(path[k - 1], v))
			{
				if (k == g1->VertexSize() && v == v0)          //если обошли все вершины и дошли до начальной,
					q1 = 1;                     // то путь найден
				else
					if (c[v] == -1)             //формируем путь, если в вершине v еще не были
					{
						c[v] = k;               //номер прохода
						path[k] = v;            //занесение вершины в найденный путь
						q1 = Gamilton(k + 1);   //ищем следующую вершину
						if (!q1) c[v] = -1;     //если путь не найден, то помечаем текущую вершину, как непройденную
					}
					else
						continue;               //если в вершине уже были, то на анализ следующей вершины
			}
		}   
		return q1;
	}

	void Restart() {
		Result();
	}

	template<class TVertex, class TEdge>
	int Result()
	{
		if (Gamilton(1))
			printGam();
		else
			cout << "Гамильтонова цикла не существует" << endl;
		return 0;
	}
private:
	Graph<Vertex, EdgeT>* g1;

	int* c;           // номер хода, на котором посещается вершина
	int* path;        // номера посещаемых вершин
	int v0 = 0;			// начальная вершина
};