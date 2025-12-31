#include "bitboards.h"
#include <iostream>

namespace bitboards {
	void showBitboard(U64& bb) {
		for (int y = 7; y >= 0; y--) {
			for (int x = 0; x < 8; x++) {
				std::cout << ((bb & (1ull << (8 * y + x))) != 0);
			}
			std::cout << std::endl;
		}
	}
}