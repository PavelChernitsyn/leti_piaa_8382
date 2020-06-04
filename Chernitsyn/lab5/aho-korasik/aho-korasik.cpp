#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <set>
#include <queue>
#include <unordered_map>

bool d_flag = false; //флаг вывода промежуточных данных

class Data
//здесь храним считанные данные
{
private:
    std::string text;                   //текст
    int n;                              //кол-во паттернов
    std::vector<std::string> patterns;  //массив паттернов
public:
    Data() = default;
    void init()
    {
        getline(std::cin, text);
        std::cin >> n;
        patterns.resize(n);
        for(int i = 0; i < n; i++)
            std::cin >> patterns[i];
    }
    void printText()
    {
        std::cout << "Text = {" << text << "};" << std::endl;
        std::cout << "N = " << n << "; Patterns = {";
        std::cout << patterns[0];
        for (int i = 1; i < n; i++)
            std::cout << ", " << patterns[i];
        std::cout << "};" << std::endl;
    }
    std::string getText() { return text; }
    std::vector<std::string> getPatterns() { return patterns; }
};

using namespace std;

class TreeNode {
public:
    explicit TreeNode(char val) : value(val) {} // Конструктор ноды

    // Отладочная функция для печати бора
    void printTrie() {
        cout << "Бор сейчас:" << endl;

        queue<TreeNode *> queue;
        queue.push(this);

        while (!queue.empty()) {
            auto curr = queue.front();
            if (!curr->value)
                cout << "Корень:" << endl;
            else
                cout << curr->dbgStr << ':' << endl;

            if (curr->suffixLink)
                cout << "\tСуффиксная ссылка: " << (curr->suffixLink == this ? "Root" : curr->suffixLink->dbgStr) << endl;

            if (curr->parent && curr->parent->value)
                cout << "\tРодитель: " << curr->parent->dbgStr << endl;
            else if (curr->parent)
                cout << "\tРодитель: Корень" << endl;

            if (!curr->children.empty()) cout << "\tПотомок: ";
            for (auto child : curr->children) {
                cout << child.second->value << ' ';
                queue.push(child.second);
            }

            queue.pop();
            cout << endl;
        }
        cout << endl;

    }

    // Вставка подстроки в бор
    void insert(const string &str) {
        auto curr = this;
        static size_t countPatterns = 0;

        for (char c : str) { // Идем по строке
            // Если из текущей вершины по текущему символу не было создано перехода
            if (curr->children.find(c) == curr->children.end()) {
                // Создаем переход
                curr->children[c] = new TreeNode(c);
                curr->children[c]->parent = curr;
                curr->children[c]->dbgStr += curr->dbgStr + c;
            }
            // Спускаемся по дереву
            curr = curr->children[c];
        }

        if (d_flag)
        {
        cout << "Вставляем строку: " << str << endl;
        printTrie();
        }

        // Показатель терминальной вершины, значение которого равно порядковому номеру добавления шаблона
        curr->numOfPattern = ++countPatterns;
    }

    // Функция для поиска подстроки в строке при помощи автомата
    vector<size_t> find(const char c) {
        static const TreeNode *curr = this; // Вершина, с которой необходимо начать следующий вызов
        string str; //для вывода найденного шаблона
        if (d_flag) 
        {
            cout << "Ищем '" << c << "' из: " << (curr->dbgStr.empty() ? "Корень" : curr->dbgStr) << endl; // Дебаг
            str = curr->dbgStr;
        }

        for (; curr != nullptr; curr = curr->suffixLink) {
            // Обходим потомков, если искомый символ среди потомков не найден, то
            // переходим по суффиксной ссылке для дальнейшего поиска
            for (auto child : curr->children)
                if (child.first == c) { // Если символ потомка равен искомому
                    curr = child.second; // Значение текущей вершины переносим на этого потомка
                    vector<size_t> found; // Вектор номеров найденных терм. вершин

                    if (d_flag) cout << "Символ '" << c << "' найден!" << endl; // Дебаг
                    if (curr->numOfPattern) { // Для пропуска пересечений, после нахождения терминальной вершины
                        if (d_flag)
                        {
                            cout << "Найден шаблон: " << str << c << endl;
                        }
                        found.push_back(curr->numOfPattern - 1); // Добавляем к найденным эту вершину
                        curr = this; // И переходим в корень
                    }
                    return found;
                }

            if (d_flag && curr->suffixLink) {
                cout << "Переходим по суффиксной ссылке: ";
                cout << (curr->suffixLink->dbgStr.empty() ? "Корень" : curr->suffixLink->dbgStr) << endl;
            }
        }
        if (d_flag) cout << "Символ '" << c << "' не найден!" << endl; // Дебаг

        curr = this;
        return {};
    }

    // Функция для построения недетерминированного автомата
    void makeAutomaton() {
        if (d_flag) cout << "Строим автомат: " << endl;

        queue<TreeNode *> queue; // Очередь для обхода в ширину

        for (auto child : children) // Заполняем очередь потомками корня
            queue.push(child.second);

        while (!queue.empty()) {
            auto curr = queue.front(); // Обрабатываем верхушку очереди

            // Для дебага
            if (d_flag) {
            cout << curr->dbgStr << ':' << endl;
            if (curr->parent && curr->parent->value)
                cout << "\tРодитель: " << curr->parent->dbgStr << endl;
            else if (curr->parent)
                cout << "\tРодитель: Корень" << endl;

            if (!curr->children.empty())
                cout << "\tПотомок: ";
            }
            //

            // Заполняем очередь потомками текущей верхушки
            for (auto child : curr->children) {
                if (d_flag) cout << child.second->value << ' '; // Дебаг
                queue.push(child.second);
            }

            // Дебаг
            if (d_flag)
            {
            if (!curr->children.empty())
                cout << endl;
            }

            queue.pop();
            auto p = curr->parent; // Ссылка на родителя обрабатываемой вершины
            char x = curr->value; // Значение обрабатываемой вершины
            if (p) p = p->suffixLink; // Если родитель существует, то переходим по суффиксной ссылке

            // Пока можно переходить по суффиксной ссылке или пока
            // не будет найден переход в символ обрабатываемой вершины
            while (p && p->children.find(x) == p->children.end())
                p = p->suffixLink; // Переходим по суффиксной ссылке

            // Суффиксная ссылка для текущей вершины равна корню, если не смогли найти переход
            // в дереве по символу текущей вершины, иначе равна найденной вершине
            curr->suffixLink = p ? p->children[x] : this;
            // Дебаг
            if (d_flag) cout << "\tСуффиксная ссылка: " << (curr->suffixLink == this ? "Корень" : curr->suffixLink->dbgStr) << endl << endl;
        }

        // Дебаг
        if (d_flag)
        {
        cout << endl;
        printTrie();
        }
    }

    ~TreeNode() { // Деструктор ноды
        for (auto child : children) delete child.second;
    }

private:
    string dbgStr = ""; // Для отладки
    char value; // Значение ноды
    size_t numOfPattern = 0; // Номер введенного паттерна
    TreeNode *parent = nullptr; // Родитель ноды
    TreeNode *suffixLink = nullptr; // Суффиксная ссылка
    unordered_map <char, TreeNode*> children; // Потомок ноды
};

class Trie {
public:
    Trie() : root('\0') {} // Конструктор бора

    void insert(const string &str) { root.insert(str); }
    vector<size_t> find(const char c) { return root.find(c); }
    void makeAutomaton() { root.makeAutomaton(); }

private:
    TreeNode root; // Корень бора
};

set <pair<size_t, size_t>> AhoCorasick(const string &text, const vector <string> &patterns)
{
    Trie bor;
    set <pair<size_t, size_t>> result;

    for (const auto &pattern : patterns) // Заполняем бор введенными паттернами
        bor.insert(pattern);

    bor.makeAutomaton(); // Из полученного бора создаем автомат (путем добавления суффиксных ссылок)

    for (size_t j = 0; j < text.size(); j++) // Проходим циклом по строке, для каждого символа строки запускаем поиск
        for (auto pos : bor.find(text[j])) // Проходим по всем найденным позициям, записываем в результат
            result.emplace(j - patterns[pos].size() + 2, pos + 1);

    return result;
}

int main(int argc, char** argv)
{
    //читаем параметры запуска. Если введён -detail или -d, выводятся промежуточные данные
	if (argc == 2 && (!strcmp(argv[1], "-detail\0") || !strcmp(argv[1], "-d\0")))
		d_flag = true;
    //if (d_flag) system("chcp 65001");

    Data D;
    D.init();

    auto res = AhoCorasick(D.getText(), D.getPatterns());
    for (auto r : res)
        cout << r.first << ' ' << r.second << endl;

    //system("pause");

    return 0;
}