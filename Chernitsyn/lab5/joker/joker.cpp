#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <queue>
#include <unordered_map>

bool d_flag = false; //флаг вывода промежуточных данных

using namespace std;

class Data
//здесь храним считанные данные
{
private:
    std::string text;       //текст
    std::string pattern;    //паттерн
    char joker;             //joke symbol
public:
    Data() = default;
    void init()
    {
        getline(std::cin, text);
        std::cin >> pattern;
        std::cin >> joker;
    }
    void printText()
    {
        std::cout << "Text = {" << text << "};" << std::endl;
        std::cout << "Pattern = {" << pattern << "};" << std::endl;
        std::cout << "Joker is '" << joker << "';" << std::endl;
    }
    std::string getText() { return text; }
    std::string getPattern() { return pattern; }
    char getJoker() { return joker; }
};

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
    void insert(const string &str, size_t pos, size_t size) {
        auto curr = this;

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

        curr->substringEntries.emplace_back(pos, size);
    }

    vector <pair<size_t, size_t>> find(const char c)
    {
        static const TreeNode *curr = this; // Вершина, с которой необходимо начать следующий вызов
        if (d_flag) cout << "Ищем '" << c << "' из: " << (curr->dbgStr.empty() ? "Корень" : curr->dbgStr) << endl; // Дебаг

        for (; curr != nullptr; curr = curr->suffixLink) {
            // Обходим потомков, если искомый символ среди потомков не найден, то
            // переходим по суффиксной ссылке для дальнейшего поиска
            for (auto child : curr->children)
                if (child.first == c) { // Если символ потомка равен искомому
                    curr = child.second; // Значение текущей вершины переносим на этого потомка
                    // вектор пар, состоящих из начала безмасочной подстроки в маске и её длины
                    vector <pair<size_t, size_t>> found;

                    // Обходим суффиксы, т.к. они тоже могут быть терминальными вершинами
                    for (auto temp = curr; temp->suffixLink; temp = temp->suffixLink)
                        for (auto el : temp->substringEntries)
                            found.push_back(el);

                    if (d_flag) cout << "Символ '" << c << "' найден!" << endl; // Дебаг
                    return found;
                }

            // Дебаг
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
            if (d_flag && !curr->children.empty())
                cout << endl;

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
        if (d_flag) {
        cout << endl;
        printTrie();
        }
    }

    ~TreeNode()
    {
        for (auto child : children)
            delete child.second;
    }

private:
    string dbgStr = "";                             // Для отладки
    char value;                                     // Значение ноды
    TreeNode *parent = nullptr;                     // Родитель ноды
    TreeNode *suffixLink = nullptr;                 // Суффиксная ссылка
    unordered_map <char, TreeNode*> children;       // Потомок ноды
    vector <pair<size_t, size_t>> substringEntries;
};

class Trie {
public:
    Trie() : root('\0') {}
    void insert(const string &str, size_t pos, size_t size) { root.insert(str, pos, size); }
    vector <pair<size_t, size_t>> find(const char c) { return root.find(c); }
    void makeAutomaton() { root.makeAutomaton(); }
private:
    TreeNode root;
};

vector<size_t> AhoCorasick(const string &text, const string &mask, char joker) {
    Trie bor;
    vector <size_t> result;
    vector <size_t> midArr(text.size()); // Массив для хранения кол-ва попаданий безмасочных подстрок в текст
    string pattern;
    size_t numSubstrs = 0; // Количество безмасочных подстрок

    for (size_t i = 0; i <= mask.size(); i++) { // Заполняем бор безмасочными подстроками маски
        char c = (i == mask.size()) ? joker : mask[i];
        if (c != joker)
            pattern += c;
        else if (!pattern.empty()) {
            numSubstrs++;
            bor.insert(pattern, i - pattern.size(), pattern.size());
            pattern.clear();
        }
    }
    if (d_flag) cout << "Бор заполнен!" << endl;

    bor.makeAutomaton();

    for (size_t j = 0; j < text.size(); j++)
        for (auto pos : bor.find(text[j])) {
            if (d_flag)
            {
                string str;
                str = text.substr(pos.first, pos.second);
                cout << "Найден фрагмент - " << str << endl;
            }
            // На найденной терминальной вершине вычисляем индекс начала маски в тексте
            int i = int(j) - int(pos.first) - int(pos.second) + 1;
            if (i >= 0 && i + mask.size() <= text.size())
            {
                midArr[i]++; // Увеличиваем её значение на 1
                if (d_flag)
                {
                    cout << "Количество безмасочных подстрок = " << numSubstrs << "; Найдено " << midArr[i] << endl;
                }
            }
        }

    for (size_t i = 0; i < midArr.size(); i++) {
        // Индекс, по которым промежуточный массив хранит количество
        // попаданий безмасочных подстрок в текст, есть индекс начала вхождения маски
        // в текст, при условии, что кол-во попаданий равно кол-ву подстрок б/м
        if (midArr[i] == numSubstrs) {
            result.push_back(i + 1);

            if (d_flag)
                {
                    cout << "Количество обнаруженных безмасочных подстрок на " << i << " позиции равно " 
                    << numSubstrs << ". Значит, шаблон найден!" << endl;
                }

            // ИНДИВИДУАЛИЗАЦИЯ
            // для пропуска пересечений, после найденного индекса, увеличиваем его на длину маски
            i += mask.size() - 1;
        }
    }

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

    for (auto ans : AhoCorasick(D.getText(), D.getPattern(), D.getJoker()))
        cout << ans << endl;

    system("pause");

    return 0;
}