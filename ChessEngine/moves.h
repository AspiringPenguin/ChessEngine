#pragma once
#include "core.h"

namespace moves {
	move encodeMove(square from, square to, piece capture, bool promote, bool isCapture, bool special1, bool special2);
}