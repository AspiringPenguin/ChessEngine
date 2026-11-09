#pragma once
#include "bitboards.h"
#include "core.h"
#include "eval.h"
#include "fileHandling.h"
#include "position.h"

#include <string>

namespace tuning {
	constexpr int actualK = 100;

	double sigmoid(int s);

	double sigmoidCustomK(int s, int k);

	class TuningPosition {
	public:
		double resultValue;
		std::vector<double> termCoefficients;

		TuningPosition(std::string annotatedFen);

		double errorSquared();
		int eval();
	};

	std::vector<TuningPosition> loadPositions(int nSkip, int offset);

	int tuneK();

	int tuningMain();
}