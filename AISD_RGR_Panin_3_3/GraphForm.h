#include <vector>

using namespace std;
template <class EdgeT> class GraphForm {
public:
	virtual bool InsertVertex(int index) = 0;
	virtual bool DeleteVertex(int index) = 0;
	virtual bool InsertEdge(int v1, int v2, EdgeT* t) = 0;
	virtual bool DeleteEdge(int v1, int v2) = 0;
	virtual int DeleteEsFromVertex(int index, bool directed) = 0;
	virtual bool hasEdge(int v1, int v2) = 0;
	virtual EdgeT* getEdge(int v1, int v2) = 0;
};

template <class EdgeT> class GraphMatrixForm : public GraphForm < EdgeT >
{
	friend class EdgeIterator;
	bool directed;
	vector<vector<EdgeT*>> matrix;

public:
	GraphMatrixForm(bool directed) { this->directed = directed; }

	bool InsertVertex(int index) {

		int size = matrix.size();
		if (index < 0 || index > size) return false;
		vector<EdgeT*> newLine;
		newLine.assign(size, NULL);
		matrix.insert(matrix.begin() + index, newLine);
		size++;
		for (int i = 0; i < size; ++i)
			matrix[i].insert(matrix[i].begin() + index, (EdgeT*)NULL);
		return true;
	}

	bool DeleteVertex(int index)
	{
		int size = matrix.size();
		if (index < 0 || index >= size)
			return false;
		matrix.erase(matrix.begin() + index);
		size--;
		for (int i = 0; i < size; i++)
			matrix[i].erase(matrix[i].begin() + index);
		return true;
	}
	bool InsertEdge(int v1, int v2, EdgeT* t)
	{
		int size = matrix.size();
		if (v1 < 0 || v2 < 0 || v1 >= size || v2 >= size || v1 == v2 || matrix[v1][v2] != NULL)
			return false;
		matrix[v1][v2] = t;
		return true;
	}
	bool DeleteEdge(int v1, int v2)
	{
		int size = matrix.size();
		if (v1 < 0 || v2 < 0 || v1 >= size || v2 >= size || v1 == v2 || matrix[v1][v2] == NULL)
			return false;
		matrix[v1][v2] = NULL;
		return true;
	}
	int DeleteEsFromVertex(int index, bool directed)
	{
		int size = matrix.size();
		int deleted = 0;
		if (index < 0 || index >= size) return 0;
		for (int i = 0; i < size; i++) {
			if (matrix[i][index] != NULL) {
				delete matrix[i][index];
				matrix[i][index] = NULL;
				deleted++;
				if (directed == false)
					matrix[index][i] = NULL;
			}
			if (matrix[index][i] != NULL) {
				delete matrix[index][i];
				matrix[index][i] = NULL;
				deleted++;
			}
		}
		return deleted;
	}
	bool hasEdge(int v1, int v2)
	{
		int size = matrix.size();
		if (v1 < 0 || v2 < 0 || v1 >= size || v2 >= size || v1 == v2)
			return false;
		if (matrix[v1][v2] != NULL)
			return true;
		return false;
	}

	EdgeT* getEdge(int v1, int v2)
	{
		int size = matrix.size();
		if (v1 < 0 || v2 < 0 || v1 >= size || v2 >= size || v1 == v2 || matrix[v1][v2] == NULL)
			throw std::exception();
		return matrix[v1][v2];
	}
	class EdgeIterator
	{
		bool end;
		GraphMatrixForm* f;
	public:
		int curI, cur;
		EdgeIterator(GraphMatrixForm* f)
		{
			this->f = f;
			begin();
		}

		bool begin()
		{
			for (int i = 0; i < f->matrix.size(); i++)
				for (int j = 0; j < f->matrix.size(); j++)
					if (f->matrix[i][j])
					{
						curI = i;
						cur = j;
						end = false;
						return true;
					}
			end = true;
			return false;
		}

		bool onEnd() { return end; }

		bool toend()
		{
			for (int i = f->matrix.size() - 1; i >= 0; i--)
				for (int j = f->matrix.size() - 1; j >= 0; j--)
					if (f->matrix[i][j]) {
						curI = i;
						cur = j;
						end = false;
						return true;
					}
			end = true;
			return false;
		}

		bool operator++()
		{
			if (end)
				return false;
			++cur;
			while (curI < f->matrix.size()) {
				while (cur < f->matrix.size()) {
					if (f->matrix[curI][cur])
						return true;
					++cur;
				}
				++curI;
				cur = (f->directed ? 0 : curI + 1);
			}
			end = true;
			return false;
		}

		EdgeT* getEdge()
		{
			if (end)
				throw std::exception();
			if (!(f->matrix[curI][cur]))
				throw std::exception();
			return f->matrix[curI][cur];
		}
	};

	class OutputEdgeIterator
	{
		int curI, cur;
		bool end;
		GraphMatrixForm* f;
	public:
		OutputEdgeIterator(GraphMatrixForm* f, int vIndex)
		{
			this->f = f;
			curI = vIndex;
			begin();
		}

		bool begin()
		{
			for (cur = 0; cur < f->matrix.size(); cur++)
				if (f->matrix[curI][cur]) {
					end = false;
					return true;
				}
			end = true;
			return false;
		}

		bool toend()
		{
			for (cur = f->matrix.size() - 1; cur >= 0; cur--)
				if (f->matrix[curI][cur]) {
					end = false;
					return true;
				}
			end = true;
			return false;
		}

		bool onEnd()
		{
			return end;
		}

		bool operator++()
		{
			if (end)
				return false;
			cur++;
			while (cur < f->matrix.size()) {
				if (f->matrix[curI][cur])
					return true;
				cur++;
			}
			end = true;
			return false;
		}

		EdgeT* getEdge()
		{
			if (end)
				throw std::exception();
			return f->matrix[curI][cur];
		}
	};
};



//форма представления список
template <class EdgeT> class GraphListForm : public GraphForm < EdgeT > {
	class Node
	{
	public:
		EdgeT* edge;
		int v2;
	};

	bool directed;
	vector<list<Node>> edgeList;
public:
	GraphListForm(bool directed) { this->directed = directed; }

	bool InsertVertex(int index)
	{
		int size = edgeList.size();
		if (index < 0 || index > size)
			return false;
		list<Node> newList;
		edgeList.insert(edgeList.begin() + index, newList);
		++size;
		for (int i = 0; i < size; ++i)
			for (typename list<Node>::iterator j = edgeList[i].begin(); j != edgeList[i].end(); ++j)
				if ((*j).v2 >= index)
					++((*j).v2);
		return true;
	}

	bool DeleteVertex(int index)
	{
		int size = edgeList.size();
		if (index < 0 || index >= size)
			return false;
		for (int i = 0; i < size; ++i)
			for (typename list<Node>::iterator j = edgeList[i].begin(); j != edgeList[i].end(); ++j)
				if ((*j).v2 == index)
				{
					edgeList[i].erase(j);
					break;
				}
		edgeList.erase(edgeList.begin() + index);
		--size;
		for (int i = 0; i < size; ++i)
			for (typename list<Node>::iterator j = edgeList[i].begin(); j != edgeList[i].end(); ++j)
				if ((*j).v2 > index)
					--((*j).v2);
		return true;
	}

	bool InsertEdge(int v1, int v2, EdgeT* t)
	{
		int size = edgeList.size();
		if (v1 < 0 || v2 < 0 || v1 >= size || v2 >= size)
			return false;
		if (v1 == v2 || hasEdge(v1, v2))
			return false;
		Node node;
		node.edge = t;
		node.v2 = v2;
		edgeList[v1].push_front(node);
		return true;
	}

	bool DeleteEdge(int v1, int v2)
	{
		int size = edgeList.size();
		if (v1 < 0 || v2 < 0 || v1 >= size || v2 >= size)
			return false;
		if (v1 == v2 || hasEdge(v1, v2) == false)
			return false;
		for (typename list<Node>::iterator j = edgeList[v1].begin(); j != edgeList[v1].end(); ++j)
			if ((*j).v2 == v2) {
				edgeList[v1].erase(j);
				break;
			}
		return true;
	}
	int DeleteEsFromVertex(int index, bool directed)
	{
		int size = edgeList.size();
		int deleted = 0;
		if (index < 0 || index >= size)
			return 0;
		for (int i = 0; i < size; ++i)
			for (typename list<Node>::iterator j = edgeList[i].begin(); j != edgeList[i].end(); ++j)
				if ((*j).v2 == index)
				{
					if (!directed)
						for (typename list<Node>::iterator k = edgeList[index].begin(); k != edgeList[index].end(); ++k)
							if ((*k).v2 == i)
							{
								edgeList[index].erase(k);
								break;
							}
					delete (*j).edge;
					edgeList[i].erase(j);
					deleted++;
					break;
				}
		return deleted;
	}
	bool hasEdge(int v1, int v2)
	{
		int size = edgeList.size();
		if (v1 < 0 || v2 < 0 || v1 >= size || v2 >= size)
			return false;
		if (v1 == v2)
			return false;
		for (typename list<Node>::iterator j = edgeList[v1].begin(); j != edgeList[v1].end(); ++j)
			if ((*j).v2 == v2)
				return true;
		return false;
	}

	EdgeT* getEdge(int v1, int v2)
	{
		int size = edgeList.size();
		if (v1 < 0 || v2 < 0 || v1 >= size || v2 >= size)
			throw std::exception();
		if (v1 == v2)
			throw std::exception();
		for (typename list<Node>::iterator j = edgeList[v1].begin(); j != edgeList[v1].end(); ++j)
			if ((*j).v2 == v2)
				return (*j).edge;
		throw std::exception();
	}
	class EdgeIterator
	{
		int curI;
		typename list<typename GraphListForm<EdgeT>::Node>::iterator cur;
		bool end;
		GraphListForm* f;
	public:
		EdgeIterator(GraphListForm* f)
		{
			this->f = f;
		}
		bool begin()
		{
			for (curI = 0; curI < f->edgeList.size(); ++curI)
				for (cur = f->edgeList[curI].begin(); cur != f->edgeList[curI].end(); ++cur)
					if ((*cur).edge) {
						end = false;
						return true;
					}
			end = true;
			return false;
		}

		bool onEnd()
		{
			return end;
		}
		bool toend()
		{
			typename list<typename GraphListForm<EdgeT>::Node>::iterator prev_curJ;
			for (curI = 0; curI < f->edgeList.size(); ++curI) {
				for (cur = f->edgeList[curI].begin(); cur != f->edgeList[curI].end(); ++cur) {
					if ((*cur).edge)
						prev_curJ = cur;
					if (!(*prev_curJ).edge) {
						end = true;
						return false;
					}
					else {
						end = false;
						cur = prev_curJ;
						return true;
					}
				}
			}
		}
		bool operator++()
		{
			if (end)
				return false;
			++cur;
			while (curI < f->edgeList.size()) {
				while (cur != f->edgeList[curI].end()) {
					if ((*cur).edge && (f->directed || !f->directed && (*cur).v2 > curI))
						return true;
					++cur;
				}
				++curI;
				if (curI < f->edgeList.size())
					cur = f->edgeList[curI].begin();
			}
			end = true;
			return true;
		}

		EdgeT* getEdge() {
			if (end)
				throw std::exception();
			return (*cur).edge;
		}

	};
	class OutputEdgeIterator
	{
	private:
		int curI;
		typename list<typename GraphListForm<EdgeT>::Node>::iterator cur;
		bool end;
		GraphListForm* f;
	public:
		OutputEdgeIterator(GraphListForm* f, int vIndex)
		{
			this->f = f;
			curI = vIndex;
			begin();
		}

		bool begin()
		{
			for (cur = f->edgeList[curI].begin(); cur != f->edgeList[curI].end(); ++cur)
				if ((*cur).edge) {
					end = false;
					return true;
				}
			end = true;
			return false;
		}

		bool onEnd()
		{
			return end;
		}

		bool toend()
		{
			typename list<typename GraphListForm<EdgeT>::Node>::iterator prev_curJ;
			for (cur = f->edgeList[curI].begin(); cur != f->edgeList[curI].end(); ++cur)
				if ((*cur).edge)
					prev_curJ = cur;
			if (!(*prev_curJ).edge) {
				end = true;
				return false;
			}
			else {
				end = false;
				cur = prev_curJ;
				return true;
			}
		}

		bool operator++()
		{
			if (end)
				return false;
			++cur;
			while (cur != f->edgeList[curI].end()) {
				if ((*cur).edge)
					return true;
				++cur;
			}
			end = true;
			return true;
		}

		EdgeT* getEdge()
		{
			if (end)
				throw std::exception();
			return (*cur).edge;
		}
	};
};
