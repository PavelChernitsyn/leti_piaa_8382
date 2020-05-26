#include <iostream>
#include <stack>
#include <string>
#include <map>
#include <set>
#include <list>

using std::map;
using std::set;
using std::pair;
using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::list;

typedef struct Triple
{
    // структура, хранящая информацию о ребре и было ли оно пройдено
    int name;
    double weight;
    mutable bool flag;
    Triple() {}
    Triple(int _name, double _weight, bool _flag=false) : name(_name), weight(_weight), flag(_flag) {}
} Triple;

struct SetCompare
{
    bool operator()(Triple v1, Triple v2)
    {
        if (v1.weight == v2.weight)
            return v1.name < v2.name;
        return  v1.weight < v2.weight;
    }
};

class Graph
{
public:
    // point хранит зависимость вершин в виде: вершина - массив смежных вершин
    // Массив смежных вершин отсортирован по возрастанию веса ребра (SetCompare)
    map<int, set<Triple, SetCompare>> point;
    int start, end;
public:
    void init();
    void print_graph();
    std::stack<int> greedySearch();
    std::stack<int> aStar();
    int heuristic(int);
    std::stack<int> reconstruction(map<int, int>);

};

void Graph::init()
/* Читаем start, end. После заполняем массив зависимостей */
{
    string input;
    //cout << "Enter start and end point: ";
    getline(cin, input);
    start = std::stoi(input.substr(0));
    end   = std::stoi(input.substr(2));

    //cout << "Enter adjacency list:" << endl;
    while (getline(cin, input))
    {
        if (input.empty()) break;
        point[std::stoi(input.substr(0))].emplace(std::stoi(input.substr(2)), std::stod(input.substr(4)));
    }
}

void Graph::print_graph()
{
    for (auto var : point)
    {
        cout << var.first << ": ";
        for (auto var2 : var.second)
            cout << var2.name << " " << var2.weight << " " << var2.flag << "; ";
        cout << std::endl;
    }
}

std::stack<int> Graph::greedySearch()
{
    // В стеке храним результат. Сразу записываем первую вершину
    // curr хранит массив смежных вершин к текущей вершине 
    std::stack<int> res;
    res.push(start);

    set<Triple, SetCompare> curr = point[res.top()];

    while (!res.empty() && res.top() != end)
    {
        bool can_go = false;
        int tmp;
        if (!curr.empty())
        {
            for (auto &var : point[res.top()]) //point[res.top()] == curr. Сделано для того, чтобы флаг изменялся
            // Ищем следующую непосещённую вершину
            {
                if (!var.flag)
                {
                    can_go = true;
                    var.flag = true;
                    tmp = var.name;
                    break;
                }
            }
        }

        if (can_go)
        {
            res.push(tmp);
            curr = point[tmp];
        } else {
            res.pop();
            if (!res.empty()) curr = point[res.top()];
        }
    }

    //зануляем флаг, чтобы не портить массив
    for (auto &var: point)
        for (auto &var2: var.second)
            var2.flag = false;

    return res;
}

void expand_stack(std::stack<int>& res)
{
    std::stack<int> tmp;
    tmp.swap(res);
    while (!tmp.empty())
    {
        res.push(tmp.top());
        tmp.pop();
    }
}

void print_stack(std::stack<int> res)
{
    while (!res.empty())
    {
        cout << res.top();
        res.pop();
    }
    cout << endl;
}

int Graph::heuristic(int curr)
{
    return abs(curr - end);
}

int minF(list <int> open, map <int, float> F){//поиск минимального значения f(x)
    int res = open.back();
    float min = F[res];

    for (auto var : open)
    {
        if (F[var] <= min){
            res = var;
            min = F[var];
        }
    }
    return res;
}

bool inList(list<int> _list, int x)
{
    for (auto var : _list)
        if (var == x) return true;
    return false;
}

std::stack<int> Graph::reconstruction(map<int, int> from)
{
    std::stack<int> res;
    int curr = end;
    while (curr != start)
    {
        res.push(curr);
        curr = from[curr];
    }
    res.push(start);
    return res;
}

std::stack<int> Graph::aStar()
{
    std::stack<int> res; //стек результата
    list<int> close; //список пройденных вершин
    list<int> open = {start}; //список рассматриваемых вершин
    map<int, int> from; //карта пути
    map <int, float> G; //хранит стоимости путей от начальной вершины
    map <int, float> F; //оценки f(x) для каждой вершины
    G[start] = 0;
    F[start] = G[start] + heuristic(start);

    while (!open.empty())
    {
        int curr = minF(open, F);

        /* Вывод для ясности */
        cout << "***Info***" << endl;
        cout << "Current: " << curr << endl;
        cout << "Close list: ";
        for (auto var : close)
            cout << var << " ";
        cout << endl << "Open list: ";
        for (auto var : open)
            cout << var << " ";
        cout << endl << "***Info end***" << endl;

        if (curr == end)
        {
            res = reconstruction(from);//востанавливаем
            return res;
        }

        open.remove(curr);
        close.push_back(curr);

        for (auto neighbor : point[curr])
        {
            if (inList(close, neighbor.name)) //если уже проходили, дальшше
                continue;
            
            float tmpG = G[curr] + neighbor.weight; //вычисление g(x) для обрабатываемого соседа

            if (!inList(open, neighbor.name) || tmpG < G[neighbor.name])
            {
                from[neighbor.name] = curr;
                G[neighbor.name] = tmpG;
                F[neighbor.name] = G[neighbor.name] + heuristic(neighbor.name);
            }

            if (!inList(open, neighbor.name))
                open.push_back(neighbor.name);
        }
    }

    return res;
}

int main()
{
    Graph one;
    one.init();
    //one.print_graph();
    
    std::stack<int> res = one.greedySearch();
    expand_stack(res);
    cout << "GreedySearch answer: ";
    print_stack(res);

    res = one.aStar();
    cout << "aStarSearch answer: ";
    print_stack(res);

    return 0;
}