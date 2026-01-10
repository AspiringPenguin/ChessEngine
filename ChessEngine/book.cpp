#include "book.h"

namespace book {
	std::string loadFile(std::string fileName) {
		std::ifstream reader(fileName);

		if (!reader) {
			std::cerr << "Error opening file " << fileName << std::endl;
			return "";
		}
		else {
			std::string line; //For reading the file

			std::string contents = ""; //To return

			bool skip = true;

			while (!reader.eof()) {
				getline(reader, line);
				if (!skip) {
					contents += "\n";
				}
				else {
					skip = false;
				}
				contents += line;
			}
			reader.close();
			return contents;
		}
	}

	std::unordered_map<U64, std::tuple<std::vector<std::tuple<std::string, int, int>>, int>> loadBook() {
		auto lookup = std::unordered_map<U64, std::tuple<std::vector<std::tuple<std::string, int, int>>, int>>();

		auto lines = split(loadFile("book.txt"), "\n");

		std::vector<std::string> sections;
		U64 hash;
		std::vector<std::tuple<std::string, int, int>> moveList;
		std::vector<std::string> moveSections;
		int numInstances;

		int counter = 0;

		for (const auto& line : lines) {
			moveList.clear();
			counter = 0;

			sections = split(line, ";"); //Split the line

			hash = std::stoull(sections[0]); //Get the hash

			for (int i = 1; i < sections.size(); i++) { //For each move-count pair
				moveSections = split(sections[i], ","); //Split the pair
				numInstances = std::stoi(moveSections[1]); //Get the count
				moveList.push_back({moveSections[0], counter, counter + numInstances-1 }); //Store it in the moveList
				counter += numInstances;
			}

			lookup[hash] = {moveList, counter};
		}

		return lookup;
	}

	std::string chooseMove(std::tuple<std::vector<std::tuple<std::string, int, int>>, int> moveList) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dist(0, std::get<1>(moveList) - 1);

		int res = dist(gen);

		for (const auto& m : std::get<0>(moveList)) {
			if (res >= std::get<1>(m) && res <= std::get<2>(m)) { //If the random number is within the bounds for the move
				return std::get<0>(m);
			}
		}

		throw std::invalid_argument("Book movelist is not valid");
	}
}