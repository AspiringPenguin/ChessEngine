#include "pch.h"
#include "CppUnitTest.h"

#include "moves.h"
#include "perft.h"
#include "position.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace boardMoveRep
{
	TEST_CLASS(MoveEncoding) {
	public:
		//Normal moves
		TEST_METHOD(From1) {
			move m = moves::encodeNormal(E2, E4, wPawn, nullPiece, false, true, false, false, false, false);
			Assert::IsTrue(moves::getFrom(m) == E2);
		}

		TEST_METHOD(To1) {
			move m = moves::encodeNormal(E2, E4, wPawn, nullPiece, false, true, false, false, false, false);
			Assert::IsTrue(moves::getTo(m) == E4);
		}

		TEST_METHOD(Piece1) {
			move m = moves::encodeNormal(E2, E4, wPawn, nullPiece, false, true, false, false, false, false);
			Assert::IsTrue(moves::getPiece(m) == wPawn);
		}

		TEST_METHOD(Capture1) {
			move m = moves::encodeNormal(E2, E4, wPawn, nullPiece, false, true, false, false, false, false);
			Assert::IsTrue(moves::getCapture(m) == nullPiece);
		}

		TEST_METHOD(EnPassant1) {
			move m = moves::encodeNormal(E2, E4, wPawn, nullPiece, false, true, false, false, false, false);
			Assert::IsTrue(moves::isEnPassant(m) == false);
		}

		TEST_METHOD(DoublePush1) {
			move m = moves::encodeNormal(E2, E4, wPawn, nullPiece, false, true, false, false, false, false);
			Assert::IsTrue(moves::isDoublePush(m) == true);
		}

		TEST_METHOD(CastleRights1) {
			move m = moves::encodeNormal(E2, E4, wPawn, nullPiece, false, true, false, false, false, false);
			Assert::IsTrue(moves::getCastleChanges(m) == std::tuple<bool, bool, bool, bool>{false, false, false, false});
		}

		//With a capture
		TEST_METHOD(From2) {
			move m = moves::encodeNormal(D8, D4, bQueen, wPawn, false, false, false, false, false, false);
			Assert::IsTrue(moves::getFrom(m) == D8);
		}

		TEST_METHOD(To2) {
			move m = moves::encodeNormal(D8, D4, bQueen, wPawn, false, false, false, false, false, false);
			Assert::IsTrue(moves::getTo(m) == D4);
		}

		TEST_METHOD(Piece2) {
			move m = moves::encodeNormal(D8, D4, bQueen, wPawn, false, false, false, false, false, false);
			Assert::IsTrue(moves::getPiece(m) == bQueen);
		}

		TEST_METHOD(Capture2) {
			move m = moves::encodeNormal(D8, D4, bQueen, wPawn, false, false, false, false, false, false);
			Assert::IsTrue(moves::getCapture(m) == wPawn);
		}

		TEST_METHOD(EnPassant2) {
			move m = moves::encodeNormal(D8, D4, bQueen, wPawn, false, false, false, false, false, false);
			Assert::IsTrue(moves::isEnPassant(m) == false);
		}

		TEST_METHOD(DoublePush2) {
			move m = moves::encodeNormal(D8, D4, bQueen, wPawn, false, false, false, false, false, false);
			Assert::IsTrue(moves::isDoublePush(m) == false);
		}

		TEST_METHOD(CastleRights2) {
			move m = moves::encodeNormal(D8, D4, bQueen, wPawn, false, false, false, false, false, false);
			Assert::IsTrue(moves::getCastleChanges(m) == std::tuple<bool, bool, bool, bool>{false, false, false, false});
		}

		//Removing castling rights
		TEST_METHOD(CastleRights3) {
			move m = moves::encodeNormal(D8, D4, bQueen, wPawn, false, false, true, false, false, false);
			Assert::IsTrue(moves::getCastleChanges(m) == std::tuple<bool, bool, bool, bool>{true, false, false, false});
		}

		TEST_METHOD(CastleRights4) {
			move m = moves::encodeNormal(D8, D4, bQueen, wPawn, false, false, false, true, false, false);
			Assert::IsTrue(moves::getCastleChanges(m) == std::tuple<bool, bool, bool, bool>{false, true, false, false});
		}

		TEST_METHOD(CastleRights5) {
			move m = moves::encodeNormal(D8, D4, bQueen, wPawn, false, false, false, false, true, false);
			Assert::IsTrue(moves::getCastleChanges(m) == std::tuple<bool, bool, bool, bool>{false, false, true, false});
		}

		TEST_METHOD(CastleRights6) {
			move m = moves::encodeNormal(D8, D4, bQueen, wPawn, false, false, false, false, false, true);
			Assert::IsTrue(moves::getCastleChanges(m) == std::tuple<bool, bool, bool, bool>{false, false, false, true});
		}

		//Test promotions
		TEST_METHOD(EnPassantPromote) {
			move m = moves::encodePromote(C7, C8, wPawn, nullPiece, wQueen, false, false, false, false);
			Assert::IsTrue(moves::isEnPassant(m) == false);
		}

		TEST_METHOD(DoublePushPromote) {
			move m = moves::encodePromote(C7, C8, wPawn, nullPiece, wQueen, false, false, false, false);
			Assert::IsTrue(moves::isDoublePush(m) == false);
		}

		TEST_METHOD(PromoteWQueen) {
			move m = moves::encodePromote(C7, C8, wPawn, nullPiece, wQueen, false, false, false, false);
			Assert::IsTrue(moves::getPromote(m, white) == wQueen);
		}
		TEST_METHOD(PromoteBQueen) {
			move m = moves::encodePromote(C7, C8, wPawn, nullPiece, bQueen, false, false, false, false);
			Assert::IsTrue(moves::getPromote(m, black) == bQueen);
		}

		TEST_METHOD(PromoteWRook) {
			move m = moves::encodePromote(C7, C8, wPawn, nullPiece, wRook, false, false, false, false);
			Assert::IsTrue(moves::getPromote(m, white) == wRook);
		}
		TEST_METHOD(PromoteBRook) {
			move m = moves::encodePromote(C7, C8, wPawn, nullPiece, bRook, false, false, false, false);
			Assert::IsTrue(moves::getPromote(m, black) == bRook);
		}

		TEST_METHOD(PromoteWBishop) {
			move m = moves::encodePromote(C7, C8, wPawn, nullPiece, wBishop, false, false, false, false);
			Assert::IsTrue(moves::getPromote(m, white) == wBishop);
		}
		TEST_METHOD(PromoteBBishop) {
			move m = moves::encodePromote(C7, C8, wPawn, nullPiece, bBishop, false, false, false, false);
			Assert::IsTrue(moves::getPromote(m, black) == bBishop);
		}

		TEST_METHOD(PromoteWKnight) {
			move m = moves::encodePromote(C7, C8, wPawn, nullPiece, wKnight, false, false, false, false);
			Assert::IsTrue(moves::getPromote(m, white) == wKnight);
		}
		TEST_METHOD(PromoteBKnight) {
			move m = moves::encodePromote(C7, C8, wPawn, nullPiece, bKnight, false, false, false, false);
			Assert::IsTrue(moves::getPromote(m, black) == bKnight);
		}

		// EnPassant
		TEST_METHOD(EnPassant3) {
			move m = moves::encodeNormal(E5, D6, wPawn, bPawn, true, false, false, false, false, false);
			Assert::IsTrue(moves::isEnPassant(m) == true);
		}
	};

	TEST_CLASS(BoardSetupZobrist) {
		//Zobrist hashes
		TEST_METHOD(ResetZobrist) {
			auto p = Position();
			Assert::IsTrue(p.zobrist == 0x463b96181691fc9c);
		}

		TEST_METHOD(FENZobrist1) {
			auto p = Position("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
			Assert::IsTrue(p.zobrist == 0x463b96181691fc9c);
		}

		TEST_METHOD(FENZobrist2) {
			auto p = Position("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
			Assert::IsTrue(p.zobrist == 0x823c9b50fd114196);
		}

		TEST_METHOD(FENZobrist3) {
			auto p = Position("rnbqkbnr/ppp1pppp/8/3p4/4P3/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 2");
			Assert::IsTrue(p.zobrist == 0x0756b94461c50fb0);
		}

		TEST_METHOD(FENZobrist4) {
			auto p = Position("rnbqkbnr/ppp1pppp/8/3pP3/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 2");
			Assert::IsTrue(p.zobrist == 0x662fafb965db29d4);
		}

		TEST_METHOD(FENZobrist5) {
			auto p = Position("rnbqkbnr/ppp1p1pp/8/3pPp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 3");
			Assert::IsTrue(p.zobrist == 0x22a48b5a8e47ff78);
		}

		TEST_METHOD(FENZobrist6) {
			auto p = Position("rnbqkbnr/ppp1p1pp/8/3pPp2/8/8/PPPPKPPP/RNBQ1BNR b kq - 0 3");
			Assert::IsTrue(p.zobrist == 0x652a607ca3f242c1);
		}

		TEST_METHOD(FENZobrist7) {
			auto p = Position("rnbq1bnr/ppp1pkpp/8/3pPp2/8/8/PPPPKPPP/RNBQ1BNR w - - 0 4");
			Assert::IsTrue(p.zobrist == 0x00fdd303c946bdd9);
		}

		TEST_METHOD(FENZobrist8) {
			auto p = Position("rnbqkbnr/p1pppppp/8/8/PpP4P/8/1P1PPPP1/RNBQKBNR b KQkq c3 0 3");
			Assert::IsTrue(p.zobrist == 0x3c8123ea7b067637);
		}

		TEST_METHOD(FENZobrist9) {
			auto p = Position("rnbqkbnr/p1pppppp/8/8/P6P/R1p5/1P1PPPP1/1NBQKBNR b Kkq - 0 4");
			Assert::IsTrue(p.zobrist == 0x5c3f9b829b279560);
		}
	};

	TEST_CLASS(MakeUndoZobrist) {
		//Test crudely by checking the zobrist hash after applying multiple moves, and after undoing them all.

		TEST_METHOD(DoublePushOnlyApply) {
			auto p = Position();

			move m1 = moves::encodeNormal(E2, E4, wPawn, nullPiece, false, true, false, false, false, false);
			move m2 = moves::encodeNormal(D7, D6, bPawn, nullPiece, false, false, false, false, false, false);
			move m3 = moves::encodeNormal(D2, D4, wPawn, nullPiece, false, true, false, false, false, false);
			move m4 = moves::encodeNormal(G7, G6, bPawn, nullPiece, false, false, false, false, false, false);
			move m5 = moves::encodeNormal(B1, C3, wKnight, nullPiece, false, false, false, false, false, false);
			move m6 = moves::encodeNormal(F8, G7, bBishop, nullPiece, false, false, false, false, false, false);
			move m7 = moves::encodeNormal(G1, F3, wKnight, nullPiece, false, false, false, false, false, false);
			move m8 = moves::encodeNormal(G8, F6, bKnight, nullPiece, false, false, false, false, false, false);
			move m9 = moves::encodeNormal(C1, E3, wBishop, nullPiece, false, false, false, false, false, false);
			move m10 = moves::encodeNormal(C8, G4, bBishop, nullPiece, false, false, false, false, false, false);
			
			p.makeMove(m1);
			p.makeMove(m2);
			p.makeMove(m3);
			p.makeMove(m4);
			p.makeMove(m5);
			p.makeMove(m6);
			p.makeMove(m7);
			p.makeMove(m8);
			p.makeMove(m9);
			p.makeMove(m10);

			Assert::IsTrue(p.zobrist == 0x2d7dfd097f29cb4e);
		}

		TEST_METHOD(DoublePushOnlyUndo) {
			auto p = Position();

			move m1 = moves::encodeNormal(E2, E4, wPawn, nullPiece, false, true, false, false, false, false);
			move m2 = moves::encodeNormal(D7, D6, bPawn, nullPiece, false, false, false, false, false, false);
			move m3 = moves::encodeNormal(D2, D4, wPawn, nullPiece, false, true, false, false, false, false);
			move m4 = moves::encodeNormal(G7, G6, bPawn, nullPiece, false, false, false, false, false, false);
			move m5 = moves::encodeNormal(B1, C3, wKnight, nullPiece, false, false, false, false, false, false);
			move m6 = moves::encodeNormal(F8, G7, bBishop, nullPiece, false, false, false, false, false, false);
			move m7 = moves::encodeNormal(G1, F3, wKnight, nullPiece, false, false, false, false, false, false);
			move m8 = moves::encodeNormal(G8, F6, bKnight, nullPiece, false, false, false, false, false, false);
			move m9 = moves::encodeNormal(C1, E3, wBishop, nullPiece, false, false, false, false, false, false);
			move m10 = moves::encodeNormal(C8, G4, bBishop, nullPiece, false, false, false, false, false, false);

			p.makeMove(m1);
			p.makeMove(m2);
			p.makeMove(m3);
			p.makeMove(m4);
			p.makeMove(m5);
			p.makeMove(m6);
			p.makeMove(m7);
			p.makeMove(m8);
			p.makeMove(m9);
			p.makeMove(m10);

			p.undoMove();
			p.undoMove();
			p.undoMove();
			p.undoMove();
			p.undoMove();
			p.undoMove();
			p.undoMove();
			p.undoMove();
			p.undoMove();
			p.undoMove();

			Assert::IsTrue(p.zobrist == 0x463b96181691fc9c);
		}

		TEST_METHOD(EnPassantApply) {
			auto p = Position();
			
			move m1 = moves::encodeNormal(A2, A4, wPawn, nullPiece, false, true, false, false, false, false);
			move m2 = moves::encodeNormal(B7, B5, bPawn, nullPiece, false, true, false, false, false, false);
			move m3 = moves::encodeNormal(H2, H4, wPawn, nullPiece, false, true, false, false, false, false);
			move m4 = moves::encodeNormal(B5, B4, bPawn, nullPiece, false, false, false, false, false, false);
			move m5 = moves::encodeNormal(C2, C4, wPawn, nullPiece, false, true, false, false, false, false);
			move m6 = moves::encodeNormal(B4, C3, bPawn, wPawn, true, false, false, false, false, false);
			move m7 = moves::encodeNormal(A1, A3, wRook, nullPiece, false, false, false, true, false, false);

			p.makeMove(m1);
			p.makeMove(m2);
			p.makeMove(m3);
			p.makeMove(m4);
			p.makeMove(m5);
			p.makeMove(m6);
			p.makeMove(m7);

			Assert::IsTrue(p.zobrist = 0x5c3f9b829b279560);
		}

		TEST_METHOD(EnPassantUndo) {
			auto p = Position();

			move m1 = moves::encodeNormal(A2, A4, wPawn, nullPiece, false, true, false, false, false, false);
			move m2 = moves::encodeNormal(B7, B5, bPawn, nullPiece, false, true, false, false, false, false);
			move m3 = moves::encodeNormal(H2, H4, wPawn, nullPiece, false, true, false, false, false, false);
			move m4 = moves::encodeNormal(B5, B4, bPawn, nullPiece, false, false, false, false, false, false);
			move m5 = moves::encodeNormal(C2, C4, wPawn, nullPiece, false, true, false, false, false, false);
			move m6 = moves::encodeNormal(B4, C3, bPawn, wPawn, true, false, false, false, false, false);
			move m7 = moves::encodeNormal(A1, A3, wRook, nullPiece, false, false, false, true, false, false);

			p.makeMove(m1);
			p.makeMove(m2);
			p.makeMove(m3);
			p.makeMove(m4);
			p.makeMove(m5);
			p.makeMove(m6);
			p.makeMove(m7);

			p.undoMove();
			p.undoMove();
			p.undoMove();
			p.undoMove();
			p.undoMove();
			p.undoMove();
			p.undoMove();

			Assert::IsTrue(p.zobrist == 0x463b96181691fc9c);
		}

		TEST_METHOD(KingsideApply) {
			auto p = Position();

			move m1 = moves::encodeNormal(E2, E4, wPawn, nullPiece, false, true, false, false, false, false);
			move m2 = moves::encodeNormal(E7, E5, bPawn, nullPiece, false, true, false, false, false, false);
			move m3 = moves::encodeNormal(G1, F3, wKnight, nullPiece, false, false, false, false, false, false);
			move m4 = moves::encodeNormal(G8, F6, bKnight, nullPiece, false, false, false, false, false, false);
			move m5 = moves::encodeNormal(F1, D3, wBishop, nullPiece, false, false, false, false, false, false);
			move m6 = moves::encodeNormal(F8, D6, bBishop, nullPiece, false, false, false, false, false, false);
			move m7 = moves::encodeCastle(white, false, true, true, true, true);
			move m8 = moves::encodeCastle(black, false, false, false, true, true);

			p.makeMove(m1);
			p.makeMove(m2);
			p.makeMove(m3);
			p.makeMove(m4);
			p.makeMove(m5);
			p.makeMove(m6);
			p.makeMove(m7);
			p.makeMove(m8);

			Assert::IsTrue(p.zobrist == 0x0d4611bae4d8b8a7);
		}

		TEST_METHOD(KingsideUndo) {
			auto p = Position();

			move m1 = moves::encodeNormal(E2, E4, wPawn, nullPiece, false, true, false, false, false, false);
			move m2 = moves::encodeNormal(E7, E5, bPawn, nullPiece, false, true, false, false, false, false);
			move m3 = moves::encodeNormal(G1, F3, wKnight, nullPiece, false, false, false, false, false, false);
			move m4 = moves::encodeNormal(G8, F6, bKnight, nullPiece, false, false, false, false, false, false);
			move m5 = moves::encodeNormal(F1, D3, wBishop, nullPiece, false, false, false, false, false, false);
			move m6 = moves::encodeNormal(F8, D6, bBishop, nullPiece, false, false, false, false, false, false);
			move m7 = moves::encodeCastle(white, false, true, true, true, true);
			move m8 = moves::encodeCastle(black, false, false, false, true, true);

			p.makeMove(m1);
			p.makeMove(m2);
			p.makeMove(m3);
			p.makeMove(m4);
			p.makeMove(m5);
			p.makeMove(m6);
			p.makeMove(m7);
			p.makeMove(m8);

			p.undoMove();
			p.undoMove();
			p.undoMove();
			p.undoMove();
			p.undoMove();
			p.undoMove();
			p.undoMove();
			p.undoMove();

			Assert::IsTrue(p.zobrist == 0x463b96181691fc9c);
		}

		TEST_METHOD(QueensideApply) {
			auto p = Position();

			move m1 = moves::encodeNormal(D2, D3, wPawn, nullPiece, false, false, false, false, false, false);
			move m2 = moves::encodeNormal(D7, D6, bPawn, nullPiece, false, false, false, false, false, false);
			move m3 = moves::encodeNormal(C1, E3, wBishop, nullPiece, false, false, false, false, false, false);
			move m4 = moves::encodeNormal(C8, E6, bBishop, nullPiece, false, false, false, false, false, false);
			move m5 = moves::encodeNormal(D1, D2, wQueen, nullPiece, false, false, false, false, false, false);
			move m6 = moves::encodeNormal(D8, D7, bQueen, nullPiece, false, false, false, false, false, false);
			move m7 = moves::encodeNormal(B1, C3, wKnight, nullPiece, false, false, false, false, false, false);
			move m8 = moves::encodeNormal(B8, C6, bKnight, nullPiece, false, false, false, false, false, false);
			move m9 = moves::encodeCastle(white, true, true, true, true, true);
			move m10 = moves::encodeCastle(black, true, false, false, true, true);

			p.makeMove(m1);
			p.makeMove(m2);
			p.makeMove(m3);
			p.makeMove(m4);
			p.makeMove(m5);
			p.makeMove(m6);
			p.makeMove(m7);
			p.makeMove(m8);
			p.makeMove(m9);
			p.makeMove(m10);

			Assert::IsTrue(p.zobrist == 0x987ff0f0beae1834);
		}

		TEST_METHOD(QueensideUndo) {
			auto p = Position();

			move m1 = moves::encodeNormal(D2, D3, wPawn, nullPiece, false, false, false, false, false, false);
			move m2 = moves::encodeNormal(D7, D6, bPawn, nullPiece, false, false, false, false, false, false);
			move m3 = moves::encodeNormal(C1, E3, wBishop, nullPiece, false, false, false, false, false, false);
			move m4 = moves::encodeNormal(C8, E6, bBishop, nullPiece, false, false, false, false, false, false);
			move m5 = moves::encodeNormal(D1, D2, wQueen, nullPiece, false, false, false, false, false, false);
			move m6 = moves::encodeNormal(D8, D7, bQueen, nullPiece, false, false, false, false, false, false);
			move m7 = moves::encodeNormal(B1, C3, wKnight, nullPiece, false, false, false, false, false, false);
			move m8 = moves::encodeNormal(B8, C6, bKnight, nullPiece, false, false, false, false, false, false);
			move m9 = moves::encodeCastle(white, true, true, true, true, true);
			move m10 = moves::encodeCastle(black, true, false, false, true, true);

			p.makeMove(m1);
			p.makeMove(m2);
			p.makeMove(m3);
			p.makeMove(m4);
			p.makeMove(m5);
			p.makeMove(m6);
			p.makeMove(m7);
			p.makeMove(m8);
			p.makeMove(m9);
			p.makeMove(m10);

			p.undoMove();
			p.undoMove();
			p.undoMove();
			p.undoMove();
			p.undoMove();
			p.undoMove();
			p.undoMove();
			p.undoMove();
			p.undoMove();
			p.undoMove();

			Assert::IsTrue(p.zobrist == 0x463b96181691fc9c);
		}
	};

	TEST_CLASS(Perft) {
		TEST_METHOD(StartDepth1) {
			auto p = perft::PerftSearcher();

			int res = p.go(1);

			Assert::IsTrue(res == 20);
		}

		TEST_METHOD(StartDepth2) {
			auto p = perft::PerftSearcher();

			int res = p.go(2);

			Assert::IsTrue(res == 400);
		}

		TEST_METHOD(StartDepth3) {
			auto p = perft::PerftSearcher();

			int res = p.go(3);

			Assert::IsTrue(res == 8902);
		}

		TEST_METHOD(StartDepth4) {
			auto p = perft::PerftSearcher();

			int res = p.go(4);

			Assert::IsTrue(res == 197281);
		}

		TEST_METHOD(StartDepth5) {
			auto p = perft::PerftSearcher();

			int res = p.go(5);

			Assert::IsTrue(res == 4865609);
		}

		TEST_METHOD(StartDepth6) {
			auto p = perft::PerftSearcher();

			int res = p.go(6);

			Assert::IsTrue(res == 119060324);
		}
	};
}
