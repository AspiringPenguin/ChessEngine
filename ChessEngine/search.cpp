#include "book.h"
#include "search.h"
#include "tt.h"

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
			return { 0.02 * wtime + 0.5 * winc - 100, 0.04 * wtime + winc - 100};
		}
		else {
			return { 0.02 * btime + 0.5 * binc - 100, 0.04 * btime + binc - 100};
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
		nodes = 0;
		ideal = 0;
		max = 0;
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

	int Searcher::getExtensions() {
		return p.inCheck() ? 1 : 0;
	}

	int Searcher::negamax(int alpha, int beta, int depth, int depthRemaining) {
		if (depthRemaining == 0) {
			return negamaxQuiescence(alpha, beta, depth);
		}
		else if (p.isDraw()) {
			nodes++;
			return 0;
		}

		const int reps = p.countRepetitions();
		if (reps == 3) {
			nodes++;
			return 0;
		}
		else if (reps == 2) {
			return negamaxNoTT(alpha, beta, depth, depthRemaining);
		}

		auto ttResult = tt::ttProbe(p.zobrist, alpha, beta, depthRemaining);
		auto resultType = std::get<0>(ttResult);

		const auto ttMove = std::get<2>(ttResult);

		if (resultType == tt::ttScore) {
			if (p.moveIsValid(ttMove)) {
				return std::get<1>(ttResult);
			}
		}

		int bestVal = -10000;
		move bestMove = -1;
		int score;
		int legalMoves = 0;
		auto moves = p.generatePseudoLegalMoves();

		int moveN = 0;

		bool firstMove = true;
		bool raisedAlpha = false;

		for (move move = getNextMove(moves, moveN, ttMove); move != -1; move = getNextMove(moves, moveN, ttMove)) {
			p.makeMove(move);
			if (!p.moveWasLegal()) {
				p.undoMove();
				continue;
			}
			legalMoves++;
			score = -negamax(-beta, -alpha, depth + 1, depthRemaining - 1 + getExtensions());
			p.undoMove();

			//Check here as the search may have been interrupted
			if ((nodes & 0xFFF) == 0) {
				if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() > max && depth != 1) {
					break;
				}
			}

			if (score >= beta) {
				tt::ttStore(p.zobrist, score, move, depthRemaining, tt::lowerBound, firstMove);
				return score;
			}
			if (score > bestVal) {
				bestVal = score;
				bestMove = move;
				if (score > alpha) {
					alpha = score;
					raisedAlpha = true;
				}
			}

			firstMove = false;
		}

		if (legalMoves == 0) {
			nodes++;
			if (p.inCheck()) { //Checkmate
				return (-10000 + depth);
			}
			return 0; //Stalemate
		}

		if (raisedAlpha) { //Exact
			tt::ttStore(p.zobrist, bestVal, bestMove, depthRemaining, tt::exact, false);
		}
		else { //Upper bound
			tt::ttStore(p.zobrist, bestVal, bestMove, depthRemaining, tt::upperBound, false);
		}

		return bestVal;
	}

	int Searcher::negamaxNoTT(int alpha, int beta, int depth, int depthRemaining) {
		if (depthRemaining == 0) {
			return negamaxQuiescence(alpha, beta, depth);
		}
		else if (p.isDraw()) {
			nodes++;
			return 0;
		}

		const int reps = p.countRepetitions();
		if (reps == 3) {
			nodes++;
			return 0;
		}

		auto ttResult = tt::ttProbe(p.zobrist, alpha, beta, depthRemaining);
		auto resultType = std::get<0>(ttResult);

		const auto ttMove = std::get<2>(ttResult);

		int bestVal = -10000;
		move bestMove = -1;
		int score;
		int legalMoves = 0;
		auto moves = p.generatePseudoLegalMoves();

		int moveN = 0;

		bool firstMove = true;
		bool raisedAlpha = false;

		for (move move = getNextMove(moves, moveN, ttMove); move != -1; move = getNextMove(moves, moveN, ttMove)) {
			p.makeMove(move);
			if (!p.moveWasLegal()) {
				p.undoMove();
				continue;
			}
			legalMoves++;
			score = -negamaxNoTT(-beta, -alpha, depth + 1, depthRemaining - 1 + getExtensions());
			p.undoMove();

			//Check here as the search may have been interrupted
			if ((nodes & 0xFFF) == 0) {
				if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() > max && depth != 1) {
					break;
				}
			}

			if (score >= beta) {
				return score;
			}
			if (score > bestVal) {
				bestVal = score;
				bestMove = move;
				if (score > alpha) {
					alpha = score;
					raisedAlpha = true;
				}
			}

			firstMove = false;
		}

		if (legalMoves == 0) {
			nodes++;
			if (p.inCheck()) { //Checkmate
				return (-10000 + depth);
			}
			return 0; //Stalemate
		}

		return bestVal;
	}

	int Searcher::negamaxQuiescence(int alpha, int beta, int depth) {
		if (p.isDraw()) {
			nodes++;
			return 0;
		}

		//Repetitions are separate due to tt issues
		const int reps = p.countRepetitions();
		if (reps == 3) {
			nodes++;
			return 0;
		}

		int bestVal = -10000;
		int score;
		int captureMoves = 0;

		score = p.evaluate() * p.toMoveSigned; //Get static eval now
		nodes++; //Have called static eval so increment
		if (score >= beta || depth == maxDepth) {
			return score;
		}
		if (score > bestVal) {
			bestVal = score;
			if (score > alpha) {
				alpha = score;
			}
		}

		auto moves = p.generatePseudoLegalQuiescenceMoves();

		int moveN = 0;

		for (move move = getNextMove(moves, moveN, -1); move != -1; move = getNextMove(moves, moveN, -1)) {
			p.makeMove(move);
			if (!p.moveWasLegal()) {
				p.undoMove();
				continue;
			}
			captureMoves++;
			score = -negamaxQuiescence(-beta, -alpha, depth + 1);
			p.undoMove();

			//Check here as the search may have been interrupted
			if ((nodes & 0xFFF) == 0) {
				if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() > max && depth != 1) {
					break;
				}
			}

			if (score >= beta) {
				return score;
			}
			if (score > bestVal) {
				bestVal = score;
				if (score > alpha) {
					alpha = score;
				}
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
		if (useBook) {
			if (book::book.contains(p.zobrist)) {
				std::string stringMove = book::chooseMove(book::book[p.zobrist]);
				return p.UCIMoveAsInternal(stringMove);
			}
		}

		auto res = getIdealAndMaxTimes(wtime, btime, winc, binc, p.toMove);

		ideal = std::get<0>(res);
		max = std::get<1>(res);

		start = std::chrono::high_resolution_clock::now();
		int depth = 0;
		move bestMove = -1;

		auto moves = p.generatePseudoLegalMoves();

		int timeSearched = 0;

		//While elapsed time is less than the ideal search time
		while (timeSearched < ideal || depth == 0) {
			depth++; 
			
			if (depth > maxDepth) {
				*stop = true;
				break;
			}

			std::cout << "info depth " << depth << std::endl;

			move _bestMove = -1;
			int bestVal = -10001;

			int moveN = 0;

			int score;

			int alpha = -10000;
			int beta = 10000;

			int legalMoves = 0;

			for (move move = getNextMove(moves, moveN, bestMove); move != -1; move = getNextMove(moves, moveN, bestMove)) {
				if (*stop) {
					break;
				}

				p.makeMove(move);
				if (!p.moveWasLegal()) {
					p.undoMove();
					continue;
				}

				legalMoves++;
				score = -negamax(-beta, -alpha, 1, depth - 1);
				p.undoMove(); 
				
				//Check here as the search may have been interrupted
				timeSearched = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();

				if (timeSearched > max && depth != 1) {
					*stop = true;
					break;
				}

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

				//get time searched
				timeSearched = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();

				if (bestVal > 9900 || bestVal < -9900) {
					std::cout << "info score mate " << (bestVal == std::abs(bestVal) ? ((-std::abs(bestVal) + 10000)/2 + 1) : (std::abs(bestVal) - 10000)/2) << " depth " << depth << " nodes " << nodes << " time " << timeSearched << " pv ";
					moves::showMove(bestMove);
				}
				else {
					std::cout << "info score cp " << bestVal << " depth " << depth << " nodes " << nodes << " time " << timeSearched << " pv ";
					moves::showMove(bestMove);
				}
			}
		}
		
		return bestMove;
	}
}