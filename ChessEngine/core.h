#pragma once
#include <string>
#include <vector>

//For all the typedefs, constexprs, structs etc.

//https://www.codeproject.com/articles/Worlds-Fastest-Bitboard-Chess-Movegenerator - tzcnt gets the square, bitloop constantly resets the lowest '1' bit to 0 until the number is 0
#define SquareOf(X) _tzcnt_u64(X) 
#define Bitloop(X) for(;X; X = _blsr_u64(X))

enum square {
	A1 = 0,
	B1 = 1,
	C1 = 2,
	D1 = 3,
	E1 = 4,
	F1 = 5,
	G1 = 6,
	H1 = 7,
	A2 = 8,
	B2 = 9,
	C2 = 10,
	D2 = 11,
	E2 = 12,
	F2 = 13,
	G2 = 14,
	H2 = 15,
	A3 = 16,
	B3 = 17,
	C3 = 18,
	D3 = 19,
	E3 = 20,
	F3 = 21,
	G3 = 22,
	H3 = 23,
	A4 = 24,
	B4 = 25,
	C4 = 26,
	D4 = 27,
	E4 = 28,
	F4 = 29,
	G4 = 30,
	H4 = 31,
	A5 = 32,
	B5 = 33,
	C5 = 34,
	D5 = 35,
	E5 = 36,
	F5 = 37,
	G5 = 38,
	H5 = 39,
	A6 = 40,
	B6 = 41,
	C6 = 42,
	D6 = 43,
	E6 = 44,
	F6 = 45,
	G6 = 46,
	H6 = 47,
	A7 = 48,
	B7 = 49,
	C7 = 50,
	D7 = 51,
	E7 = 52,
	F7 = 53,
	G7 = 54,
	H7 = 55,
	A8 = 56,
	B8 = 57,
	C8 = 58,
	D8 = 59,
	E8 = 60,
	F8 = 61,
	G8 = 62,
	H8 = 63,

	nullSquare = -1,
};

static std::string squareNames[64] = {  "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
										"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
										"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
										"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
										"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
										"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
										"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
										"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8"  };

enum piece {
	nullPiece = 0,

	wPawn = 1,
	wKnight = 2,
	wBishop = 3,
	wRook = 4,
	wQueen = 5,
	wKing = 6,

	bPawn = 9,
	bKnight = 10,
	bBishop = 11,
	bRook = 12,
	bQueen = 13,
	bKing = 14,
};

static std::string pieceNames[16] = { "null", "wPawn", "wKnight", "wBishop", "wRook", "wQueen", "wKing", "error", "error", "bPawn", "bKnight", "bBishop", "bRook", "bQueen", "bKing", "error"};

enum color {
	white = 0,
	black = 1,
};

//Move type
typedef unsigned int move;

//Bitboards
typedef unsigned __int64 U64;

//Max possible depth
constexpr int maxDepth = 64;

//Characters to represent pieces
static char pieceChars[16] = { '.', 'P', 'N', 'B', 'R', 'Q', 'K', ' ', ' ', 'p', 'n', 'b', 'r', 'q', 'k', ' ' };

//String split
std::vector<std::string> split(std::string string, std::string delimiter);

bool sameColor(square sq1, square sq2);

bool sameColor(unsigned char sq1, unsigned char sq2);
