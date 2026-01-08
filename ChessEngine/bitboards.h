#pragma once
#include <array>
#include "core.h"

//For helper functions with bitboards

namespace bitboards {
	void showBitboard(U64 bb);

	constexpr U64 notAFile = ~0x0101010101010101;
	constexpr U64 notABFile = ~0x0303030303030303;
	constexpr U64 notGHFile = ~0xC0C0C0C0C0C0C0C0;
	constexpr U64 notHFile = ~0x8080808080808080;

	constexpr U64 rank1 = 0x00000000000000FF;
	constexpr U64 rank2 = 0x000000000000FF00;
	constexpr U64 rank3 = 0x0000000000FF0000;
	constexpr U64 rank4 = 0x00000000FF000000;
	constexpr U64 rank5 = 0x000000FF00000000;
	constexpr U64 rank6 = 0x0000FF0000000000;
	constexpr U64 rank7 = 0x00FF000000000000;
	constexpr U64 rank8 = 0xFF00000000000000;

	static std::array<U64, 8> ranks = {rank1, rank2, rank3, rank4, rank5, rank6, rank7, rank8};

	constexpr U64 AFile = 0x0101010101010101;
	constexpr U64 BFile = 0x0202020202020202;
	constexpr U64 CFile = 0x0404040404040404;
	constexpr U64 DFile = 0x0808080808080808;
	constexpr U64 EFile = 0x1010101010101010;
	constexpr U64 FFile = 0x2020202020202020;
	constexpr U64 GFile = 0x4040404040404040;
	constexpr U64 HFile = 0x8080808080808080;

	constexpr U64 mainDiag = 0x8040201008040201;
	constexpr U64 oppoDiag = 0x0102040810204080;

	static std::array<U64, 8> files = { AFile, BFile, CFile, DFile, EFile, FFile, GFile, HFile };
}