#include "engine.h"
#include "zobrist.h"
#include <iostream>

namespace engine {
	std::array<piece, 64> mailbox{ }; //Initialise as 0s = empty
	U64 bitboards[16]{ }; //Initialise as 0s

	U64 zobrist = 0;

	move moves[maxDepth]{};
	int moveNum = 0;

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

		//Bitboards
		bitboards[wPawn] ^= (1ull << A2);
		bitboards[wPawn] ^= (1ull << B2);
		bitboards[wPawn] ^= (1ull << C2);
		bitboards[wPawn] ^= (1ull << D2);
		bitboards[wPawn] ^= (1ull << E2);
		bitboards[wPawn] ^= (1ull << F2);
		bitboards[wPawn] ^= (1ull << G2);
		bitboards[wPawn] ^= (1ull << H2);

		bitboards[wRook] ^= (1ull << A1);
		bitboards[wRook] ^= (1ull << H1);
		bitboards[wKnight] ^= (1ull << B1);
		bitboards[wKnight] ^= (1ull << G1);
		bitboards[wBishop] ^= (1ull << C1);
		bitboards[wBishop] ^= (1ull << F1);
		bitboards[wQueen] ^= (1ull << D1);
		bitboards[wKing] ^= (1ull << E1);

		bitboards[bPawn] ^= (1ull << A7);
		bitboards[bPawn] ^= (1ull << B7);
		bitboards[bPawn] ^= (1ull << C7);
		bitboards[bPawn] ^= (1ull << D7);
		bitboards[bPawn] ^= (1ull << E7);
		bitboards[bPawn] ^= (1ull << F7);
		bitboards[bPawn] ^= (1ull << G7);
		bitboards[bPawn] ^= (1ull << H7);

		bitboards[bRook] ^= (1ull << A8);
		bitboards[bRook] ^= (1ull << H8);
		bitboards[bKnight] ^= (1ull << B8);
		bitboards[bKnight] ^= (1ull << G8);
		bitboards[bBishop] ^= (1ull << C8);
		bitboards[bBishop] ^= (1ull << F8);
		bitboards[bQueen] ^= (1ull << D8);
		bitboards[bKing] ^= (1ull << E8);

		//Zobrist hashing
		zobrist = 0;
		zobrist ^= zobrist::zobristPieces(mailbox);


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
