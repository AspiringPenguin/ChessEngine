#pragma once

//The guts of the engine are here - board rep, move gen, eval, search

#include "core.h"
#include <string>

namespace engine{
	void loadFEN(std::string fen);
	void reset();

	void makeMove(move& m);
	void makeMove(move& m, bool reversible);
	void undoMove();
}