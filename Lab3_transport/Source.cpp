#include <conio.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;
ifstream in("in.txt");
int a, b;
const int putin = 2147483637;

// матрица состоит из элементов, которые хранят значения цены, текущего количества и текущего потенциала
struct element
{
	int price;
	int val;
	int extra;

	element()
	{
		price = 0;
		val = 0;
		extra = 0;
	}
};

// минимальный элемент из двух - вспомогательная функция
int min(int q, int w)
{
	return (q < w ? q : w);
}

// максимальный элемент из двух - вспомогательная функция
int max(int q, int w)
{
	return (q > w ? q : w);
}

// вывод матрицы (1 - вывод тарифов; 2 - вывод values)
void vivod(vector<vector<element>>& M, int mode) 
{
	switch (mode)
	{
	case 1:
		cout << "Prices" << endl;
		for (int i = 0; i < a; i++) 
		{
			for (int j = 0; j < b; j++)
				cout << M[i][j].price << " ";
			cout << endl;
		}
		cout << endl << endl;
		break;
	case 2:
		for (int i = 0; i < a; i++)
		{
			for (int j = 0; j < b; j++)
				cout << "\t" << M[i][j].val << " ";
			cout << endl;
		}
		break;
	}
	return;
}

// запись матрицы
void vvod(vector<vector<element>>& M, vector<int>& A_prov, vector<int>& B_cons)
{
	for (int i = 0; i < a; i++) {
		for (int j = 0; j < b; j++)
			in >> M[i][j].price;
	}
	for (int i = 0; i < a; i++)
		in >> A_prov[i];
	for (int i = 0; i < b; i++)
		in >> B_cons[i];
	return;
}

// суммирует элементы line-й строки матрицы, кроме column-го элемента
int sum_line(vector <vector<element>> M, int line, int column)
{
	int sum = 0;
	for (int i = 0; i < b; i++)
	{
		sum += i == column ? 0 : M[line][i].val;
	}
	return sum;
}

// суммирует элементы column-го столбца матрицы, кроме line-го элемента
int sum_column(vector <vector<element>> M, int line, int column)
{
	int sum = 0;
	for (int i = 0; i < a; i++)
	{
		sum += i == line ? 0 : M[i][column].val;
	}
	return sum;
}

// заполняю values
void fill_values(vector <vector<element>> &M, vector<int> A_prov, vector<int> B_cons)
{
	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < b; j++)
		{
			M[i][j].val = min(B_cons[j] - sum_column(M, i, j), A_prov[i] - sum_line(M, i, j));
		}
	}

	return;
}

// поиск координат максимальной цены по индексам непустых значений values
void find_max_from_not_empty(int &k, int &l, vector<vector<element>> M) 
{
	int max = 0;
	for (int i = 0; i < a; i++) {
		for (int j = 0; j < b; j++) {
			if (M[i][j].val != 0) {
				if (M[i][j].price > max) {
					max = M[i][j].price;
					k = i; l = j;
				}
			}
		}
	}
	return;
}

// Заполнение векторов потенциалов
void fill_UV(vector<vector<element>> M, vector<int> &V, vector<int> &U, int k, int l) {
	V[k] = M[k][l].price / 2;
	U[l] = M[k][l].price - V[k];
	bool proverka = true, proverka2 = true, t = false;
	while (proverka) {
		proverka = false;
		t = false;
		for (int i = 0; i < a; i++) {
			for (int j = 0; j < b; j++) {
				if (M[i][j].val != 0) {
					if (V[i] == putin && U[j] == putin) {
						proverka = 1;
						cout << "U NAS NEROZHDENNIY!!!" << endl;
						if (proverka2 == false) {
							V[i] = M[i][j].price / 2;
							U[j] = M[i][j].price - V[i];
							proverka2 = 1;

						}
					}
					if (V[i] != putin) {
						if (U[j] == putin) {
							U[j] = M[i][j].price - V[i];
							t = 1;
						}
					}
					if (U[j] != putin) {
						if (V[i] == putin) {
							V[i] = M[i][j].price - U[j];
							t = 1;
						}
					}

				}
			}
		}
		if (t == 0)
			proverka2 = 0;
	}
	return;
}

// заполняем таблицу потенциалов
void fill_extra(vector<vector<element>> &M, vector<int> &V, vector<int> &U) {
	for (int i = 0; i < a; i++) {
		for (int j = 0; j < b; j++) {
			if (M[i][j].val == 0)
				M[i][j].extra = V[i] + U[j];
		}
	}
	return;
}

// найден ли оптимальный план?
bool is_perfect(vector<vector<element>> M)
{
	//for (int i = 0; i < a; i++) {
	//	for (int j = 0; j < b; j++)
	//	{
	//		if (M[i][j].extra > M[i][j].price)
	//		{
	//			cout << M[i][j].extra << " " << M[i][j].price;
	//			return false;
	//		}
	//	}
	//}
	//return true;
	int maxVal = 0;
	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < b; j++)
		{
			if (M[i][j].val == 0)
			{
				if (M[i][j].extra - M[i][j].price > maxVal)
				{
					maxVal = M[i][j].extra - M[i][j].price;
				}
			}
		}
	}
	return maxVal <= 0;

}

// подсчёт текущей стоимости перевозок
int count(vector<vector<element>> M) {
	int full = 0;
	for (int i = 0; i < a; i++) {
		for (int j = 0; j < b; j++) {
			full += M[i][j].price * M[i][j].val;
		}
	}
	return full;
}

// совершаем перестановки values в таблице.
void perfect_try(vector<vector<element>> &M)
{
	int profit = 0, change_value;
	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < b; j++) // для каждого элемента
		{
			if (M[i][j].val > 0) // если элемент больше нуля
			{
				for (int k = 0; k < a; k++) // проходим по столбцу и пытаемся поменять с каждым
				{
					if (k != i) // элемент с самим собой не меняем
					{
						for (int l = 0; l < b; l++) // в выбранной строке идём по элементам и пытаемся поменять
						{
							if (l != j) // опять не меняем с самим собой
							{
								change_value = min(M[i][j].val, M[k][l].val);
								profit = M[i][j].price - M[k][j].price + M[k][l].price - M[i][l].price;
								if ((profit > 0) && (change_value > 0))
								{
									M[i][j].val -= change_value;
									M[k][j].val += change_value;
									M[k][l].val -= change_value;
									M[i][l].val += change_value;
									cout << "Move " << change_value << " from [" << i << "][" << j << "] to [" << k << "][" << j << "] and from [" << k << "][" << l << "] to [" << i << "][" << l << "]" << endl;
								}
							}
						}
					}
				}
			}
		}
	}
}

// очистить все потенциалы перед следующей итерацией
void clear_UVE(vector<vector<element>> &M, vector<int> &V, vector<int> &U)
{
	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < b; j++)
		{
			M[i][j].extra = 0;
		}
		V[i] = putin;
	}
	for (int j = 0; j < b; j++)
	{
		U[j] = putin;
	}
}

struct point
{
	int a;
	int b;

	point(int q, int w)
	{
		a = q;
		b = w;
	}
};

point potentialPoint(vector<vector<element>> M)
{
	point potential_point(0, 0);
	int maxVal = 0;
	for (int i = 0; i < a; i++)
	{
		for (int j = 0; j < b; j++)
		{
			if (M[i][j].val == 0)
			{
				if (M[i][j].extra - M[i][j].price > maxVal)
				{
					maxVal = M[i][j].extra - M[i][j].price;
					potential_point.a = i;
					potential_point.b = j;
				}
			}
		}
	}
	return potential_point;
}

bool look_vertically(vector<vector<element>>&, int, int, point, vector<point>&);
bool look_horisontally(vector<vector<element>> &M, int line, int column, point startPoint, vector<point>& path)
{	
	for (int i = 0; i < b; i++)
	{
		if ((i != column) && (M[line][i].val > 0))
		{
			if (look_vertically(M, line, i, startPoint, path))
			{
				point p(line, i);
				path.push_back(p);
				//cout << line << " " << i << endl;
				return true;
			}
		}
	}

	return false;
}
bool look_vertically(vector<vector<element>> &M, int line, int column, point startPoint, vector<point>& path)
{
	for (int i = 0; i < a; i++)
	{
		if ((i == startPoint.a) && (column == startPoint.b))
		{
			point p(i, column);
			path.push_back(p);
			return true;
		}
		if ((i != line) && (M[i][column].val > 0))
		{
			if (look_horisontally(M, i, column, startPoint, path))
			{
				point p(i, column);
				path.push_back(p);
				//cout << i << " " << column << endl;
				return true;
			}
		}
	}
	return false;
}

int minimal_value(vector<vector<element>> M, vector<point> path)
{
	int minVal = M[path[1].a][path[1].b].val;
	for (int i = 3; i < path.size(); i += 2)
	{
		minVal = min(minVal, M[path[i].a][path[i].b].val);
	}
	return minVal;
}

void perfect_try2(vector<vector<element>> &M, point start, vector<point>& path)
{
	if (!look_horisontally(M, start.a, start.b, start, path))
	{
		cout << "some problem?" << endl;
	}
	else
	{
		int minVal = minimal_value(M, path);
		for (int i = 0; i < path.size(); i += 2)
		{
			M[path[i].a][path[i].b].val += minVal;
			M[path[i + 1].a][path[i + 1].b].val -= minVal;
		}
	}

	return;
}

int main()
{
	in >> a >> b;
	vector <vector<element>> M(a, vector<element> (b, element()));
	vector <int> req_consumer(b); // для А
	vector <int> req_provider(a); // для B
	vvod(M, req_provider, req_consumer); // ввели исходные данные
	fill_values(M, req_provider, req_consumer); // составили опорный план
	int k, l; // координаты самого большого тарифа
	vector <int> U(b, putin);
	vector <int> V(a, putin);

	vivod(M, 1);
	cout << "First plan:" << endl;
	vivod(M, 2);

	point first_point(6, 0);
	vector <point> path;
	int iter = 1;
	do {
		cout << "Iteration " << iter << endl;
		clear_UVE(M, V, U);
		path.clear();
		find_max_from_not_empty(k, l, M);
		fill_UV(M, V, U, k, l);
		fill_extra(M, V, U);
		first_point = potentialPoint(M);
		perfect_try2(M, first_point, path);
		vivod(M, 2);
		cout << count(M) << endl;
		iter++;
	} while (!is_perfect(M) && (iter < 10));
	cout << "Iteration " << iter << endl;
	vivod(M, 2);
	cout << "Best total: " << count(M) << endl;


	_getch();

	return 0;
}

