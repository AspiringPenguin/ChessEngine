#include "uci.h"

#include "moves.h"
#include "search.h"
#include "tt.h"

#include <iostream>
#include <string>
#include <vector>

namespace uci {
	int uciMain() {
        std::cout << "id name AspireBot" << std::endl;
        std::cout << "id author AspiringPenguin" << std::endl;
        std::cout << "option name OwnBook type check default true" << std::endl;
        std::cout << "option name Hash type spin default 4 min 1 max 4096" << std::endl;
        std::cout << "uciok" << std::endl;

        //Uci loop

        std::vector<std::string> chunks;
        int chunkPos;
        std::string fenString;
        move m;
        std::string optionName;
        std::string optionValue;

        unsigned int wtime, winc, btime, binc;

        std::string cliInput;
        bool useBook;

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
            else if (cliInput.substr(0, 9) == "setoption") {
                chunks = split(cliInput, " ");

                optionName = chunks[2];

                if (chunks.size() < 5) { //No value
                    continue;
                }

                optionValue = chunks[4];

                if (optionName == "OwnBook") {
                    if (optionValue == "true") {
                        useBook = true;
                    }
                    else if (optionValue == "false") {
                        useBook = false;
                    }
                }

                else if (optionName == "Hash") {
                    tt::setSize(std::stoi(optionValue));
                }
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

                if (s.getToMove() == white) {
                    m = s.go<white>(wtime, btime, winc, binc, &stop, useBook);
                }
                else {
                    m = s.go<black>(wtime, btime, winc, binc, &stop, useBook);
                }

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
        return 0;
	}
}