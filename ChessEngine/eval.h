#pragma once
#include <array>
#include "position.h"

namespace eval {
	//https://www.chessprogramming.org/Simplified_Evaluation_Function
	constexpr int pawnValueStart = 100;
	constexpr int pawnValueEnd = 100;
	constexpr int knightValueStart = 330;
	constexpr int knightValueEnd = 310;
	constexpr int bishopValueStart = 310;
	constexpr int bishopValueEnd = 330;
	constexpr int rookValueStart = 500;
	constexpr int rookValueEnd = 500;
	constexpr int queenValueStart = 900;
	constexpr int queenValueEnd = 900;

	static int pieceValuesStart[16] = { 0, pawnValueStart, knightValueStart, bishopValueStart, rookValueStart, queenValueStart, 0, 0,
								 0, -pawnValueStart, -knightValueStart, -bishopValueStart, -rookValueStart, -queenValueStart, 0, 0 };

	static int pieceValuesEnd[16] = { 0, pawnValueEnd, knightValueEnd, bishopValueEnd, rookValueEnd, queenValueEnd, 0, 0,
							   0, -pawnValueEnd, -knightValueEnd, -bishopValueEnd, -rookValueEnd, -queenValueEnd, 0, 0 };

	//psqs - https://www.chessprogramming.org/Simplified_Evaluation_Function
	//These are A8 = 0, H8=7, A1=56, H1=63

	static std::array<int, 64> pawnBonusesStart = { 0,  0,  0,  0,  0,  0,  0,  0,
													50, 50, 50, 50, 50, 50, 50, 50,
													10, 10, 20, 30, 30, 20, 10, 10,
													 5,  5, 10, 25, 25, 10,  5,  5,
													 0,  0,  0, 20, 20,  0,  0,  0,
													 5, -5,-10,  0,  0,-10, -5,  5,
													 5, 10, 10,-20,-20, 10, 10,  5,
													 0,  0,  0,  0,  0,  0,  0,  0 };

	static std::array<int, 64> pawnBonusesEnd = {  0,  0,  0,  0,  0,  0,  0,  0,
												30, 50, 50, 50, 50, 50, 50, 30,
												15, 30, 30, 30, 30, 30, 30, 15,
													0, 15, 15, 15, 15, 15, 15,  0,
												-5,  5,  5, 5,   5,  5,  5, -5,
												-10, -5,-10,  0,  0,-10, -5,-10,
												-20,-20,-20,-20,-20,-20,-20,-20,
													0,  0,  0,  0,  0,  0,  0,  0 };

	static std::array<int, 64> knightBonusesStart = { -50,-40,-30,-30,-30,-30,-40,-50,
													-40,-20,  0,  0,  0,  0,-20,-40,
													-30,  0, 10, 15, 15, 10,  0,-30,
													-30,  5, 15, 20, 20, 15,  5,-30,
													-30,  0, 15, 20, 20, 15,  0,-30,
													-30,  5, 10, 15, 15, 10,  5,-30,
													-40,-20,  0,  5,  5,  0,-20,-40,
													-50,-40,-30,-30,-30,-30,-40,-50 };

	static std::array<int, 64> knightBonusesEnd = { -50,-40,-30,-30,-30,-30,-40,-50,
													-40,-20,  0,  0,  0,  0,-20,-40,
													-30,  0, 10, 15, 15, 10,  0,-30,
													-30,  5, 15, 20, 20, 15,  5,-30,
													-30,  0, 15, 20, 20, 15,  0,-30,
													-30,  5, 10, 15, 15, 10,  5,-30,
													-40,-20,  0,  5,  5,  0,-20,-40,
													-50,-40,-30,-30,-30,-30,-40,-50 };

	static std::array<int, 64> bishopBonusesStart = { -20,-10,-10,-10,-10,-10,-10,-20,
														-10,  0,  0,  0,  0,  0,  0,-10,
														-10,  0,  5, 10, 10,  5,  0,-10,
														-10,  5,  5, 10, 10,  5,  5,-10,
														-10,  0, 10, 10, 10, 10,  0,-10,
														-10, 10, 10, 10, 10, 10, 10,-10,
														-10,  5,  0,  0,  0,  0,  5,-10,
														-20,-10,-10,-10,-10,-10,-10,-20, };

	static std::array<int, 64> bishopBonusesEnd = { -20,-10,-10,-10,-10,-10,-10,-20,
													-10,  0,  0,  0,  0,  0,  0,-10,
													-10,  0,  5, 10, 10,  5,  0,-10,
													-10,  5,  5, 10, 10,  5,  5,-10,
													-10,  0, 10, 10, 10, 10,  0,-10,
													-10, 10, 10, 10, 10, 10, 10,-10,
													-10,  5,  0,  0,  0,  0,  5,-10,
													-20,-10,-10,-10,-10,-10,-10,-20, };

	static std::array<int, 64> rookBonusesStart = { 0,  0,  0,  0,  0,  0,  0,  0,
													5, 10, 10, 10, 10, 10, 10,  5,
													-5,  0,  0,  0,  0,  0,  0, -5,
													-5,  0,  0,  0,  0,  0,  0, -5,
													-5,  0,  0,  0,  0,  0,  0, -5,
													-5,  0,  0,  0,  0,  0,  0, -5,
													-5,  0,  0,  0,  0,  0,  0, -5,
													0,  0,  0,  5,  5,  0,  0,  0  };

	static std::array<int, 64> rookBonusesEnd = { 0,  0,  0,  0,  0,  0,  0,  0,
												5, 10, 10, 10, 10, 10, 10,  5,
												-5,  0,  0,  0,  0,  0,  0, -5,
												-5,  0,  0,  0,  0,  0,  0, -5,
												-5,  0,  0,  0,  0,  0,  0, -5,
												-5,  0,  0,  0,  0,  0,  0, -5,
												-5,  0,  0,  0,  0,  0,  0, -5,
												0,  0,  0,  5,  5,  0,  0,  0 };

	static std::array<int, 64> queenBonusesStart = { -20,-10,-10, -5, -5,-10,-10,-20,
													-10,  0,  0,  0,  0,  0,  0,-10,
													-10,  0,  5,  5,  5,  5,  0,-10,
														-5,  0,  5,  5,  5,  5,  0, -5,
														0,  0,  5,  5,  5,  5,  0, -5,
													-10,  5,  5,  5,  5,  5,  0,-10,
													-10,  0,  5,  0,  0,  0,  0,-10,
													-20,-10,-10, -5, -5,-10,-10,-20 };

	static std::array<int, 64> queenBonusesEnd = { -20,-10,-10, -5, -5,-10,-10,-20,
													-10,  0,  0,  0,  0,  0,  0,-10,
													-10,  0,  5,  5,  5,  5,  0,-10,
													-5,  0,  5,  5,  5,  5,  0, -5,
													0,  0,  5,  5,  5,  5,  0, -5,
													-10,  5,  5,  5,  5,  5,  0,-10,
													-10,  0,  5,  0,  0,  0,  0,-10,
													-20,-10,-10, -5, -5,-10,-10,-20 };

	static std::array<int, 64> kingBonusesStart = { -30,-40,-40,-50,-50,-40,-40,-30,
													-30,-40,-40,-50,-50,-40,-40,-30,
													-30,-40,-40,-50,-50,-40,-40,-30,
													-30,-40,-40,-50,-50,-40,-40,-30,
													-20,-30,-30,-40,-40,-30,-30,-20,
													-10,-20,-20,-20,-20,-20,-20,-10,
													20, 20,  0,  0,  0,  0, 20, 20,
													20, 30, 10,  0,  0, 10, 30, 20 };

	static std::array<int, 64> kingBonusesEnd = { -50,-40,-30,-20,-20,-30,-40,-50,
													-30,-20,-10,  0,  0,-10,-20,-30,
													-30,-10, 20, 30, 30, 20,-10,-30,
													-30,-10, 30, 40, 40, 30,-10,-30,
													-30,-10, 30, 40, 40, 30,-10,-30,
													-30,-10, 20, 30, 30, 20,-10,-30,
													-30,-30,  0,  0,  0,  0,-30,-30,
													-50,-30,-30,-30,-30,-30,-30,-50 };

	static std::array<int, 64> pieceBonusesStart[6] = { pawnBonusesStart, knightBonusesStart, bishopBonusesStart, rookBonusesStart, queenBonusesStart, kingBonusesStart};
	static std::array<int, 64> pieceBonusesEnd[6] = { pawnBonusesEnd, knightBonusesEnd, bishopBonusesEnd, rookBonusesEnd, queenBonusesEnd, kingBonusesEnd };

	//https://www.chessprogramming.org/Tapered_Eval - however I'm not considering promotions as impacting phase as they are an endgame feature and are likely to be dealt with by search
	constexpr int pawnPhase = 0;
	constexpr int knightPhase = 1;
	constexpr int bishopPhase = 1;
	constexpr int rookPhase = 2;
	constexpr int queenPhase = 4;

	static int maxPhase = 16 * pawnPhase + 4 * knightPhase + 4 * bishopPhase + 4 * rookPhase + 2 * queenPhase;

	static int piecePhases[16] = { 0, pawnPhase, knightPhase, bishopPhase, rookPhase, queenPhase, 0, 0,
							0, pawnPhase, knightPhase, bishopPhase, rookPhase, queenPhase, 0, 0 };

	static int pieceControlValues[16] = { 0, 9, 7, 6, 4, 2, 0, 0, 0, -9, -7, -6, -4, -2, 0, 0 };

	std::array<int, 64> generateKnightMoveCounts();

	static std::array<int, 64> knightMoveCounts = generateKnightMoveCounts();

	int getBoardControl(const Position* p);

	//										   0  1  2   3   4    5    6
	static std::array<int, 7> nPawnMaluses = { 0, 0, 40, 80, 180, 350, 800 };

	//															    Defends: 0   1   2
	static std::array<int, 3> pawnAttackDefendValues[3] = { /*0 defenders*/ {0,  0 , -10},
															/*1 defender */ {5,  10, 0},
															/*2 defenders*/ {10, 20, 15}};

	//											    1  2   3   4   5   6    7
	static std::array<int, 7> passedPawnBonuses = { 0, 10, 20, 40, 80, 140, 200 };

	U64 passedPawnMask(square sq, color c);
	std::array<std::array<U64, 64>, 2> generatePassedPawnMasks();

	static auto passedPawnMasks = generatePassedPawnMasks();

	int getPawnEval(const Position* p);
}