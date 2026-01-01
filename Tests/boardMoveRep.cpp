#include "pch.h"
#include "CppUnitTest.h"

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
}
