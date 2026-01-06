#pragma once

//The guts of the engine are here - board rep, move gen, eval, search

#include "core.h"
#include <string>

namespace engine{
	void updateBitboards();
	void showPosition(color perspective = white);

	void reset();
	void loadFEN(std::string fen);

	void makeMove(move& m);
	void makeMove(move& m, bool reversible);
	void undoMove();

	bool isDraw();

	std::vector<move> generatePseudoLegalMoves();

	bool kingInCheck();
	bool castleWasLegal();

	//For testing
	void debugPosition();
	void debugPositions();
	U64 getZobrist();
	U64 getBitboard(int n);
}