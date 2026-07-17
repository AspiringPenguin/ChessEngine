#include "book.h"
#include "core.h"
#include "search.h"
#include "tt.h"

#include <iostream>
#include <random>

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

	SearchNode::SearchNode() {
		p = Position();
		positiveSide = white;
		init();
	}

	SearchNode::SearchNode(const std::string& fen) {
		p = Position(fen);
		positiveSide = p.toMove;
		init();
	}

	SearchNode::SearchNode(SearchNode const & toCopy) {
		p = Position(toCopy.p);
		positiveSide = toCopy.positiveSide;
		init();
	}

	void SearchNode::init() {
		nodes = 0;
		ideal = 0;
		max = 0;
	}

	void SearchNode::loadStart() {
		p.loadStart();
		positiveSide = white;
	}

	void SearchNode::loadFEN(const std::string& fen) {
		p.loadFEN(fen);
		positiveSide = p.toMove;
	}

	void SearchNode::makeMove(const move& m) {
		p.makeMove(m, false);
	}

	void SearchNode::showPosition() {
		p.showPosition();
	}

	color SearchNode::getToMove() {
		return p.toMove;
	}

	move SearchNode::UCIMoveAsInternal(const std::string& move) {
		return p.UCIMoveAsInternal(move);
	}

	int SearchNode::getNodes() {
		return nodes;
	}

	void SearchNode::resetNodes() {
		nodes = 0;
	}

	int SearchNode::getExtensions(int extensionsCount) {
		if (extensionsCount == 16) {
			return 0;
		}
		return p.inCheck() ? 1 : 0;
	}

	template <color c, nodeType nType, bool useTTScore>
	int SearchNode::negamax(int alpha, int beta, int depth, int depthRemaining, int extensionsCount) {
		if (depthRemaining == 0) {
			return negamaxQuiescence<c>(alpha, beta, depth);
		}
		else if (p.isDraw()) {
			nodes++;
			return 0;
		}

		const int reps = p.countRepetitions();
		if constexpr (useTTScore) {
			if (reps == 3) {
				nodes++;
				return 0;
			}
			else if (reps == 2) {
				return negamax<c, nType, false>(alpha, beta, depth, depthRemaining, extensionsCount);
			}
		}
		else {
			if (reps == 3) {
				nodes++;
				return 0;
			}
		}

		auto ttResult = tt::ttProbe(p.zobrist, alpha, beta, depthRemaining);
		auto resultType = std::get<0>(ttResult);
		const auto ttMove = std::get<2>(ttResult);

		if constexpr (useTTScore) {
			const int ttScore = std::get<1>(ttResult);
			if (resultType == tt::ttScore) {
				if (p.moveIsValid(ttMove)) {
					return ttScore;
				}
			}
			if constexpr (nType == NonPV) {
				//Futility pruning using TT score from a ply less
				if (resultType == tt::ttMaybeScore) {
					if (p.moveIsValid(ttMove)) {
						if (ttScore > (beta + 250)) {
							return ttScore;
						}
					}
				}
			}
		}

		int bestVal = -10000;
		move bestMove = -1;
		int score;
		int legalMoves = 0;
		auto moves = p.generatePseudoLegalMoves<c>();

		int moveN = 0;

		bool firstMove = true;
		bool raisedAlpha = false;

		int extensions;

		for (move move = getNextMove(moves, moveN, ttMove); move != -1; move = getNextMove(moves, moveN, ttMove)) {
			p.makeMove(move);
			if (!p.moveWasLegal()) {
				p.undoMove();
				continue;
			}
			legalMoves++;

			extensions = getExtensions(extensionsCount);

			if constexpr (nType == PV) {
				if (firstMove) {
					score = -negamax<color(1 - c), PV, useTTScore>(-beta, -alpha, depth + 1, depthRemaining - 1 + extensions, extensionsCount + extensions);
				}
				else {
					score = -negamax<color(1 - c), NonPV, useTTScore>(-alpha - 1, -alpha, depth + 1, depthRemaining - 1 + extensions, extensionsCount + extensions);
					if (alpha < score && score < beta) {
						score = -negamax<color(1 - c), NonPV, useTTScore>(-beta, -alpha, depth + 1, depthRemaining - 1 + extensions, extensionsCount + extensions);
					}
				}
			}
			else {
				score = -negamax<color(1 - c), NonPV, useTTScore>(-alpha - 1, -alpha, depth + 1, depthRemaining - 1 + extensions, extensionsCount + extensions);
				if (alpha < score && score < beta) {
					score = -negamax<color(1 - c), NonPV, useTTScore>(-beta, -alpha, depth + 1, depthRemaining - 1 + extensions, extensionsCount + extensions);
				}
			}
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

	template <color c>
	int SearchNode::negamaxQuiescence(int alpha, int beta, int depth) {
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

		int score;
		int captureMoves = 0;

		int bestVal = p.evaluate() * p.toMoveSigned; //Get static eval now
		nodes++; //Have called static eval so increment
		if (bestVal >= beta || depth == maxDepth) {
			return bestVal;
		}
		if (bestVal > alpha) {
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
			captureMoves++;
			score = -negamaxQuiescence<color(1 - c)>(-beta, -alpha, depth + 1);

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

		return bestVal;
	}

	template<color c>
	void SearchNode::generateChildren() {
		auto moves = p.generatePseudoLegalMoves<c>();

		for (move m : moves) {
			p.makeMove(m);
			if (p.moveWasLegal()) {
				children.push_back(std::make_shared<SearchNode>((SearchNode const &) *this));
			}
			p.undoMove();
		}
	}

	template<color c>
	int SearchNode::selectExpandBackpropogate()
	{
		//Special cases
		if (children.size() == 0) {
			return 0;
		}

		//Normal case
		return 0;
	}

	//To avoid compiler errors
	template move SearchNode::go<white>(int wtime, int btime, int winc, int binc, bool useBook);
	template move SearchNode::go<black>(int wtime, int btime, int winc, int binc, bool useBook);

	//High level search
	template <color c>
	move SearchNode::go(int wtime, int btime, int winc, int binc, bool useBook) {
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

		auto moves = p.generatePseudoLegalMoves<c>();

		//For pseudo-random numbers
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dist(0, 1);

		int moveNum = std::floor(dist(gen) * moves.size());

		move m;

		while (true) {
			m = moves[moveNum];
			p.makeMove(m);
			if (p.moveWasLegal()) {
				p.undoMove();
				return m;
			}
			p.undoMove();
			moveNum++;
			moveNum %= moves.size();
		}

		return -1;
	}
}