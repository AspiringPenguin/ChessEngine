#include "pch.h"
#include "CppUnitTest.h"

#include "moves.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(Moves)
	{
	public:
		TEST_METHOD(CheckFrom)
		{
			move m = moves::encodeMove(H8, A1, nullPiece, false, false, false, false, false, false, false, false);
			Assert::IsTrue(moves::getFrom(m) == H8);
		}
	};
}
