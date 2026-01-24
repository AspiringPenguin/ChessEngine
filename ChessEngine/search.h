#pragma once
#include "core.h"
#include "moves.h"
#include "position.h"
#include <chrono>

namespace search {
	int scoreMove(const move& m, const move& ttMove);

	move getNextMove(std::vector<move>& moves, int& moveN, const move& ttMove);

	std::tuple<int, int> getIdealAndMaxTimes(int wtime, int btime, int winc, int binc, color toMove);

	class Searcher {
	protected:
		Position p;
		int negamax(int alpha, int beta, int depth, int depthRemaining);
		int negamaxNoTT(int alpha, int beta, int depth, int depthRemaining);
		int negamaxQuiescence(int alpha, int beta, int depth);
		int nodes;

		int ideal;
		int max;

		std::chrono::steady_clock::time_point start;

	public:
		Searcher();
		Searcher(const std::string& fen);

		void init();

		void loadStart();
		void loadFEN(const std::string& fen);

		void makeMove(const move& m);
		void showPosition();

		move UCIMoveAsInternal(const std::string& move);

		int getNodes();
		void resetNodes();

		int getExtensions();

		move go(int wtime, int btime, int winc, int binc, bool* stop, bool useBook = true);
	};
}