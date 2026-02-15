//A file for perft tests, UCI and any other interfaces to the engine

#include <chrono> //For timing
#include <iostream>

#include "perft.h"
#include "search.h"
#include "tt.h"

#ifdef TEST
int main() {
    auto p = search::Searcher();
    p.showPosition();
    p.makeMove(moves::encodeNormal(E2, E4, wPawn, nullPiece, false, true, false, false, false, false));
    p.showPosition();
}
#endif

#ifndef TEST
int main() {
    std::string cliInput;

    std::getline(std::cin, cliInput, '\n');

    if (cliInput == "uci") {
        std::cout << "id name AspireBot" << std::endl;
        std::cout << "id author AspiringPenguin" << std::endl;
        std::cout << "uciok" << std::endl;

        //Uci loop

        std::vector<std::string> chunks;
        int chunkPos;
        std::string fenString;
        move m;

        unsigned int wtime, winc, btime, binc;

        search::Searcher s = search::Searcher();

        bool stop = false;

        while (true) {
            std::getline(std::cin, cliInput, '\n');

            s.resetNodes();

            if (cliInput == "quit") {
                break;
            }
            else if (cliInput == "isready") {
                std::cout << "readyok" << std::endl;
            }
            else if (cliInput == "ucinewgame") {
                s.loadStart();
                tt::setSize(128);
            }
            else if (cliInput.substr(0, 2) == "go") {
                stop = false;

                chunks = split(cliInput, " ");

                chunkPos = 1;

                wtime = 0;
                btime = 0;
                winc = 0;
                binc = 0;

                while (chunks.size() != chunkPos) {
                    if (chunks[chunkPos] == "wtime") {
                        wtime = std::stoul(chunks[chunkPos + 1]);
                    }
                    else if (chunks[chunkPos] == "btime") {
                        btime = std::stoul(chunks[chunkPos + 1]);
                    }
                    else if (chunks[chunkPos] == "winc") {
                        winc = std::stoul(chunks[chunkPos + 1]);
                    }
                    else if (chunks[chunkPos] == "binc") {
                        binc = std::stoul(chunks[chunkPos + 1]);
                    }

                    chunkPos += 2;
                }

                m = s.go(wtime, btime, winc, binc, &stop);

                std::cout << "bestmove ";
                moves::showMove(m);
                std::cout << std::endl;
            }
            else if (cliInput.substr(0, 8) == "position") {
                chunks = split(cliInput, " ");
                if (chunks[1] == "fen") {
                    fenString = chunks[2];
                    chunkPos = 3;
                    while (chunks.size() != chunkPos && chunks[chunkPos] != "moves") {
                        fenString += " ";
                        fenString += chunks[chunkPos];
                        chunkPos++;
                    }
                    s.loadFEN(fenString);
                }
                else if (chunks[1] == "startpos") {
                    s.loadStart();
                    chunkPos = 2;
                }
                else {
                    chunkPos = -1; //To avoid compiler warning
                    continue;
                }

                if (chunks.size() == chunkPos) {
                    continue;
                }

                if (chunks[chunkPos] == "moves") {
                    chunkPos++;
                    while (chunks.size() != chunkPos) {
                        m = s.UCIMoveAsInternal(chunks[chunkPos]);
                        s.makeMove(m);
                        chunkPos++;
                    }
                }
            }
        }
    }
    else if (cliInput == "perft") {
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
#endif

// Todo - search and eval:
// Pawn eval - hash table and more detailed analysis
// Some form of king safety analysis

// Todo - longer term core aims:
// Rework core board representation and as such perft and search to remove branches and improve performance
// PVS and Aspriation windows
// Staged move gen in search? - will need better tt to make worthwhile I think

// Automated tuning of eval by texel's tuning

// Better book generator, larger opening book