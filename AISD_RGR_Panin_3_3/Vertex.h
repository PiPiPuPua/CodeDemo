#pragma once
template <class Name, class Data>
class Vertex
{
private:
	Name name;
	Data data;
	int index;
	bool nameSet, dataSet;
public:
	Vertex();
	Vertex(Name name, Data data);
	void setName(Name name);
	void setData(Data data);
	Name getName() { return name; }
	Data getData() { return data; }
	bool isNameSet() { return nameSet; }
	bool isDataSet() { return dataSet; }
};
template <class Name, class Data>
Vertex <Name, Data>::Vertex() {
	nameSet = false;
	dataSet = false;
	index = -1;
}

template <class Name, class Data>
Vertex <Name, Data>::Vertex(Name name, Data data) {
	this->name = name;
	this->data = data;
	index = -1;
	nameSet = true;
	dataSet = true;
}
template <class Name, class Data>
void Vertex <Name, Data>::setName(Name name)
{
	this->name = name;
	index = -1;
	nameSet = true;
}
template <class Name, class Data>
void Vertex <Name, Data>::setData(Data data)
{
	this->data = data;
	index = -1;
	dataSet = true;
}
