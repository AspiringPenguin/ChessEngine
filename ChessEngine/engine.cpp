#include "engine.h"
#include <iostream>

namespace engine {
	piece mailbox[64]{ }; //Initialise as 0s = empty
	U64 bitboards[16]{ }; //Initialise as 0s

	move moves[maxDepth];
	int moveNum;

	void showPosition(color perspective) {
		if (perspective == black) {
			for (int y = 0; y < 8; y++) {
				for (int x = 7; x >= 0; x--) {
					std::cout << pieceChars[mailbox[y * 8 + x]];
				}
				std::cout << std::endl;
			}
		}
		else {
			for (int y = 7; y >= 0; y--) {
				for (int x = 0; x < 8; x++) {
					std::cout << pieceChars[mailbox[y * 8 + x]];
				}
				std::cout << std::endl;
			}
		}
	}

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
