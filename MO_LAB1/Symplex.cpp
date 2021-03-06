#include "Symplex.h"

Symplex::Symplex(std::ifstream& input)
{
    std::vector<double> v1;
    std::vector<std::vector<double>> v2;
    std::vector<double> v3;

    /*считываение условие задачи из файла*/
    while (!input.eof())
    {
        std::string current_str;
        std::getline(input, current_str, '\n');

        if (current_str.find("A:") != -1)
        {
            std::string res = "";
            std::getline(input, res, 'b');
            std::string num_str = "";

            res.erase(res.end() - 1);  // удаляю перевод строки
            res.erase(res.begin());    // удаляю первый [
            res.erase(res.end() - 1);  // удаляю последний ]

            int num = std::count(res.begin(), res.end(), '\n');
            v2.resize(num + 1);
            size_t index = 0;

            for (size_t i = 0; i < res.size(); ++i)
            {
                if (res[i] == '\n')
                    ++index;
                else if (res[i] != ' ' && res[i] != '[' && res[i] != ']')
                    num_str += res[i];
                else if (res[i] == ' ' && res[i - 1] != '[' && res[i - 1] != '\n')
                {
                    v2[index].push_back(std::stod(num_str));
                    num_str.clear();
                }
            }
        }
        else if (current_str.find("c:") != -1 || current_str.find(":") != -1)
        {
            std::string com;
            std::getline(input, com, '\n');
            std::string res = "";

            for (char i : com)
            {
                if (i != ' ' && i != '[' && i != ']')
                    res += i;
                else if (i == ' ' || i == ']')
                {
                    if (current_str.find("c:") != -1)
                    {
                        v1.push_back(std::stod(res));
                        res.clear();
                    }
                    else
                    {
                        v3.push_back(std::stod(res));
                        res.clear();
                    }
                }
            }
        }

    }

    /*создание симплекс-таблицы*/
    table_.resize(v2.size() + 1); // плюс строка с функцией

    /*заполняем свободные коэффициенты*/
    for (size_t i = 0; i < table_.size(); ++i)
    {
        if (i != table_.size() - 1)
            table_[i].push_back(v3[i]);
        else
            table_[i].push_back(0);
    }

    /*заполняем основную часть таблицы*/
    for (size_t i = 0; i < v2.size(); ++i)
        for (size_t j = 0; j < v2[i].size(); ++j)
            table_[i].push_back(v2[i][j]);

    /*заполнение целевой функции*/
    for (size_t i = 0; i < v1.size(); ++i)
        table_[table_.size() - 1].push_back(v1[i]);

    /*заполнение векторов базисными и свободными переменными*/
    for (size_t i = 0; i < table_[0].size() - 1; ++i)
        free_.push_back('x' + std::to_string(i + 1));

    for (size_t i = table_[0].size(); i < table_[0].size() + table_.size() - 1; ++i)
        basis_.push_back('x' + std::to_string(i));

    for (size_t i = 1; i < table_[0].size(); ++i)
        goal_func.push_back(table_[table_.size() - 1][i]);
}

void Symplex::print() const
{
    size_t bas_indx = 0;

    std::cout << "\t\t" << 'S' << "\t\t";
    for (auto& i : free_)
        std::cout << i << "\t\t";
    std::cout << std::endl;

    for (auto& i : table_)
    {
        if (bas_indx != basis_.size())
        {
            std::cout << basis_[bas_indx] << "  ";
            ++bas_indx;
        }
        else
            std::cout << "F " << "  ";

        for (double j : i)
        {
            if (j == -0.00)
                std::cout << std::setprecision(2) << std::fixed << std::setw(5) << 0.00 << "\t";
            else
                std::cout << std::setprecision(2) << std::fixed << std::setw(5) << j << "\t";
        }
        std::cout << std::endl;
    }
}

bool Symplex::is_optimal() const
{
    size_t counter = 0;
    for (size_t i = 1; i < table_[table_.size() - 1].size(); ++i)
        if (table_[table_.size() - 1][i] <= 0)
            ++counter;

    return (counter == table_[table_.size() - 1].size() - 1) ? false : true;
}

int Symplex::find_column() const
{
    bool is_negative = 1;
    for (size_t i = 0; i < table_.size() - 1; ++i)
        if (table_[i][0] < 0)
        {
            is_negative = 0;
            for (size_t j = 1; j < table_[i].size(); ++j)
                if (table_[i][j] < 0)
                    return j;
        }

    if (is_negative)
    {
        for (size_t i = 0; i < table_[table_.size() - 1].size(); ++i)
            if (table_[table_.size() - 1][i] > 0)
                return i;
    }
}

int Symplex::find_str() const
{
    int min_index = 0;
    for (size_t i = 0; i < table_.size() - 1; ++i)
        if (table_[i][0] / table_[i][permissive_column_] > 0 &&
        (table_[i][0] / table_[i][permissive_column_] < table_[min_index][0] / table_[min_index][permissive_column_]))
            min_index = i;

    return min_index;
}

void Symplex::do_step()
{
    permissive_column_ = find_column();
    permissive_str_ = find_str();

    /*swap переменных xi и xj*/
    std::string temp = basis_[permissive_str_];
    basis_[permissive_str_] = free_[permissive_column_ - 1];
    free_[permissive_column_ - 1] = temp;

    /*создаем матрицу, в кот. буду переписываться новые значения*/
    std::vector<std::vector<double>> copy_to(table_.size());
    for (auto& i : copy_to)
        i.resize(table_[0].size());

    /*переопределяем элементы таблицы*/
    for (size_t i = 0; i < table_.size(); ++i)
        for (size_t j = 0; j < table_[i].size(); ++j)
        {
            if (i == permissive_str_ && j != permissive_column_)
                copy_to[i][j] = table_[i][j] / table_[permissive_str_][permissive_column_];
            else if (i != permissive_str_ && j == permissive_column_)
                copy_to[i][j] = -table_[i][j] / table_[permissive_str_][permissive_column_];
            else if (i == permissive_str_ && j == permissive_column_)
                copy_to[i][j] = 1 / table_[i][j];
            else
                copy_to[i][j] =
                        table_[i][j] - ((table_[permissive_str_][j] * table_[i][permissive_column_]) /
                        table_[permissive_str_][permissive_column_]);
        }

    table_ = copy_to;
}

double Symplex::get_solution() const
{
    return -table_[table_.size() - 1][0];
}

void Symplex::do_examination() const
{
    std::vector<std::pair<std::string, double>> solutions(basis_.size());

    for (size_t i = 0; i < basis_.size(); ++i)
        solutions[i].first = "x" + std::to_string(i + 1);

    for (size_t j = 0; j < solutions.size(); ++j)
        for (size_t i = 0; i < table_.size() - 1; ++i)
            if (solutions[j].first == basis_[i])
                solutions[j].second = table_[i][0];

    for (size_t j = 0; j < solutions.size(); ++j)
        for (size_t i = 0; i < table_[0].size(); ++i)
            if (solutions[j].first == free_[i])
                solutions[j].second = table_[table_.size() - 1][i + 1];

    for (const auto& i : solutions)
        std::cout << i.first << " = " << i.second << std::endl;

    for (size_t i = 0; i < solutions.size(); ++i)
        if (i != solutions.size() - 1)
            std::cout << std::to_string(goal_func[i])<< " * " << std::to_string(solutions[i].second) << " + ";
        else
            std::cout << std::to_string(goal_func[i])<< " * " << std::to_string(solutions[i].second);

    std::cout << " = " << get_solution();
}
