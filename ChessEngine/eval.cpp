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


    int getBoardControl(const Position& p) {
        int control = 0;

        U64 pieceBB;
        U64 blockerBB;
        square sq;

        //Pawns - capture squares
        //B - G files
        control += 2 * pieceControlValues[wPawn] * __popcnt64(p.bitboards[wPawn] & ~(bitboards::AFile | bitboards::HFile));
        control += 2 * pieceControlValues[bPawn] * __popcnt64(p.bitboards[bPawn] & ~(bitboards::AFile | bitboards::HFile));

        //A and H
        control += pieceControlValues[wPawn] * __popcnt64(p.bitboards[wPawn] & (bitboards::AFile | bitboards::HFile));
        control += pieceControlValues[bPawn] * __popcnt64(p.bitboards[bPawn] & (bitboards::AFile | bitboards::HFile));

        //Knights
        pieceBB = p.bitboards[wKnight];

        Bitloop(pieceBB) {
            sq = square(SquareOf(pieceBB));
            control += pieceControlValues[wKnight] * knightMoveCounts[sq];
        }

        pieceBB = p.bitboards[bKnight];

        Bitloop(pieceBB) {
            sq = square(SquareOf(pieceBB));
            control += pieceControlValues[bKnight] * knightMoveCounts[sq];
        }

        //Bishops
        pieceBB = p.bitboards[wBishop];
        blockerBB = p.allBitboard & ~p.bitboards[wBishop] & ~p.bitboards[wQueen];

        Bitloop(pieceBB) {
            sq = square(SquareOf(pieceBB));
            control += pieceControlValues[wBishop] * __popcnt64(moveGen::bishopMoveLookup[sq][_pext_u64(blockerBB, moveGen::bishopPextMasks[sq])]);
        }

        pieceBB = p.bitboards[bBishop];
        blockerBB = p.allBitboard & ~p.bitboards[bBishop] & ~p.bitboards[bQueen];

        Bitloop(pieceBB) {
            sq = square(SquareOf(pieceBB));
            control += pieceControlValues[bBishop] * __popcnt64(moveGen::bishopMoveLookup[sq][_pext_u64(blockerBB, moveGen::bishopPextMasks[sq])]);
        }

        //Rooks
        pieceBB = p.bitboards[wRook];
        blockerBB = p.allBitboard & ~p.bitboards[wRook] & ~p.bitboards[wQueen];

        Bitloop(pieceBB) {
            sq = square(SquareOf(pieceBB));
            control += pieceControlValues[wRook] * __popcnt64(moveGen::rookMoveLookup[sq][_pext_u64(blockerBB, moveGen::rookPextMasks[sq])]);
        }

        pieceBB = p.bitboards[bRook];
        blockerBB = p.allBitboard & ~p.bitboards[bRook] & ~p.bitboards[bQueen];

        Bitloop(pieceBB) {
            sq = square(SquareOf(pieceBB));
            control += pieceControlValues[bRook] * __popcnt64(moveGen::rookMoveLookup[sq][_pext_u64(blockerBB, moveGen::rookPextMasks[sq])]);
        }

        //Queen
        //Diagonals
        pieceBB = p.bitboards[wQueen];
        blockerBB = p.allBitboard & ~p.bitboards[wBishop] & ~p.bitboards[wQueen];

        Bitloop(pieceBB) {
            sq = square(SquareOf(pieceBB));
            control += pieceControlValues[wQueen] * __popcnt64(moveGen::bishopMoveLookup[sq][_pext_u64(blockerBB, moveGen::bishopPextMasks[sq])]);
        }

        pieceBB = p.bitboards[bQueen];
        blockerBB = p.allBitboard & ~p.bitboards[bBishop] & ~p.bitboards[bQueen];

        Bitloop(pieceBB) {
            sq = square(SquareOf(pieceBB));
            control += pieceControlValues[bQueen] * __popcnt64(moveGen::bishopMoveLookup[sq][_pext_u64(blockerBB, moveGen::bishopPextMasks[sq])]);
        }

        //Ranks/files
        pieceBB = p.bitboards[wQueen];
        blockerBB = p.allBitboard & ~p.bitboards[wRook] & ~p.bitboards[wQueen];

        Bitloop(pieceBB) {
            sq = square(SquareOf(pieceBB));
            control += pieceControlValues[wQueen] * __popcnt64(moveGen::rookMoveLookup[sq][_pext_u64(blockerBB, moveGen::rookPextMasks[sq])]);
        }

        pieceBB = p.bitboards[bRook];
        blockerBB = p.allBitboard & ~p.bitboards[bRook] & ~p.bitboards[bQueen];

        Bitloop(pieceBB) {
            sq = square(SquareOf(pieceBB));
            control += pieceControlValues[bQueen] * __popcnt64(moveGen::rookMoveLookup[sq][_pext_u64(blockerBB, moveGen::rookPextMasks[sq])]);
        }

        return control;
    }
}