#include "eval.h"

#include "bitboards.h"
#include "fileHandling.h"
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
    int pawnDefenceBonus;

    std::array<int, 16> pieceControlValues;

    std::vector<int> loadEvalTerms(std::string file) {
        std::string evalTerms = files::loadFile(file);
        std::vector<std::string> evalLines = split(evalTerms, "\n");

        auto allTerms = std::vector<int>();
        bool first = true;
        std::vector<std::string> lineTerms;

        for (const std::string& line : evalLines) {
            if (line.starts_with("//") || line == "") {
                continue;
            }
            lineTerms = split(line, ",");
            for (const std::string& term : lineTerms) {
                allTerms.push_back(std::stoi(term));
            }
        }

        return allTerms;
    }

    void loadEval() {
        auto allTerms = loadEvalTerms("eval.txt");

		//https://www.chessprogramming.org/PeSTO%27s_Evaluation_Function
		pawnValueStart = allTerms[0];
		pawnValueEnd = allTerms[5];
		knightValueStart = allTerms[1];
		knightValueEnd = allTerms[6];
		bishopValueStart = allTerms[2];
		bishopValueEnd = allTerms[7];
		rookValueStart = allTerms[3];
		rookValueEnd = allTerms[8];
		queenValueStart = allTerms[4];
		queenValueEnd = allTerms[9];

		pieceValuesStart = { 0, pawnValueStart, knightValueStart, bishopValueStart, rookValueStart, queenValueStart, 0, 0,
							 0, -pawnValueStart, -knightValueStart, -bishopValueStart, -rookValueStart, -queenValueStart, 0, 0 };

		pieceValuesEnd = { 0, pawnValueEnd, knightValueEnd, bishopValueEnd, rookValueEnd, queenValueEnd, 0, 0,
						   0, -pawnValueEnd, -knightValueEnd, -bishopValueEnd, -rookValueEnd, -queenValueEnd, 0, 0 };

		//psqs - https://www.chessprogramming.org/Simplified_Evaluation_Function
		//These are A8 = 0, H8=7, A1=56, H1=63

		std::copy(allTerms.begin() + 10, allTerms.begin() + 74, std::begin(pawnBonusesStart));
		std::copy(allTerms.begin() + 74, allTerms.begin() + 138, std::begin(pawnBonusesEnd));

		std::copy(allTerms.begin() + 138, allTerms.begin() + 202, std::begin(knightBonusesStart));
		std::copy(allTerms.begin() + 202, allTerms.begin() + 266, std::begin(knightBonusesEnd));

		std::copy(allTerms.begin() + 266, allTerms.begin() + 330, std::begin(bishopBonusesStart));
		std::copy(allTerms.begin() + 330, allTerms.begin() + 394, std::begin(bishopBonusesEnd));

		std::copy(allTerms.begin() + 394, allTerms.begin() + 458, std::begin(rookBonusesStart));
		std::copy(allTerms.begin() + 458, allTerms.begin() + 522, std::begin(rookBonusesEnd));

		std::copy(allTerms.begin() + 522, allTerms.begin() + 586, std::begin(queenBonusesStart));
		std::copy(allTerms.begin() + 586, allTerms.begin() + 650, std::begin(queenBonusesEnd));

		std::copy(allTerms.begin() + 650, allTerms.begin() + 714, std::begin(kingBonusesStart));
		std::copy(allTerms.begin() + 714, allTerms.begin() + 778, std::begin(kingBonusesEnd));

        std::copy(allTerms.begin() + 778, allTerms.begin() + 785, std::begin(passedPawnBonuses));

        pawnDefenceBonus = allTerms[785];

        std::copy(allTerms.begin() + 786, allTerms.begin() + 802, std::begin(pieceControlValues));

		pieceBonusesStart = {pawnBonusesStart, knightBonusesStart, bishopBonusesStart, rookBonusesStart, queenBonusesStart, kingBonusesStart};
		pieceBonusesEnd = {pawnBonusesEnd, knightBonusesEnd, bishopBonusesEnd, rookBonusesEnd, queenBonusesEnd, kingBonusesEnd};

		knightMoveCounts = generateKnightMoveCounts();

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

        //Pawns that are defended by other pawns are good
        //White pawns
        //Defend to right
        pieceBB = p->bitboards[wPawn];
        U64 intersectionBB = pieceBB & ((pieceBB & (~bitboards::HFile)) << 9);
        value += pawnDefenceBonus * __popcnt64(intersectionBB);

        //Defend to left
        intersectionBB = pieceBB & ((pieceBB & (~bitboards::AFile)) << 7);
        value += pawnDefenceBonus * __popcnt64(intersectionBB);


        //Black pawns
        //Defend to right
        pieceBB = p->bitboards[bPawn];
        intersectionBB = pieceBB & ((pieceBB & (~bitboards::HFile)) >> 7);
        value -= pawnDefenceBonus * __popcnt64(intersectionBB);

        //Defend to left
        intersectionBB = pieceBB & ((pieceBB & (~bitboards::AFile)) >> 9);
        value -= pawnDefenceBonus * __popcnt64(intersectionBB);

        return value;
    }
}