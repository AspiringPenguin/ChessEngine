#pragma once

//The guts of the engine are here - board rep, move gen, eval, search

#include "core.h"

static piece mailbox[64]{ null };
static U64 bitboards[16]{ 0 };