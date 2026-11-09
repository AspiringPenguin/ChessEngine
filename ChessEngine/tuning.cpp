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

		auto allCoefficients = std::vector<double>();

		const int phase = pos.phase;
		const int maxPhase = eval::maxPhase;

		const double mgCoeff = ((double) phase) / ((double) maxPhase);
		const double egCoeff = 1.0  - mgCoeff;

		//MG pieces
		allCoefficients.push_back(mgCoeff * ((double) (((int) __popcnt64(pos.bitboards[wPawn])) - ((int) __popcnt64(pos.bitboards[bPawn])))));
		allCoefficients.push_back(mgCoeff * ((double) (((int)__popcnt64(pos.bitboards[wKnight])) - ((int)__popcnt64(pos.bitboards[bKnight])))));
		allCoefficients.push_back(mgCoeff * ((double) (((int)__popcnt64(pos.bitboards[wBishop])) - ((int)__popcnt64(pos.bitboards[bBishop])))));
		allCoefficients.push_back(mgCoeff * ((double) (((int)__popcnt64(pos.bitboards[wRook])) - ((int)__popcnt64(pos.bitboards[bRook])))));
		allCoefficients.push_back(mgCoeff * ((double) (((int) __popcnt64(pos.bitboards[wQueen])) - ((int) __popcnt64(pos.bitboards[bQueen])))));

		//EG pieces
		allCoefficients.push_back(egCoeff * ((double) (((int)__popcnt64(pos.bitboards[wPawn])) - ((int)__popcnt64(pos.bitboards[bPawn])))));
		allCoefficients.push_back(egCoeff * ((double) (((int)__popcnt64(pos.bitboards[wKnight])) - ((int)__popcnt64(pos.bitboards[bKnight])))));
		allCoefficients.push_back(egCoeff * ((double) (((int)__popcnt64(pos.bitboards[wBishop])) - ((int)__popcnt64(pos.bitboards[bBishop])))));
		allCoefficients.push_back(egCoeff * ((double) (((int)__popcnt64(pos.bitboards[wRook])) - ((int)__popcnt64(pos.bitboards[bRook])))));
		allCoefficients.push_back(egCoeff * ((double) (((int)__popcnt64(pos.bitboards[wQueen])) - ((int)__popcnt64(pos.bitboards[bQueen])))));
		
		//Bitboards
		U64 wMask;
		U64 bMask;
		bool whitePiece;
		bool blackPiece;
		int j;

		//Pawns MG
		for (int i = 0; i < 64; i++) {
			j = i ^ 56; //Flipped to white perspective
			wMask = (1ull << j);
			bMask = (1ull << i);
			whitePiece = ((pos.bitboards[wPawn] & wMask) != 0);
			blackPiece = ((pos.bitboards[bPawn] & bMask) != 0);

			allCoefficients.push_back(mgCoeff * (whitePiece - blackPiece));
		}

		//Pawns EG
		for (int i = 0; i < 64; i++) {
			j = i ^ 56; //Flipped to white perspective
			wMask = (1ull << j);
			bMask = (1ull << i);
			whitePiece = ((pos.bitboards[wPawn] & wMask) != 0);
			blackPiece = ((pos.bitboards[bPawn] & bMask) != 0);

			allCoefficients.push_back(egCoeff * (whitePiece - blackPiece));
		}

		//Knights MG
		for (int i = 0; i < 64; i++) {
			j = i ^ 56; //Flipped to white perspective
			wMask = (1ull << j);
			bMask = (1ull << i);
			whitePiece = ((pos.bitboards[wKnight] & wMask) != 0);
			blackPiece = ((pos.bitboards[bKnight] & bMask) != 0);

			allCoefficients.push_back(mgCoeff * (whitePiece - blackPiece));
		}

		//Knights EG
		for (int i = 0; i < 64; i++) {
			j = i ^ 56; //Flipped to white perspective
			wMask = (1ull << j);
			bMask = (1ull << i);
			whitePiece = ((pos.bitboards[wKnight] & wMask) != 0);
			blackPiece = ((pos.bitboards[bKnight] & bMask) != 0);

			allCoefficients.push_back(egCoeff * (whitePiece - blackPiece));
		}

		//Bishops MG
		for (int i = 0; i < 64; i++) {
			j = i ^ 56; //Flipped to white perspective
			wMask = (1ull << j);
			bMask = (1ull << i);
			whitePiece = ((pos.bitboards[wBishop] & wMask) != 0);
			blackPiece = ((pos.bitboards[bBishop] & bMask) != 0);

			allCoefficients.push_back(mgCoeff * (whitePiece - blackPiece));
		}

		//Bishops EG
		for (int i = 0; i < 64; i++) {
			j = i ^ 56; //Flipped to white perspective
			wMask = (1ull << j);
			bMask = (1ull << i);
			whitePiece = ((pos.bitboards[wBishop] & wMask) != 0);
			blackPiece = ((pos.bitboards[bBishop] & bMask) != 0);

			allCoefficients.push_back(egCoeff * (whitePiece - blackPiece));
		}

		//Rooks MG
		for (int i = 0; i < 64; i++) {
			j = i ^ 56; //Flipped to white perspective
			wMask = (1ull << j);
			bMask = (1ull << i);
			whitePiece = ((pos.bitboards[wRook] & wMask) != 0);
			blackPiece = ((pos.bitboards[bRook] & bMask) != 0);

			allCoefficients.push_back(mgCoeff * (whitePiece - blackPiece));
		}

		//Rooks EG
		for (int i = 0; i < 64; i++) {
			j = i ^ 56; //Flipped to white perspective
			wMask = (1ull << j);
			bMask = (1ull << i);
			whitePiece = ((pos.bitboards[wRook] & wMask) != 0);
			blackPiece = ((pos.bitboards[bRook] & bMask) != 0);

			allCoefficients.push_back(egCoeff * (whitePiece - blackPiece));
		}

		//Queens MG
		for (int i = 0; i < 64; i++) {
			j = i ^ 56; //Flipped to white perspective
			wMask = (1ull << j);
			bMask = (1ull << i);
			whitePiece = ((pos.bitboards[wQueen] & wMask) != 0);
			blackPiece = ((pos.bitboards[bQueen] & bMask) != 0);

			allCoefficients.push_back(mgCoeff * (whitePiece - blackPiece));
		}

		//Queens EG
		for (int i = 0; i < 64; i++) {
			j = i ^ 56; //Flipped to white perspective
			wMask = (1ull << j);
			bMask = (1ull << i);
			whitePiece = ((pos.bitboards[wQueen] & wMask) != 0);
			blackPiece = ((pos.bitboards[bQueen] & bMask) != 0);

			allCoefficients.push_back(egCoeff * (whitePiece - blackPiece));
		}

		//Kings MG
		for (int i = 0; i < 64; i++) {
			j = i ^ 56; //Flipped to white perspective
			wMask = (1ull << j);
			bMask = (1ull << i);
			whitePiece = ((pos.bitboards[wKing] & wMask) != 0);
			blackPiece = ((pos.bitboards[bKing] & bMask) != 0);

			allCoefficients.push_back(mgCoeff * (whitePiece - blackPiece));
		}

		//Kings EG
		for (int i = 0; i < 64; i++) {
			j = i ^ 56; //Flipped to white perspective
			wMask = (1ull << j);
			bMask = (1ull << i);
			whitePiece = ((pos.bitboards[wKing] & wMask) != 0);
			blackPiece = ((pos.bitboards[bKing] & bMask) != 0);

			allCoefficients.push_back(egCoeff * (whitePiece - blackPiece));
		}

		//Passed pawns
		auto ppCoeffs = std::vector<double>(7, 0);

		U64 pieceBB = pos.bitboards[wPawn];
		U64 blockerBB = pos.bitboards[bPawn];
		square sq;
		Bitloop(pieceBB) {
			sq = square(SquareOf(pieceBB));
			if ((eval::passedPawnMasks[white][sq] & blockerBB) == 0) {
				ppCoeffs[sq >> 3]++;
			}
		}

		pieceBB = pos.bitboards[bPawn];
		blockerBB = pos.bitboards[wPawn];
		Bitloop(pieceBB) {
			sq = square(SquareOf(pieceBB));
			if ((eval::passedPawnMasks[black][sq] & blockerBB) == 0) {
				ppCoeffs[7 - (sq >> 3)]++;
			}
		}

		for (auto term : ppCoeffs) {
			allCoefficients.push_back(term);
		}

		//Defended pawns
		int value = 0;
		//Pawns that are defended by other pawns are good
	    //White pawns
	    //Defend to right
		pieceBB = pos.bitboards[wPawn];
		U64 intersectionBB = pieceBB & ((pieceBB & (~bitboards::HFile)) << 9);
		value += __popcnt64(intersectionBB);

		//Defend to left
		intersectionBB = pieceBB & ((pieceBB & (~bitboards::AFile)) << 7);
		value += __popcnt64(intersectionBB);


		//Black pawns
		//Defend to right
		pieceBB = pos.bitboards[bPawn];
		intersectionBB = pieceBB & ((pieceBB & (~bitboards::HFile)) >> 7);
		value -= __popcnt64(intersectionBB);

		//Defend to left
		intersectionBB = pieceBB & ((pieceBB & (~bitboards::AFile)) >> 9);
		value -= __popcnt64(intersectionBB);

		allCoefficients.push_back(value);

		//Board control
		//Empty
		allCoefficients.push_back(0);

		//white pawns
		allCoefficients.push_back(2 * ((int) __popcnt64(pos.bitboards[wPawn] & ~(bitboards::AFile | bitboards::HFile)))
									+ ((int) __popcnt64(pos.bitboards[wPawn] & (bitboards::AFile | bitboards::HFile))));

		//white knights
		value = 0;

		pieceBB = pos.bitboards[wKnight];

		Bitloop(pieceBB) {
			sq = square(SquareOf(pieceBB));
			value += eval::knightMoveCounts[sq];
		}

		allCoefficients.push_back(value);

		//white bishops
		value = 0;

		pieceBB = pos.bitboards[wBishop];
		blockerBB = pos.allBitboard & ~pos.bitboards[wBishop] & ~pos.bitboards[wQueen];

		Bitloop(pieceBB) {
			sq = square(SquareOf(pieceBB));
			value += __popcnt64(moveGen::bishopMoveLookup[sq][_pext_u64(blockerBB, moveGen::bishopPextMasks[sq])]);
		}

		allCoefficients.push_back(value);

		//white rooks
		value = 0;

		pieceBB = pos.bitboards[wRook];
		blockerBB = pos.allBitboard & ~pos.bitboards[wRook] & ~pos.bitboards[wQueen];

		Bitloop(pieceBB) {
			sq = square(SquareOf(pieceBB));
			value += __popcnt64(moveGen::rookMoveLookup[sq][_pext_u64(blockerBB, moveGen::rookPextMasks[sq])]);
		}

		allCoefficients.push_back(value);

		//white queen
		value = 0;

		pieceBB = pos.bitboards[wQueen];

		//Diagonals
		blockerBB = pos.allBitboard & ~pos.bitboards[wBishop] & ~pos.bitboards[wQueen];
		Bitloop(pieceBB) {
			sq = square(SquareOf(pieceBB));
			value += __popcnt64(moveGen::bishopMoveLookup[sq][_pext_u64(blockerBB, moveGen::bishopPextMasks[sq])]);
		}

		//Ranks/files
		blockerBB = pos.allBitboard & ~pos.bitboards[wRook] & ~pos.bitboards[wQueen];
		Bitloop(pieceBB) {
			sq = square(SquareOf(pieceBB));
			value += __popcnt64(moveGen::rookMoveLookup[sq][_pext_u64(blockerBB, moveGen::rookPextMasks[sq])]);
		}

		allCoefficients.push_back(value);

		//white king - empty
		allCoefficients.push_back(0);

		//Empty x2
		allCoefficients.push_back(0);
		allCoefficients.push_back(0);

		//black pawns
		allCoefficients.push_back(-2 * ((int) __popcnt64(pos.bitboards[bPawn] & ~(bitboards::AFile | bitboards::HFile)))
									 - ((int) __popcnt64(pos.bitboards[bPawn] & (bitboards::AFile | bitboards::HFile))));

		//black knights
		value = 0;

		pieceBB = pos.bitboards[bKnight];

		Bitloop(pieceBB) {
			sq = square(SquareOf(pieceBB));
			value -= eval::knightMoveCounts[sq];
		}

		allCoefficients.push_back(value);

		//black bishops
		value = 0;

		pieceBB = pos.bitboards[bBishop];
		blockerBB = pos.allBitboard & ~pos.bitboards[bBishop] & ~pos.bitboards[bQueen];

		Bitloop(pieceBB) {
			sq = square(SquareOf(pieceBB));
			value -= __popcnt64(moveGen::bishopMoveLookup[sq][_pext_u64(blockerBB, moveGen::bishopPextMasks[sq])]);
		}

		allCoefficients.push_back(value);

		//black rooks
		value = 0;

		pieceBB = pos.bitboards[bRook];
		blockerBB = pos.allBitboard & ~pos.bitboards[bRook] & ~pos.bitboards[bQueen];

		Bitloop(pieceBB) {
			sq = square(SquareOf(pieceBB));
			value -= __popcnt64(moveGen::rookMoveLookup[sq][_pext_u64(blockerBB, moveGen::rookPextMasks[sq])]);
		}

		allCoefficients.push_back(value);

		//black queen
		value = 0;

		pieceBB = pos.bitboards[bQueen];

		//Diagonals
		blockerBB = pos.allBitboard & ~pos.bitboards[bBishop] & ~pos.bitboards[bQueen];
		Bitloop(pieceBB) {
			sq = square(SquareOf(pieceBB));
			value -= __popcnt64(moveGen::bishopMoveLookup[sq][_pext_u64(blockerBB, moveGen::bishopPextMasks[sq])]);
		}

		//Ranks/files
		blockerBB = pos.allBitboard & ~pos.bitboards[bRook] & ~pos.bitboards[bQueen];
		Bitloop(pieceBB) {
			sq = square(SquareOf(pieceBB));
			value -= __popcnt64(moveGen::rookMoveLookup[sq][_pext_u64(blockerBB, moveGen::rookPextMasks[sq])]);
		}

		allCoefficients.push_back(value);

		//black king - empty
		allCoefficients.push_back(0);
		
		//Empty
		allCoefficients.push_back(0);

		//Convert into condensed form
		usefulCoefficients = std::vector<std::tuple<int, double>>();

		for (int i = -1; auto coeff : allCoefficients) {
			i++;

			if (!(std::abs(coeff) < 0.01)) { //Avoiding strange floating-point 0 issues
				usefulCoefficients.push_back({i, coeff});
			}
		}
	}

	double TuningPosition::errorSquared() {
		int eval = 0; //Use vector of values
		return std::pow(sigmoid(eval - resultValue), 2.0f);
	}

	int TuningPosition::eval(const std::vector<int>& evalTerms) {
		double total = 0;
		for (const auto& pair : usefulCoefficients) {
			total += evalTerms[std::get<0>(pair)] * std::get<1>(pair);
		}
		return std::round(total);
	}

	std::vector<TuningPosition> loadPositions(int nSkip, int offset) {
		auto lines = split(files::loadFile("positions.txt"), "\n");
		auto tuningPositions = std::vector<TuningPosition>();

		for (int n = -1; auto& line : lines) {
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
		auto evalTerms = eval::loadEvalTerms("eval.txt");

		std::cout << positions[0].eval(evalTerms) << std::endl;

		return 0;
	}

	int tuningMain() {
		std::cout << "Not implemented." << std::endl;
		return 0;
	}
}