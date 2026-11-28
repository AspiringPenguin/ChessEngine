#include "core.h"
#include "search.h"

#include <iostream>

namespace search {
	int scoreMove(const move& m, const move& ttMove) {
		if (m == ttMove) {
			return 255;
		}
		return moves::getMVVLVAScore(m);
	}

	move getNextMove(std::vector<move>& moves, int& moveN, const move& ttMove) {
		if (moveN == moves.size()) {
			return -1;
		}

		move _;
		for (int i = 0; i < (moves.size() - 1 - moveN); i++) {
			if (scoreMove(moves[i], ttMove) > scoreMove(moves[i + 1], ttMove)) {
				_ = moves[i + 1];
				moves[i + 1] = moves[i];
				moves[i] = _;
			}
		}
		return moves[(moves.size() - 1 - moveN++)];
	}

	std::tuple<int, int> getIdealAndMaxTimes(int wtime, int btime, int winc, int binc, color toMove) {
		if (toMove == white){
			return { 0.03 * wtime + 0.5 * winc - 100, 0.05 * wtime + winc - 100};
		}
		else {
			return { 0.03 * btime + 0.5 * binc - 100, 0.05 * btime + binc - 100};
		}
	}

	Searcher::Searcher() {
		p = Position();
		init();
	}

	Searcher::Searcher(const std::string& fen) {
		p = Position(fen);
		init();
	}

	void Searcher::init() {
		clearPVTable();
	}

	void Searcher::loadStart() {
		p.loadStart();
	}

	void Searcher::loadFEN(const std::string& fen) {
		p.loadFEN(fen);
	}

	void Searcher::makeMove(const move& m) {
		p.makeMove(m, false);
	}

	color Searcher::getToMove() {
		return p.toMove;
	}

	void Searcher::clearPVTable() {
		for (int i = 0; i < extendedMaxDepth; i++) {
			pvLengthTable[i] = 0;
			for (int j = 0; j < extendedMaxDepth; j++) {
				pvTable[i][j] = (1 << 27); //Null move
			}
		}
	}

	std::string Searcher::asFEN() {
		return p.asFEN();
	}

	void Searcher::generateQuiescencePV() {
		clearPVTable();
		if (p.toMove == white) {
			generateQuiescencePV<white>(-10000, 10000, 0);
		}
		else {
			generateQuiescencePV<black>(-10000, 10000, 0);
		}
	}

	template <color c>
	int Searcher::generateQuiescencePV(int alpha, int beta, int depth) {
		if (p.isDraw()) {
			pvLengthTable[depth] = 0;
			return 0;
		}

		//Repetitions are separate due to tt issues
		const int reps = p.countRepetitions();
		if (reps == 3) {
			pvLengthTable[depth] = 0;
			return 0;
		}

		int score;

		int bestVal = p.evaluate() * p.toMoveSigned; //Get static eval now

		if (bestVal >= beta || depth == maxDepth) {
			pvLengthTable[depth] = 0;
			return bestVal;
		}
		if (bestVal > alpha) {
			pvLengthTable[depth] = 0;
			alpha = bestVal;
		}

		auto moves = p.generatePseudoLegalQuiescenceMoves<c>();

		int moveN = 0;

		for (move move = getNextMove(moves, moveN, -1); move != -1; move = getNextMove(moves, moveN, -1)) {
			p.makeMove(move);
			if (!p.moveWasLegal()) {
				p.undoMove();
				continue;
			}
			score = -generateQuiescencePV<color(1 - c)>(-beta, -alpha, depth + 1);

			p.undoMove();

			if (score >= beta) {
				pvLengthTable[depth] = 0;
				return score;
			}
			if (score > bestVal) {
				bestVal = score;
				if (score > alpha) {
					alpha = score;
					pvTable[depth][0] = move;
					std::copy(std::begin(pvTable[depth + 1]), std::begin(pvTable[depth + 1]) + pvLengthTable[depth + 1], std::begin(pvTable[depth]) + 1);
					pvLengthTable[depth] = pvLengthTable[depth + 1] + 1;
				}
			}
		}

		return bestVal;
	}
}