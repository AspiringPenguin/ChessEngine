#pragma once
#include "core.h"

namespace moves {
	move encodeMove(square from, square to, piece capture, bool promote, bool isCapture, bool special1, bool special2);

	square getFrom(move m);
	square getTo(move m);
	piece getCapture(move m);
	bool getPromote(move m);
	bool getCaptureFlag(move m);
	bool getSpecial1(move m);
	bool getSpecial2(move m);
}