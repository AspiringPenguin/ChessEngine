#pragma once
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "core.h"

namespace datasetGen {
	struct fenPosition {
		std::string fen;
		int gameID;
		int result;
	};

	std::vector<fenPosition> loadPositions();

	int main();
}