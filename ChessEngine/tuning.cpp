#include "tuning.h"

namespace tuning {
	double sigmoid(int s) {
		double denom = 1 + std::pow(10, -1 * actualK * ((double)s) * (1/400));

		return 1 / denom;
	}

	TuningPosition::TuningPosition(std::string annotatedFen) {
		auto infoPieces = split(annotatedFen, ";");

		resultValue = std::stoi(infoPieces[1]);

		searcher = search::Searcher(infoPieces[0]);
	}

	double TuningPosition::errorSquared() {
		double qSearchResult = searcher.negamaxQuiescence<white, false>(-10000, 10000, 0));
		return std::pow(sigmoid(qSearchResult - resultValue), 2.0f);
	}

	int tuningMain() {
		std::cout << "Not implemented." << std::endl;
		return 0;
	}
}