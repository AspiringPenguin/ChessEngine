#pragma once
#include "core.h"
#include "moves.h"
#include "position.h"

namespace search {
	int scoreMove(const move& m, const move& ttMove);

	move getNextMove(std::vector<move>& moves, int& moveN, const move& ttMove);

	class Searcher {
	protected:
		Position p;
		int negamax(int alpha, int beta, int depth, int depthRemaining);
		int negamaxQuiescence(int alpha, int beta, int depth);
		int nodes;

	public:
		Searcher();
		Searcher(const std::string& fen);

		void makeMove(const move& m);
		void showPosition();

		int getNodes();
		void resetNodes();

		move go(int wtime, int btime, int winc, int binc, bool useBook = true, bool* stop);
	};
}