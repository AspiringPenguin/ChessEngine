#pragma once
#include "core.h"
#include "moves.h"

#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace book {
	std::string loadFile(std::string fileName);
	//Load the book to a lookup table by zobrist hash, giving a vector of tuples containing a move, lower and upper bound in terms of index
	std::unordered_map<U64, std::tuple<std::vector<std::tuple<std::string, int, int>>, int>> loadBook();

	static auto book = loadBook();

	std::string chooseMove(std::tuple<std::vector<std::tuple<std::string, int, int>>, int> moveList);

}