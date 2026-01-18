#include "search.h"
#include "tt.h"

#include <chrono>
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
			return { 0.03 * wtime + 0.5 * winc, 0.05 * wtime + winc };
		}
		else {
			return { 0.03 * btime + 0.5 * binc, 0.05 * btime + binc };
		}
	}

	Searcher::Searcher() {
		p = Position();
	}

	Searcher::Searcher(const std::string& fen) {
		p = Position(fen);
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

	void Searcher::showPosition() {
		p.showPosition();
	}

	move Searcher::UCIMoveAsInternal(const std::string& move) {
		return p.UCIMoveAsInternal(move);
	}

	int Searcher::getNodes() {
		return nodes;
	}

	void Searcher::resetNodes() {
		nodes = 0;
	}

	int Searcher::negamax(int alpha, int beta, int depth, int depthRemaining) {
		if (depthRemaining == 0) {
			nodes++;
			return negamaxQuiescence(alpha, beta, depth);
		}
		else if (p.isDraw()) {
			nodes++;
			return 0;
		}

		auto ttResult = tt::ttProbe(p.zobrist, alpha, beta, depthRemaining);
		auto resultType = std::get<0>(ttResult);

		if (resultType == tt::ttScore) {
			return std::get<1>(ttResult);
		}

		const move ttMove = std::get<1>(ttResult);

		int bestVal = -100000;
		int score;
		int legalMoves = 0;
		auto moves = p.generatePseudoLegalMoves();

		int moveN = 0;

		bool firstMove = true;
		bool raisedAlpha = false;

		for (move move = getNextMove(moves, moveN, ttMove); move != -1; move = getNextMove(moves, moveN, ttMove)) {
			makeMove(move);
			if (!p.moveWasLegal()) {
				p.undoMove();
				continue;
			}
			legalMoves++;
			score = -negamax(-beta, -alpha, depth + 1, depthRemaining - 1);
			p.undoMove();

			if (score > bestVal) {
				bestVal = score;
				if (score > alpha) {
					alpha = score;
					raisedAlpha = true;
				}
			}
			if (score >= beta) {
				tt::ttStore(p.zobrist, score, move, depthRemaining, tt::lowerBound, firstMove);
				return bestVal;
			}

			firstMove = false;
		}

		if (legalMoves == 0) {
			nodes++;
			if (p.inCheck()) { //Checkmate
				return (-100000 + depth);
			}
			return 0; //Stalemate
		}

		if (raisedAlpha) { //Exact
			tt::ttStore(p.zobrist, score, -1, depthRemaining, tt::exact, false);
		}
		else { //Upper bound
			tt::ttStore(p.zobrist, score, -1, depthRemaining, tt::upperBound, false);
		}

		return bestVal;
	}

	int Searcher::negamaxQuiescence(int alpha, int beta, int depth) {
		if (p.isDraw()) {
			nodes++;
			return 0;
		}
		int bestVal = -100000;
		int score;
		int captureMoves = 0;
		auto moves = p.generatePseudoLegalQuiescenceMoves();

		int moveN = 0;

		for (move move = getNextMove(moves, moveN, -1); move != -1; move = getNextMove(moves, moveN, -1)) {
			makeMove(move);
			if (!p.moveWasLegal()) {
				p.undoMove();
				continue;
			}
			captureMoves++;
			score = -negamaxQuiescence(-beta, -alpha, depth + 1);
			p.undoMove();

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
			return p.evaluate() * p.toMoveSigned;
		}

		return bestVal;
	}

	//High level search
	move Searcher::go(int wtime, int btime, int winc, int binc, bool* stop, bool useBook) {
		auto [ideal, max] = getIdealAndMaxTimes(wtime, btime, winc, binc, p.toMove);

		auto start = std::chrono::high_resolution_clock::now();
		int depth = 0;
		move bestMove = -1;
		//While elapsed time is less than the ideal search time
		while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() < ideal) {
			depth++;

			std::cout << "info depth " << depth << std::endl;

			move _bestMove = -1;
			int bestVal = -10000;

			auto moves = p.generatePseudoLegalMoves();
			int moveN = 0;

			int score;

			int alpha = -10000;
			int beta = 10000;

			int legalMoves = 0;

			for (move move = getNextMove(moves, moveN, bestMove); move != -1; move = getNextMove(moves, moveN, bestMove)) {
				if (*stop) {
					break;
				}
				else if (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count() < max) {
					*stop = true;
					break;
				}

				makeMove(move);
				if (!p.moveWasLegal()) {
					p.undoMove();
					continue;
				}
				legalMoves++;
				score = -negamax(-beta, -alpha, depth + 1, depth - 1);
				p.undoMove();

				if (score > bestVal) {
					bestVal = score;
					_bestMove = move;
					if (score > alpha) {
						alpha = score;
					}
				}
			}

			if (!(*stop)) { //if we weren't interrupted
				bestMove = _bestMove; //update best move

				std::cout << "info score cp " << bestVal * p.toMoveSigned << " depth " << depth << " nodes " << nodes << std::endl;
			}
		}
		
		return bestMove;
	}
}