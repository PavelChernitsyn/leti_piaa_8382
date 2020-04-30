#include <iostream>
#include <vector>
#include <ctime>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;

struct Square
{
    int x;
    int y;
    int size;
};

vector <Square> minArrOfSqr;
int N;
int minNumOfSqr;

void insert(vector<vector<int>>& sqr, int x, int y, int size);

int cast_strToInt(string str)
/* -1 - код ошибки */
{
    int x;
    try
	{
		x = std::stoi(str);
	}
	catch (std::invalid_argument const &e)
	{
	    std::cout << "Bad input: std::invalid_argument thrown" << '\n';
        return -1;
	}
	catch (std::out_of_range const &e)
	{
	    std::cout << "Integer overflow: std::out_of_range thrown" << '\n';
        return -1;
	}
    return x;
}

void init(vector<vector<int>>& sqr)
{
    sqr.resize(N);
    for (int i = 0; i < N; i++)
    {
        sqr[i].resize(N);
        for (int j = 0; j < N; j++)
        {
            sqr[i][j] = 0;
        }
    }
}

void myInput(int &k, vector<vector<int>>& sqr_answ, vector<vector<int>>& sqr, 
                vector<Square>& arrOfSqr, int& S)
{
    /* Читаем N_str, кастим к int и не завершаем цикл, пока не будет считано
     * корректное число. Попутно выкидываем exception'ы в консоль, чтобы пользователь
     * знал, где косячит */
    string tmp_str;
    do
    {
        cout << "Enter a number from 2 to 40: ";
        cin >> tmp_str;
        N = cast_strToInt(tmp_str);
    } while (N < 2 || N > 40);

    init(sqr_answ);
    init(sqr);

    if (N % 2 == 0) 
    {
        k = N / 2;
        N = 2;
    }
    else if (N % 3 == 0) 
    {
        k = N / 3;
        N = 3;
    }
    else if (N % 5 == 0) 
    {
        k = N / 5;
        N = 5;
    }

    minNumOfSqr = 2 * N + 1;

    arrOfSqr.push_back({ 0, 0, (N + 1) / 2 });
    arrOfSqr.push_back({ 0, (N + 1) / 2, N / 2 });
    arrOfSqr.push_back({ (N + 1) / 2, 0, N / 2 });

    insert(sqr, 0, 0, (N + 1) / 2);
    insert(sqr, 0, (N + 1) / 2, N / 2);
    insert(sqr, (N + 1) / 2, 0, N / 2);
    S = N * N - ((N + 1) / 2) * ((N + 1) / 2) - 2 * (N / 2) * (N / 2);
}

void print_sqr(vector <vector <int>>& sqr, int k) 
{
    for (int i = 0; i < N * k; i++)
    {
        for (int j = 0; j < N * k; j++)
        {
            cout.width(3);
            cout << sqr[i][j];
        }
        cout << "\n";
    }
}
int can_insert(vector<vector<int>>& sqr, int x, int y, int size) 
{
    if ((x + size) > sqr.size() || (y + size) > sqr.size()) return 0;
    for (int i = y; i < y + size; i++)
    {
        for (int j = x; j < x + size; j++)
        {
            if (sqr[i][j] != 0) return 0;
        }
    }
    return 1;
}

void insert(vector<vector<int>>& sqr, int x, int y, int size) 
{
    for (int i = y; i < y + size; i++)
    {
        for (int j = x; j < x + size; j++)
        {
            sqr[i][j] = size;
        }
    }

}
void remove_last(vector<vector<int>>& sqr, vector<Square>& sqrik_mas) 
{
    Square tmp = *(sqrik_mas.rbegin());
    sqrik_mas.pop_back();

    for (int i = tmp.y; i < tmp.y + tmp.size; i++)
    {
        for (int j = tmp.x; j < tmp.x + tmp.size; j++)
        {
            sqr[i][j] = 0;
        }
    }
}


void rec_fill(vector<vector<int>>& sqr, int curSpace, int curSize, int countSqr, vector<Square>& arrOfSqr) 
{
    if (countSqr == minNumOfSqr - 1 && curSpace > curSize* curSize) return;

    bool flag = false;
    for (int y = 0; y < N && !flag; y++)
    {
        for (int x = 0; x < N && !flag; x++)
        {
            if (sqr[y][x] == 0) 
            {
                if (can_insert(sqr, x, y, curSize)) 
                {
                    /* Нашли, куда можно вставить квадрат и вставили */
                    insert(sqr, x, y, curSize);
                    flag = true;
                    curSpace -= curSize * curSize;
                    arrOfSqr.push_back({ x, y, curSize });
                }
                else 
                {
                    return;
                }
            }
            else 
            {
                x += sqr[y][x] - 1;
            }
        }
    }
    if (countSqr + 1 == minNumOfSqr) 
    {
        /*Уже не лучший вариант расположения, так что назад */
        remove_last(sqr, arrOfSqr);
        return;
    }
    if (countSqr + 1 < minNumOfSqr && curSpace == 0) 
    {
        minNumOfSqr = countSqr + 1;
        minArrOfSqr.assign(arrOfSqr.begin(), arrOfSqr.end());
        remove_last(sqr, arrOfSqr);
        return;
    }
    for (int i = (N + 1) / 2; i > 0; i--)
    {
        if (i * i <= curSpace)
            rec_fill(sqr, curSpace, i, countSqr + 1, arrOfSqr);
    }
    remove_last(sqr, arrOfSqr);
}

int main()
{
    int k = 1;
    vector<vector<int>> sqr_answ;
    vector<vector<int>> sqr;
    vector<Square> arrOfSqr;
    int S;

    /* myInput читает N и заполняет поля, от него зависящие.
       Также вставляются три первых квадрата */
    myInput(k, sqr_answ, sqr, arrOfSqr, S);

    time_t start = clock();
    for (int i = (N + 1) / 2; i > 0; i--) {
        rec_fill(sqr, S, i, arrOfSqr.size(), arrOfSqr);
    }

    time_t end = clock();
    cout << "RunTime: " << (end - start) / 1000 << " seconds\n";
    cout << minNumOfSqr << "\n";

    for (int i = 0; i < minArrOfSqr.size(); i++)
    {
        cout << minArrOfSqr[i].x * k + 1 << " " << 
            minArrOfSqr[i].y * k + 1 << " " << minArrOfSqr[i].size * k << endl;
        insert(sqr_answ, minArrOfSqr[i].x * k, minArrOfSqr[i].y * k, minArrOfSqr[i].size * k);
    }

    print_sqr(sqr_answ, k);
    return 0;
}