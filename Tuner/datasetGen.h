#pragma once
#include <fstream>
#include <generator>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "core.h"
#include "search.h"

namespace datasetGen {
	std::generator<std::string> loadFileByLines(std::string fileName);

	struct fenPosition {
		std::string fen;
		int gameID;
		int result;
	};

	std::vector<fenPosition> loadPositions();

	int main();
}