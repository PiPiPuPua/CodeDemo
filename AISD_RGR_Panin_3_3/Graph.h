#pragma once
#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <iomanip>
#include <sstream>
#include <stack>
#include <string>
#include <utility>

#include "Vertex.h"
#include "EdgeDesc.h"
#include "GraphForm.h"

using namespace std;

string i_to_s(int x)
{
	stringstream ss;
	ss << x;
	return ss.str();
}

template <class Vertex, class EdgeT>
class Graph
{
protected:
	vector<Vertex*> vertexInput;  //Вектор вставленных
	GraphForm<EdgeT>* data;       //Данные графа
	bool directed;                //Направленный граф
	bool dense;                   //М-граф
	int edgeCounter;              //Число рёбер
	int size;					  //размер

	int getIndexFromName(string n);
	bool DeleteVertex(int index);

public:
	Graph(); //Конструктор пустого L-графа
	Graph(int vertexCount, bool directed, bool dense); //Конструктор с заданным числом вершин, заданной ориентированности и заданного типа
	Graph(int vertexCount, int edgeCount, bool directed, bool dense); //Конструктор с заданным числом вершин, случайных рёбер, заданной ориентированности и типа
	Graph(const Graph<Vertex, EdgeT>& G); //Конструктор копирования
	~Graph(); //Деструктор
	int VertexSize();//возвращает число вершин в графе
	int EdgeSize(); //возвращает число ребер в графе
	bool CheckDense(); //возвращает форму представления графа
	bool Directed();//возвращает тип графа   
	int getIndex(Vertex* v);
	double K(); //возвращает коэффициент насыщенности графа	
	void ToListGraph();//преобразует граф к L-графу 
	void ToMatrixGraph();//преобразует граф к M-графу 
	Vertex* InsertVertex();//добавляет вершину к графу и возвращает адрес дескриптора вновь созданной вершины 
	bool DeleteVertex(Vertex* v);//удаляет вершину из графа, заданную адресом дескриптора v 
	bool DeleteVertex(string n);
	EdgeT* InsertEdge(Vertex* v1, Vertex* v2);//добавляет ребро (v1, v2) к графу, соединяющую вершины, заданные адресами дескрипторов v1 и v2	
	bool InsertEdge(string v1, string v2);
	bool DeleteEdge(Vertex* v1, Vertex* v2);//удаляет ребро, соединяющее вершины, заданные адресами дескрипторов v1 и v2
	bool DeleteEdge(string v1, string v2);
	EdgeT* getEdge(Vertex* v1, Vertex* v2);	//Получение ребра
	bool hasEdge(Vertex* v1, Vertex* v2);//Проверка на существованиеие ребра 
	bool hasEdge(int v1, int v2);
	bool WriteNameVertex(string v1, string str);//Записать данные о вершине
	bool WriteDataVertex(string v1, int d);//Записать данные о вершине
	int ReadDataVertex(string v1);//Прочитать данные о вершине
	void print();//Вывод на экран
	int ReadWeightEdge(string _v1, string _v2);//Прочитать вес ребра
	int ReadDataEdge(string _v1, string _v2);//Прочитать данные о ребре
	bool WriteWeightEdge(string _v1, string _v2, int w);//Записать вес ребра
	bool  WriteDataEdge(string _v1, string _v2, int data);//Записать данные о ребре
	Vertex* getVertexFromName(string n);//Получение вершины из графа
	Vertex* getVertex(int index);

	class VertexIterator
	{
	private:
		Graph<Vertex, EdgeT>* graph; //Обрабатываемый граф
		bool end;	//Состояние итератора
		int cur;	//Текущее положение
	public:
		VertexIterator(Graph<Vertex, EdgeT>& g) { graph = &g; };
		bool begin()
		{
			if (graph->vertexInput.size() == 0)
			{
				end = true;
				return false;
			}
			cur = 0;
			end = false;
			return true;
		}
		bool operator++()
		{
			try {
				if (end) {
					cur = -1;
					throw std::exception();
				}
			}
			catch (const std::exception&) {
				throw std::exception();
			}
			cur++;
			if (graph->vertexInput.size() - 1 == cur)
				end = true;
			return true;
		}
		bool toend()
		{
			end = true;
			cur = graph->vertexInput.size() - 1;
			if (cur < 0)
				return false;
			else
				return true;
		}
		bool onEnd()
		{
			return end;
		}
		Vertex* operator*()
		{
			if (cur >= graph->vertexInput.size() || cur == -1)
				throw std::exception();
			return graph->vertexInput[cur];
		}
		string ReadVertex()
		{
			Vertex* v = NULL;
			try
			{
				v = operator*();
			}
			catch (...) { throw std::exception(); }
			if (v->isNameSet())
				return v->getName();
			else
				throw std::exception();
		}
		int ReadDataVertex()
		{
			Vertex* v = NULL;
			try
			{
				v = operator*();
			}
			catch (...) { throw std::exception(); } {
				if (v->isDataSet())
					return v->getData();
				else throw std::exception();
			}
		}
		bool WriteDataVertex(int d)
		{
			Vertex* v;
			try
			{
				v = operator*();
			}
			catch (const std::exception&)
			{
				return false;
			}
			v->setData(d);
			return true;
		}
		bool WriteNameVertex(string str)
		{
			Vertex* v;
			try
			{
				v = operator*();
			}
			catch (const std::exception&)
			{
				return false;
			}
			v->setName(str);
			return true;
		}
	};
	class EdgeIterator {
	private:
		Graph<Vertex, EdgeT>* graph;
		typename GraphMatrixForm<EdgeT>::EdgeIterator* mIt;
		typename GraphListForm<EdgeT>::EdgeIterator* lIt;
		bool useM;
		bool end;
	public:


		EdgeIterator(Graph<Vertex, EdgeT>& g)
		{
			graph = &g;
			if (graph->CheckDense())
			{
				mIt = new typename GraphMatrixForm<EdgeT>::EdgeIterator(static_cast<GraphMatrixForm<EdgeT>*>(g.data));
				useM = true;
			}
			else {
				lIt = new typename GraphListForm<EdgeT>::EdgeIterator(static_cast<GraphListForm<EdgeT>*>(g.data));
				useM = false;
			}
		}
		bool begin()
		{
			if (useM) return
				mIt->begin();
			else
				return lIt->begin();
		}
		bool operator++()
		{
			if (useM)
				return ++(*mIt);
			else
				return ++(*lIt);
		}
		bool onEnd()
		{
			if (useM)
				return mIt->onEnd();
			else
				return lIt->onEnd();
		}
		bool toend()
		{
			if (useM)
				return mIt->toend();
			else
				return lIt->toend();
		}
		EdgeT* operator*()
		{
			if (onEnd())
				throw std::exception();
			if (useM)
				return mIt->getEdge();
			else
				return lIt->getEdge();
		}
		string ReadEdge()
		{
			string str1, str2, str3, str4;
			EdgeT* e = operator*();
			Vertex* _v1, * _v2;
			_v1 = e->V1();
			_v2 = e->V2();
			if (!(_v1->isNameSet()))
				str1 = "Имя ребра не установлено";
			else
				str1 = _v1->getName();
			if (!(_v2->isNameSet()))
				str2 = "Данные ребра не установлены";
			else
				str2 = _v2->getName();
			if (e->isWeightSet() == false)
				str3 = "Вес ребра не установлен";
			else
				str3 = i_to_s(e->getWeight());
			str4 = "Исходящая вершина: " + str1 + ". Входящая вершина: " + str2 + ". Вес ребра: " + str3;
			return str4;
		}
		int ReadDataEdge()
		{
			EdgeT* f = operator*();
			if (f->isDataSet())
				return f->getData();
			else
				throw std::exception();
		}

		bool WriteDataEdge(int d)
		{
			EdgeT* e;
			try {
				e = operator*();
			}
			catch (const std::exception&) {
				return false;
			}
			e->setData(d);
			return true;
		}

		bool WriteWeightEdge(int w)
		{
			EdgeT* e;
			try {
				e = operator*();
			}
			catch (const std::exception&) {
				return false;
			}
			e->setWeight(w);
			return true;
		}
	};
	class OutputEdgeIterator {
		Graph<Vertex, EdgeT>* graph;
		typename GraphMatrixForm<EdgeT>::OutputEdgeIterator* mIt;
		typename GraphListForm<EdgeT>::OutputEdgeIterator* lIt;
		bool useM;
		bool end;
	public:
		OutputEdgeIterator(Graph<Vertex, EdgeT>& g, Vertex& v) {
			graph = &g;
			mIt = NULL;
			lIt = NULL;
			int curV = graph->getIndex(&v);
			if (graph->CheckDense()) {
				mIt = new typename GraphMatrixForm<EdgeT>::OutputEdgeIterator(static_cast<GraphMatrixForm<EdgeT>*>(g.data), curV);
				useM = true;
			}
			else {
				lIt = new typename GraphListForm<EdgeT>::OutputEdgeIterator(static_cast<GraphListForm<EdgeT>*>(g.data), curV);
				useM = false;
			}
		}

		~OutputEdgeIterator()
		{
			if (mIt != NULL)
				delete mIt;
			if (lIt != NULL)
				delete lIt;
		}
		int begin() {
			if (useM)
				return mIt->begin();
			else
				return lIt->begin();
		}
		bool operator++() {
			if (useM)
				return ++(*mIt);
			else
				return ++(*lIt);
		}

		bool toend() {
			if (useM)
				return mIt->toend();
			else
				return lIt->toend();
		}
		bool onEnd() {
			if (useM)
				return mIt->onEnd();
			else
				return lIt->onEnd();
		}
		EdgeT* operator*() {
			if (onEnd())
				throw std::exception();
			if (useM)
				return mIt->getEdge();
			else
				return lIt->getEdge();
		}

		string ReadEdge() {
			try
			{
				string str1, str2, str3, str4;
				EdgeT* e = operator*();
				Vertex* v1, * _v2;
				v1 = e->V1();
				_v2 = e->V2();
				if (!(v1->isNameSet()))
					str1 = "Имя ребра не установлено";
				else
					str1 = v1->getName();
				if (!(_v2->isNameSet()))
					str2 = "Данные ребра не установлены";
				else
					str2 = _v2->getName();
				if (e->isWeightSet() == false)
					str3 = "Вес ребра не установлен";
				else
					str3 = i_to_s(e->getWeight());
				str4 = "Исходящая вершина: " + str1 + ". Входящая вершина: " + str2 + ". Вес ребра: " + str3;
				return str4;
			}
			catch (const std::exception&)
			{
				throw std::exception();
			}
		}

		int ReadDataEdge() {
			EdgeT* e;
			e = operator*();
			if (e->isDataSet())
				return e->getData();
			else
				throw std::exception();
		}

		bool WriteDataEdge(int d) {
			EdgeT* e;
			try {
				e = operator*();
			}
			catch (const std::exception&) {
				return false;
			}
			e->setData(d);
			return true;
		}

		bool WriteWeightEdge(int w) {
			EdgeT* e;
			try {
				e = operator*();
			}
			catch (const std::exception&) {
				return false;
			}
			e->setWeight(w);
			return true;
		}
		int Val() {
			if (graph->CheckDense()) {
				return stoi(mIt->getEdge()->V2()->getName());
			}
			else {
				return stoi(lIt->getEdge()->V2()->getName());
			}
		}
	};
	friend class VertexIterator;
	friend class EdgeIterator;
	friend class OutputEdgeIterator;
};


template <class Vertex, class EdgeT>
int Graph<Vertex, EdgeT>::getIndexFromName(string n)
{
	int index = 0;
	for (int i = 0; i < vertexInput.size(); i++) {
		if (vertexInput[index]->getName() == n)
			break;
		index++;
	}
	if (index == vertexInput.size())
		throw "Вершина не найдена";
	return index;
}
template <class Vertex, class EdgeT>
Graph<Vertex, EdgeT>::Graph()
{
	size = 0;
	data = new GraphListForm<EdgeT>(false);
	directed = false;
	dense = false;
	edgeCounter = 0;
}
template <class Vertex, class EdgeT>
Graph<Vertex, EdgeT>::Graph(int V, bool D, bool F)
{
	size = 0;
	edgeCounter = 0;
	directed = D;
	dense = F;
	if (F)
		data = new GraphMatrixForm<EdgeT>(D);
	else
		data = new GraphListForm<EdgeT>(D);
	for (int i = 0; i < V; i++)
		data->InsertVertex(i);
	for (int i = 0; i < V; i++) {
		Vertex* v = new Vertex();
		v->setName(i_to_s(size));
		size++;
		vertexInput.push_back(v);
		data->InsertVertex(i);
	}
}
template <class Vertex, class EdgeT>
Graph<Vertex, EdgeT>::Graph(int V, int E, bool D, bool F)
{
	edgeCounter = 0;
	size = 0;
	directed = D;
	dense = F;
	if (F)
		data = new GraphMatrixForm<EdgeT>(D);
	else
		data = new GraphListForm<EdgeT>(D);
	for (int i = 0; i < V; i++)
		data->InsertVertex(i);
	for (int i = 0; i < V; i++) {
		Vertex* v = new Vertex();
		v->setName(i_to_s(size));
		size++;
		vertexInput.push_back(v);
		data->InsertVertex(i);
	}
	if (E <= 0) return;
	if (D)
	{
		if (E > V * (V - 1))
			E = V * (V - 1);
		srand((unsigned)time(NULL));
	}
	else
	{
		if (E > (V * (V - 1)) / 2)
			E = (V * (V - 1)) / 2;
	}
	int v1, v2;
	while (edgeCounter < E) {
		v1 = rand() % V;
		v2 = rand() % V;
		if (v1 == v2)
			continue;
		try {
			if (data->hasEdge(v1, v2))
				continue;
		}
		catch (const std::exception&)
		{
			throw std::exception();
		}


		EdgeT* t = new EdgeT(vertexInput[v1], vertexInput[v2]);
		data->InsertEdge(v1, v2, t);
		t->setWeight((rand() % 9) + 1);
		if (D == false)
			data->InsertEdge(v2, v1, t);
		edgeCounter++;
	}
}
template <class Vertex, class EdgeT>
Graph<Vertex, EdgeT>::Graph(const Graph<Vertex, EdgeT>& G)
{
	size = 0;
	delete data;
	if (G.CheckDense())
		data = new GraphMatrixForm<EdgeT>(*(G.data));
	else
		data = new GraphListForm<EdgeT>(*(G.data));
	directed = G.directed;
	dense = G.dense;
}
template <class Vertex, class EdgeT>
Graph<Vertex, EdgeT>::~Graph()
{
	while (vertexInput.size() != 0)
		DeleteVertex(0);
	delete data;
}
template <class Vertex, class EdgeT>
int Graph<Vertex, EdgeT>::VertexSize()
{
	return vertexInput.size();
}
template <class Vertex, class EdgeT>
int Graph<Vertex, EdgeT>::EdgeSize()
{
	return edgeCounter;
}
template <class Vertex, class EdgeT>
bool Graph<Vertex, EdgeT>::Directed()
{
	return directed;
}
template <class Vertex, class EdgeT>
bool Graph<Vertex, EdgeT>::CheckDense()
{
	return dense;
}
template <class Vertex, class EdgeT>
double Graph<Vertex, EdgeT>::K()
{
	int max = vertexInput.size() * (vertexInput.size() - 1);
	if (!directed) {
		max /= 2;
	}
	if (max == 0) {
		return -1;
	}
	else {
		return (double)edgeCounter / (double)max;
	}
}
template <class Vertex, class EdgeT>
void Graph<Vertex, EdgeT>::ToListGraph()
{
	GraphForm<EdgeT>* newData = new GraphListForm<EdgeT>(this->directed);
	for (int i = 0; i < vertexInput.size(); i++)
		newData->InsertVertex(i);
	for (int i = 0; i < vertexInput.size(); i++)
		for (int j = 0; j < vertexInput.size(); j++)
			if (data->hasEdge(i, j))
				newData->InsertEdge(i, j, data->getEdge(i, j));
	delete data;
	data = newData;
	dense = false;
}
template <class Vertex, class EdgeT>
void Graph<Vertex, EdgeT>::ToMatrixGraph()
{
	GraphForm<EdgeT>* newData = new GraphMatrixForm<EdgeT>(this->directed);
	for (int i = 0; i < vertexInput.size(); i++)
		newData->InsertVertex(i);
	for (int i = 0; i < vertexInput.size(); i++)
		for (int j = 0; j < vertexInput.size(); j++)
			if (data->hasEdge(i, j))
				newData->InsertEdge(i, j, data->getEdge(i, j));
	delete data;
	data = newData;
	dense = true;
}
template <class Vertex, class EdgeT>
Vertex* Graph<Vertex, EdgeT>::InsertVertex()
{
	Vertex* v = new Vertex;
	if (data->InsertVertex(vertexInput.size()) == false)
		throw std::exception();
	v->setName(i_to_s(size));
	size++;
	vertexInput.push_back(v);
	return v;
}
template <class Vertex, class EdgeT>
bool Graph<Vertex, EdgeT>::DeleteVertex(Vertex* v)
{
	int index = getIndex(v);
	edgeCounter -= data->DeleteEsFromVertex(index, directed);
	if (data->DeleteVertex(index))
	{
		vertexInput.erase(vertexInput.begin() + index);
		return true;
	}
	return false;
}
template <class Vertex, class EdgeT>
bool Graph<Vertex, EdgeT>::DeleteVertex(int index)
{
	edgeCounter -= data->DeleteEsFromVertex(index, directed);
	if (data->DeleteVertex(index)) {
		vertexInput.erase(vertexInput.begin() + index);
		return true;
	}
	return false;
}
template <class Vertex, class EdgeT>
bool Graph<Vertex, EdgeT>::DeleteVertex(string n)
{
	Vertex* v = NULL;
	try {
		v = getVertexFromName(n);
	}
	catch (const std::exception&) {
		return false;
	}
	return DeleteVertex(v);
}
template <class Vertex, class EdgeT>
Vertex* Graph<Vertex, EdgeT>::getVertex(int index)
{
	if (index < 0 || index >= vertexInput.size())
		throw std::exception();
	return vertexInput[index];
}
template <class Vertex, class EdgeT>
Vertex* Graph<Vertex, EdgeT>::getVertexFromName(string name)
{
	int i;
	for (i = 0; i < vertexInput.size(); i++)
		if (vertexInput[i]->getName() == name)
			return vertexInput[i];
	throw std::exception();
}
template <class Vertex, class EdgeT>
int Graph<Vertex, EdgeT>::getIndex(Vertex* v)
{
	int index = 0;
	for (int i = 0; i < vertexInput.size(); ++i) {
		if (vertexInput[index] == v)
			break;
		index++;
	}
	if (index == vertexInput.size())
		throw std::exception();
	return index;
}
template <class Vertex, class EdgeT>
bool Graph<Vertex, EdgeT>::InsertEdge(string v1, string v2)
{
	Vertex* _v1 = new Vertex;
	Vertex* _v2 = new Vertex;
	try {
		_v1 = getVertexFromName(v1);
	}
	catch (const std::exception&)
	{
		return false;
	}
	try
	{
		_v2 = getVertexFromName(v2);
	}
	catch (const std::exception&)
	{
		return false;
	}
	try
	{
		InsertEdge(_v1, _v2);
	}
	catch (const std::exception&)
	{
		return false;
	}
	return true;
}
template <class Vertex, class EdgeT>
EdgeT* Graph<Vertex, EdgeT>::InsertEdge(Vertex* v1, Vertex* v2)
{
	EdgeT* e = new EdgeT(v1, v2);
	if (!data->InsertEdge(getIndex(v1), getIndex(v2), e)) throw std::exception();
	if (directed == false)
		data->InsertEdge(getIndex(v2), getIndex(v1), e);
	edgeCounter++;
	return e;
}
template <class Vertex, class EdgeT>
bool Graph<Vertex, EdgeT>::DeleteEdge(string v1, string v2)
{
	Vertex* _v1 = new Vertex;
	Vertex* _v2 = new Vertex;
	try
	{
		_v1 = getVertexFromName(v1);
	}
	catch (const std::exception&)
	{
		return false;
	}
	try
	{
		_v2 = getVertexFromName(v2);
	}
	catch (const std::exception&)
	{
		return false;
	}
	if (!DeleteEdge(_v1, _v2)) return false;
	else return true;
}

template <class Vertex, class EdgeT>
bool Graph<Vertex, EdgeT>::DeleteEdge(Vertex* v1, Vertex* v2)
{
	if (data->DeleteEdge(getIndex(v1), getIndex(v2)))
	{
		edgeCounter--;
		if (directed == false)
			data->DeleteEdge(getIndex(v2), getIndex(v1));
		return true;
	}
	else
		return false;
}
template <class Vertex, class EdgeT>
bool Graph<Vertex, EdgeT>::hasEdge(Vertex* v1, Vertex* v2)
{
	int ind1, ind2;
	try {
		ind1 = getIndex(v1);
	}
	catch (const std::exception&) {
		return false;
	}
	try {
		ind2 = getIndex(v2);
	}
	catch (const std::exception&)
	{
		return false;
	}
	return data->hasEdge(getIndex(v1), getIndex(v2));
}
template <class Vertex, class EdgeT>
bool Graph<Vertex, EdgeT>::hasEdge(int v1, int v2)
{
	if (v1 < 0 || v1 >= vertexInput.size())
		return false;
	if (v2 < 0 || v2 >= vertexInput.size())
		return false;
	return data->hasEdge(v1, v2);
}
template <class Vertex, class EdgeT>
EdgeT* Graph<Vertex, EdgeT>::getEdge(Vertex* v1, Vertex* v2)
{
	EdgeT* e;
	try {
		e = data->getEdge(getIndex(v1), getIndex(v2));
	}
	catch (const std::exception&) {
		throw std::exception();
	}
	return e;
}
template <class Vertex, class EdgeT>
bool  Graph<Vertex, EdgeT>::WriteDataEdge(string _v1, string _v2, int data)
{
	EdgeT* e;
	Vertex* v1, * v2;
	try {
		v1 = getVertexFromName(_v1);
	}
	catch (const std::exception&) {
		return false;
	}
	try {
		v2 = getVertexFromName(_v2);
	}
	catch (const std::exception&) {
		return false;
	}
	try {
		e = getEdge(v1, v2);
		e->setData(data);
	}
	catch (const std::exception&) {
		return false;
	}
	return true;
}
template <class Vertex, class EdgeT>
bool Graph<Vertex, EdgeT>::WriteWeightEdge(string _v1, string _v2, int w)
{
	EdgeT* e;
	Vertex* v1, * v2;
	try {
		v1 = getVertexFromName(_v1);
	} 
	catch (const std::exception&) {
		return false;
	}
	try {
		v2 = getVertexFromName(_v2);
	}
	catch (const std::exception&) {
		return false;
	}
	try {
		e = getEdge(v1, v2);
		e->setWeight(w);
	}
	catch (const std::exception&) {
		return false;
	}
	return true;
}
template <class Vertex, class EdgeT>
int Graph<Vertex, EdgeT>::ReadDataEdge(string _v1, string _v2)
{
	EdgeT* e;
	Vertex* v1, * v2;
	try {
		v1 = getVertexFromName(_v1);
		v2 = getVertexFromName(_v2);
	}
	catch (const std::exception&) {
		return -1;
	}
	try {
		e = getEdge(v1, v2);
	}
	catch (const std::exception&) {
		return -1;
	}
	try {
		if (e->isDataSet())
			return e->getData();
		else
			return -1;
	}
	catch (const std::exception&) {
		return -1;
	}
}
template <class Vertex, class EdgeT>
int Graph<Vertex, EdgeT>::ReadWeightEdge(string _v1, string _v2)
{
	EdgeT* e;
	Vertex* v1, * v2;
	try {
		v1 = getVertexFromName(_v1);
		v2 = getVertexFromName(_v2);
		e = getEdge(v1, v2);
	}
	catch (const std::exception&) {
		return -1;
	}
	try {
		if (e->isWeightSet())
			return e->getWeight();
		else
			throw "Вес ребра не установлен";
	}
	catch (const std::exception&) {
		return -1;
	}
}
template <class Vertex, class EdgeT>
void Graph<Vertex, EdgeT>::print()
{
	int i, j;
	Vertex* v;
	EdgeT* e;
	if (CheckDense())
	{
		cout << setw(5) << "  ";
		for (i = 0; i < VertexSize(); i++)
		{
			v = getVertex(i);
			cout << setw(4) << v->getName();
		}
		cout << endl;
		for (i = 0; i < 5 * VertexSize() + 4; i++)
			cout << "_";
		cout << endl;
		for (i = 0; i < VertexSize(); i++)
		{
			v = getVertex(i);
			cout << setw(4) << v->getName() << "|";
			for (j = 0; j < VertexSize(); j++)
				if (hasEdge(i, j))
				{
					e = getEdge(getVertex(i), getVertex(j));
					cout << setw(4) << e->getWeight();
				}
				else
					cout << setw(4) << "-";
			cout << endl;
		}
	}
	else
	{
		for (i = 0; i < VertexSize(); i++) {
			v = getVertex(i);
			cout << v->getName() << ": ";
			OutputEdgeIterator out(*this, *v);
			while (!out.onEnd())
			{
				cout << (*out)->V2()->getName() << "; ";
				++out;
			}
			cout << endl;
		}
	}
};
template <class Vertex, class EdgeT>
int Graph<Vertex, EdgeT>::ReadDataVertex(string v1)
{
	Vertex* v;
	try
	{
		v = getVertexFromName(v1);
	}
	catch (const std::exception&) {
		return -1;
	}
	int d;
	d = v->getData();
	try {
		if (v->isDataSet())
			return d;
		else
			throw std::exception();
	}
	catch (const std::exception&) {
		return -1;
	}
}
template <class Vertex, class EdgeT>
bool Graph<Vertex, EdgeT>::WriteDataVertex(string v1, int d)
{
	Vertex* v;
	try
	{
		v = getVertexFromName(v1);
	}
	catch (const std::exception&) {
		return false;
	};
	v->setData(d);
	return true;
}
template <class Vertex, class EdgeT>
bool Graph<Vertex, EdgeT>::WriteNameVertex(string v1, string str)
{
	Vertex* v;
	try
	{
		v = getVertexFromName(v1);
	}
	catch (const std::exception&) {
		throw std::exception();
	};
	v->setName(str);
	return true;
}


