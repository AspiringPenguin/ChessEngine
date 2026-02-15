#include "perft.h"
#include "moves.h"
#include <iostream>

namespace perft {
	PerftSearcher::PerftSearcher() {
		p = Position();
	}

	PerftSearcher::PerftSearcher(const std::string& fen) {
		p = Position(fen);
	}

	int PerftSearcher::go(int depth, bool debug) {
		if (p.toMove == white) {
			return perft<white>(depth, debug); //For handling toMove remplates in future
		}
		else {
			return perft<black>(depth, debug); //For handling toMove remplates in future
		}
	}

	template <color c> int PerftSearcher::perft(int depthRemaining, bool top) {
		if (depthRemaining == 0) {
			return 1;
		}

		U64 nodes = 0;
		U64 _nodes;

		auto moves = p.generatePseudoLegalMoves<c>();

		for (auto& move : moves) {
			p.makeMove(move);
			if (p.moveWasLegal()) {
				_nodes = perft<color(1 - c)>(depthRemaining - 1, false);
				if (top) {
					moves::showMove(move);
					std::cout << _nodes << std::endl;
				}
				nodes += _nodes;
			}

			p.undoMove();
		}

		return nodes;
	}
}