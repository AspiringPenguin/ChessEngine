//A file for perft tests, UCI and any other interfaces to the engine

#include <chrono> //For timing
#include <iostream>

#include "bitboards.h"
#include "engine.h"
#include "moves.h"
#include "moveGenHelpers.h"

#ifdef PERFT
int main() {
    U64 res;

    std::cout << "Starting position:" << std::endl;
    engine::reset();
    for (int i = 1; i < 8; i++) {
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
    return 0;
}
#endif

#ifdef TEST
int main() {
    U64 res;

    std::cout << "Starting position:" << std::endl;
    engine::reset();
    for (int i = 1; i < 8; i++) {
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
    return 0;
}
#endif

#ifdef TEST
int main() {
    return 0;
}
#endif

//Todo:
// Eval with material and psq tables - https://www.chessprogramming.org/Simplified_Evaluation_Function
// Move ordering
// Search
// UCI
// Book