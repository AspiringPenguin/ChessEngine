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
		return perft(depth, debug); //For handling toMove remplates in future
	}

	int PerftSearcher::perft(int depthRemaining, bool top) {
		if (depthRemaining == 0) {
			return 1;
		}

		U64 nodes = 0;
		U64 _nodes;

		auto moves = p.generatePseudoLegalMoves();

		for (auto& move : moves) {
			p.makeMove(move);
			if (p.moveWasLegal()) {
				_nodes = perft(depthRemaining - 1, false);
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