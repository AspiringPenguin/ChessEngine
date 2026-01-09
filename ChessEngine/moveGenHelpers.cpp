#include "moveGenHelpers.h"
#include "bitboards.h"
#include <intrin.h>
#include <immintrin.h>
#include <iostream>

namespace moveGen {
	std::array<U64, 64> generateKnightLookup() {
		auto bitboards = std::array<U64, 64>();

		U64 bb = 0;

		for (int i = 0; i < 64; i++) {
			bb = (1ull << i);

			//From https://www.chessprogramming.org/Knight_Pattern

			U64 l1 = (bb >> 1) & bitboards::notHFile;
			U64 l2 = (bb >> 2) & bitboards::notGHFile;
			U64 r1 = (bb << 1) & bitboards::notAFile;
			U64 r2 = (bb << 2) & bitboards::notABFile;
			U64 h1 = l1 | r1;
			U64 h2 = l2 | r2;

			bitboards[i] = (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
		}

		return bitboards;
	}

	std::array<U64, 64> generateKingLookup() {
		auto bitboards = std::array<U64, 64>();

		U64 bb = 0;

		for (int i = 0; i < 64; i++) {
			bb = (1ull << i);

			//From https://www.chessprogramming.org/King_Pattern

			bb |= (bb << 1 & bitboards::notAFile) | (bb >> 1 & bitboards::notHFile);

			bitboards[i] = (bb | bb >> 8 | bb << 8) & ~(1ull << i);
		}

		return bitboards;
	}

	constexpr U64 nonEdgeFileMask = ~(bitboards::rank1 | bitboards::rank8);
	constexpr U64 nonEdgeRankMask = ~(bitboards::AFile | bitboards::HFile);

	std::array<U64, 64> generateRookPextMasks() {
		auto masks = std::array<U64, 64>();

		U64 squareBB;
		U64 mask;


		for (int i = 0; i < 64; i++) {
			squareBB = (1ull << i);
			mask = (bitboards::files[i % 8] & nonEdgeFileMask) | (bitboards::ranks[i / 8] & nonEdgeRankMask);
			mask &= ~squareBB;

			masks[i] = mask;
		}

		return masks;
	}

	//From https://www.chessprogramming.org/On_an_empty_Board#By_Calculation_3
	static inline U64 diagonalMask(int sq) {
		int diag = (sq & 7) - (sq >> 3);
		return diag >= 0 ? bitboards::mainDiag >> diag * 8 : bitboards::mainDiag << -diag * 8;
	}

	static inline U64 antiDiagMask(int sq) {
		int diag = 7 - (sq & 7) - (sq >> 3);
		return diag >= 0 ? bitboards::oppoDiag >> diag * 8 : bitboards::oppoDiag << -diag * 8;
	}

	constexpr U64 excludeEdges = ~(bitboards::AFile | bitboards::HFile | bitboards::rank1 | bitboards::rank8);

	std::array<U64, 64> generateBishopPextMasks() {
		auto masks = std::array<U64, 64>();

		for (int i = 0; i < 64; i++) {
			int diag = 8 * (i & 7) - (i & 56);
			int nort = -diag & (diag >> 31);
			int sout = diag & (-diag >> 31);
			masks[i] = (diagonalMask(i) | antiDiagMask(i)) & excludeEdges;
		}

		return masks;
	}

	std::array<bool, 64> bitboardToMailbox(U64 bb) {
		auto mailbox = std::array<bool, 64>();

		for (int i = 0; i < 64; i++) {
			mailbox[i] = ((bb & (1ull << i)) != 0);
		}

		return mailbox;
	}

	std::vector<std::vector<U64>> generateRookMoveLookup(std::array<U64, 64> pextMasks) {
		auto table = std::vector<std::vector<U64>>();
		table.assign(64, std::vector<U64>());

		for (int i = 0; i < 64; i++) {
			table[i] = generateRookMoveLookupBySquare(square(i), pextMasks[i]);
		}

		return table;
	}

	constexpr std::array<int, 4> rookShiftsX = { 0, 1, 0, -1 };
	constexpr std::array<int, 4> rookShiftsY = { 1, 0, -1, 0 };

	std::vector<U64> generateRookMoveLookupBySquare(square sq, U64 mask) {
		auto moveBBs = std::vector<U64>();
		moveBBs.assign((1ull << 12), 0xFFFFFFFFFFFFFFFF); //Fill out the vector so vec[i] works. Use 0xFFFFFF... to easily catch bugs

		U64 moveBB;
		std::array<bool, 64> mailbox;
		int posX;
		int posY;
		int directionX;
		int directionY;

		for (U64 i = 0; i < (1ull << 12); i++) {
			moveBB = 0;
			mailbox = bitboardToMailbox(_pdep_u64(i, mask));

			for (int j = 0; j < 4; j++) {
				directionX = rookShiftsX[j];
				directionY = rookShiftsY[j];
				posX = sq % 8;
				posY = sq / 8;

				//Apply the shifts once to get first square
				posX += directionX;
				posY += directionY;
				while (posX <= 7 && posX >= 0 && posY <= 7 && posY >= 0) {
					//Add to BB
					moveBB |= (1ull << (posX + posY * 8));

					//Check for occupancy, after adding to BB for captures
					if (mailbox[posX + posY * 8]) {
						break;
					}

					//Apply to get the next one
					posX += directionX;
					posY += directionY;
				}
			}

			moveBBs[i] = moveBB;
		}

		return moveBBs;
	}

	std::vector<std::vector<U64>> generateBishopMoveLookup(std::array<U64, 64> pextMasks) {
		auto table = std::vector<std::vector<U64>>();
		table.assign(64, std::vector<U64>());

		for (int i = 0; i < 64; i++) {
			table[i] = generateBishopMoveLookupBySquare(square(i), pextMasks[i]);
		}

		return table;
	}

	constexpr std::array<int, 4> bishopShiftsX = { 1, 1, -1, -1 };
	constexpr std::array<int, 4> bishopShiftsY = { 1, -1, 1, -1 };

	std::vector<U64> generateBishopMoveLookupBySquare(square sq, U64 mask) {
		auto moveBBs = std::vector<U64>();
		moveBBs.assign((1ull << 12), 0xFFFFFFFFFFFFFFFF); //Fill out the vector so vec[i] works. Use 0xFFFFFF... to easily catch bugs

		U64 moveBB;
		std::array<bool, 64> mailbox;
		int posX;
		int posY;
		int directionX;
		int directionY;

		for (U64 i = 0; i < (1ull << 10); i++) {
			moveBB = 0;
			mailbox = bitboardToMailbox(_pdep_u64(i, mask));

			for (int j = 0; j < 4; j++) {
				directionX = bishopShiftsX[j];
				directionY = bishopShiftsY[j];
				posX = sq % 8;
				posY = sq / 8;

				//Apply the shifts once to get first square
				posX += directionX;
				posY += directionY;
				while (posX <= 7 && posX >= 0 && posY <= 7 && posY >= 0) {
					//Add to BB
					moveBB |= (1ull << (posX + posY * 8));

					//Check for occupancy, after adding to BB for captures
					if (mailbox[posX + posY * 8]) {
						break;
					}

					//Apply to get the next one
					posX += directionX;
					posY += directionY;
				}
			}

			moveBBs[i] = moveBB;
		}

		return moveBBs;
	}
}