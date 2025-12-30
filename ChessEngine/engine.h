#pragma once

//The guts of the engine are here - board rep, move gen, eval, search

#include "core.h"
#include <string>

namespace engine{
	static piece mailbox[64]{ null };
	static U64 bitboards[16]{ 0 };

	static move moves[maxDepth];
	static int moveNum;

	void loadFEN(std::string fen);
	void reset();

	void makeMove(move& m);
	void makeMove(move& m, bool reversible);
	void undoMove();
}