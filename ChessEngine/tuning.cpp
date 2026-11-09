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

		searcher = search::Searcher(infoPieces[0]);
	}

	double TuningPosition::errorSquared() {
		double qSearchResult;
		if (searcher.getToMove() == white) {
			qSearchResult = searcher.negamaxQuiescence<white, false>(-10000, 10000, 0);
		}
		else {
			qSearchResult = searcher.negamaxQuiescence<black, false>(-10000, 10000, 0);
		}
		return std::pow(sigmoid(qSearchResult - resultValue), 2.0f);
	}

	int tuningMain() {
		std::cout << "Not implemented." << std::endl;
		return 0;
	}
}