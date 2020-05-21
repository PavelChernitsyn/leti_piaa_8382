#include <iostream>
#include <stack>
#include <string>
#include <map>
#include <set>

using std::map;
using std::set;
using std::pair;
using std::string;
using std::cout;
using std::cin;
using std::endl;

typedef struct Triple
{
    // структура, хранящая информацию о ребре и было ли оно пройдено
    int first;
    double second;
    mutable bool third;
    Triple() {}
    Triple(int name, double weight, bool flag=false) : first(name), second(weight), third(flag) {}
} Triple;

struct SetCompare
{
    bool operator()(Triple v1, Triple v2)
    {
        if (v1.second == v2.second)
            return v1.first < v2.first;
        return  v1.second < v2.second;
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
            cout << var2.first << " " << var2.second << " " << var2.third << "; ";
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
                if (!var.third)
                {
                    can_go = true;
                    var.third = true;
                    tmp = var.first;
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

int main()
{
    Graph one;
    one.init();
    //one.print_graph();
    
    auto res = one.greedySearch();
    string res_str;
    expand_stack(res);

    while (!res.empty())
    {
        cout << res.top();
        res.pop();
    }
    cout << endl;

    return 0;
}