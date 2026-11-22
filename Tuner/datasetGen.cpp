#include "datasetGen.h"

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
		//Work out the relative indices of the positions to process
		int numPositionsToProcess = (numPositions > 100) ? 10 : (numPositions / 10);

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dist(0, numPositions);

		int counter = 0;
		std::vector<int> values;
		int res;
		bool found;

		while (values.size() < numPositionsToProcess) {
			res = dist(gen);
			found = false;

			for (auto value : values) {
				if (res == value) {
					found = true;
					break;
				}
			}

			if (!found) {
				values.push_back(res);
			}
		}
		
		std::string result = "";

		for (auto value : values) {
			//load fen to a Searcher object and run a quiescence search to pull out a PV.
			//Apply that PV and store the final FEN and result of the game in the string to return

			result += std::to_string(value) + " ";
		}

		return result;
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