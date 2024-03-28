#pragma once
#include "Graph.h"
#include <map>
#include <set>
#include <queue>
typedef Vertex <string, int> TVertex;
typedef Edge<TVertex, int> TEdge;
typedef Graph <TVertex, TEdge> TGraph;

template <class VertexT, class EdgeT>
class Classification {
private:
	Graph<VertexT, EdgeT>* graph;
	VertexT* vertex;
	stringstream res;

	struct top
	{
		int color;
		int d;
		VertexT* p;
		int f;
	};
	int time;
	map<VertexT*, top> vtop;
	set<VertexT*> visited;
	queue<EdgeT*> path;
	string AnalyzeEdge(VertexT* u, VertexT* v)
	{
		string edgeType = "";
		if (vtop[u].d < vtop[v].d && vtop[u].f > vtop[v].f && vtop[v].p == u)
			edgeType = "Древесное";
		if (vtop[u].d > vtop[v].d && vtop[u].f < vtop[v].f)
			edgeType = "Обратное";
		if (vtop[u].d < vtop[v].d && vtop[u].f > vtop[v].f && vtop[v].p != u)
			edgeType = "Прямое";
		if (vtop[u].d > vtop[v].d && vtop[u].f > vtop[v].f)
			edgeType = "Поперечное";
		return edgeType;
	}

	void DFS_path_output(VertexT* u)
	{
		TGraph::OutputEdgeIterator* itOut = new TGraph::OutputEdgeIterator(*graph, *u);
		EdgeT* curEdge = NULL;
		itOut->begin();
		visited.insert(u);
		while (!(itOut->onEnd()))
		{
			curEdge = *(*itOut);
			VertexT* v = NULL;
			if (curEdge->V1() == u)
				v = curEdge->V2();
			else
				v = curEdge->V1();
			res << u->getName() << "->" << v->getName() << " - " << AnalyzeEdge(u, v) << endl;
			if (visited.find(v) == visited.end())
				DFS_path_output(v);
			(*itOut)++;
		}
	}
	void DFS_path_all()
	{
		while (!path.empty())
		{
			EdgeT* curEdge = path.front();
			path.pop();
			VertexT* u = curEdge->V1();
			VertexT* v = curEdge->V2();
			res << u->getName() << "->" << v->getName() << " - " << AnalyzeEdge(u, v) << endl;
		}
	}
	void DFS_visit_all()
	{
		vtop.clear();
		time = 0;

		TGraph::VertexIterator* it = new TGraph::VertexIterator(*graph);
		it->begin();
		while (!(it->onEnd()))
		{
			vtop[*(*it)].color = 0;
			vtop[*(*it)].p = NULL;
			++(*it);
		}

		DFS_visit(vertex);

		it->begin();
		while (!(it->onEnd()))
		{
			if (vtop[*(*it)].color == 0)
				DFS_visit(*(*it));
			++(*it);
		}
	}
	void DFS_visit(VertexT* u)
	{
		vtop[u].color = 1;
		vtop[u].d = ++time;

		TGraph::OutputEdgeIterator* itout = new TGraph::OutputEdgeIterator(*graph, *u);

		itout->begin();
		EdgeT* cedge = NULL;

		while (!(itout->onEnd()))
		{
			cedge = *(*itout);
			path.push(cedge);
			VertexT* v = NULL;
			if (cedge->V1() == u)
				v = cedge->V2();
			else
				v = cedge->V1();

			if (vtop[v].color == 0)
			{
				vtop[v].p = u;
				DFS_visit(v);
			}
			++(*itout);
		}
		vtop[u].color = 2;
		vtop[u].f = time++;
	}
public:
	Classification(Graph<VertexT, EdgeT>* g, VertexT* v) :
		graph(g), vertex(v)
	{
		restart();
	}
	Classification(const Classification<VertexT, EdgeT>& t1) :
		graph(t1.g), vertex(t1.v) {}

	void set(Graph<VertexT, EdgeT>* g, VertexT* v) {
		graph = g;
		vertex = v;
		restart();
	}
	void restart() {
		res.str("");
		DFS_visit_all();
		visited.clear();
		DFS_path_all();
	}
	string result()
	{
		return res.str();
	}
};

