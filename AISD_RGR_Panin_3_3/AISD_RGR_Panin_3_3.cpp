#include <cstdlib>
#include <ctime>
#include "Classification.h"
#include "Task1.h"
#include "Task2.h"
#include "Graph.h"

typedef Vertex <string, int> TVertex;
typedef Edge<TVertex, int> TEdge;
typedef Graph <TVertex, TEdge> TGraph;

typedef Task1 <TVertex, TEdge> task1;
typedef Task2 <TVertex, TEdge> task2;

typedef Classification<TVertex, TEdge> classification;

void getmenu() {
	cout << "����:" << endl;
	cout << "_________�������� ������_________" << endl;
	cout << "[0] ������� ������ L-����" << endl;
	cout << "[1] ������� ���� � V ���������" << endl;
	cout << "[2] ������� ���� � V ���������, � E ���������� �������" << endl << endl;

	cout << "_________�������� ��� ������_________" << endl;
	cout << "[3] ����������� ����" << endl;
	cout << "[4] ����� ������ � �����" << endl;
	cout << "[5] ����� ����� � �����" << endl;
	cout << "[6] ����� ���� �����" << endl;
	cout << "[7] ����� ����� ������������� �����" << endl;
	cout << "[8] ����������� ������������ �����" << endl;
	cout << "[9] ������������� ���� � L-�����" << endl;
	cout << "[10] ������������� ���� � M-�����" << endl;
	cout << "[11] �������� ������� � �����" << endl;
	cout << "[12] ������� ������� �� �����" << endl;
	cout << "[13] �������� ����� � ����" << endl;
	cout << "[14] ������� ����� �� �����" << endl;
	cout << "[15] �������� �� ������������� ����� � �����" << endl << endl;

	cout << "_________���������� ������_________" << endl;
	cout << "[16] ��������� ������ � �������" << endl;
	cout << "[17] �������� ������ � �������" << endl;
	cout << "[18] ������������ ��� �������" << endl << endl;

	cout << "_________���������� ����_________" << endl;
	cout << "[19] ��������� ��� �����" << endl;
	cout << "[20] ��������� ������ � �����" << endl;
	cout << "[21] �������� ������ � �����" << endl;
	cout << "[22] �������� ��� �����" << endl << endl;

	cout << "_________�������� ������_________" << endl;
	cout << "[23] ���������� �������� ������ �� ��������� �������" << endl;
	cout << "[24] ���������� �������� ������ �� �������� �������" << endl;
	cout << "[25] ������� � ��������� �������" << endl;
	cout << "[26] ��������� ������� �� ������� ������� ���������" << endl;
	cout << "[27] ��������� ������ ������� �� ������� ������� ���������" << endl;
	cout << "[28] �������� ������ ������� �� ������� ������� ���������" << endl;
	cout << "[29] �������� ��� ������� �� ������� ������� ���������" << endl << endl;

	cout << "_________�������� �����_________" << endl;
	cout << "[30] ���������� �������� ����� �� ��������� �����" << endl;
	cout << "[31] ���������� �������� ����� �� �������� �����" << endl;
	cout << "[32] ������� � ��������� �������" << endl;
	cout << "[33] ��������� ������� �� ������� ������� ���������" << endl;
	cout << "[34] ��������� ������ ����� �� ������� ������� ���������" << endl;
	cout << "[35] �������� ������ � ����� �� ������� ������� ���������" << endl;
	cout << "[36] �������� ��� ����� �� ������� ������� ���������" << endl << endl;

	cout << "_________�������� ��������� �����_________" << endl;
	cout << "[37] ������� �������� ���. �����" << endl;
	cout << "[38] ���������� �������� ��������� ����� �� ��������� �����" << endl;
	cout << "[39] ���������� �������� ��������� ����� �� �������� �����" << endl;
	cout << "[40] ������� � ��������� �������" << endl;
	cout << "[41] ��������� ������� ����� � ��� �� ������� ������� ���������" << endl;
	cout << "[42] ��������� ������ ����� �� ������� ������� ���������" << endl;
	cout << "[43] �������� ������ � ����� �� ������� ������� ���������" << endl;
	cout << "[44] �������� ��� ����� �� ������� ������� ���������" << endl << endl;

	cout << "[45] ������ 2" << endl;
	cout << "[46] ������ 3" << endl << endl;

	cout << "[47] ����� �� ���������" << endl << endl;
}
void clear(TGraph*& graph, TGraph::VertexIterator*& vIt, TGraph::EdgeIterator*& eIt, TGraph::OutputEdgeIterator*& oIt)
{
	if (graph != NULL)
		delete graph;
	if (vIt != NULL)
		delete vIt;
	if (eIt != NULL)
		delete eIt;
	if (oIt != NULL)
		delete oIt;
	graph = NULL;
	vIt = NULL;
	eIt = NULL;
	oIt = NULL;
}

int main(int argc, char* argv[])
{
	int tmp, tmp1, tmp2, tmp3, tmp4;
	string v, v1, v2, cur;
	bool on = false;
	TGraph* graph = NULL;
	TGraph::VertexIterator* vIt = NULL;
	TGraph::EdgeIterator* eIt = NULL;
	TGraph::OutputEdgeIterator* oIt = NULL;
	setlocale(LC_ALL, "Russian");
	getmenu();
	while (true)
	{
		cout << "������� �������: ";
		int command;
		cin >> command;

		if (graph == NULL && command > 2 && command < 47)
		{
			cout << "���� �� ������" << endl;
			continue;
		}

		if (oIt == NULL && command > 37 && command < 45)
		{
			cout << "�������� �� ������" << endl;
			continue;
		}

		switch (command)
		{
		case 0:
		{
			clear(graph, vIt, eIt, oIt);
			graph = new TGraph();
			vIt = new TGraph::VertexIterator(*graph);
			eIt = new TGraph::EdgeIterator(*graph);
			cout << endl << "���� ������" << endl;
			break;
		}
		case 1:
		{
			cout << "������� ���������� ������: ";
			cin >> tmp1;
			cout << "��������������� �� ���� (0 - ���, 1 - ��): ";
			cin >> tmp2;
			if (tmp2 == 1)
				on = true;
			cout << "����� ����� (0 - ������, 1 - �������): ";
			cin >> tmp3;
			clear(graph, vIt, eIt, oIt);
			graph = new TGraph(tmp1, tmp2, tmp3);
			vIt = new TGraph::VertexIterator(*graph);
			eIt = new TGraph::EdgeIterator(*graph);
			cout << endl << "���� ������" << endl;
			break;
		}
		case 2:
		{
			cout << "������� ���������� ������: ";
			cin >> tmp1;
			cout << "������� ���������� �����: ";
			cin >> tmp2;
			cout << "��������������� �� ���� (0 - ���, 1 - ��): ";
			cin >> tmp3;
			if (tmp3 == 1)
				on = true;
			cout << "����� ����� (0 - L, 1 - M): ";
			cin >> tmp4;
			clear(graph, vIt, eIt, oIt);
			try
			{
				graph = new TGraph(tmp1, tmp2, tmp3, tmp4);
				vIt = new TGraph::VertexIterator(*graph);
				eIt = new TGraph::EdgeIterator(*graph);
			}
			catch (const std::exception&)
			{
				cout << "����������" << endl;
			}
			cout << endl << "���� ������" << endl;
			break;
		}

		case 3:
			graph->print();
			break;

		case 4:
			try
			{
				cout << "����� ������ � �����: " << graph->VertexSize() << endl;
			}
			catch (const std::exception&)
			{
				cout << "����������" << endl;
			}
			break;

		case 5:
			cout << "����� ����� � �����: " << graph->EdgeSize() << endl;
			break;

		case 6:
			cout << "0) �����������������\n1) ���������������\n��� �����: " << graph->Directed() << endl;
			break;

		case 7:
			cout << "0) - L-����\n1) - M-����\n ����� ������������� �����:" << graph->CheckDense() << endl;
			break;

		case 8:
			cout << "����������� ������������: " << graph->K() << endl;
			break;

		case 9:
		{
			if (!graph->CheckDense())
			{
				cout << graph->CheckDense() << endl;
				break;
			}
			if (vIt != NULL)
				delete vIt;
			if (eIt != NULL)
				delete eIt;
			graph->ToListGraph();
			vIt = new TGraph::VertexIterator(*graph);
			eIt = new TGraph::EdgeIterator(*graph);
			cout << !graph->CheckDense() << endl;
			break;
		}

		case 10:
		{
			if (graph->CheckDense())
			{
				cout << !graph->CheckDense() << endl;
				break;
			}
			if (vIt != NULL)
				delete vIt;
			if (eIt != NULL)
				delete eIt;
			graph->ToMatrixGraph();
			vIt = new TGraph::VertexIterator(*graph);
			eIt = new TGraph::EdgeIterator(*graph);
			cout << graph->CheckDense() << endl;
			break;
		}

		case 11:
		{
			try
			{
				if (graph->InsertVertex())
					cout << "������� ��������� � ����" << endl;
				else
					cout << "������� �� ��������� � ����" << endl;
			}
			catch (const std::exception&)
			{
				cout << "����������" << endl;
			}
			break;
		}
		case 12:
		{
			cout << "������� ��������� �������: ";
			cin >> v1;
			cout << graph->DeleteVertex(v1) << endl;
			break;
		}

		case 13:
		{
			cout << "������� �������, �� ������� ������� �����: ";
			cin >> v1;
			cout << "������� �������, � ������� ������ �����: ";
			cin >> v2;
			cout << graph->InsertEdge(v1, v2) << endl;
			break;
		}

		case 14:
		{
			cout << "������� ������� 1: ";
			cin >> v1;
			cout << "������� ������� 2: ";
			cin >> v2;
			cout << graph->DeleteEdge(v1, v2) << endl;
			break;
		}
		case 15:
		{
			cout << "������� ������� 1: ";
			cin >> tmp1;
			cout << "������� ������� 2: ";
			cin >> tmp2;
			cout << graph->hasEdge(tmp1, tmp2) << endl;
			break;
		}
		case 16:
		{
			cout << "������� ��� �������: ";
			cin >> v;
			try
			{
				tmp = graph->ReadDataVertex(v);
				if (tmp != -1) {
					cout << endl << "������ �������: " << tmp << endl;
				}
				else {
					cout << "��� ������." << endl;
				}

			}
			catch (const std::exception&)
			{
				cout << "��� ������. ������� ��� ������ \"16\"" << endl;
			}

			break;
		}

		case 17:
		{
			cout << "������� ��� �������: ";
			cin >> v;
			cout << "������� ������: ";
			cin >> tmp;
			cout << endl << graph->WriteDataVertex(v, tmp) << endl;
			break;
		}
		case 18:
		{
			cout << "������� ������ ��� �������: ";
			cin >> v1;
			cout << "������� ����� ���: ";
			cin >> v2;
			cout << graph->WriteNameVertex(v1, v2) << endl;
			break;
		}
		case 19:
		{
			cout << "������� ��� ��������� �������: ";
			cin >> v1;
			cout << "������� ��� �������� �������: ";
			cin >> v2;
			tmp = graph->ReadWeightEdge(v1, v2);
			if (tmp != -1)
				cout << "��� �����: " << tmp << endl;
			break;
		}

		case 20:
		{
			cout << "������� ��� ��������� �������: ";
			cin >> v1;
			cout << "������� ��� �������� �������: ";
			cin >> v2;
			try
			{
				tmp = graph->ReadDataEdge(v1, v2);
				if (tmp != -1)
					cout << "������ �����: " << tmp << endl;
			}
			catch (const std::exception&)
			{
				cout << "������ ����� �� �����������" << endl;
			}

			break;
		}
		case 21:
		{
			cout << "������� ��� ��������� �������: ";
			cin >> v1;
			cout << "������� ��� �������� �������: ";
			cin >> v2;
			cout << "������� ������: ";
			cin >> tmp;
			try
			{
				cout << graph->WriteDataEdge(v1, v2, tmp) << endl;
			}
			catch (const std::exception&)
			{
				cout << "0" << endl;
			}

			break;
		}
		case 22:
		{
			cout << "������� ��� ��������� �������: ";
			cin >> v1;
			cout << "������� ��� �������� �������: ";
			cin >> v2;
			cout << "������� ���: ";
			cin >> tmp;
			cout << graph->WriteWeightEdge(v1, v2, tmp) << endl;
			break;
		}
		case 23:
		{
			if (!(vIt->begin()))
				cout << "�������� �� ����������" << endl;
			break;
		}
		case 24:
		{
			if (!(vIt->toend()))
				cout << "�������� �� ����������" << endl;
			break;
		}

		case 25:
		{
			try
			{
				++(*vIt);

			}
			catch (const std::exception&)
			{
				cout << "�������� �� ����������" << endl;
			}
			break;
		}

		case 26:
		{
			try
			{
				cout << vIt->ReadVertex() << endl;
			}
			catch (const std::exception&)
			{
				cout << "�������� �� ����������" << endl;
			}

			break;
		}

		case 27:
		{
			try
			{
				cout << vIt->ReadDataVertex() << endl;
			}
			catch (const std::exception&)
			{
				cout << "�������� �� ����������" << endl;
			}
			break;
		}

		case 28:
		{
			cout << "������� ������: ";
			cin >> tmp;
			cout << vIt->WriteDataVertex(tmp) << endl;
			break;
		}

		case 29:
		{
			cout << "������� ���: ";
			cin >> cur;
			cout << vIt->WriteNameVertex(cur) << endl;
			break;
		}
		case 30:
		{
			if (eIt->begin())
				cout << "�������� ���������� �� ��������� �����" << endl;
			break;
		}

		case 31:
		{
			if (eIt->toend())
				cout << "�������� ���������� �� �������� �����" << endl;
			break;
		}

		case 32:
		{
			try
			{
				++(*eIt);

			}
			catch (const std::exception&)
			{
				cout << "�������� �� ����������" << endl;
			}
			break;
		}

		case 33:
		{
			try
			{
				cout << eIt->ReadEdge() << endl;
			}
			catch (const std::exception&)
			{
				cout << "�������� �� ����������" << endl;
			}
			break;
		}

		case 34:
		{
			try
			{
				cout << eIt->ReadDataEdge() << endl;
			}
			catch (const std::exception&)
			{
				cout << "��� ������" << endl;
			}
			break;
		}

		case 35:
		{
			cout << "������� ������: ";
			cin >> tmp;
			cout << eIt->WriteDataEdge(tmp) << endl;
			break;
		}

		case 36:
		{
			cout << "������� ���: ";
			cin >> tmp;
			cout << eIt->WriteWeightEdge(tmp) << endl;
			break;
		}

		case 37:
		{
			cout << "������� ��� �������: ";
			cin >> v;
			try
			{
				oIt = new TGraph::OutputEdgeIterator(*graph, *(graph->getVertexFromName(v)));
				cout << "�������� ������" << endl;
			}
			catch (const std::exception&)
			{
				cout << "�������� �������: " << endl;
			}
			break;
		}
		case 38:
		{
			if (oIt->begin())
				cout << "�������� ���������� �� ��������� �����" << endl;
			break;
		}

		case 39:
		{
			if (!(oIt->toend()))
				cout << "�������� ���������� �� �������� �����" << endl;
			break;
		}

		case 40:
		{
			if (!(++(*oIt)))
				cout << "�������� �� ����������" << endl;
			break;
		}

		case 41:
		{
			try
			{
				cout << oIt->ReadEdge() << endl;
			}
			catch (const std::exception&)
			{
				cout << "�������� �� ����������" << endl;
			}

			break;
		}

		case 42:
		{
			try
			{
				cout << oIt->ReadDataEdge() << endl;
			}
			catch (const std::exception&)
			{
				cout << "��� ������" << endl;
			}

			break;
		}

		case 43:
		{
			cout << "������� ������: ";
			cin >> tmp;
			cout << oIt->WriteDataEdge(tmp) << endl;
			break;
		}

		case 44:
		{
			cout << "������� ���: ";
			cin >> tmp;
			cout << oIt->WriteWeightEdge(tmp) << endl;
			break;
		}
		case 45:
		{
			if (graph->Directed()) {

				task1* tsk1 = new Task1<TVertex, TEdge>(graph);
				tsk1->Result<TVertex, TEdge>();
			}
			else
				cout << "���� ������ ���� ���������������" << endl;

			break;
		}
		case 46:
		{
			cout << "������� ����� �������: " << endl;
			cin >> tmp;
			task2* tsk2 = new Task2<TVertex, TEdge>(graph);
			tsk2->Result(tmp);

			break;
		}
		case 47:
		{
			return 0;
		}
		default:
			cout << "����������� �������" << endl;
			break;
		}
	}
	return 0;
}