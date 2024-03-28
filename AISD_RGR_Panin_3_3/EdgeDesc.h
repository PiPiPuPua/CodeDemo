#pragma once
template <class Vertex, class Data>
class Edge {
private:
	Vertex* v1, * v2;
	int weight = 1;
	Data data;
	bool weightSet, dataSet;
public:
	Edge(Vertex*, Vertex*);
	Edge(Vertex*, Vertex*, int);
	Edge(Vertex*, Vertex*, int, Data);
	void setWeight(int weight) { this->weight = weight; weightSet = true; }
	void setData(Data data) { this->data = data; dataSet = true; }
	int getWeight() { return weight; }
	Data getData() { return data; }
	bool isWeightSet() { return weightSet; }
	bool isDataSet() { return dataSet; }
	Vertex* V1() { return v1; }
	Vertex* V2() { return v2; }
};
template <class Vertex, class Data>
Edge<Vertex, Data>::Edge(Vertex* v1, Vertex* v2) {
	this->v1 = v1;
	this->v2 = v2;
	weightSet = false;
	dataSet = false;
	weight = 0;
}
template <class Vertex, class Data>
Edge<Vertex, Data>::Edge(Vertex* v1, Vertex* v2, int weight) {
	this->v1 = v1;
	this->v2 = v2;
	weightSet = true;
	dataSet = false;
	this->weight = weight;
}
template <class Vertex, class Data>
Edge<Vertex, Data>::Edge(Vertex* v1, Vertex* v2, int weight, Data data) {
	this->v1 = v1;
	this->v2 = v2;
	weightSet = true;
	dataSet = true;
	this->weight = weight;
	this->data = data;
}
