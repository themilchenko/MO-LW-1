
#pragma once

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>


class Symplex
{
	/*ìàòðèöà ïðåîáðàçîâàíèé*/
	std::vector<std::vector<double>> table_;

	/*ðåçðåøàþùèå òàáëèöà è ñòðîêà*/
	int permissive_str_;
	int permissive_column_;

	/*âåêòîðà ïåðåìåííûõ äëÿ òàáëèöû*/
	std::vector<std::string> basis_;
	std::vector<std::string> free_;

public:
	/*êîíñòðóêòîð äëÿ ñáîðêè òàáëèöû èç ôàéëà*/
	Symplex(std::ifstream& input);

	/*ïðîâåðêà íà îïòèìàëüíîñòü*/
	bool is_optimal() const;

	/*ïå÷àòü òàáëèöû*/
	void print() const;

	/*ïîèñê ðàçðåøàþùåé êîëíêè è ñðîêè*/
	int find_column() const;
	int find_str() const;

	/*îäíà èòåðàöèÿ ïðåîáðàçîâàíèÿ*/
	void do_step();

	/*ãåòòåð îïòèìàëüíîãî ðåøåíèÿ*/
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
