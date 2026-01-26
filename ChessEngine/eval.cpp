#include "eval.h"

#include "bitboards.h"
#include <intrin.h>

namespace eval {
    int getBoardControl(const Position& p) {
        int control = 0;

        //Pawns - capture squares
        //B - G files
        control += 2 * pieceControlValues[wPawn] * __popcnt64(p.bitboards[wPawn] & ~(bitboards::AFile | bitboards::HFile));
        control += 2 * pieceControlValues[bPawn] * __popcnt64(p.bitboards[bPawn] & ~(bitboards::AFile | bitboards::HFile));

        //A and H
        control += pieceControlValues[wPawn] * __popcnt64(p.bitboards[wPawn] & (bitboards::AFile | bitboards::HFile));
        control += pieceControlValues[bPawn] * __popcnt64(p.bitboards[bPawn] & (bitboards::AFile | bitboards::HFile));

        //Knights

        return control;
    }
}