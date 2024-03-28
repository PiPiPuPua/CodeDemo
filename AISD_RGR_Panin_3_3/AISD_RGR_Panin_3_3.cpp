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
	cout << "Меню:" << endl;
	cout << "_________Создание графов_________" << endl;
	cout << "[0] Создать пустой L-граф" << endl;
	cout << "[1] Создать граф с V вершинами" << endl;
	cout << "[2] Создать граф с V вершинами, с E случайными ребрами" << endl << endl;

	cout << "_________Операции над графом_________" << endl;
	cout << "[3] Распечатать граф" << endl;
	cout << "[4] Число вершин в графе" << endl;
	cout << "[5] Число ребер в графе" << endl;
	cout << "[6] Опрос типа графа" << endl;
	cout << "[7] Опрос формы представления графа" << endl;
	cout << "[8] Коэффициент насыщенности графа" << endl;
	cout << "[9] Преобразовать граф к L-форме" << endl;
	cout << "[10] Преобразовать граф к M-форме" << endl;
	cout << "[11] Добавить вершину к графу" << endl;
	cout << "[12] Удалить вершину из графа" << endl;
	cout << "[13] Вставить ребро в граф" << endl;
	cout << "[14] Удалить ребро из графа" << endl;
	cout << "[15] Проверка на существование ребра в графе" << endl << endl;

	cout << "_________Дескриптор вершин_________" << endl;
	cout << "[16] Прочитать данные о вершине" << endl;
	cout << "[17] Записать данные о вершине" << endl;
	cout << "[18] Перезаписать имя вершины" << endl << endl;

	cout << "_________Дескриптор рёбер_________" << endl;
	cout << "[19] Прочитать вес ребра" << endl;
	cout << "[20] Прочитать данные о ребре" << endl;
	cout << "[21] Записать данные о ребре" << endl;
	cout << "[22] Записать вес ребра" << endl << endl;

	cout << "_________Итератор вершин_________" << endl;
	cout << "[23] Установить итератор вершин на начальную вершину" << endl;
	cout << "[24] Установить итератор вершин на конечную вершину" << endl;
	cout << "[25] Переход к следующей позиции" << endl;
	cout << "[26] Прочитать вершину по текущей позиции итератора" << endl;
	cout << "[27] Прочитать данные вершины по текущей позиции итератора" << endl;
	cout << "[28] Записать данные вершины по текущей позиции итератора" << endl;
	cout << "[29] Записать имя вершины по текущей позиции итератора" << endl << endl;

	cout << "_________Итератор ребер_________" << endl;
	cout << "[30] Установить итератор ребер на начальное ребро" << endl;
	cout << "[31] Установить итератор ребер на конечное ребро" << endl;
	cout << "[32] Переход к следующей позиции" << endl;
	cout << "[33] Прочитать вершину по текущей позиции итератора" << endl;
	cout << "[34] Прочитать данные ребра по текущей позиции итератора" << endl;
	cout << "[35] Записать данные о ребре по текущей позиции итератора" << endl;
	cout << "[36] Записать вес ребра по текущей позиции итератора" << endl << endl;

	cout << "_________Итератор исходящих ребер_________" << endl;
	cout << "[37] Создать итератор исх. ребер" << endl;
	cout << "[38] Установить итератор исходящих ребер на начальное ребро" << endl;
	cout << "[39] Установить итератор исходящих ребер на конечное ребро" << endl;
	cout << "[40] Переход к следующей позиции" << endl;
	cout << "[41] Прочитать вершины ребра и вес по текущей позиции итератора" << endl;
	cout << "[42] Прочитать данные ребра по текущей позиции итератора" << endl;
	cout << "[43] Записать данные о ребре по текущей позиции итератора" << endl;
	cout << "[44] Записать вес ребра по текущей позиции итератора" << endl << endl;

	cout << "[45] ЗАДАЧА 2" << endl;
	cout << "[46] ЗАДАЧА 3" << endl << endl;

	cout << "[47] Выход из программы" << endl << endl;
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
		cout << "Введите команду: ";
		int command;
		cin >> command;

		if (graph == NULL && command > 2 && command < 47)
		{
			cout << "Граф не создан" << endl;
			continue;
		}

		if (oIt == NULL && command > 37 && command < 45)
		{
			cout << "Итератор не создан" << endl;
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
			cout << endl << "Граф создан" << endl;
			break;
		}
		case 1:
		{
			cout << "Введите количество вершин: ";
			cin >> tmp1;
			cout << "Ориентированный ли граф (0 - нет, 1 - да): ";
			cin >> tmp2;
			if (tmp2 == 1)
				on = true;
			cout << "Форму графа (0 - список, 1 - матрица): ";
			cin >> tmp3;
			clear(graph, vIt, eIt, oIt);
			graph = new TGraph(tmp1, tmp2, tmp3);
			vIt = new TGraph::VertexIterator(*graph);
			eIt = new TGraph::EdgeIterator(*graph);
			cout << endl << "Граф создан" << endl;
			break;
		}
		case 2:
		{
			cout << "Введите количество вершин: ";
			cin >> tmp1;
			cout << "Введите количество ребер: ";
			cin >> tmp2;
			cout << "Ориентированный ли граф (0 - нет, 1 - да): ";
			cin >> tmp3;
			if (tmp3 == 1)
				on = true;
			cout << "Форма графа (0 - L, 1 - M): ";
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
				cout << "Исключение" << endl;
			}
			cout << endl << "Граф создан" << endl;
			break;
		}

		case 3:
			graph->print();
			break;

		case 4:
			try
			{
				cout << "Число вершин в графе: " << graph->VertexSize() << endl;
			}
			catch (const std::exception&)
			{
				cout << "Исключение" << endl;
			}
			break;

		case 5:
			cout << "Число ребер в графе: " << graph->EdgeSize() << endl;
			break;

		case 6:
			cout << "0) Неориентированный\n1) Ориентированный\nТип графа: " << graph->Directed() << endl;
			break;

		case 7:
			cout << "0) - L-граф\n1) - M-граф\n Форма предстваления графа:" << graph->CheckDense() << endl;
			break;

		case 8:
			cout << "Коэффициент насыщенности: " << graph->K() << endl;
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
					cout << "Вершина добавлена в граф" << endl;
				else
					cout << "Вершина не добавлена в граф" << endl;
			}
			catch (const std::exception&)
			{
				cout << "Исключение" << endl;
			}
			break;
		}
		case 12:
		{
			cout << "Введите удаляемую вершину: ";
			cin >> v1;
			cout << graph->DeleteVertex(v1) << endl;
			break;
		}

		case 13:
		{
			cout << "Введите вершину, из которой выходит ребро: ";
			cin >> v1;
			cout << "Введите вершину, в которую входит ребро: ";
			cin >> v2;
			cout << graph->InsertEdge(v1, v2) << endl;
			break;
		}

		case 14:
		{
			cout << "Введите вершину 1: ";
			cin >> v1;
			cout << "Введите вершину 2: ";
			cin >> v2;
			cout << graph->DeleteEdge(v1, v2) << endl;
			break;
		}
		case 15:
		{
			cout << "Введите вершину 1: ";
			cin >> tmp1;
			cout << "Введите вершину 2: ";
			cin >> tmp2;
			cout << graph->hasEdge(tmp1, tmp2) << endl;
			break;
		}
		case 16:
		{
			cout << "Введите имя вершины: ";
			cin >> v;
			try
			{
				tmp = graph->ReadDataVertex(v);
				if (tmp != -1) {
					cout << endl << "Данные вершины: " << tmp << endl;
				}
				else {
					cout << "Нет данных." << endl;
				}

			}
			catch (const std::exception&)
			{
				cout << "Нет данных. Команда для записи \"16\"" << endl;
			}

			break;
		}

		case 17:
		{
			cout << "Введите имя вершины: ";
			cin >> v;
			cout << "Введите данные: ";
			cin >> tmp;
			cout << endl << graph->WriteDataVertex(v, tmp) << endl;
			break;
		}
		case 18:
		{
			cout << "Введите старое имя вершины: ";
			cin >> v1;
			cout << "Введите новое имя: ";
			cin >> v2;
			cout << graph->WriteNameVertex(v1, v2) << endl;
			break;
		}
		case 19:
		{
			cout << "Введите имя исходящей вершины: ";
			cin >> v1;
			cout << "Введите имя входящей вершины: ";
			cin >> v2;
			tmp = graph->ReadWeightEdge(v1, v2);
			if (tmp != -1)
				cout << "Вес ребра: " << tmp << endl;
			break;
		}

		case 20:
		{
			cout << "Введите имя исходящей вершины: ";
			cin >> v1;
			cout << "Введите имя входящей вершины: ";
			cin >> v2;
			try
			{
				tmp = graph->ReadDataEdge(v1, v2);
				if (tmp != -1)
					cout << "Данные ребра: " << tmp << endl;
			}
			catch (const std::exception&)
			{
				cout << "Данные ребра не установлены" << endl;
			}

			break;
		}
		case 21:
		{
			cout << "Введите имя исходящей вершины: ";
			cin >> v1;
			cout << "Введите имя входящей вершины: ";
			cin >> v2;
			cout << "Введите данные: ";
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
			cout << "Введите имя исходящей вершины: ";
			cin >> v1;
			cout << "Введите имя входящей вершины: ";
			cin >> v2;
			cout << "Введите вес: ";
			cin >> tmp;
			cout << graph->WriteWeightEdge(v1, v2, tmp) << endl;
			break;
		}
		case 23:
		{
			if (!(vIt->begin()))
				cout << "Итератор не установлен" << endl;
			break;
		}
		case 24:
		{
			if (!(vIt->toend()))
				cout << "Итератор не установлен" << endl;
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
				cout << "Итератор не установлен" << endl;
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
				cout << "Итератор не установлен" << endl;
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
				cout << "Итератор не установлен" << endl;
			}
			break;
		}

		case 28:
		{
			cout << "Введите данные: ";
			cin >> tmp;
			cout << vIt->WriteDataVertex(tmp) << endl;
			break;
		}

		case 29:
		{
			cout << "Введите имя: ";
			cin >> cur;
			cout << vIt->WriteNameVertex(cur) << endl;
			break;
		}
		case 30:
		{
			if (eIt->begin())
				cout << "Итератор установлен на начальное ребро" << endl;
			break;
		}

		case 31:
		{
			if (eIt->toend())
				cout << "Итератор установлен на конечное ребро" << endl;
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
				cout << "Итератор не установлен" << endl;
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
				cout << "Итератор не установлен" << endl;
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
				cout << "Нет данных" << endl;
			}
			break;
		}

		case 35:
		{
			cout << "Введите данные: ";
			cin >> tmp;
			cout << eIt->WriteDataEdge(tmp) << endl;
			break;
		}

		case 36:
		{
			cout << "Введите вес: ";
			cin >> tmp;
			cout << eIt->WriteWeightEdge(tmp) << endl;
			break;
		}

		case 37:
		{
			cout << "Введите имя вершины: ";
			cin >> v;
			try
			{
				oIt = new TGraph::OutputEdgeIterator(*graph, *(graph->getVertexFromName(v)));
				cout << "Итератор создан" << endl;
			}
			catch (const std::exception&)
			{
				cout << "Неверная вершина: " << endl;
			}
			break;
		}
		case 38:
		{
			if (oIt->begin())
				cout << "Итератор установлен на начальное ребро" << endl;
			break;
		}

		case 39:
		{
			if (!(oIt->toend()))
				cout << "Итератор установлен на конечное ребро" << endl;
			break;
		}

		case 40:
		{
			if (!(++(*oIt)))
				cout << "Итератор не установлен" << endl;
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
				cout << "Итератор не установлен" << endl;
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
				cout << "Нет данных" << endl;
			}

			break;
		}

		case 43:
		{
			cout << "Введите данные: ";
			cin >> tmp;
			cout << oIt->WriteDataEdge(tmp) << endl;
			break;
		}

		case 44:
		{
			cout << "Введите вес: ";
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
				cout << "Граф должен быть ориентированным" << endl;

			break;
		}
		case 46:
		{
			cout << "Введите номер вершины: " << endl;
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
			cout << "Неизвестная команда" << endl;
			break;
		}
	}
	return 0;
}