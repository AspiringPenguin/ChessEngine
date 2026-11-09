//A file for perft tests, UCI and any other interfaces to the engine

#include <chrono> //For timing
#include <iostream>

#include "eval.h"
#include "perft.h"
#include "search.h"
#include "tt.h"
#include "tuning.h"
#include "uci.h"

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
    eval::loadEval();

    std::string cliInput;

    std::getline(std::cin, cliInput, '\n');

    if (cliInput == "uci") {
        return uci::uciMain();
    }
    else if (cliInput == "perft") {
        return perft::perftMain();
    }
    else if (cliInput == "debugMove") {
        move inputMove;
        while (true) {
            std::getline(std::cin, cliInput, '\n');
            inputMove = std::stoi(cliInput);
            moves::debugMove(inputMove);
        }
    }
    else if (cliInput == "eval") {
        std::getline(std::cin, cliInput, '\n');

        auto pos = Position(cliInput);

        std::cout << pos.evaluate() << std::endl;
    }
    else if (cliInput == "findK") {
        return tuning::tuneK();
    }
    else if (cliInput == "tuneEval") {
        return tuning::tuningMain();
    }
    else {
        std::cout << "Unrecognised mode command: " << cliInput << std::endl;
    }

    return 0;
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