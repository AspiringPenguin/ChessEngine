#include "eval.h"

#include "bitboards.h"
#include "moveGenHelpers.h"
#include <intrin.h>

namespace eval {
    std::array<int, 64> generateKnightMoveCounts() {
        auto values = std::array<int, 64>();

        for (int i = 0; i < 64; i++) {
            values[i] = __popcnt64(moveGen::knightLookup[i]);
        }

        return values;
    }

	//https://www.chessprogramming.org/PeSTO%27s_Evaluation_Function
	int pawnValueStart;
	int pawnValueEnd;
	int knightValueStart;
	int knightValueEnd;
	int bishopValueStart;
	int bishopValueEnd;
	int rookValueStart;
	int rookValueEnd;
	int queenValueStart;
	int queenValueEnd;

	std::array<int, 16> pieceValuesStart;
	std::array<int, 16> pieceValuesEnd;

	//psqs - https://www.chessprogramming.org/Simplified_Evaluation_Function
	//These are A8 = 0, H8=7, A1=56, H1=63

	std::array<int, 64> pawnBonusesStart;
	std::array<int, 64> pawnBonusesEnd;

	std::array<int, 64> knightBonusesStart;
	std::array<int, 64> knightBonusesEnd;

	std::array<int, 64> bishopBonusesStart;
	std::array<int, 64> bishopBonusesEnd;

	std::array<int, 64> rookBonusesStart;
	std::array<int, 64> rookBonusesEnd;

	std::array<int, 64> queenBonusesStart;
	std::array<int, 64> queenBonusesEnd;

	std::array<int, 64> kingBonusesStart;
	std::array<int, 64> kingBonusesEnd;

	std::array<std::array<int, 64>, 6> pieceBonusesStart;
	std::array<std::array<int, 64>, 6> pieceBonusesEnd;

	std::array<int, 64> knightMoveCounts;
	std::array<std::array<U64, 64>, 2> passedPawnMasks;

	std::array<int, 7> passedPawnBonuses;

    void loadEval() {
		//https://www.chessprogramming.org/PeSTO%27s_Evaluation_Function
		pawnValueStart = 82;
		pawnValueEnd = 94;
		knightValueStart = 337;
		knightValueEnd = 281;
		bishopValueStart = 365;
		bishopValueEnd = 297;
		rookValueStart = 477;
		rookValueEnd = 512;
		queenValueStart = 1025;
		queenValueEnd = 936;

		pieceValuesStart = { 0, pawnValueStart, knightValueStart, bishopValueStart, rookValueStart, queenValueStart, 0, 0,
							 0, -pawnValueStart, -knightValueStart, -bishopValueStart, -rookValueStart, -queenValueStart, 0, 0 };

		pieceValuesEnd = { 0, pawnValueEnd, knightValueEnd, bishopValueEnd, rookValueEnd, queenValueEnd, 0, 0,
						   0, -pawnValueEnd, -knightValueEnd, -bishopValueEnd, -rookValueEnd, -queenValueEnd, 0, 0 };

		//psqs - https://www.chessprogramming.org/Simplified_Evaluation_Function
		//These are A8 = 0, H8=7, A1=56, H1=63

		pawnBonusesStart = {
		  0,   0,   0,   0,   0,   0,  0,   0,
		 98, 134,  61,  95,  68, 126, 34, -11,
		 -6,   7,  26,  31,  65,  56, 25, -20,
		-14,  13,   6,  21,  23,  12, 17, -23,
		-27,  -2,  -5,  12,  17,   6, 10, -25,
		-26,  -4,  -4, -10,   3,   3, 33, -12,
		-35,  -1, -20, -23, -15,  24, 38, -22,
		  0,   0,   0,   0,   0,   0,  0,   0,
		};

		pawnBonusesEnd = {
		  0,   0,   0,   0,   0,   0,   0,   0,
		178, 173, 158, 134, 147, 132, 165, 187,
		 94, 100,  85,  67,  56,  53,  82,  84,
		 32,  24,  13,   5,  -2,   4,  17,  17,
		 13,   9,  -3,  -7,  -7,  -8,   3,  -1,
		  4,   7,  -6,   1,   0,  -5,  -1,  -8,
		 13,   8,   8,  10,  13,   0,   2,  -7,
		  0,   0,   0,   0,   0,   0,   0,   0,
		};

		knightBonusesStart = {
		-167, -89, -34, -49,  61, -97, -15, -107,
		 -73, -41,  72,  36,  23,  62,   7,  -17,
		 -47,  60,  37,  65,  84, 129,  73,   44,
		  -9,  17,  19,  53,  37,  69,  18,   22,
		 -13,   4,  16,  13,  28,  19,  21,   -8,
		 -23,  -9,  12,  10,  19,  17,  25,  -16,
		 -29, -53, -12,  -3,  -1,  18, -14,  -19,
		-105, -21, -58, -33, -17, -28, -19,  -23,
		};

		knightBonusesEnd = {
		-58, -38, -13, -28, -31, -27, -63, -99,
		-25,  -8, -25,  -2,  -9, -25, -24, -52,
		-24, -20,  10,   9,  -1,  -9, -19, -41,
		-17,   3,  22,  22,  22,  11,   8, -18,
		-18,  -6,  16,  25,  16,  17,   4, -18,
		-23,  -3,  -1,  15,  10,  -3, -20, -22,
		-42, -20, -10,  -5,  -2, -20, -23, -44,
		-29, -51, -23, -15, -22, -18, -50, -64,
		};

		bishopBonusesStart = {
		-29,   4, -82, -37, -25, -42,   7,  -8,
		-26,  16, -18, -13,  30,  59,  18, -47,
		-16,  37,  43,  40,  35,  50,  37,  -2,
		 -4,   5,  19,  50,  37,  37,   7,  -2,
		 -6,  13,  13,  26,  34,  12,  10,   4,
		  0,  15,  15,  15,  14,  27,  18,  10,
		  4,  15,  16,   0,   7,  21,  33,   1,
		-33,  -3, -14, -21, -13, -12, -39, -21,
		};

		bishopBonusesEnd = {
		-14, -21, -11,  -8, -7,  -9, -17, -24,
		 -8,  -4,   7, -12, -3, -13,  -4, -14,
		  2,  -8,   0,  -1, -2,   6,   0,   4,
		 -3,   9,  12,   9, 14,  10,   3,   2,
		 -6,   3,  13,  19,  7,  10,  -3,  -9,
		-12,  -3,   8,  10, 13,   3,  -7, -15,
		-14, -18,  -7,  -1,  4,  -9, -15, -27,
		-23,  -9, -23,  -5, -9, -16,  -5, -17,
		};

		rookBonusesStart = {
		 32,  42,  32,  51, 63,  9,  31,  43,
		 27,  32,  58,  62, 80, 67,  26,  44,
		 -5,  19,  26,  36, 17, 45,  61,  16,
		-24, -11,   7,  26, 24, 35,  -8, -20,
		-36, -26, -12,  -1,  9, -7,   6, -23,
		-45, -25, -16, -17,  3,  0,  -5, -33,
		-44, -16, -20,  -9, -1, 11,  -6, -71,
		-19, -13,   1,  17, 16,  7, -37, -26,
		};

		rookBonusesEnd = {
		13, 10, 18, 15, 12,  12,   8,   5,
		11, 13, 13, 11, -3,   3,   8,   3,
		 7,  7,  7,  5,  4,  -3,  -5,  -3,
		 4,  3, 13,  1,  2,   1,  -1,   2,
		 3,  5,  8,  4, -5,  -6,  -8, -11,
		-4,  0, -5, -1, -7, -12,  -8, -16,
		-6, -6,  0,  2, -9,  -9, -11,  -3,
		-9,  2,  3, -1, -5, -13,   4, -20,
		};

		queenBonusesStart = {
		-28,   0,  29,  12,  59,  44,  43,  45,
		-24, -39,  -5,   1, -16,  57,  28,  54,
		-13, -17,   7,   8,  29,  56,  47,  57,
		-27, -27, -16, -16,  -1,  17,  -2,   1,
		 -9, -26,  -9, -10,  -2,  -4,   3,  -3,
		-14,   2, -11,  -2,  -5,   2,  14,   5,
		-35,  -8,  11,   2,   8,  15,  -3,   1,
		 -1, -18,  -9,  10, -15, -25, -31, -50,
		};

		queenBonusesEnd = {
		 -9,  22,  22,  27,  27,  19,  10,  20,
		-17,  20,  32,  41,  58,  25,  30,   0,
		-20,   6,   9,  49,  47,  35,  19,   9,
		  3,  22,  24,  45,  57,  40,  57,  36,
		-18,  28,  19,  47,  31,  34,  39,  23,
		-16, -27,  15,   6,   9,  17,  10,   5,
		-22, -23, -30, -16, -16, -23, -36, -32,
		-33, -28, -22, -43,  -5, -32, -20, -41,
		};

		kingBonusesStart = {
		-65,  23,  16, -15, -56, -34,   2,  13,
		 29,  -1, -20,  -7,  -8,  -4, -38, -29,
		 -9,  24,   2, -16, -20,   6,  22, -22,
		-17, -20, -12, -27, -30, -25, -14, -36,
		-49,  -1, -27, -39, -46, -44, -33, -51,
		-14, -14, -22, -46, -44, -30, -15, -27,
		  1,   7,  -8, -64, -43, -16,   9,   8,
		-15,  36,  12, -54,   8, -28,  24,  14,
		};

		kingBonusesEnd = {
		-74, -35, -18, -18, -11,  15,   4, -17,
		-12,  17,  14,  17,  17,  38,  23,  11,
		 10,  17,  23,  15,  20,  45,  44,  13,
		 -8,  22,  24,  27,  26,  33,  26,   3,
		-18,  -4,  21,  24,  27,  23,   9, -11,
		-19,  -3,  11,  21,  23,  16,   7,  -9,
		-27, -11,   4,  13,  14,   4,  -5, -17,
		-53, -34, -21, -11, -28, -14, -24, -43
		};

		pieceBonusesStart = {pawnBonusesStart, knightBonusesStart, bishopBonusesStart, rookBonusesStart, queenBonusesStart, kingBonusesStart};
		pieceBonusesEnd = {pawnBonusesEnd, knightBonusesEnd, bishopBonusesEnd, rookBonusesEnd, queenBonusesEnd, kingBonusesEnd};

		std::array<int, 64> knightMoveCounts = generateKnightMoveCounts();

		//				      1  2   3   4   5   6   7
		passedPawnBonuses = { 0, 10, 20, 40, 60, 80, 0 };

		passedPawnMasks = generatePassedPawnMasks();
    }

    int getBoardControl(const Position* p) {
        int control = 0;

        U64 pieceBB;
        U64 blockerBB;
        square sq;

        //Pawns - capture squares
        //B - G files
        control += 2 * pieceControlValues[wPawn] * __popcnt64(p->bitboards[wPawn] & ~(bitboards::AFile | bitboards::HFile));
        control += 2 * pieceControlValues[bPawn] * __popcnt64(p->bitboards[bPawn] & ~(bitboards::AFile | bitboards::HFile));

        //A and H
        control += pieceControlValues[wPawn] * __popcnt64(p->bitboards[wPawn] & (bitboards::AFile | bitboards::HFile));
        control += pieceControlValues[bPawn] * __popcnt64(p->bitboards[bPawn] & (bitboards::AFile | bitboards::HFile));

        //Knights
        pieceBB = p->bitboards[wKnight];

        Bitloop(pieceBB) {
            sq = square(SquareOf(pieceBB));
            control += pieceControlValues[wKnight] * knightMoveCounts[sq];
        }

        pieceBB = p->bitboards[bKnight];

        Bitloop(pieceBB) {
            sq = square(SquareOf(pieceBB));
            control += pieceControlValues[bKnight] * knightMoveCounts[sq];
        }

        //Bishops
        pieceBB = p->bitboards[wBishop];
        blockerBB = p->allBitboard & ~p->bitboards[wBishop] & ~p->bitboards[wQueen];

        Bitloop(pieceBB) {
            sq = square(SquareOf(pieceBB));
            control += pieceControlValues[wBishop] * __popcnt64(moveGen::bishopMoveLookup[sq][_pext_u64(blockerBB, moveGen::bishopPextMasks[sq])]);
        }

        pieceBB = p->bitboards[bBishop];
        blockerBB = p->allBitboard & ~p->bitboards[bBishop] & ~p->bitboards[bQueen];

        Bitloop(pieceBB) {
            sq = square(SquareOf(pieceBB));
            control += pieceControlValues[bBishop] * __popcnt64(moveGen::bishopMoveLookup[sq][_pext_u64(blockerBB, moveGen::bishopPextMasks[sq])]);
        }

        //Rooks
        pieceBB = p->bitboards[wRook];
        blockerBB = p->allBitboard & ~p->bitboards[wRook] & ~p->bitboards[wQueen];

        Bitloop(pieceBB) {
            sq = square(SquareOf(pieceBB));
            control += pieceControlValues[wRook] * __popcnt64(moveGen::rookMoveLookup[sq][_pext_u64(blockerBB, moveGen::rookPextMasks[sq])]);
        }

        pieceBB = p->bitboards[bRook];
        blockerBB = p->allBitboard & ~p->bitboards[bRook] & ~p->bitboards[bQueen];

        Bitloop(pieceBB) {
            sq = square(SquareOf(pieceBB));
            control += pieceControlValues[bRook] * __popcnt64(moveGen::rookMoveLookup[sq][_pext_u64(blockerBB, moveGen::rookPextMasks[sq])]);
        }

        //Queen
        //Diagonals
        pieceBB = p->bitboards[wQueen];
        blockerBB = p->allBitboard & ~p->bitboards[wBishop] & ~p->bitboards[wQueen];

        Bitloop(pieceBB) {
            sq = square(SquareOf(pieceBB));
            control += pieceControlValues[wQueen] * __popcnt64(moveGen::bishopMoveLookup[sq][_pext_u64(blockerBB, moveGen::bishopPextMasks[sq])]);
        }

        pieceBB = p->bitboards[bQueen];
        blockerBB = p->allBitboard & ~p->bitboards[bBishop] & ~p->bitboards[bQueen];

        Bitloop(pieceBB) {
            sq = square(SquareOf(pieceBB));
            control += pieceControlValues[bQueen] * __popcnt64(moveGen::bishopMoveLookup[sq][_pext_u64(blockerBB, moveGen::bishopPextMasks[sq])]);
        }

        //Ranks/files
        pieceBB = p->bitboards[wQueen];
        blockerBB = p->allBitboard & ~p->bitboards[wRook] & ~p->bitboards[wQueen];

        Bitloop(pieceBB) {
            sq = square(SquareOf(pieceBB));
            control += pieceControlValues[wQueen] * __popcnt64(moveGen::rookMoveLookup[sq][_pext_u64(blockerBB, moveGen::rookPextMasks[sq])]);
        }

        pieceBB = p->bitboards[bRook];
        blockerBB = p->allBitboard & ~p->bitboards[bRook] & ~p->bitboards[bQueen];

        Bitloop(pieceBB) {
            sq = square(SquareOf(pieceBB));
            control += pieceControlValues[bQueen] * __popcnt64(moveGen::rookMoveLookup[sq][_pext_u64(blockerBB, moveGen::rookPextMasks[sq])]);
        }

        return control;
    }

    U64 passedPawnMask(square sq, color c) {
        U64 mask;
        if (c == white) {
            mask = bitboards::files[sq & 7];
            mask <<= (sq & 56) + 8;
            mask |= ((mask << 1) & ~bitboards::AFile);
            mask |= ((mask >> 1) & ~bitboards::HFile);
            return mask;
        }
        else {
            mask = bitboards::files[sq & 7];
            mask >>= 56 - (sq & 56) + 8;
            mask |= ((mask << 1) & ~bitboards::AFile);
            mask |= ((mask >> 1) & ~bitboards::HFile);
            return mask;
        }
    }

    std::array<std::array<U64, 64>, 2> generatePassedPawnMasks() {
        auto masks = std::array<std::array<U64, 64>, 2>();
        for (color c : {white, black}) {
            for (int s = 0; s < 64; s++) {
                masks[c][s] = passedPawnMask(square(s), c);
            }
        }
        return masks;
    }

    int getPawnEval(const Position* p) {
        int value = 0;

        //Passed pawn bonuses, with lookup for rank
        //White pawns
        U64 pieceBB = p->bitboards[wPawn];
        U64 blockerBB = p->bitboards[bPawn];
        square sq;
        Bitloop(pieceBB) {
            sq = square(SquareOf(pieceBB));
            if ((passedPawnMasks[white][sq] & blockerBB) == 0) {
                value += passedPawnBonuses[sq >> 3];
            }
        }

        //Black pawns
        pieceBB = p->bitboards[bPawn];
        blockerBB = p->bitboards[wPawn];
        Bitloop(pieceBB) {
            sq = square(SquareOf(pieceBB));
            if ((passedPawnMasks[black][sq] & blockerBB) == 0) {
                value -= passedPawnBonuses[7 - (sq >> 3)];
            }
        }
        return value;
    }
}