
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#pragma once

class Symplex
{
	/*������� ��������������*/
	std::vector<std::vector<double>> table_;

	/*����������� ������� � ������*/
	int permissive_str_;
	int permissive_column_;

	/*������� ���������� ��� �������*/
	std::vector<std::string> basis_;
	std::vector<std::string> free_;

public:
	/*����������� ��� ������ ������� �� �����*/
	Symplex(std::ifstream& input);

	/*�������� �� �������������*/
	bool is_optimal() const;

	/*������ �������*/
	void print() const;

	/*����� ����������� ������ � �����*/
	int find_column() const;
	int find_str() const;

	/*���� �������� ��������������*/
	void do_step();

	/*������ ������������ �������*/
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