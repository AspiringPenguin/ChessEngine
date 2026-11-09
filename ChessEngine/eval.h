#pragma once
#include <array>
#include "position.h"

namespace eval {
	//https://www.chessprogramming.org/PeSTO%27s_Evaluation_Function
	extern int pawnValueStart;
	extern int pawnValueEnd;
	extern int knightValueStart;
	extern int knightValueEnd;
	extern int bishopValueStart;
	extern int bishopValueEnd;
	extern int rookValueStart;
	extern int rookValueEnd;
	extern int queenValueStart;
	extern int queenValueEnd;

	extern std::array<int, 16> pieceValuesStart;
	extern std::array<int, 16> pieceValuesEnd;

	//psqs - https://www.chessprogramming.org/Simplified_Evaluation_Function
	//These are A8 = 0, H8=7, A1=56, H1=63

	extern std::array<int, 64> pawnBonusesStart;

	extern std::array<int, 64> pawnBonusesEnd;

	extern std::array<int, 64> knightBonusesStart;

	extern std::array<int, 64> knightBonusesEnd;

	extern std::array<int, 64> bishopBonusesStart;

	extern std::array<int, 64> bishopBonusesEnd;

	extern std::array<int, 64> rookBonusesStart;

	extern std::array<int, 64> rookBonusesEnd;

	extern std::array<int, 64> queenBonusesStart;

	extern std::array<int, 64> queenBonusesEnd;

	extern std::array<int, 64> kingBonusesStart;

	extern std::array<int, 64> kingBonusesEnd;

	extern std::array<std::array<int, 64>, 6> pieceBonusesStart;
	extern std::array<std::array<int, 64>, 6> pieceBonusesEnd;

	//https://www.chessprogramming.org/Tapered_Eval - however I'm not considering promotions as impacting phase as they are an endgame feature and are likely to be dealt with by search
	constexpr int pawnPhase = 0;
	constexpr int knightPhase = 1;
	constexpr int bishopPhase = 1;
	constexpr int rookPhase = 2;
	constexpr int queenPhase = 4;

	const static int maxPhase = pawnPhase * 16 + knightPhase * 4 + bishopPhase * 4 + rookPhase * 4 + queenPhase * 2;

	const static int piecePhases[16] = { 0, pawnPhase, knightPhase, bishopPhase, rookPhase, queenPhase, 0, 0,
										 0, pawnPhase, knightPhase, bishopPhase, rookPhase, queenPhase, 0, 0 };

	extern std::array<int, 16> pieceControlValues;

	extern std::array<int, 7> passedPawnBonuses;

	extern int pawnDefenceBonus;

	std::vector<int> loadEvalTerms(std::string file);

	//Other Functions
	void loadEval();
	std::array<int, 64> generateKnightMoveCounts();

	int getBoardControl(const Position* p);
	U64 passedPawnMask(square sq, color c);
	std::array<std::array<U64, 64>, 2> generatePassedPawnMasks();

	int getPawnEval(const Position* p);

	extern std::array<int, 64> knightMoveCounts;
	extern std::array<std::array<U64, 64>, 2> passedPawnMasks;
}