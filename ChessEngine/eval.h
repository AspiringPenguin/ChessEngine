#pragma once

namespace eval {
	//https://www.chessprogramming.org/Simplified_Evaluation_Function
	constexpr int pawnValueStart = 100;
	constexpr int pawnValueEnd = 100;
	constexpr int knightValueStart = 320;
	constexpr int knightValueEnd = 320;
	constexpr int bishopValueStart = 330;
	constexpr int bishopValueEnd = 330;
	constexpr int rookValueStart = 500;
	constexpr int rookValueEnd = 500;
	constexpr int queenValueStart = 900;
	constexpr int queenValueEnd = 900;

	int pieceValuesStart[16] = { 0, pawnValueStart, knightValueStart, bishopValueStart, rookValueStart, queenValueStart, 0, 0,
								 0, pawnValueStart, knightValueStart, bishopValueStart, rookValueStart, queenValueStart, 0, 0 };

	int pieceValuesEnd[16] = { 0, pawnValueEnd, knightValueEnd, bishopValueEnd, rookValueEnd, queenValueEnd, 0, 0,
							   0, pawnValueEnd, knightValueEnd, bishopValueEnd, rookValueEnd, queenValueEnd, 0, 0 };

	//https://www.chessprogramming.org/Tapered_Eval
	constexpr int pawnPhase = 0;
	constexpr int knightPhase = 1;
	constexpr int bishopPhase = 1;
	constexpr int rookPhase = 2;
	constexpr int queenPhase = 4;

	int piecePhases[16] = { 0, pawnPhase, knightPhase, bishopPhase, rookPhase, queenPhase, 0, 0,
							0, pawnPhase, knightPhase, bishopPhase, rookPhase, queenPhase, 0, 0 };
}