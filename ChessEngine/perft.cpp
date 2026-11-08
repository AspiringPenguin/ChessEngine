#include "perft.h"
#include "moves.h"
#include <chrono>
#include <iostream>

namespace perft {
	PerftSearcher::PerftSearcher() {
		p = Position();
	}

	PerftSearcher::PerftSearcher(const std::string& fen) {
		p = Position(fen);
	}

	int PerftSearcher::go(int depth, bool debug) {
		if (p.toMove == white) {
			return perft<white>(depth, debug); //For handling toMove remplates in future
		}
		else {
			return perft<black>(depth, debug); //For handling toMove remplates in future
		}
	}

	template <color c> int PerftSearcher::perft(int depthRemaining, bool top) {
		if (depthRemaining == 0) {
			return 1;
		}

		U64 nodes = 0;
		U64 _nodes;

		auto moves = p.generatePseudoLegalMoves<c>();

		for (auto& move : moves) {
			p.makeMove(move);
			if (p.moveWasLegal()) {
				_nodes = perft<color(1 - c)>(depthRemaining - 1, false);
				if (top) {
					moves::showMove(move);
					std::cout << _nodes << std::endl;
				}
				nodes += _nodes;
			}

			p.undoMove();
		}

		return nodes;
	}

	int perftMain() {
        U64 res;

        perft::PerftSearcher p;

        std::cout << "Starting position:" << std::endl;
        p = perft::PerftSearcher();
        for (int i = 1; i < 7; i++) {
            auto start = std::chrono::high_resolution_clock::now();
            res = p.go(i, false);
            auto end = std::chrono::high_resolution_clock::now();
            auto ms_int = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << i << " " << res << " " << (ms_int.count() / (1000000.0f)) << "s " << static_cast<int>(res / (ms_int.count() / (1000000.0f))) << "nps" << std::endl;
        }

        std::cout << std::endl << "Kiwipete:" << std::endl;
        p = perft::PerftSearcher("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
        for (int i = 1; i < 6; i++) {
            auto start = std::chrono::high_resolution_clock::now();
            res = p.go(i, false);
            auto end = std::chrono::high_resolution_clock::now();
            auto ms_int = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << i << " " << res << " " << (ms_int.count() / (1000000.0f)) << "s " << static_cast<int>(res / (ms_int.count() / (1000000.0f))) << "nps" << std::endl;
        }

        std::cout << std::endl << "Position 3:" << std::endl;
        p = perft::PerftSearcher("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
        for (int i = 1; i < 8; i++) {
            auto start = std::chrono::high_resolution_clock::now();
            res = p.go(i, false);
            auto end = std::chrono::high_resolution_clock::now();
            auto ms_int = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << i << " " << res << " " << (ms_int.count() / (1000000.0f)) << "s " << static_cast<int>(res / (ms_int.count() / (1000000.0f))) << "nps" << std::endl;
        }

        std::cout << std::endl << "Position 4:" << std::endl;
        p = perft::PerftSearcher("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
        for (int i = 1; i < 6; i++) {
            auto start = std::chrono::high_resolution_clock::now();
            res = p.go(i, false);
            auto end = std::chrono::high_resolution_clock::now();
            auto ms_int = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << i << " " << res << " " << (ms_int.count() / (1000000.0f)) << "s " << static_cast<int>(res / (ms_int.count() / (1000000.0f))) << "nps" << std::endl;
        }

        std::cout << std::endl << "Position 5:" << std::endl;
        p = perft::PerftSearcher("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
        for (int i = 1; i < 6; i++) {
            auto start = std::chrono::high_resolution_clock::now();
            res = p.go(i, false);
            auto end = std::chrono::high_resolution_clock::now();
            auto ms_int = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << i << " " << res << " " << (ms_int.count() / (1000000.0f)) << "s " << static_cast<int>(res / (ms_int.count() / (1000000.0f))) << "nps" << std::endl;
        }

        std::cout << std::endl << "Position 6:" << std::endl;
        p = perft::PerftSearcher("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
        for (int i = 1; i < 6; i++) {
            auto start = std::chrono::high_resolution_clock::now();
            res = p.go(i, false);
            auto end = std::chrono::high_resolution_clock::now();
            auto ms_int = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            std::cout << i << " " << res << " " << (ms_int.count() / (1000000.0f)) << "s " << static_cast<int>(res / (ms_int.count() / (1000000.0f))) << "nps" << std::endl;
        }
	}
}