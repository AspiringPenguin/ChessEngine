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
								 0, -pawnValueStart, -knightValueStart, -bishopValueStart, -rookValueStart, -queenValueStart, 0, 0 };

	int pieceValuesEnd[16] = { 0, pawnValueEnd, knightValueEnd, bishopValueEnd, rookValueEnd, queenValueEnd, 0, 0,
							   0, -pawnValueEnd, -knightValueEnd, -bishopValueEnd, -rookValueEnd, -queenValueEnd, 0, 0 };

	//psqs - https://www.chessprogramming.org/Simplified_Evaluation_Function
	int pawnBonusesStart[64] = { 0,  0,  0,  0,  0,  0,  0,  0,
								50, 50, 50, 50, 50, 50, 50, 50,
								10, 10, 20, 30, 30, 20, 10, 10,
								 5,  5, 10, 25, 25, 10,  5,  5,
								 0,  0,  0, 20, 20,  0,  0,  0,
								 5, -5,-10,  0,  0,-10, -5,  5,
								 5, 10, 10,-20,-20, 10, 10,  5,
								 0,  0,  0,  0,  0,  0,  0,  0 };

	int pawnBonusesEnd[64] = {  0,  0,  0,  0,  0,  0,  0,  0,
								30, 50, 50, 50, 50, 50, 50, 30,
								15, 30, 30, 30, 30, 30, 30, 15,
								 0, 15, 15, 15, 15, 15, 15,  0,
								-5,  5,  5, 5,   5,  5,  5, -5,
							   -10, -5,-10,  0,  0,-10, -5,-10,
							   -20,-20,-20,-20,-20,-20,-20,-20,
								 0,  0,  0,  0,  0,  0,  0,  0 };

	int knightBonusesStart[64] = { -50,-40,-30,-30,-30,-30,-40,-50,
									-40,-20,  0,  0,  0,  0,-20,-40,
									-30,  0, 10, 15, 15, 10,  0,-30,
									-30,  5, 15, 20, 20, 15,  5,-30,
									-30,  0, 15, 20, 20, 15,  0,-30,
									-30,  5, 10, 15, 15, 10,  5,-30,
									-40,-20,  0,  5,  5,  0,-20,-40,
									-50,-40,-30,-30,-30,-30,-40,-50 };

	int knightBonusesEnd[64] = { -50,-40,-30,-30,-30,-30,-40,-50,
									-40,-20,  0,  0,  0,  0,-20,-40,
									-30,  0, 10, 15, 15, 10,  0,-30,
									-30,  5, 15, 20, 20, 15,  5,-30,
									-30,  0, 15, 20, 20, 15,  0,-30,
									-30,  5, 10, 15, 15, 10,  5,-30,
									-40,-20,  0,  5,  5,  0,-20,-40,
									-50,-40,-30,-30,-30,-30,-40,-50 };

	int bishopBonusesStart[64] = { -20,-10,-10,-10,-10,-10,-10,-20,
									-10,  0,  0,  0,  0,  0,  0,-10,
									-10,  0,  5, 10, 10,  5,  0,-10,
									-10,  5,  5, 10, 10,  5,  5,-10,
									-10,  0, 10, 10, 10, 10,  0,-10,
									-10, 10, 10, 10, 10, 10, 10,-10,
									-10,  5,  0,  0,  0,  0,  5,-10,
									-20,-10,-10,-10,-10,-10,-10,-20, };

	int bishopBonusesEnd[64] = { -20,-10,-10,-10,-10,-10,-10,-20,
									-10,  0,  0,  0,  0,  0,  0,-10,
									-10,  0,  5, 10, 10,  5,  0,-10,
									-10,  5,  5, 10, 10,  5,  5,-10,
									-10,  0, 10, 10, 10, 10,  0,-10,
									-10, 10, 10, 10, 10, 10, 10,-10,
									-10,  5,  0,  0,  0,  0,  5,-10,
									-20,-10,-10,-10,-10,-10,-10,-20, };

	int rookBonusesStart[64] = { 0,  0,  0,  0,  0,  0,  0,  0,
								5, 10, 10, 10, 10, 10, 10,  5,
								-5,  0,  0,  0,  0,  0,  0, -5,
								-5,  0,  0,  0,  0,  0,  0, -5,
								-5,  0,  0,  0,  0,  0,  0, -5,
								-5,  0,  0,  0,  0,  0,  0, -5,
								-5,  0,  0,  0,  0,  0,  0, -5,
								0,  0,  0,  5,  5,  0,  0,  0  };

	int rookBonusesEnd[64] = { 0,  0,  0,  0,  0,  0,  0,  0,
								5, 10, 10, 10, 10, 10, 10,  5,
								-5,  0,  0,  0,  0,  0,  0, -5,
								-5,  0,  0,  0,  0,  0,  0, -5,
								-5,  0,  0,  0,  0,  0,  0, -5,
								-5,  0,  0,  0,  0,  0,  0, -5,
								-5,  0,  0,  0,  0,  0,  0, -5,
								0,  0,  0,  5,  5,  0,  0,  0 };

	int queenBonusesStart[64] = { -20,-10,-10, -5, -5,-10,-10,-20,
								-10,  0,  0,  0,  0,  0,  0,-10,
								-10,  0,  5,  5,  5,  5,  0,-10,
								 -5,  0,  5,  5,  5,  5,  0, -5,
								  0,  0,  5,  5,  5,  5,  0, -5,
								-10,  5,  5,  5,  5,  5,  0,-10,
								-10,  0,  5,  0,  0,  0,  0,-10,
								-20,-10,-10, -5, -5,-10,-10,-20 };

	int queenBonusesEnd[64] = { -20,-10,-10, -5, -5,-10,-10,-20,
								-10,  0,  0,  0,  0,  0,  0,-10,
								-10,  0,  5,  5,  5,  5,  0,-10,
								 -5,  0,  5,  5,  5,  5,  0, -5,
								  0,  0,  5,  5,  5,  5,  0, -5,
								-10,  5,  5,  5,  5,  5,  0,-10,
								-10,  0,  5,  0,  0,  0,  0,-10,
								-20,-10,-10, -5, -5,-10,-10,-20 };

	int kingBonusesStart[64] = { -30,-40,-40,-50,-50,-40,-40,-30,
								-30,-40,-40,-50,-50,-40,-40,-30,
								-30,-40,-40,-50,-50,-40,-40,-30,
								-30,-40,-40,-50,-50,-40,-40,-30,
								-20,-30,-30,-40,-40,-30,-30,-20,
								-10,-20,-20,-20,-20,-20,-20,-10,
								 20, 20,  0,  0,  0,  0, 20, 20,
								 20, 30, 10,  0,  0, 10, 30, 20 };

	int kingBonusesEnd[64] = { -50,-40,-30,-20,-20,-30,-40,-50,
								-30,-20,-10,  0,  0,-10,-20,-30,
								-30,-10, 20, 30, 30, 20,-10,-30,
								-30,-10, 30, 40, 40, 30,-10,-30,
								-30,-10, 30, 40, 40, 30,-10,-30,
								-30,-10, 20, 30, 30, 20,-10,-30,
								-30,-30,  0,  0,  0,  0,-30,-30,
								-50,-30,-30,-30,-30,-30,-30,-50 };

	//https://www.chessprogramming.org/Tapered_Eval - however I'm not considering promotions as impacting phase as they are an endgame feature and are likely to be dealt with by search
	constexpr int pawnPhase = 0;
	constexpr int knightPhase = 1;
	constexpr int bishopPhase = 1;
	constexpr int rookPhase = 2;
	constexpr int queenPhase = 4;

	consteval int maxPhase = 16 * pawnPhase + 4 * knightPhase + 4 * bishopPhase + 4 * rookPhase + 2 * queenPhase;

	int piecePhases[16] = { 0, pawnPhase, knightPhase, bishopPhase, rookPhase, queenPhase, 0, 0,
							0, pawnPhase, knightPhase, bishopPhase, rookPhase, queenPhase, 0, 0 };
}