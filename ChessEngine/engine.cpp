#include "bitboards.h"
#include "engine.h"
#include "eval.h"
#include "moves.h"
#include "moveGenHelpers.h"
#include "zobrist.h"
#include <exception>
#include <intrin.h>
#include <iostream>

using bitboards::showBitboard;

namespace engine {
	std::array<piece, 64> mailbox{ }; //Initialise as 0s = empty
	U64 bitboards[16]{ }; //Initialise as 0s
	U64 colorBitboards[2]{ }; //0 = white, 1 = black
	U64 allBitboard = 0;

	U64 zobrist = 0;

	move moves[maxDepth]{};
	int counters[maxDepth]{}; //For resetting of 50 move clock - the counter before the matching move was applied
	move lastMove; //Keep the last move at the root here - the last irreversible move

	int counter = 0; //For the 50-move rule, add/subtract 1 every 'half-move', draw when >= 100
	
	constexpr int numPositions = 100 + maxDepth; //So we still have accurate history after making and unmaking lots of moves
	U64 positions[numPositions]; //Use circularly to store last 100 positions' zobrist hashes
	int positionsHead = 0; //First position, inclusive
	int positionsTail = 0; //Last position, exclusive
	//If head == (tail) all mod numPositions increment both and overwrite 

	int moveNum = -1;

	color toMove;
	int toMoveSigned;

	//Eval stuff
	int phase;
	int materialStart;
	int materialEnd;
	int bonusesStart;
	int bonusesEnd;

	//Castling rights
	bool wKingside = true;
	bool wQueenside = true;
	bool bKingside = true;
	bool bQueenside = true;

	//For enPassant
	square enPassantSquare = nullSquare;

	//Set up aggregate bitboards
	void updateBitboards() {
		colorBitboards[white] = bitboards[wPawn] | bitboards[wKnight] | bitboards[wBishop] | bitboards[wRook] | bitboards[wQueen] | bitboards[wKing];
		colorBitboards[black] = bitboards[bPawn] | bitboards[bKnight] | bitboards[bBishop] | bitboards[bRook] | bitboards[bQueen] | bitboards[bKing];
		allBitboard = colorBitboards[white] | colorBitboards[black];
	}

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
		std::fill(std::begin(counters), std::end(counters), 0);
		counter = 0; //50 move rule

		//Clear draw position history
		std::fill(std::begin(positions), std::end(positions), 0); 
		positionsHead = 0;
		positionsTail = 0;

		lastMove = 0;

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

		updateBitboards();

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

		positions[positionsTail] = zobrist;
		positionsTail++;

		calculatePhase();
		calculateMaterialStart();
		calculateMaterialEnd();
		calculateBonusesStart();
		calculateBonusesEnd();
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
		std::fill(std::begin(counters), std::end(counters), 0);
		counter = 0; //50 move rule

		//Clear draw position history
		std::fill(std::begin(positions), std::end(positions), 0); 
		positionsHead = 0;
		positionsTail = 0;

		//Get info from FEN
		const auto info = split(fen, " ");
		std::string board = info[0];
		std::string _toMove = info[1];
		std::string castling = info[2];
		std::string enPassant = info[3];

		//Turn the board into a A1->H8 indexed list of characters representing pieces or empty spaces

		//Numbers to n*'.'
		for (int i = 0; i < 71; i++) {
			if (std::isdigit(board[i])) {
				board = board.substr(0, i) + std::string(board[i]-48, '.') + board.substr(i + 1);
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

		updateBitboards();

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
			enPassantSquare = stringToSquare(enPassant);
			U64 bb = 1ull << enPassantSquare;
			if (toMove == white) {
				bb = (((bb & bitboards::notHFile) << 1) | ((bb & bitboards::notAFile) >> 1)) >> 8;
				if (bb & bitboards[wPawn]) {
					zobrist ^= zobrist::values[772 + (enPassantSquare % 8)];
				}
			}
			else {
				bb = (((bb & bitboards::notHFile) << 1) | ((bb & bitboards::notAFile) >> 1)) << 8;
				if (bb & bitboards[bPawn]) {
					zobrist ^= zobrist::values[772 + (enPassantSquare % 8)];
				}
			}

			lastMove = moves::encodeMove(square(enPassantSquare + 8 * toMoveSigned), square(enPassantSquare - 8 * toMoveSigned), piece(wPawn | (toMove << 3)), nullPiece, false, false, true, false, false, false, false);
		}
		else {
			lastMove = 0; 
		}

		positions[positionsTail] = zobrist;
		positionsTail++;

		calculatePhase();
		calculateMaterialStart();
		calculateMaterialEnd();
		calculateBonusesStart();
		calculateBonusesEnd();
	}

	void makeMove(move& m, bool reversible) {
		if (reversible) {
			//Store this move at the incremented value of moveNum
			moves[++moveNum] = m;

			//Also store the current 50-move counter value
			#pragma warning(push)
			#pragma warning(disable:6386)
			counters[moveNum] = counter; //This will never overflow in practice unless my search has a huge bug in it.
			#pragma warning(pop)
		}
		else if (moveNum != -1) {
			throw std::out_of_range("Irreversible make move was attempted with temporary moves applied");
		}
		else {
			lastMove = m; //Set lastMove to m to deal with returning to root zobrist
		}

		counter++; //Increment 50 move counter, reset later

		const square from = moves::getFrom(m);
		const square to = moves::getTo(m);
		const piece p = moves::getPiece(m);
		const piece toPiece = moves::getPromoteFlag(m) ? moves::getPromote(m, toMove) : p;

		//Handle promotions by adding and removing
		materialStart -= eval::pieceValuesStart[p];
		materialStart += eval::pieceValuesStart[toPiece];
		materialEnd -= eval::pieceValuesEnd[p];
		materialEnd += eval::pieceValuesEnd[toPiece];

		const piece capture = moves::getCapture(m);
		int pos;
		square lTo;
		move _lastMove = (moveNum == -1 ? lastMove : moves[moveNum - 1]);

		counter = ((p & 0b0111) == wPawn || capture != nullPiece) ? 0 : counter; //Reset 50 move counter if a pawn move or a capture

		//Undo zobrist hashing for last move being double push where en passant was pseudo-legal
		//This must go here before any bitboards are updated
		if (moves::isDoublePush(_lastMove)) { //So may need to remove zobrist hash element for en passant
			lTo = moves::getTo(_lastMove);
			U64 bb = 1ull << lTo; //Get where it went to
			if (toMove == white) { //White is moving now, and the lastMove was by black
				bb = (((bb & bitboards::notHFile) << 1) | ((bb & bitboards::notAFile) >> 1));
				if (bb & bitboards[wPawn]) {
					zobrist ^= zobrist::values[772 + (lTo % 8)];
				}
			}
			else {
				bb = (((bb & bitboards::notHFile) << 1) | ((bb & bitboards::notAFile) >> 1));
				if (bb & bitboards[bPawn]) {
					zobrist ^= zobrist::values[772 + (lTo % 8)];
				}
			}
		}

		if ((p & 0b0111) == 6&& std::abs(from - to) == 2) { //Is a king and the distance moved is 2, not 1, 7, 8 or 9 so therefore castling
			if (p == wKing) {
				if (to == G1) {
					//Update mailbox
					mailbox[E1] = nullPiece;
					mailbox[F1] = wRook;
					mailbox[G1] = wKing;
					mailbox[H1] = nullPiece;

					//Update bitboards
					bitboards[wKing] ^= (1ull << E1) | (1ull << G1);
					bitboards[wRook] ^= (1ull << F1) | (1ull << H1);
					colorBitboards[white] ^= (1ull << E1) | (1ull << F1) | (1ull << G1) | (1ull << H1);
					allBitboard ^= (1ull << E1) | (1ull << F1) | (1ull << G1) | (1ull << H1);

					//Update zobrist hash for pieces
					zobrist ^= zobrist::values[64 * zobrist::pieceLookup[wKing] + E1] ^ zobrist::values[64 * zobrist::pieceLookup[wKing] + G1];
					zobrist ^= zobrist::values[64 * zobrist::pieceLookup[wRook] + F1] ^ zobrist::values[64 * zobrist::pieceLookup[wRook] + H1];
				}
				else {
					//Update mailbox
					mailbox[E1] = nullPiece;
					mailbox[D1] = wRook;
					mailbox[C1] = wKing;
					mailbox[A1] = nullPiece;

					//Update bitboards
					bitboards[wKing] ^= (1ull << E1) | (1ull << C1);
					bitboards[wRook] ^= (1ull << A1) | (1ull << D1);
					colorBitboards[white] ^= (1ull << A1) | (1ull << C1) | (1ull << D1) | (1ull << E1);
					allBitboard ^= (1ull << A1) | (1ull << C1) | (1ull << D1) | (1ull << E1);

					//Update zobrist hash for pieces
					zobrist ^= zobrist::values[64 * zobrist::pieceLookup[wKing] + E1] ^ zobrist::values[64 * zobrist::pieceLookup[wKing] + C1];
					zobrist ^= zobrist::values[64 * zobrist::pieceLookup[wRook] + A1] ^ zobrist::values[64 * zobrist::pieceLookup[wRook] + D1];
				}
			}
			else {
				if (to == G8) {
					//Update mailbox
					mailbox[E8] = nullPiece;
					mailbox[F8] = bRook;
					mailbox[G8] = bKing;
					mailbox[H8] = nullPiece;

					//Update bitboards
					bitboards[bKing] ^= (1ull << E8) | (1ull << G8);
					bitboards[bRook] ^= (1ull << F8) | (1ull << H8);
					colorBitboards[black] ^= (1ull << E8) | (1ull << F8) | (1ull << G8) | (1ull << H8);
					allBitboard ^= (1ull << E8) | (1ull << F8) | (1ull << G8) | (1ull << H8);

					//Update zobrist hash for pieces
					zobrist ^= zobrist::values[64 * zobrist::pieceLookup[bKing] + E8] ^ zobrist::values[64 * zobrist::pieceLookup[bKing] + G8];
					zobrist ^= zobrist::values[64 * zobrist::pieceLookup[bRook] + F8] ^ zobrist::values[64 * zobrist::pieceLookup[bRook] + H8];
				}
				else {
					//Update mailbox
					mailbox[E8] = nullPiece;
					mailbox[D8] = bRook;
					mailbox[C8] = bKing;
					mailbox[A8] = nullPiece;

					//Update bitboards
					bitboards[bKing] ^= (1ull << E8) | (1ull << C8);
					bitboards[bRook] ^= (1ull << A8) | (1ull << D8);
					colorBitboards[black] ^= (1ull << A8) | (1ull << C8) | (1ull << D8) | (1ull << E8);
					allBitboard ^= (1ull << A8) | (1ull << C8) | (1ull << D8) | (1ull << E8);

					//Update zobrist hash for pieces
					zobrist ^= zobrist::values[64 * zobrist::pieceLookup[bKing] + E8] ^ zobrist::values[64 * zobrist::pieceLookup[bKing] + C8];
					zobrist ^= zobrist::values[64 * zobrist::pieceLookup[bRook] + A8] ^ zobrist::values[64 * zobrist::pieceLookup[bRook] + D8];
				}
			}
			calculateBonusesStart();
			calculateBonusesEnd();
		}
		else { //Its a normal move

			//Update mailbox
			mailbox[from] = nullPiece;
			mailbox[to] = toPiece;

			//Update psq values
			#pragma warning(push)
			#pragma warning(disable:6385)
			bonusesStart -= (eval::pieceBonusesStart[(p & 0b111) - 1][from ^ 56 * (1 - toMove)]) * toMoveSigned;
			bonusesStart += (eval::pieceBonusesStart[(toPiece & 0b111) - 1][to ^ 56 * (1 - toMove)]) * toMoveSigned;
			bonusesEnd -= (eval::pieceBonusesEnd[(p & 0b111) - 1][from ^ 56 * (1 - toMove)]) * toMoveSigned;
			bonusesEnd += (eval::pieceBonusesEnd[(toPiece & 0b111) - 1][to ^ 56 * (1 - toMove)]) * toMoveSigned;
			#pragma warning(pop)

			//Update zobrist hash
			zobrist ^= zobrist::values[64 * zobrist::pieceLookup[p] + from];
			zobrist ^= zobrist::values[64 * zobrist::pieceLookup[toPiece] + to];

			//Update bitboards
			bitboards[p] ^= (1ull << from);
			bitboards[toPiece] ^= (1ull << to);
			colorBitboards[p >> 3] ^= (1ull << from) | (1ull << to);
			allBitboard ^= (1ull << from) | (1ull << to);

			//Handle enPassant
			if (moves::isEnPassant(m)) {
				materialStart -= eval::pieceValuesStart[capture];
				materialEnd -= eval::pieceValuesEnd[capture];

				pos = to + (-8 * toMoveSigned); //Calculate square to clear
				mailbox[pos] = nullPiece; //Mailbox wasn't updated
				bitboards[capture] ^= (1ull << pos);
				zobrist ^= zobrist::values[64 * zobrist::pieceLookup[capture] + pos];
				colorBitboards[capture >> 3] ^= (1ull << pos);
				allBitboard ^= (1ull << pos);

				//Update start and end psq values
				#pragma warning(push)
				#pragma warning(disable:6385)
				bonusesStart -= (eval::pieceBonusesStart[(p & 0b111) - 1][pos ^ (56 * toMove)]) * toMoveSigned * -1;
				bonusesEnd -= (eval::pieceBonusesEnd[(p & 0b111) - 1][pos ^ (56 * toMove)]) * toMoveSigned * -1;
				#pragma warning(pop)
			}
			else if (capture != nullPiece) {
				phase -= eval::piecePhases[capture];
				materialStart -= eval::pieceValuesStart[capture];
				materialEnd -= eval::pieceValuesEnd[capture];

				//Update start and end psq values
				#pragma warning(push)
				#pragma warning(disable:6385)
				bonusesStart -= (eval::pieceBonusesStart[(p & 0b111) - 1][to ^ (56 * toMove)]) * toMoveSigned * -1;
				bonusesEnd -= (eval::pieceBonusesEnd[(p & 0b111) - 1][to ^ (56 * toMove)]) * toMoveSigned * -1;
				#pragma warning(pop)

				bitboards[capture] ^= (1ull << to);
				zobrist ^= zobrist::values[64 * zobrist::pieceLookup[capture] + to];
				colorBitboards[capture >> 3] ^= (1ull << to);
				allBitboard ^= (1ull << to);
			}

			if (moves::isDoublePush(m)) { //May need to add zobrist hash element for enPassant
				U64 bb = 1ull << to;
				bb = (((bb & bitboards::notHFile) << 1) | ((bb & bitboards::notAFile) >> 1));
				if (toMove == black) { //Will be white next turn, not updated yet
					if (bb & bitboards[wPawn]) {
						zobrist ^= zobrist::values[772 + (to % 8)];
					}
				}
				else {
					if (bb & bitboards[bPawn]) {
						zobrist ^= zobrist::values[772 + (to % 8)];
					}
				}
			}
		}

		//Update castling rights
		const auto rights = moves::getCastleChanges(m);
		if (std::get<0>(rights)) {
			wKingside = false;
			zobrist ^= zobrist::values[768];
		}
		if (std::get<1>(rights)) {
			wQueenside = false;
			zobrist ^= zobrist::values[769];
		}
		if (std::get<2>(rights)) {
			bKingside = false;
			zobrist ^= zobrist::values[770];
		}
		if (std::get<3>(rights)) {
			bQueenside = false;
			zobrist ^= zobrist::values[771];
		}

		toMove = color(1 - toMove); //Flip toMove
		toMoveSigned = -1 * toMoveSigned;
		zobrist ^= zobrist::values[780];

		if (!reversible) {
			lastMove = m; //Update lastMove
		}

		//Add position to repetition history
		positions[positionsTail] = zobrist;
		positionsHead = (positionsHead == positionsTail) ? (positionsHead + 1): positionsHead; //If we just overwrote the first element, increment positionsHead
		positionsTail++; //Increment the tail

		//If both greater or equal to numPositions subtract numPositions
		if (positionsHead >= numPositions && positionsTail >= numPositions) {
			positionsHead -= numPositions;
			positionsTail -= numPositions;
		}
	}

	void makeMove(move& m) {
		makeMove(m, true);
	}

	void undoMove() {
		if (moveNum == -1) {
			throw std::out_of_range("Undo move was attempted with no moves to undo.");
		}

		move m = moves[moveNum];
		counter = counters[moveNum--]; //Get the counter state before the last move and then decrement moveNum
		move _lastMove = moveNum == -1 ? lastMove : moves[moveNum]; //ie the move before m

		const square from = moves::getFrom(m);
		const square to = moves::getTo(m);
		const piece p = moves::getPiece(m);
		const piece toPiece = moves::getPromoteFlag(m) ? moves::getPromote(m, color(1 - toMove)) : p;
		const piece capture = moves::getCapture(m);
		square lTo;
		square pos;

		//Handle promotions by removing and adding
		materialStart += eval::pieceValuesStart[p];
		materialStart -= eval::pieceValuesStart[toPiece];
		materialEnd += eval::pieceValuesEnd[p];
		materialEnd -= eval::pieceValuesEnd[toPiece];

		if ((p & 0b0111) == 6 && std::abs(from - to) == 2) { //Is a king and the distance moved is 2, not 1, 7, 8 or 9 so therefore castling
			if (p == wKing) {
				if (to == G1) {
					//Update mailbox
					mailbox[E1] = wKing;
					mailbox[F1] = nullPiece;
					mailbox[G1] = nullPiece;
					mailbox[H1] = wRook;

					//Update bitboards
					bitboards[wKing] ^= (1ull << E1) | (1ull << G1);
					bitboards[wRook] ^= (1ull << F1) | (1ull << H1);
					colorBitboards[white] ^= (1ull << E1) | (1ull << F1) | (1ull << G1) | (1ull << H1);
					allBitboard ^= (1ull << E1) | (1ull << F1) | (1ull << G1) | (1ull << H1);

					//Update zobrist hash for pieces
					zobrist ^= zobrist::values[64 * zobrist::pieceLookup[wKing] + E1] ^ zobrist::values[64 * zobrist::pieceLookup[wKing] + G1];
					zobrist ^= zobrist::values[64 * zobrist::pieceLookup[wRook] + F1] ^ zobrist::values[64 * zobrist::pieceLookup[wRook] + H1];
				}
				else {
					//Update mailbox
					mailbox[E1] = wKing;
					mailbox[D1] = nullPiece;
					mailbox[C1] = nullPiece;
					mailbox[A1] = wRook;

					//Update bitboards
					bitboards[wKing] ^= (1ull << E1) | (1ull << C1);
					bitboards[wRook] ^= (1ull << A1) | (1ull << D1);
					colorBitboards[white] ^= (1ull << A1) | (1ull << C1) | (1ull << D1) | (1ull << E1);
					allBitboard ^= (1ull << A1) | (1ull << C1) | (1ull << D1) | (1ull << E1);

					//Update zobrist hash for pieces
					zobrist ^= zobrist::values[64 * zobrist::pieceLookup[wKing] + E1] ^ zobrist::values[64 * zobrist::pieceLookup[wKing] + C1];
					zobrist ^= zobrist::values[64 * zobrist::pieceLookup[wRook] + A1] ^ zobrist::values[64 * zobrist::pieceLookup[wRook] + D1];
				}
			}
			else {
				if (to == G8) {
					//Update mailbox
					mailbox[E8] = bKing;
					mailbox[F8] = nullPiece;
					mailbox[G8] = nullPiece;
					mailbox[H8] = bRook;

					//Update bitboards
					bitboards[bKing] ^= (1ull << E8) | (1ull << G8);
					bitboards[bRook] ^= (1ull << F8) | (1ull << H8);
					colorBitboards[black] ^= (1ull << E8) | (1ull << F8) | (1ull << G8) | (1ull << H8);
					allBitboard ^= (1ull << E8) | (1ull << F8) | (1ull << G8) | (1ull << H8);

					//Update zobrist hash for pieces
					zobrist ^= zobrist::values[64 * zobrist::pieceLookup[bKing] + E8] ^ zobrist::values[64 * zobrist::pieceLookup[bKing] + G8];
					zobrist ^= zobrist::values[64 * zobrist::pieceLookup[bRook] + F8] ^ zobrist::values[64 * zobrist::pieceLookup[bRook] + H8];
				}
				else {
					//Update mailbox
					mailbox[E8] = bKing;
					mailbox[D8] = nullPiece;
					mailbox[C8] = nullPiece;
					mailbox[A8] = bRook;

					//Update bitboards
					bitboards[bKing] ^= (1ull << E8) | (1ull << C8);
					bitboards[bRook] ^= (1ull << A8) | (1ull << D8);
					colorBitboards[black] ^= (1ull << A8) | (1ull << C8) | (1ull << D8) | (1ull << E8);
					allBitboard ^= (1ull << A8) | (1ull << C8) | (1ull << D8) | (1ull << E8);

					//Update zobrist hash for pieces
					zobrist ^= zobrist::values[64 * zobrist::pieceLookup[bKing] + E8] ^ zobrist::values[64 * zobrist::pieceLookup[bKing] + C8];
					zobrist ^= zobrist::values[64 * zobrist::pieceLookup[bRook] + A8] ^ zobrist::values[64 * zobrist::pieceLookup[bRook] + D8];
				}
			}
			calculateBonusesStart();
			calculateBonusesEnd();
		}
		else {
			if (moves::isDoublePush(m)) { //May need to remove zobrist hash element for enPassant
				U64 bb = 1ull << to;
				bb = (((bb & bitboards::notHFile) << 1) | ((bb & bitboards::notAFile) >> 1));
				if (toMove == white) { //The double push was by black, so the capture must be by white
					if (bb & bitboards[wPawn]) {
						zobrist ^= zobrist::values[772 + (to % 8)];
					}
				}
				else {
					if (bb & bitboards[bPawn]) {
						zobrist ^= zobrist::values[772 + (to % 8)];
					}
				}
			}

			//Update mailbox
			mailbox[from] = p;
			mailbox[to] = capture;

			//Update psq values
			#pragma warning(push)
			#pragma warning(disable:6385)
			bonusesStart += (eval::pieceBonusesStart[(p & 0b111) - 1][from ^ 56 * toMove]) * toMoveSigned * -1;
			bonusesStart -= (eval::pieceBonusesStart[(toPiece & 0b111) - 1][to ^ 56 * toMove]) * toMoveSigned * -1;
			bonusesEnd += (eval::pieceBonusesEnd[(p & 0b111) - 1][from ^ 56 * toMove]) * toMoveSigned * -1;
			bonusesEnd -= (eval::pieceBonusesEnd[(toPiece & 0b111) - 1][to ^ 56 * toMove]) * toMoveSigned * -1;
			#pragma warning(pop)

			//Update zobrist hash
			zobrist ^= zobrist::values[64 * zobrist::pieceLookup[p] + from];
			zobrist ^= zobrist::values[64 * zobrist::pieceLookup[toPiece] + to];

			//Update bitboards
			bitboards[p] ^= (1ull << from);
			bitboards[toPiece] ^= (1ull << to);
			colorBitboards[p >> 3] ^= (1ull << from) | (1ull << to);
			allBitboard ^= (1ull << from) | (1ull << to);

			//Handle enPassant
			if (moves::isEnPassant(m)) {
				materialStart += eval::pieceValuesStart[capture];
				materialEnd += eval::pieceValuesEnd[capture];

				pos = square(to + (8 * toMoveSigned)); //Calculate square to put the pawn back into
				mailbox[pos] = capture; //Put the captured piece in the right place
				mailbox[to] = nullPiece; //And remove it from the original place
				bitboards[capture] ^= (1ull << pos); //Add it back to the bitboard
				colorBitboards[capture >> 3] ^= (1ull << pos); //Add it back to the color-specific and main bitboards
				allBitboard ^= (1ull << pos);
				zobrist ^= zobrist::values[64 * zobrist::pieceLookup[capture] + pos]; //And the zobrist hash

				//Update start and end psq values
				#pragma warning(push)
				#pragma warning(disable:6385)
				bonusesStart += (eval::pieceBonusesStart[(p & 0b111) - 1][pos ^ (56 * (1 - toMove))]) * toMoveSigned;
				bonusesEnd += (eval::pieceBonusesEnd[(p & 0b111) - 1][pos ^ (56 * (1 - toMove))]) * toMoveSigned;
				#pragma warning(pop)
			}
			else if (capture != nullPiece) { //If its non en-passant capture
				phase += eval::piecePhases[capture];
				materialStart += eval::pieceValuesStart[capture];
				materialEnd += eval::pieceValuesEnd[capture];

				bitboards[capture] ^= (1ull << to); //Add it back to the bitboard
				colorBitboards[capture >> 3] ^= (1ull << to);
				allBitboard ^= (1ull << to);
				zobrist ^= zobrist::values[64 * zobrist::pieceLookup[capture] + to]; //Add it back to the hash

				//Update start and end psq values
				#pragma warning(push)
				#pragma warning(disable:6385)
				bonusesStart += (eval::pieceBonusesStart[(p & 0b111) - 1][to ^ (56 * (1 - toMove))]) * toMoveSigned;
				bonusesEnd += (eval::pieceBonusesEnd[(p & 0b111) - 1][to ^ (56 * (1 - toMove))]) * toMoveSigned;
				#pragma warning(pop)
			}
		}

		//Readd zobrist hashing for new last move if its double push where en passant was pseudo-legal
		//This must go here after bitboards are updated
		if (moves::isDoublePush(_lastMove)) { //So may need to remove zobrist hash element for en passant
			lTo = moves::getTo(_lastMove);
			U64 bb = 1ull << lTo; //Get where it went to
			if (toMove == black) { //The move we are currently undoing was played by white, so this move before was played by black
				bb = (((bb & bitboards::notHFile) << 1) | ((bb & bitboards::notAFile) >> 1));
				if (bb & bitboards[wPawn]) {
					zobrist ^= zobrist::values[772 + (lTo % 8)];
				}
			}
			else {
				bb = (((bb & bitboards::notHFile) << 1) | ((bb & bitboards::notAFile) >> 1));
				if (bb & bitboards[bPawn]) {
					zobrist ^= zobrist::values[772 + (lTo % 8)];
				}
			}
		}

		//Update castling rights
		const auto rights = moves::getCastleChanges(m);
		if (std::get<0>(rights)) {
			wKingside = true;
			zobrist ^= zobrist::values[768];
		}
		if (std::get<1>(rights)) {
			wQueenside = true;
			zobrist ^= zobrist::values[769];
		}
		if (std::get<2>(rights)) {
			bKingside = true;
			zobrist ^= zobrist::values[770];
		}
		if (std::get<3>(rights)) {
			bQueenside = true;
			zobrist ^= zobrist::values[771];
		}

		toMove = color(1 - toMove); //Flip toMove
		toMoveSigned = -1 * toMoveSigned;
		zobrist ^= zobrist::values[780];

		positionsTail--; //Remove the positions just by decrementing the tail - it will be overwritten when necessary in makeMove
	}

	bool isDraw() {
		if (counter == 100) {
			return true;
		}
		//Repetition
		int count = 0;
		for (int i = positionsHead; i <= positionsTail; i++) {
			count = (positions[i % numPositions] == zobrist) ? count + 1 : count;
		}
		if (count == 3) {
			return true;
		}

		U64 kings = bitboards[wKing] | bitboards[bKing];

		//Insufficient material
		if ((allBitboard & ~kings) == 0) { //Just kings
			return true;
		}

		U64 bishops = bitboards[wBishop] | bitboards[bBishop];
		U64 minors = bitboards[wKnight] | bitboards[bKnight] | bishops;

		if (allBitboard == (kings | minors) && __popcnt64(minors) == 1) { //Only minor pieces left, only one minor piece left
			return true;
		}

		unsigned long index1;
		_BitScanForward64(&index1, bitboards[wBishop]);
		unsigned long index2;
		_BitScanForward64(&index2, bitboards[bBishop]);

		if (allBitboard == (kings | bishops) && __popcnt64(bitboards[wBishop]) == 1 && __popcnt64(bitboards[bBishop]) == 1) { //A bishop of each color and kings
			return sameColor(index1, index2); //Is a draw if they are the same colour
		}

		//There are more pseudo-draws that would require a complex heuristic, something to consider once search and basic eval is written

		return false; //Stalemate is handled in eval
	}

	//Move gen!!
	std::vector<move> generatePseudoLegalMoves() {
		auto generatedMoves = std::vector<move>();
		generatedMoves.reserve(218);

		U64 movesBB;
		square sq;
		piece p;
		
		//King
		p = piece(wKing + (toMove << 3));
		const square kingPos = square(SquareOf(bitboards[p]));

		//Captures
		movesBB = moveGen::kingLookup[kingPos] & colorBitboards[1 - toMove];
		Bitloop(movesBB) {
			square sq = square(SquareOf(movesBB));
			generatedMoves.push_back(moves::encodeNormal(kingPos, sq, p, mailbox[sq], false, false,
				(toMove == white && wKingside), (toMove == white && wQueenside), (toMove == black && bKingside), (toMove == black && bQueenside)));
		}

		//Non-captures
		movesBB = moveGen::kingLookup[kingPos] & ~allBitboard;
		Bitloop(movesBB) {
			sq = square(SquareOf(movesBB));
			generatedMoves.push_back(moves::encodeNormal(kingPos, sq, p, nullPiece, false, false,
				(toMove == white && wKingside), (toMove == white && wQueenside), (toMove == black && bKingside), (toMove == black && bQueenside)));
		}

		//Castling
		if ((moveGen::whiteKingCastleMask & allBitboard) == 0 && wKingside && toMove == white) { //Can white kingside castle
			generatedMoves.push_back(moves::encodeCastle(white, false, wKingside, wQueenside, false, false));
		}

		if ((moveGen::whiteQueenCastleMask & allBitboard) == 0 && wQueenside && toMove == white) { //Can white queenside castle
			generatedMoves.push_back(moves::encodeCastle(white, true, wKingside, wQueenside, false, false));
		}

		if ((moveGen::blackKingCastleMask & allBitboard) == 0 && bKingside && toMove == black) { //Can black kingside castle
			generatedMoves.push_back(moves::encodeCastle(black, false, false, false, bKingside, bQueenside));
		}

		if ((moveGen::blackQueenCastleMask & allBitboard) == 0 && bQueenside && toMove == black) { //Can black queenside castle
			generatedMoves.push_back(moves::encodeCastle(black, true, false, false, bKingside, bQueenside));
		}

		//If double check return here?

		//Pawns
		p = piece(wPawn + (toMove << 3));

		const U64 dpRank = (toMove == white) ? bitboards::rank3 : bitboards::rank6;
		constexpr U64 prRank = bitboards::rank8 | bitboards::rank1;

		//Single push
		movesBB = ((toMove == white) ? bitboards[p] << 8 : bitboards[p] >> 8) & ~allBitboard & ~prRank; //Not promotion

		Bitloop(movesBB) {
			sq = square(SquareOf(movesBB));
			generatedMoves.push_back(moves::encodeNormal(square(sq - (8 * toMoveSigned)), sq, p, nullPiece, false, false, false, false, false, false));
		}

		movesBB = ((toMove == white) ? bitboards[p] << 8 : bitboards[p] >> 8) & ~allBitboard & prRank; //Promotion

		Bitloop(movesBB) {
			sq = square(SquareOf(movesBB));
			for (piece promotePiece : {wKnight, wBishop, wRook, wQueen}) {
				generatedMoves.push_back(moves::encodePromote(square(sq - (8 * toMoveSigned)), sq, p, nullPiece, promotePiece, false, false, false, false));
			}
		}

		//Double push
		movesBB = (((toMove == white) ? bitboards[p] << 8 : bitboards[p] >> 8) & ~allBitboard & dpRank); //Get double pushable pawns on 3rd/6th rank
		movesBB = (((toMove == white) ? movesBB << 8 : movesBB >> 8) & ~allBitboard); //And push again

		Bitloop(movesBB) {
			sq = square(SquareOf(movesBB));
			generatedMoves.push_back(moves::encodeNormal(square(sq - (16*toMoveSigned)), sq, p, nullPiece, false, true, false, false, false, false));
		}

		//Left captures - white perspective so add 7 for white and subtract 9 for black - (<< (8*toMoveSigned - 1)
		movesBB = (((toMove == white) ? (bitboards[p] & ~bitboards::AFile) << 7 : (bitboards[p] & ~bitboards::AFile) >> 9) & colorBitboards[1 - toMove]) & ~prRank; //Not promotion

		Bitloop(movesBB) {
			sq = square(SquareOf(movesBB));
			generatedMoves.push_back(moves::encodeNormal(square(sq - (8 * toMoveSigned - 1)), sq, p, mailbox[sq], false, false,
				false, false, false, false)); //Only way to remove rights is taking rook, and this doesn't cover promotion
		}

		movesBB = (((toMove == white) ? (bitboards[p] & ~bitboards::AFile) << 7 : (bitboards[p] & ~bitboards::AFile) >> 9) & colorBitboards[1 - toMove]) & prRank; //Promotion

		Bitloop(movesBB) {
			sq = square(SquareOf(movesBB));
			for (piece promotePiece : {wKnight, wBishop, wRook, wQueen}) {
				generatedMoves.push_back(moves::encodePromote(square(sq - (8 * toMoveSigned - 1)), sq, p, mailbox[sq], promotePiece,
					wKingside && sq == H1, wQueenside && sq == A1, bKingside && sq == H8, bQueenside && sq == A8));
				//Only way to remove rights is taking rook, so check for castle rook squares
			}
		}

		//Right captures - white perspective so add 9 for white and subtract 7 for black - (<< (8*toMoveSigned + 1)
		movesBB = (((toMove == white) ? (bitboards[p] & ~bitboards::HFile) << 9 : (bitboards[p] & ~bitboards::HFile) >> 7) & colorBitboards[1 - toMove]) & ~prRank; //Not promotion
		 
		Bitloop(movesBB) {
			sq = square(SquareOf(movesBB));
			generatedMoves.push_back(moves::encodeNormal(square(sq - (8 * toMoveSigned + 1)), sq, p, mailbox[sq], false, false,
				false, false, false, false)); //Only way to remove rights is taking rook, and this doesn't cover promotion
		}

		movesBB = (((toMove == white) ? (bitboards[p] & ~bitboards::HFile) << 9 : (bitboards[p] & ~bitboards::HFile) >> 7) & colorBitboards[1 - toMove]) & prRank; //Promotion

		Bitloop(movesBB) {
			sq = square(SquareOf(movesBB));
			for (piece promotePiece : {wKnight, wBishop, wRook, wQueen}) {
				generatedMoves.push_back(moves::encodePromote(square(sq - (8 * toMoveSigned + 1)), sq, p, mailbox[sq], promotePiece,
					wKingside && sq == H1, wQueenside && sq == A1, bKingside && sq == H8, bQueenside && sq == A8));
				//Only way to remove rights is taking rook, so check for castle rook squares
			}
		}

		//En Passant
		if (moves::isDoublePush((moveNum == -1) ? lastMove : moves[moveNum])) {
			const square epSquare = square(moves::getTo((moveNum == -1) ? lastMove : moves[moveNum]) + (8 * toMoveSigned));
			movesBB = (1ull << epSquare);
			movesBB = (toMove == white) ? ((movesBB & ~bitboards::AFile) >> 9) | ((movesBB & ~bitboards::HFile) >> 7) : ((movesBB & ~bitboards::HFile) << 9) | ((movesBB & ~bitboards::AFile) << 7);
			movesBB &= bitboards[p]; //Get pawns that can attack the square

			Bitloop(movesBB) {
				square pos = square(SquareOf(movesBB));

				generatedMoves.push_back(moves::encodeNormal(pos, epSquare, p, piece(p ^ 0b1000), true, false, false, false, false, false));
			}
		}

		//Knights
		p = piece(wKnight + (toMove << 3));

		U64 piecesBB = bitboards[p];

		square pos, to;

		Bitloop(piecesBB) { //For each knight
			pos = square(SquareOf(piecesBB)); //Get its square

			//Captures
			movesBB = moveGen::knightLookup[pos] & colorBitboards[1 - toMove];
			Bitloop(movesBB) {
				to = square(SquareOf(movesBB));

				generatedMoves.push_back(moves::encodeNormal(pos, to, p, mailbox[to], false, false,
					wKingside && to == H1, wQueenside && to == A1, bKingside && to == H8, bQueenside && to == A8));
				//Only way to remove rights is taking rook, so check for castle rook squares
			}

			//Non-captures
			movesBB = moveGen::knightLookup[pos] & ~allBitboard; 
			Bitloop(movesBB) {
				to = square(SquareOf(movesBB));

				generatedMoves.push_back(moves::encodeNormal(pos, to, p, nullPiece, false, false, false, false, false, false));
			}
		}

		//Bishop
		p = piece(wBishop + (toMove << 3));

		piecesBB = bitboards[p];

		Bitloop(piecesBB) { //For each bishop
			pos = square(SquareOf(piecesBB)); //Get its square

			//Captures
			movesBB = moveGen::bishopMoveLookup[pos][_pext_u64(allBitboard, moveGen::bishopPextMasks[pos])] & colorBitboards[1 - toMove];
			Bitloop(movesBB) {
				to = square(SquareOf(movesBB));

				generatedMoves.push_back(moves::encodeNormal(pos, to, p, mailbox[to], false, false,
					wKingside && to == H1, wQueenside && to == A1, bKingside && to == H8, bQueenside && to == A8));
				//Only way to remove rights is taking rook, so check for castle rook squares
			}

			//Non-captures
			movesBB = moveGen::bishopMoveLookup[pos][_pext_u64(allBitboard, moveGen::bishopPextMasks[pos])] & ~allBitboard;
			Bitloop(movesBB) {
				to = square(SquareOf(movesBB));

				generatedMoves.push_back(moves::encodeNormal(pos, to, p, nullPiece, false, false, false, false, false, false));
			}
		}

		//Rook
		p = piece(wRook + (toMove << 3));

		piecesBB = bitboards[p];

		Bitloop(piecesBB) { //For each rook
			pos = square(SquareOf(piecesBB)); //Get its square

			//Captures
			movesBB = moveGen::rookMoveLookup[pos][_pext_u64(allBitboard, moveGen::rookPextMasks[pos])] & colorBitboards[1 - toMove];
			Bitloop(movesBB) {
				to = square(SquareOf(movesBB));

				generatedMoves.push_back(moves::encodeNormal(pos, to, p, mailbox[to], false, false,
					wKingside && (to == H1 || pos == H1), wQueenside && (to == A1 || pos == A1), bKingside && (to == H8 || pos == H8), bQueenside && (to == A8 || pos == A8)));
				//Be careful, rooks can move off castle squares as well as being captured by a rook
			}

			//Non-captures
			movesBB = moveGen::rookMoveLookup[pos][_pext_u64(allBitboard, moveGen::rookPextMasks[pos])] & ~allBitboard;
			Bitloop(movesBB) {
				to = square(SquareOf(movesBB));

				generatedMoves.push_back(moves::encodeNormal(pos, to, p, mailbox[to], false, false,
					wKingside && (pos == H1), wQueenside && (pos == A1), bKingside && (pos == H8), bQueenside && (pos == A8)));
				//Be careful, rooks can move off castle squares 
			}
		}

		//Queen
		p = piece(wQueen + (toMove << 3));

		piecesBB = bitboards[p];

		Bitloop(piecesBB) { //For each queen
			pos = square(SquareOf(piecesBB)); //Get its square

			//Captures like rook
			movesBB = moveGen::rookMoveLookup[pos][_pext_u64(allBitboard, moveGen::rookPextMasks[pos])] & colorBitboards[1 - toMove];
			Bitloop(movesBB) {
				to = square(SquareOf(movesBB));

				generatedMoves.push_back(moves::encodeNormal(pos, to, p, mailbox[to], false, false,
					wKingside && (to == H1), wQueenside && (to == A1), bKingside && (to == H8), bQueenside && (to == A8)));
			}
			//Only way to remove rights is taking rook, so check for castle rook squares

			//Captures like bishop
			movesBB = moveGen::bishopMoveLookup[pos][_pext_u64(allBitboard, moveGen::bishopPextMasks[pos])] & colorBitboards[1 - toMove];
			Bitloop(movesBB) {
				to = square(SquareOf(movesBB));

				generatedMoves.push_back(moves::encodeNormal(pos, to, p, mailbox[to], false, false,
					wKingside && (to == H1), wQueenside && (to == A1), bKingside && (to == H8), bQueenside && (to == A8)));
			}
			//Only way to remove rights is taking rook, so check for castle rook squares

			//Non-captures like rook
			movesBB = moveGen::rookMoveLookup[pos][_pext_u64(allBitboard, moveGen::rookPextMasks[pos])] & ~allBitboard;
			Bitloop(movesBB) {
				to = square(SquareOf(movesBB));

				generatedMoves.push_back(moves::encodeNormal(pos, to, p, nullPiece, false, false, false, false, false, false));
			}

			//Non-captures like bishop
			movesBB = moveGen::bishopMoveLookup[pos][_pext_u64(allBitboard, moveGen::bishopPextMasks[pos])] & ~allBitboard;
			Bitloop(movesBB) {
				to = square(SquareOf(movesBB));

				generatedMoves.push_back(moves::encodeNormal(pos, to, p, nullPiece, false, false, false, false, false, false));
			}
		}

		return generatedMoves;
	}

	U64 getAttacked(color by) {
		U64 attacks = 0;
		U64 pieceBB;
		square from;

		//king
		pieceBB = bitboards[wKing + (toMove << 3)];
		Bitloop(pieceBB) {
			from = square(SquareOf(pieceBB));

			attacks |= moveGen::kingLookup[from];
		}

		//Pawns
		attacks |= ((by == white) ? (bitboards[wPawn + (toMove << 3)] & ~bitboards::AFile) << 7 : (bitboards[wPawn + (toMove << 3)] & ~bitboards::AFile) >> 9);
		attacks |= ((by == white) ? (bitboards[wPawn + (toMove << 3)] & ~bitboards::HFile) << 9 : (bitboards[wPawn + (toMove << 3)] & ~bitboards::HFile) >> 7);

		//Knights
		pieceBB = bitboards[wKnight + (toMove << 3)];
		Bitloop(pieceBB) {
			from = square(SquareOf(pieceBB));

			attacks |= moveGen::knightLookup[from];
		}

		//Bishops and queen
		pieceBB = bitboards[wBishop + (toMove << 3)] | bitboards[wQueen + (toMove << 3)];
		Bitloop(pieceBB) {
			from = square(SquareOf(pieceBB));

			attacks |= moveGen::bishopMoveLookup[from][_pext_u64(allBitboard, moveGen::bishopPextMasks[from])];
		}

		//Rooks and queen
		pieceBB = bitboards[wRook + (toMove << 3)] | bitboards[wQueen + (toMove << 3)];
		Bitloop(pieceBB) {
			from = square(SquareOf(pieceBB));

			attacks |= moveGen::rookMoveLookup[from][_pext_u64(allBitboard, moveGen::rookPextMasks[from])];
		}

		return attacks;
	}

	//To check if a move was legal
	bool moveWasLegal() {
		const move _lastMove = moves[moveNum]; //We have a problem if moveNum is -1 and this is being called

		if ((moves::getPiece(_lastMove) & 0b0111) == 6 && std::abs(moves::getFrom(_lastMove) - moves::getTo(_lastMove)) == 2) {
			return  castleWasLegal();
		}

		//We want to check if the side that just moved is in check
		U64 kingBB = bitboards[wKing + ((1 - toMove) << 3)];
		square kingPos = square(SquareOf(kingBB));

		//Check for each enemy piece type here
		bool pawn = ((toMove == white) ? (((kingBB & ~bitboards::HFile) >> 7) | ((kingBB & ~bitboards::AFile) >> 9)) : (((kingBB & ~bitboards::AFile) << 7) | ((kingBB & ~bitboards::HFile) << 9))) & bitboards[wPawn + (toMove << 3)];
		
		bool knight = moveGen::knightLookup[kingPos] & bitboards[wKnight + (toMove << 3)];

		bool bishopQueen = moveGen::bishopMoveLookup[kingPos][_pext_u64(allBitboard, moveGen::bishopPextMasks[kingPos])] & (bitboards[wBishop + (toMove << 3)] | bitboards[wQueen + (toMove << 3)]);

		bool rookQueen = moveGen::rookMoveLookup[kingPos][_pext_u64(allBitboard, moveGen::rookPextMasks[kingPos])] & (bitboards[wRook + (toMove << 3)] | bitboards[wQueen + (toMove << 3)]);

		bool king = moveGen::kingLookup[kingPos] & bitboards[wKing + (toMove << 3)];

		return !(pawn || knight || bishopQueen || rookQueen || king);
	}

	bool inCheck() {
		//We want to check if the side to move
		U64 kingBB = bitboards[wKing + (toMove << 3)];
		square kingPos = square(SquareOf(kingBB));

		//Check for each enemy piece type here
		bool pawn = ((toMove == white) ? (((kingBB & ~bitboards::HFile) >> 7) | ((kingBB & ~bitboards::AFile) >> 9)) : (((kingBB & ~bitboards::AFile) << 7) | ((kingBB & ~bitboards::HFile) << 9))) & bitboards[wPawn + ((1-toMove) << 3)];

		bool knight = moveGen::knightLookup[kingPos] & bitboards[wKnight + ((1 - toMove) << 3)];

		bool bishopQueen = moveGen::bishopMoveLookup[kingPos][_pext_u64(allBitboard, moveGen::bishopPextMasks[kingPos])] & (bitboards[wBishop + ((1 - toMove) << 3)] | bitboards[wQueen + ((1 - toMove) << 3)]);

		bool rookQueen = moveGen::rookMoveLookup[kingPos][_pext_u64(allBitboard, moveGen::rookPextMasks[kingPos])] & (bitboards[wRook + ((1 - toMove) << 3)] | bitboards[wQueen + ((1 - toMove) << 3)]);

		bool king = moveGen::kingLookup[kingPos] & bitboards[wKing + ((1 - toMove) << 3)];

		return !(pawn || knight || bishopQueen || rookQueen || king);
	}

	//Special case for castling
	//Use if ((p & 0b0111) == 6 && std::abs(from - to) == 2) { //Is a king and the distance moved is 2, not 1, 7, 8 or 9 so therefore castling
	//To detect
	bool castleWasLegal() {
		const move _lastMove = moves[moveNum]; //We have a problem if moveNum is -1 and this is being called

		U64 squares = 0xFFFFFFFFFFFFFFFF;

		switch (moves::getTo(_lastMove)) {
		case C1:
			squares = moveGen::whiteQueenCastleCheckMask;
			break;
		case G1:
			squares = moveGen::whiteKingCastleCheckMask;
			break;
		case C8:
			squares = moveGen::blackQueenCastleCheckMask;
			break;
		case G8:
			squares = moveGen::blackKingCastleCheckMask;
			break;
		}

		return ((squares & getAttacked(toMove)) == 0);
	}

	int evaluate() { //In centipawns
		//Do these for now as they are not incrementally updated
		//calculateBonusesStart();
		//calculateBonusesEnd();

		//Actual calculation
		int tempo = (20 * toMoveSigned) * inCheck();
		std::cout << bonusesStart << " " << bonusesEnd << std::endl;
		return ((phase * (materialStart + bonusesStart) + (eval::maxPhase - phase) * (materialEnd + bonusesEnd)) / eval::maxPhase); // +tempo;
	}

	void calculatePhase() { //For FEN loading and reset - incrementally update the rest
		phase = 0;
		for (const piece& p : mailbox) {
			phase += eval::piecePhases[p];
		}
	}

	void calculateMaterialStart() {
		materialStart = 0;

		for (const piece& p : mailbox) {
			materialStart += eval::pieceValuesStart[p];
		}
	}

	void calculateMaterialEnd() {
		materialEnd = 0;

		for (const piece& p : mailbox) {
			materialEnd += eval::pieceValuesEnd[p];
		}
	}

	void calculateBonusesStart() {
		bonusesStart = 0;

		for (int i = -1; const piece& p : mailbox) {
			i++;
			if (p == nullPiece) {
				continue;
			}
			if ((p >> 3) == white) {
				bonusesStart += eval::pieceBonusesStart[(p & 0b111) - 1][i ^ 56];
			}
			else {
				#pragma warning(push)
				#pragma warning(disable:6385)
				bonusesStart -= eval::pieceBonusesStart[(p & 0b111) - 1][i];
				#pragma warning(pop)
			}
		}
	}

	void calculateBonusesEnd() {
		bonusesEnd = 0;

		for (int i = -1; const piece& p : mailbox) {
			i++;
			if (p == nullPiece) {
				continue;
			}
			if ((p >> 3) == white) {
				bonusesEnd += eval::pieceBonusesEnd[(p & 0b111) - 1][i ^ 56];
			}
			else {
				#pragma warning(push)
				#pragma warning(disable:6385)
				bonusesEnd -= eval::pieceBonusesEnd[(p & 0b111) - 1][i];
				#pragma warning(pop)
			}
		}
	}

	//For UCI
	move UCIMoveAsInternal(std::string m) {
		square from = stringToSquare(m.substr(0, 2));
		square to = stringToSquare(m.substr(2, 2));

		piece p = mailbox[from];
		piece capture = mailbox[to];

		if ((p & 0b0111) == wPawn && capture == nullPiece && std::abs(from-to) != 8 && std::abs(from - to) != 16) { //Is a pawn, capture is null, not a normal push
			return moves::encodeNormal(from, to, p, piece(p ^ 0b1000), true, false, false, false, false, false);
		}
		else if ((p & 0b0111) == wPawn && std::abs(from - to) == 16) { //Is a pawn, is a double push
			return moves::encodeNormal(from, to, p, nullPiece, false, true, false, false, false, false);
		}
		else if (m.length() == 5) { //Is a promotion
			switch (m[4]) {
			case 'q':
				return moves::encodePromote(from, to, p, capture, wQueen,
					wKingside && to == H1, wQueenside && to == A1, bKingside && to == H8, bQueenside && to == A8);

			case 'r':
				return moves::encodePromote(from, to, p, capture, wRook,
					wKingside && to == H1, wQueenside && to == A1, bKingside && to == H8, bQueenside && to == A8);
			case 'b':
				return moves::encodePromote(from, to, p, capture, wBishop,
					wKingside && to == H1, wQueenside && to == A1, bKingside && to == H8, bQueenside && to == A8);
			case 'n':
				return moves::encodePromote(from, to, p, capture, wKnight,
					wKingside && to == H1, wQueenside && to == A1, bKingside && to == H8, bQueenside && to == A8);
			}
		}
		else if ((p & 0b0111) == wKing && std::abs(from - to) == 2) { //Is a king move and it moved by 2 or -2 so is a castling move
			if (m == "e1g1") { //W Kingside
				return moves::encodeCastle(white, false, wKingside, wQueenside, false, false);
			}
			if (m == "e1c1") { //W Queenside
				return moves::encodeCastle(white, true, wKingside, wQueenside, false, false);
			}
			if (m == "e8g8") { //Queenside
				return moves::encodeCastle(black, false, false, false, bKingside, bQueenside);
			}
			if (m == "e8c8") { //B Queenside
				return moves::encodeCastle(black, true, false, false, bKingside, bQueenside);
			}
		}

		return moves::encodeNormal(from, to, p, capture, false, false, wKingside && to == H1, wQueenside && to == A1, bKingside && to == H8, bQueenside && to == A8);
	}

	//Debug stuff
	U64 perft(int depth, bool top) {
		if (depth == 0) {
			return 1;
		}

		U64 nodes = 0;
		U64 _nodes;

		auto moves = generatePseudoLegalMoves();

		for (auto& move : moves) {
			makeMove(move);
			if (moveWasLegal()) {
				_nodes = perft(depth - 1, false);
				if (top) {
					moves::showMove(move);
					std::cout << _nodes << std::endl;
				}
				nodes += _nodes;
			}

			undoMove();
		}

		return nodes;
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
		for (color c : {white, black}) {
			std::cout << c << std::endl;
			std::cout << colorBitboards[c] << std::endl;
			bitboards::showBitboard(colorBitboards[c]);
		}
		std::cout << zobrist << std::endl;
	}

	void debugPositions() {
		std::cout << positionsHead << " " << positionsTail << std::endl;
		for (size_t i = positionsHead; i < positionsTail; i++) {
			std::cout << positions[i % numPositions] << std::endl;
		}
	}

	U64 getZobrist() {
		return zobrist;
	}

	U64 getBitboard(int n) {
		return bitboards[n];
	}

	U64 getAllBitboard() {
		return allBitboard;
	}
}
