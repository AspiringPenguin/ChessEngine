#pragma once
#include "core.h"
#include "moves.h"
#include "position.h"
#include <array>
#include <chrono>

namespace search {
	constexpr int extendedMaxDepth = maxDepth + 16;

	int scoreMove(const move& m, const move& ttMove);

	move getNextMove(std::vector<move>& moves, int& moveN, const move& ttMove);

	class Searcher {
	protected:
		Position p;

	public:
		Searcher();
		Searcher(const std::string& fen);

		void init();

		void loadStart();
		void loadFEN(const std::string& fen);
		std::string asFEN();

		void makeMove(const move& m);

		color getToMove();

		void clearPVTable();

		std::array<std::array<int, extendedMaxDepth>, extendedMaxDepth> pvTable;
		std::array<int, extendedMaxDepth> pvLengthTable;

		template <color c> int generateQuiescencePV(int alpha, int beta, int depth);
		void generateQuiescencePV();
	};
}