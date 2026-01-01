#include "pch.h"
#include "CppUnitTest.h"

#include "engine.h"
#include "moves.h"

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

	TEST_CLASS(BoardSetup) {
		//Zobrist hashes
		TEST_METHOD(ResetZobrist) {
			engine::reset();
			Assert::IsTrue(engine::getZobrist() == 0x463b96181691fc9c);
		}

		TEST_METHOD(FENZobrist1) {
			engine::loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
			Assert::IsTrue(engine::getZobrist() == 0x463b96181691fc9c);
		}

		TEST_METHOD(FENZobrist2) {
			engine::loadFEN("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");
			Assert::IsTrue(engine::getZobrist() == 0x823c9b50fd114196);
		}

		TEST_METHOD(FENZobrist3) {
			engine::loadFEN("rnbqkbnr/ppp1pppp/8/3p4/4P3/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 2");
			Assert::IsTrue(engine::getZobrist() == 0x0756b94461c50fb0);
		}

		TEST_METHOD(FENZobrist4) {
			engine::loadFEN("rnbqkbnr/ppp1pppp/8/3pP3/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 2");
			Assert::IsTrue(engine::getZobrist() == 0x662fafb965db29d4);
		}

		TEST_METHOD(FENZobrist5) {
			engine::loadFEN("rnbqkbnr/ppp1p1pp/8/3pPp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 0 3");
			Assert::IsTrue(engine::getZobrist() == 0x22a48b5a8e47ff78);
		}

		TEST_METHOD(FENZobrist6) {
			engine::loadFEN("rnbqkbnr/ppp1p1pp/8/3pPp2/8/8/PPPPKPPP/RNBQ1BNR b kq - 0 3");
			Assert::IsTrue(engine::getZobrist() == 0x652a607ca3f242c1);
		}

		TEST_METHOD(FENZobrist7) {
			engine::loadFEN("rnbq1bnr/ppp1pkpp/8/3pPp2/8/8/PPPPKPPP/RNBQ1BNR w - - 0 4");
			Assert::IsTrue(engine::getZobrist() == 0x00fdd303c946bdd9);
		}

		TEST_METHOD(FENZobrist8) {
			engine::loadFEN("rnbqkbnr/p1pppppp/8/8/PpP4P/8/1P1PPPP1/RNBQKBNR b KQkq c3 0 3");
			Assert::IsTrue(engine::getZobrist() == 0x3c8123ea7b067637);
		}

		TEST_METHOD(FENZobrist9) {
			engine::loadFEN("rnbqkbnr/p1pppppp/8/8/P6P/R1p5/1P1PPPP1/1NBQKBNR b Kkq - 0 4");
			Assert::IsTrue(engine::getZobrist() == 0x5c3f9b829b279560);
		}
	};
}
