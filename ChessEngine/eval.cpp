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

        //Punish n-pawns, n>1 with lookup table for maluses

        //General defends/defenders checking

        return value;
    }
}