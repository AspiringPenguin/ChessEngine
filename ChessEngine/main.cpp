//A file for perft tests, UCI and any other interfaces to the engine

#include <chrono> //For timing
#include <iostream>

#include "bitboards.h"
#include "book.h"
#include "engine.h"
#include "moves.h"
#include "moveGenHelpers.h"
#include "tt.h"

#ifdef PERFT
int main() {
    U64 res;

    std::cout << "Starting position:" << std::endl;
    engine::reset();
    for (int i = 1; i < 7; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        res = engine::perft(i, false);
        auto end = std::chrono::high_resolution_clock::now();
        auto ms_int = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << i << " " << res << " " << (ms_int.count() / (1000000.0f)) << "s " << static_cast<int>(res / (ms_int.count() / (1000000.0f))) << "nps" << std::endl;
    }

    std::cout << std::endl << "Kiwipete:" << std::endl;
    engine::loadFEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    for (int i = 1; i < 6; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        res = engine::perft(i, false);
        auto end = std::chrono::high_resolution_clock::now();
        auto ms_int = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << i << " " << res << " " << (ms_int.count() / (1000000.0f)) << "s " << static_cast<int>(res / (ms_int.count() / (1000000.0f))) << "nps" << std::endl;
    }

    std::cout << std::endl << "Position 3:" << std::endl;
    engine::loadFEN("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
    for (int i = 1; i < 8; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        res = engine::perft(i, false);
        auto end = std::chrono::high_resolution_clock::now();
        auto ms_int = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << i << " " << res << " " << (ms_int.count() / (1000000.0f)) << "s " << static_cast<int>(res / (ms_int.count() / (1000000.0f))) << "nps" << std::endl;
    }

    std::cout << std::endl << "Position 4:" << std::endl;
    engine::loadFEN("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
    for (int i = 1; i < 6; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        res = engine::perft(i, false);
        auto end = std::chrono::high_resolution_clock::now();
        auto ms_int = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << i << " " << res << " " << (ms_int.count() / (1000000.0f)) << "s " << static_cast<int>(res / (ms_int.count() / (1000000.0f))) << "nps" << std::endl;
    }

    std::cout << std::endl << "Position 5:" << std::endl;
    engine::loadFEN("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    for (int i = 1; i < 6; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        res = engine::perft(i, false);
        auto end = std::chrono::high_resolution_clock::now();
        auto ms_int = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << i << " " << res << " " << (ms_int.count() / (1000000.0f)) << "s " << static_cast<int>(res / (ms_int.count() / (1000000.0f))) << "nps" << std::endl;
    }

    std::cout << std::endl << "Position 6:" << std::endl;
    engine::loadFEN("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    for (int i = 1; i < 6; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        res = engine::perft(i, false);
        auto end = std::chrono::high_resolution_clock::now();
        auto ms_int = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << i << " " << res << " " << (ms_int.count() / (1000000.0f)) << "s " << static_cast<int>(res / (ms_int.count() / (1000000.0f))) << "nps" << std::endl;
    }

    return 0;
}
#endif

#ifdef UCI
move runSearch(unsigned int wtime, unsigned int btime, unsigned int winc, unsigned int binc) {

}

int main() {
    std::string cliInput;
    std::vector<std::string> chunks;
    int chunkPos;
    std::string fenString;
    move m;

    unsigned int wtime, winc, btime, binc;

    while (true) {
        std::getline(std::cin, cliInput, '\n');

        if (cliInput == "uci") {
            std::cout << "id name AspireBot" << std::endl;
            std::cout << "id author AspiringPenguin" << std::endl;
            std::cout << "uciok" << std::endl;
        }
        else if (cliInput == "quit") {
            break;
        }
        else if (cliInput == "isready") {
            std::cout << "readyok" << std::endl;
        }
        else if (cliInput == "ucinewgame") {
            engine::reset();
            tt::setSize(128);
        }
        else if (cliInput.substr(0, 2) == "go") {
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

            m = runSearch(wtime, btime, winc, binc);

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
                engine::loadFEN(fenString);
            }
            else if (chunks[1] == "startpos") {
                engine::reset();
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
                    m = engine::UCIMoveAsInternal(chunks[chunkPos]);
                    engine::makeMove(m);
                    chunkPos++;
                }
            }
        }
    }
}
#endif

#ifdef TEST
int main() {
    engine::reset();
    tt::setSize(128);

    /*std::string userMove;
    move move;
    int counter = 0;

    while (true) {
        std::cin >> userMove;

        if (userMove == "end") {
            while (counter > 0) {
                engine::undoMove();
                engine::showPosition();
                std::cout << engine::evaluate() << std::endl;
                std::cout << engine::getZobrist() << std::endl << std::endl;
                counter--;
            }
            continue;
        }

        move = engine::UCIMoveAsInternal(userMove);
        engine::makeMove(move);
        engine::showPosition();
        std::cout << engine::evaluate() << std::endl;
        std::cout << engine::getZobrist() << std::endl << std::endl;
        counter++;
    }*/


    std::string m;
    std::string userMove;
    move move;
    int counter;

    bool white = true;

    while (true) {
        counter = 1;

        if (white) {
            m = book::chooseMove(book::book[engine::getZobrist()]);
            move = engine::UCIMoveAsInternal(m);
            std::cout << m << std::endl;
            engine::makeMove(move);
            engine::showPosition();
            std::cout << engine::evaluate() << std::endl;
            std::cout << engine::getZobrist() << std::endl << std::endl;

            white = false;
        }
        else {
            white = true;
        }

        for (int j = 0; j < 5; j++) {
            std::cout << "> ";
            std::cin >> userMove;
            move = engine::UCIMoveAsInternal(userMove);
            engine::makeMove(move);
            engine::showPosition();
            std::cout << engine::evaluate() << std::endl;
            std::cout << engine::getZobrist() << std::endl << std::endl;
            counter++;

            if (!book::book.contains(engine::getZobrist())) {
                break;
            }
            m = book::chooseMove(book::book[engine::getZobrist()]);
            move = engine::UCIMoveAsInternal(m);
            std::cout << m << std::endl;
            engine::makeMove(move);
            engine::showPosition();
            std::cout << engine::evaluate() << std::endl;
            std::cout << engine::getZobrist() << std::endl << std::endl;
            counter++;
        }
        std::cout << std::endl << std::endl;
    }

    /*engine::loadFEN("8/8/6q1/8/8/4k3/7K/8 b - - 0 1");
    //engine::reset();
    tt::setSize(128);

    std::string stringMove;
    move m;
    int nodes;

    while (true){
        if (book::book.contains(engine::getZobrist())) {
            stringMove = book::chooseMove(book::book[engine::getZobrist()]);
            m = engine::UCIMoveAsInternal(stringMove);
        }
        else {
            for (int i = 1; i < 12; i++) {
                auto start = std::chrono::high_resolution_clock::now();
                m = engine::negamaxSearch(i);
                auto end = std::chrono::high_resolution_clock::now();
                auto ms_int = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
                std::cout << i << " ";
                moves::showMove(m);
                nodes = engine::getNodes();
                std::cout << nodes << " " << ms_int.count() / (1000000.0f) << "s " << static_cast<int>(nodes / (ms_int.count() / (1000000.0f))) << "nps" << std::endl;
                engine::resetNodes();
            }
        }
        if (m == -1) {
            std::cout << "no legal moves for computer" << std::endl;
            break;
        }
        engine::makeMove(m);
        moves::showMove(m);
        std::cout << std::endl;
        engine::showPosition();
        std::cout << "> ";
        std::cin >> stringMove;
        m = engine::UCIMoveAsInternal(stringMove);
        engine::makeMove(m);
        engine::showPosition();
    }*/

    return 0;
}
#endif

#ifndef UCI
#ifndef PERFT
#ifndef TEST
int main() {
    return 0;
}
#endif
#endif
#endif

// Todo - search and eval:
// Proper search routine based on time limit with iterative deepening

// Todo - longer term core aims:
// Improve board representation by removing if and conditionals in make/unmake move by using more of the 32 move bits?
// and adding some padding to eval
// Remove move gen conditionals by making it templated and constexpr if for toMove, with matching perft and searches