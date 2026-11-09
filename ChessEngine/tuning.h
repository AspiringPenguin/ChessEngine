#pragma once
#include "core.h"
#include "fileHandling.h"
#include "search.h"

#include <string>

namespace tuning {
	constexpr int actualK = 100;

	double sigmoid(int s);

	double sigmoidCustomK(int s, int k);

	class TuningPosition {
		search::Searcher searcher;
	public:
		double resultValue;
		TuningPosition(std::string annotatedFen);

		double errorSquared();
	};

	int tuningMain();
}