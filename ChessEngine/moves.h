#pragma once
#include "core.h"
#include <tuple>

namespace moves {
	move encodeMove(square from, square to, piece p, piece capture, bool promote, bool enPassant, bool doublePush, bool wkc, bool wqc, bool bkc, bool bqc);

	move encodeNormal(square from, square to, piece p, piece capture, bool enPassant, bool doublePush, bool wkc, bool wqc, bool bkc, bool bqc);

	move encodePromote(square from, square to, piece p, piece capture, piece promote, bool wkc, bool wqc, bool bkc, bool bqc);

	move encodeCastle(color side, bool queenside, bool wkc, bool wqc, bool bkc, bool bqc);

	square getFrom(const move& m);

	square getTo(const move& m);

	piece getPiece(const move& m);

	piece getCapture(const move& m);

	bool getSpecial1(const move& m);

	bool getSpecial2(const move& m);

	bool getPromoteFlag(const move& m);

	piece getPromote(const move& m, const color& color);

	bool isEnPassant(const move& m);

	bool isDoublePush(const move& m);

	std::tuple<bool, bool, bool, bool> getCastleChanges(const move& m);

	void debugMove(move m);
	void showMove(move m);
}