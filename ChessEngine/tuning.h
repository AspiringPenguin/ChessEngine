#pragma once
#include "bitboards.h"
#include "core.h"
#include "eval.h"
#include "fileHandling.h"
#include "position.h"

#include <string>
#include <tuple>

namespace tuning {
	constexpr int actualK = 100;

	double sigmoid(int s);

	double sigmoidCustomK(int s, int k);

	class TuningPosition {
	public:
		double resultValue;
		std::vector<std::tuple<int, double>> usefulCoefficients; //Int index of term, Double coefficent of term

		TuningPosition(std::string annotatedFen);

		double errorSquared();
		int eval();
	};

	std::vector<TuningPosition> loadPositions(int nSkip, int offset);

	int tuneK();

	int tuningMain();
}