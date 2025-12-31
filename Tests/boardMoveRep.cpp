#include "pch.h"
#include "CppUnitTest.h"

#include "moves.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace boardMoveRep
{
	TEST_CLASS(Moves) {
	public:
		TEST_METHOD(CheckFrom1) { //Check the right section is being extracted
			move m = moves::encodeMove(H8, A1, nullPiece, nullPiece, false, false, false, false, false, false, false, false);
			Assert::IsTrue(moves::getFrom(m) == H8);
		}

		TEST_METHOD(CheckFrom2) { //Check nothing overwrites
			move m = moves::encodeMove(A1, H8, bKing, bKing, true, true, true, true, true, true, true, true);
			Assert::IsTrue(moves::getFrom(m) == A1);
		}

		TEST_METHOD(CheckTo1) { //Check the right section is being extracted
			move m = moves::encodeMove(A1, H8, nullPiece, nullPiece, false, false, false, false, false, false, false, false);
			Assert::IsTrue(moves::getTo(m) == H8);
		}

		TEST_METHOD(CheckTo2) { //Check nothing overwrites
			move m = moves::encodeMove(H8, A1, bKing, bKing,  true, true, true, true, true, true, true, true);
			Assert::IsTrue(moves::getTo(m) == A1);
		}

		TEST_METHOD(CheckPiece1) { //Check the right section is being extracted
			move m = moves::encodeMove(A1, A1, bKing, nullPiece, false, false, false, false, false, false, false, false);
			Assert::IsTrue(moves::getPiece(m) == bKing);
		}

		TEST_METHOD(CheckPiece2) { //Check nothing overwrites
			move m = moves::encodeMove(H8, H8, nullPiece, bKing, true, true, true, true, true, true, true, true);
			Assert::IsTrue(moves::getPiece(m) == nullPiece);
		}

		TEST_METHOD(CheckCapture1) { //Check the right section is being extracted
			move m = moves::encodeMove(A1, A1, nullPiece, bKing, false, false, false, false, false, false, false, false);
			Assert::IsTrue(moves::getCapture(m) == bKing);
		}

		TEST_METHOD(CheckCapture2) { //Check nothing overwrites
			move m = moves::encodeMove(H8, H8, bKing, nullPiece, true, true, true, true, true, true, true, true);
			Assert::IsTrue(moves::getCapture(m) == nullPiece);
		}

		//bool promote, bool special1, bool special2, bool enPassant, bool wkc, bool wqc, bool bkc, bool bqc

		TEST_METHOD(CheckPromoteFlag1) { //Check the right section is being extracted
			move m = moves::encodeMove(A1, A1, nullPiece, nullPiece, true, false, false, false, false, false, false, false);
			Assert::IsTrue(moves::getPromoteFlag(m) == true);
		}

		TEST_METHOD(CheckPromoteFlag2) { //Check nothing overwrites
			move m = moves::encodeMove(H8, H8, bKing, bKing, false, true, true, true, true, true, true, true);
			Assert::IsTrue(moves::getPromoteFlag(m) == false);
		}
	};
}
