//A file for perft tests, UCI and any other interfaces to the engine

#include <chrono> //For timing
#include <iostream>

#include "bitboards.h"
#include "book.h"
#include "engine.h"
#include "moves.h"
#include "moveGenHelpers.h"

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
int main() {
    engine::loadFEN("8/1Kn1p3/1p5N/4p1q1/4k1N1/3R2p1/Qn2B3/7R w - - 0 1");

    std::string userMove;
    move m;

    while (true){
        m = engine::negamaxSearch(6);
        engine::makeMove(m);
        moves::showMove(m);
        engine::showPosition();
        std::cout << engine::getNodes() << std::endl;
        engine::resetNodes();
        std::cout << "> ";
        std::cin >> userMove;
        m = engine::UCIMoveAsInternal(userMove);
        engine::makeMove(m);
    }
}
#endif

#ifdef TEST
int main() {
    engine::reset();

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

    return 0;
}
#endif

//Todo:
// Transposition table, move ordering & search search
// UCI