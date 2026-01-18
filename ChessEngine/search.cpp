#include "search.h"

namespace search {
	/* copy-pasted from engine.h
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

	int negamax(int alpha, int beta, int depth, int depthRemaining) {
		if (depthRemaining == 0) {
			nodes++;
			return negamaxQuiescence(alpha, beta, depth);
		}
		else if (isDraw()) {
			nodes++;
			return 0;
		}

		auto ttResult = tt::ttProbe(zobrist, alpha, beta, depthRemaining);
		auto resultType = std::get<0>(ttResult);

		if (resultType == tt::ttScore) {
			return std::get<1>(ttResult);
		}

		const move ttMove = std::get<1>(ttResult);

		int bestVal = -100000;
		int score;
		int legalMoves = 0;
		auto moves = generatePseudoLegalMoves();

		int moveN = 0;

		bool firstMove = true;
		bool raisedAlpha = false;

		for (move move = getNextMove(moves, moveN, ttMove); move != -1; move = getNextMove(moves, moveN, ttMove)) {
			makeMove(move);
			if (!moveWasLegal()) {
				undoMove();
				continue;
			}
			legalMoves++;
			score = -negamax(-beta, -alpha, depth + 1, depthRemaining - 1);
			undoMove();

			if (score > bestVal) {
				bestVal = score;
				if (score > alpha) {
					alpha = score;
					raisedAlpha = true;
				}
			}
			if (score >= beta) {
				tt::ttStore(zobrist, score, move, depthRemaining, tt::lowerBound, firstMove);
				return bestVal;
			}

			firstMove = false;
		}

		if (legalMoves == 0) {
			nodes++;
			if (inCheck()) { //Checkmate
				return (-100000 + depth);
			}
			return 0; //Stalemate
		}

		if (raisedAlpha) { //Exact
			tt::ttStore(zobrist, score, -1, depthRemaining, tt::exact, false);
		}
		else { //Upper bound
			tt::ttStore(zobrist, score, -1, depthRemaining, tt::upperBound, false);
		}

		return bestVal;
	}

	int negamaxQuiescence(int alpha, int beta, int depth) {
		if (isDraw()) {
			nodes++;
			return 0;
		}
		int bestVal = -100000;
		int score;
		int captureMoves = 0;
		auto moves = generatePseudoLegalQuiescenceMoves();

		int moveN = 0;

		for (move move = getNextMove(moves, moveN, -1); move != -1; move = getNextMove(moves, moveN, -1)) {
			makeMove(move);
			if (!moveWasLegal()) {
				undoMove();
				continue;
			}
			captureMoves++;
			score = -negamaxQuiescence(-beta, -alpha, depth + 1);
			undoMove();

			if (score > bestVal) {
				bestVal = score;
				if (score > alpha) {
					alpha = score;
				}
			}
			if (score >= beta) {
				return bestVal;
			}
		}

		if (captureMoves == 0) {
			nodes++;
			return evaluate() * toMoveSigned;
		}

		return bestVal;
	}

	move negamaxSearch(int depth) {
		int bestVal = -100000;
		move bestMove = -1;
		int alpha = -100000;
		int beta = 100000;
		int score;
		int legalMoves = 0;
		auto moves = generatePseudoLegalMoves();

		int moveN = 0;

		for (move move = getNextMove(moves, moveN, -1); move != -1; move = getNextMove(moves, moveN, -1)) {
			makeMove(move);
			if (!moveWasLegal()) {
				undoMove();
				continue;
			}
			legalMoves++;
			score = -negamax(-beta, -alpha, 1, depth - 1);
			undoMove();

			if (score > bestVal) {
				bestVal = score;
				bestMove = move;
				if (score > alpha) {
					alpha = score;
				}
			}
			if (score >= beta) {
				return bestMove;
			}
		}

		if (legalMoves == 0) {
			if (inCheck()) { //Checkmate
				return -1;
			}
			return -1; //Stalemate
		}

		return bestMove;
	}*/
}