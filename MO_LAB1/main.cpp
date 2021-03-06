// IU8-34, Milchenko Ivan, 14 Variant
// F = cx -> max
// Ax <= b

#include "Symplex.h"

int main()
{
	std::string path = "/Users/ivanmilchenko/MO-lab-01/input.txt";
	std::ifstream input(path);
	if (!input)
		throw std::runtime_error("Unable to open file " + path);

	Symplex problem(input);

	std::cout << "THIS IS YOUR PROBLEM IN SYMPLEX TABLE:\n";
	problem.print();

	size_t iterations = 0;

    while (problem.is_optimal())
	{
		++iterations;
		std::cout << "THE " << iterations << " STEP:\n";
		problem.do_step();
		problem.print();
	}

	std::cout << "THE SOLUTION IS " << problem.get_solution() << std::endl;
    	problem.do_examination();

	return 0;
}
