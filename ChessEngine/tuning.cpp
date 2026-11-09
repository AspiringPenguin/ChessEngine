#include "tuning.h"

namespace tuning {
	double sigmoid(int s) {
		double denom = 1 + std::pow(10, -1 * actualK * ((double)s) * (1/400));

		return 1 / denom;
	}

	double sigmoidCustomK(int s, int k) {
		double denom = 1 + std::pow(10, -1 * ((double)k) * ((double)s) * (1 / 400));

		return 1 / denom;
	}

	TuningPosition::TuningPosition(std::string annotatedFen) {
		auto infoPieces = split(annotatedFen, ";");

		resultValue = std::stoi(infoPieces[1]);

		auto pos = Position(infoPieces[0]);


		//Convert position to vector of values
	}

	double TuningPosition::errorSquared() {
		int eval = 0; //Use vector of values
		return std::pow(sigmoid(eval - resultValue), 2.0f);
	}

	int TuningPosition::eval() {
		return 0; //Dot product using the vectors, can optimise later
	}

	std::vector<TuningPosition> loadPositions(int nSkip, int offset) {
		auto lines = split(files::loadFile("positions.txt"), "\n");
		auto tuningPositions = std::vector<TuningPosition>();

		for (int n = -1; auto line : lines) {
			n++;

			if (line == "") {
				continue;
			}

			if ((n % nSkip) == offset) {
				tuningPositions.push_back(TuningPosition(line));
			}
		}

		return tuningPositions;
	}

	int tuneK() {
		auto positions = loadPositions(1, 0);

		return 0;
	}

	int tuningMain() {
		std::cout << "Not implemented." << std::endl;
		return 0;
	}
}