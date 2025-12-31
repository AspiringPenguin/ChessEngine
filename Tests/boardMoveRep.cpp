#include "pch.h"
#include "CppUnitTest.h"

#include "moves.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace boardMoveRep
{
	TEST_CLASS(Moves) {
	public:
		TEST_METHOD(CheckFrom1) { //Check the right section is being extracted
			move m = moves::encodeMove(H8, A1, nullPiece, false, false, false, false, false, false, false, false);
			Assert::IsTrue(moves::getFrom(m) == H8);
		}

		TEST_METHOD(CheckFrom2) { //Check nothing overwrites
			move m = moves::encodeMove(A1, H8, bKing, true, true, true, true, true, true, true, true);
			Assert::IsTrue(moves::getFrom(m) == A1);
		}

		TEST_METHOD(CheckTo1) { //Check the right section is being extracted
			move m = moves::encodeMove(A1, H8, nullPiece, false, false, false, false, false, false, false, false);
			Assert::IsTrue(moves::getTo(m) == H8);
		}

		TEST_METHOD(CheckTo2) { //Check nothing overwrites
			move m = moves::encodeMove(H8, A1, bKing, true, true, true, true, true, true, true, true);
			Assert::IsTrue(moves::getTo(m) == A1);
		}
	};
}
