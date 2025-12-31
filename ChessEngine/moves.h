#pragma once
#include "core.h"

namespace moves {
	move encodeMove(square from, square to, piece capture, bool promote, bool special1, bool special2, bool enPassant);

	square getFrom(const move& m);
	square getTo(const move& m);
	piece getCapture(const move& m);
	bool getPromoteFlag(const move& m);
	piece getPromote(const move& m, const color& color); //Interpret special flags
	bool getSpecial1(const move& m);
	bool getSpecial2(const move& m);
	bool getEnPassant(const move& m);
}