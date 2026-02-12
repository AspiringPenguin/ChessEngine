#pragma once
#include "core.h"
#include <array>

constexpr int numPositions = 100 + maxDepth; //So we still have accurate history after making and unmaking lots of moves

class Position {
protected:
	move moves[maxDepth]{};
	int counters[maxDepth]{}; //For resetting of 50 move clock - the counter before the matching move was applied
	move lastMove; //Keep the last move at the root here - the last irreversible move

	int counter = 0; //For the 50-move rule, add/subtract 1 every 'half-move', draw when >= 100

	U64 positions[numPositions]; //Use circularly to store last 100 positions' zobrist hashes
	int positionsHead = 0; //First position, inclusive
	int positionsTail = 0; //Last position, exclusive
	//If head == (tail) all mod numPositions increment both and overwrite 

	int moveNum = -1;

	//Eval stuff
	int phase;
	int materialStart;
	int materialEnd;
	int bonusesStart;
	int bonusesEnd;

	//Castling rights
	bool wKingside = true;
	bool wQueenside = true;
	bool bKingside = true;
	bool bQueenside = true;

	//For enPassant
	square enPassantSquare = nullSquare;

	void updateBitboards();

	void calculatePhase();
	void calculateMaterialStart();
	void calculateMaterialEnd();

	void calculateBonusesStart();
	void calculateBonusesEnd();
public:
	std::array<piece, 64> mailbox{ }; //Initialise as 0s = empty
	U64 bitboards[16]{ }; //Initialise as 0s
	U64 colorBitboards[2]{ }; //0 = white, 1 = black
	U64 allBitboard = 0;

	U64 zobrist = 0;

	color toMove;
	int toMoveSigned;

	Position();
	Position(const std::string& fen);

	void showPosition(color perspective = white);

	void loadStart();
	void loadFEN(const std::string& fen);

	void makeMove(const move& m);
	void makeMove(const move& m, bool reversible);
	void undoMove();

	int countRepetitions();

	bool isDraw();

	std::vector<move> generatePseudoLegalMoves();
	template <color c> std::vector<move> generatePseudoLegalMoves();
	std::vector<move> generatePseudoLegalQuiescenceMoves();

	U64 getAttacked(color by);

	bool moveWasLegal();
	bool inCheck();
	bool castleWasLegal();

	move UCIMoveAsInternal(std::string m);

	bool moveIsValid(const move& m);

	int evaluate();
};