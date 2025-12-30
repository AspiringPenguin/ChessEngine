#include "engine.h"
#include <iostream>

namespace engine {
	piece mailbox[64]{ nullPiece };
	U64 bitboards[16]{ 0 };

	move moves[maxDepth];
	int moveNum;

	void reset() {
		//Mailbox
		mailbox[A1] = wRook;
		mailbox[B1] = wKnight;
		mailbox[C1] = wBishop;
		mailbox[D1] = wQueen;
		mailbox[E1] = wKing;
		mailbox[F1] = wBishop;
		mailbox[G1] = wKnight;
		mailbox[H1] = wRook;

		for (int n = A2; n <= H2; n++) {
			mailbox[n] = wPawn;
		}

		for (int n = A3; n <= H6; n++) {
			mailbox[n] = nullPiece;
		}

		for (int n = A7; n <= H7; n++) {
			mailbox[n] = bPawn;
		}

		mailbox[A8] = bRook;
		mailbox[B8] = bKnight;
		mailbox[C8] = bBishop;
		mailbox[D8] = bQueen;
		mailbox[E8] = bKing;
		mailbox[F8] = bBishop;
		mailbox[G8] = bKnight;
		mailbox[H8] = bRook;

		for (piece p : engine::mailbox) {
			std::cout << p << std::endl;
		}
		//Bitboards
	}

	void loadFEN() {

	}

	void makeMove(move& m, bool reversible) {

	}

	void makeMove(move& m) {
		makeMove(m, true);
	}

	void undoMove() {

	}
}
