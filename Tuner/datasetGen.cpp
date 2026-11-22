#include "datasetGen.h"

//Current final dataset aim:
//min(10, numPositions/10) random positions from each game with the position having had the PV from quiescence applied
//So algorithm: load all FENS as strings with metadata (gameID, result)
//All FENs are in gameID order, so we iterate over them, count how many there are, then select up min(10, numPositions/10) randomly to have major processing (qSearch) applied
//Store as FEN;result form.

namespace datasetGen {
	std::vector<fenPosition> loadPositions() {
		auto lines = split(loadFile("positions.txt"), "\n");

		auto positions = std::vector<fenPosition>();
		std::vector<std::string> parts;

		for (const std::string& line : lines) {
			if (line == "") {
				continue;
			}
			parts = split(line, ";");
			positions.push_back(fenPosition{ parts[0], std::stoi(parts[1]), std::stoi(parts[2]) });
		}

		return positions;
	}

	std::string processGames(int startIndex, int numPositions, std::vector<fenPosition>& positions) {
		return positions[startIndex].fen;
	}

	int main() {
		auto positions = loadPositions();

		int currentGameID = 0;
		int currentGameStartIndex = 0;

		std::string finalOutput = "";

		for (int index = -1;  auto position : positions) {
			index++;
			if (position.gameID != currentGameID) {
				//Process these games
				finalOutput += processGames(currentGameStartIndex, index - currentGameStartIndex, positions);
				finalOutput += "\n"; //This goes here so there is not a \n after the final line with data on it

				//Now reset
				currentGameID++;
				currentGameStartIndex = index;
			}
			else if (index == (positions.size() - 1)) { //This is the last game
				finalOutput += processGames(currentGameStartIndex, index - currentGameStartIndex + 1, positions);
			}
		}

		writeToFile("processedPositions.txt", finalOutput);

		return 0;
	}
}