#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <stack>

using std::map;
using std::pair;
using std::string;
using std::cout;
using std::cin;
using std::endl;

struct Node
//структура хранит метку вершины и map соседних вершин и величину потока через ребро
{
    bool markFlag;              //Активна ли метка
    pair<int, char> mark;       //Какой поток пришел и откуда
    map<char, pair<int, int>> neighbors;  //мапа вида вершина - {поток туда / поток обратно}
    Node() : markFlag(false) {}
    pair<int,int>& operator[](const char elem)
    {
        return neighbors[elem];
    }
};

class Graph
{
private:
    map<char, Node> point;
    char start, end;
public:
    void init();
    void print_graph();
    void print_for_stepik();
    int searchMaxFlow();
    char max_neighbors_flow(map<char, pair<int, int>>, string);
};

void Graph::print_for_stepik()
{
    for (auto var : point)
    {
        for (auto var2 : var.second.neighbors)
            cout << var.first << " " << var2.first << " " << var2.second.second << endl;
    }
}

void Graph::init()
/* Читаем start, end. После заполняем массив зависимостей */
{
    string input;
    int n;
    cin >> n;
    //cout << "Enter start and end point: ";
    cin >> start;
    cin >> end;

    char from, to;
    int flow;
    //cout << "Enter adjacency list:" << endl;
    for (int i=0; i<n; i++)
    {
        cin >> from >> to >> flow;
        point[from].neighbors[to].first = flow;
    }
}

void Graph::print_graph()
{
    for (auto var : point)
    {
        cout << var.first << ": ";
        for (auto var2 : var.second.neighbors)
            cout << var2.first << " " << var2.second.first << "/" << var2.second.second << "; ";
        cout << std::endl;
    }
}

char Graph::max_neighbors_flow(map<char, pair<int, int>> n_mas, string n_list)
// Ищем соседнюю вершину с максимальным возможным потоком
// Возвращает либо вершину, либо '-', если поток везде нуль
{
    char max = n_list[0]; //первый элемент в мапе
    for (auto var : n_list)
    {
        if (n_mas[var].first > n_mas[max].first)
        {
            max = var;
        }
        //cout << "one : " << n_mas[var].first << " two: " << n_mas[max].first << endl;
    }
    return max;
}

int Graph::searchMaxFlow()
{
    char curr = start;
    point[curr].markFlag = true; //метка у начальной вершины всегда активна, чтобы не выйти за пределы
    point[curr].mark.first = 99999;
    string neighrors_list; //контейнер соседей
    int sum = 0, flow;

    while (1)
    {
        for (auto var : point[curr].neighbors)
        //заполняем контейнер соседей
        {
            if (!point[var.first].markFlag && var.second.first != 0)
                neighrors_list.push_back(var.first);
        }
        //cout << neighrors_list << endl;

        if (neighrors_list.empty())
        {
            if (curr == start) 
            {
                return sum; //конец алгоритма
            } else {
                curr = point[curr].mark.second; //флаг оставляем активным, чтобы не заходить больше сюда
                continue;
            }
        }
        char next = max_neighbors_flow(point[curr].neighbors, neighrors_list);

        point[next].mark = {std::min(point[curr][next].first, point[curr].mark.first), curr};
        point[next].markFlag = true;
        //cout << "next: " << next << "; mark[" << point[next].mark.first << "/" << point[next].mark.second << "]" << endl;
        curr = next;

        if (curr == end)
        {
            cout << "We have reached the final peak! Through way:";
            std::stack<char> out; //стек для промежуточного вывода
            sum += point[curr].mark.first;
            flow = point[curr].mark.first;
            while (curr != start)
            {
                out.push(curr);
                next = curr;
                point[curr].markFlag = false;
                curr = point[curr].mark.second;
                point[curr][next].first -= flow;
                point[curr][next].second += flow;
            }
            out.push(start);

            while (!out.empty())
            {
                cout << " " << out.top();
                out.pop();
            }
            cout << ". Current flow: " << flow << endl;
        }
        neighrors_list.clear();
    }
}

int main()
{
    Graph one;
    one.init();
    //one.print_graph();
    cout << one.searchMaxFlow() << endl;
    one.print_for_stepik();
    //one.print_graph();

    return 0;
}