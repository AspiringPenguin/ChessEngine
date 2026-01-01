#include "bitboards.h"
#include "engine.h"
#include "zobrist.h"
#include <exception>
#include <iostream>

namespace engine {
	std::array<piece, 64> mailbox{ }; //Initialise as 0s = empty
	U64 bitboards[16]{ }; //Initialise as 0s

	U64 zobrist = 0;

	move moves[maxDepth]{};
	int moveNum = -1;

	color toMove;
	int toMoveSigned;

	//Castling rights
	bool wKingside = true;
	bool wQueenside = true;
	bool bKingside = true;
	bool bQueenside = true;

	//For enPassant
	square enPassantSquare = nullSquare;


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
		//Status stuff
		toMove = white;
		toMoveSigned = 1;
		moveNum = -1;
		enPassantSquare = nullSquare;
		std::fill(std::begin(moves), std::end(moves), 0);

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

		//Castling rights
		wKingside = true;
		wQueenside = true;
		bKingside = true;
		bQueenside = true;

		//Zobrist hashing
		zobrist = 0;
		//Pieces
		zobrist ^= zobrist::zobristPieces(mailbox);
		//Castling rights
		zobrist ^= zobrist::values[768];
		zobrist ^= zobrist::values[769];
		zobrist ^= zobrist::values[770];
		zobrist ^= zobrist::values[771];
		//White to move
		zobrist ^= zobrist::values[780];
	}

	void loadFEN(std::string fen) {
		//Reset some stuff
		std::fill(std::begin(bitboards), std::end(bitboards), 0);
		std::fill(std::begin(mailbox), std::end(mailbox), nullPiece);
		zobrist = 0;
		wKingside = false;
		wQueenside = false;
		bKingside = false;
		bQueenside = false;

		enPassantSquare = nullSquare;
		moveNum = -1;
		std::fill(std::begin(moves), std::end(moves), 0);

		//Get info from FEN
		const auto info = split(fen, " ");
		std::string board = info[0];
		std::string _toMove = info[1];
		std::string castling = info[2];
		std::string enPassant = info[3];

		//Turn the board into a A1->H8 indexed list of characters representing pieces or empty spaces

		//Numbers to n*'.'
		for (int i = 0; i < 64; i++) {
			if (std::isdigit(board[i])) {
				board = board.substr(0, i) + std::string(std::stoi(std::string(1, board[i])), '.') + board.substr(i+1);
			}
		}
		
		//Translate the board, flip, then flip each row back
		std::reverse(board.begin(), board.end());

		auto rows = split(board, "/");
		for (int i = 0; i < 8; i++) {
			std::reverse(rows[i].begin(), rows[i].end());
		}

		//Remake the board from the rows
		board = "";
		for (const auto& row : rows) {
			board += row;
		}

		//Process the result into board representation now
		for (int n = -1; char c : board) {
			n++;
			//Linear search through char conversion to find value
			for (int i = -1; char p : pieceChars) {
				i++;
				if (p == c) {
					mailbox[n] = piece(i);
					bitboards[piece(i)] ^= (1ull << n);
					break;
				}
			}
		}

		//Update zobrist hash for pieces
		zobrist = zobrist::zobristPieces(mailbox);

		//Castling
		if (castling.find("K") != std::string::npos) {
			wKingside = true;
			zobrist ^= zobrist::values[768];
		}
		if (castling.find("Q") != std::string::npos) {
			wQueenside = true;
			zobrist ^= zobrist::values[769];
		}
		if (castling.find("k") != std::string::npos) {
			bKingside = true;
			zobrist ^= zobrist::values[770];
		}
		if (castling.find("q") != std::string::npos) {
			bQueenside = true;
			zobrist ^= zobrist::values[771];
		}

		//Whose move it is
		toMove = color(_toMove == "b");
		if (toMove == white) {
			toMoveSigned = 1;
			zobrist ^= zobrist::values[780];
		}
		else {
			toMoveSigned = -1;
		}

		//En passant - polyglot standard only adds en-passant hash when a pseudo-legal en passant move is possible
		if (enPassant != "-") {
			enPassantSquare = square((enPassant[0] - 97) + (enPassant[1] - 49) * 8);
			U64 bb = 1ull << enPassantSquare;
			if (toMove == white) {
				bb = ((bb << 1) | (bb >> 1)) >> 8;
				if (bb & bitboards[wPawn]) {
					zobrist ^= zobrist::values[772 + (enPassantSquare % 8)];
				}
			}
			else {
				bb = ((bb << 1) | (bb >> 1)) << 8;
				if (bb & bitboards[bPawn]) {
					zobrist ^= zobrist::values[772 + (enPassantSquare % 8)];
				}
			}
		}
	}

	void makeMove(move& m, bool reversible) {
		if (reversible) {
			moves[++moveNum] = m;
		}
		else if (moveNum == -1) {
			throw std::out_of_range("Irreversible make move was attempted with temporary moves applied");
		}

		//Make move here
	}

	void makeMove(move& m) {
		makeMove(m, true);
	}

	void undoMove() {
		if (moveNum == -1) {
			throw std::out_of_range("Undo move was attempted with no moves to undo.");
		}

		//Undo move here
	}

	void debugPosition() {
		std::cout << toMove << std::endl;
		std::cout << enPassantSquare << std::endl;
		std::cout << wKingside << " " << wQueenside << " " << bKingside << " " << bQueenside << std::endl;
		showPosition();
		for (piece p : {wPawn, wKnight, wBishop, wRook, wQueen, wKing, bPawn, bKnight, bBishop, bRook, bQueen, bKing}) {
			std::cout << p << std::endl;
			std::cout << bitboards[p] << std::endl;
			bitboards::showBitboard(bitboards[p]);
		}
		std::cout << zobrist << std::endl;
	}

	U64 getZobrist() {
		return zobrist;
	}

	U64 getBitboard(int n) {
		return bitboards[n];
	}
}
