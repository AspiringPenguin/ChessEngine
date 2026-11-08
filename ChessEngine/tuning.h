#pragma once
#include "core.h"
#include "fileHandling.h"
#include "search.h"

#include <string>

namespace tuning {
	constexpr int actualK = 100;

	double sigmoid(int s);

	class TuningPosition {
		search::Searcher searcher;
	public:
		double resultValue;
		TuningPosition(std::string annotatedFen);

		double errorSquared();
	};

	int tuningMain();
}