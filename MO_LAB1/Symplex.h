#pragma once

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

class Symplex
{
	/*матрица преобразований*/
	std::vector<std::vector<double>> table_;

	/*резрешающие таблица и строка*/
	int permissive_str_;
	int permissive_column_;

	/*вектора переменных для таблицы*/
	std::vector<std::string> basis_;
	std::vector<std::string> free_;

public:
	/*конструктор для сборки таблицы из файла*/
	Symplex(std::ifstream& input);

	/*проверка на оптимальность*/
	bool is_optimal() const;

	/*печать таблицы*/
	void print() const;

	/*поиск разрешающей колнки и сроки*/
	int find_column() const;
	int find_str() const;

	/*одна итерация преобразования*/
	void do_step();

	/*геттер оптимального решения*/
	double get_solution() const;

	~Symplex()
	{
		for (auto& i : table_)
			i.clear();
		table_.clear();
		basis_.clear();
		free_.clear();
	}
};
