#include "stdafx.h"

#include "../MineSweeperLib/MineSweeperFactory.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MineSweeper
{		
	TEST_CLASS(UnitTest1)
	{
	public:
        TEST_METHOD(TestMineIsASquare)
        {
            const std::string sMineFieldSpec =
                "4 4\n"
                "....\n"
                "....\n"
                ".*..\n"
                "....\n"
                "0 0\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            bool bIsMineASquare = mineSweeper.GetMineFields()[0].IsSquareAMine(1, 2);

            Assert::IsTrue(bIsMineASquare);
        }

        TEST_METHOD(Test1AdjacentSquareIsAMine)
        {
            const std::string sMineFieldSpec =
                "3 3\n"
                "...\n"
                ".*.\n"
                "...\n"
                "0 0\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            int nAdjacentSquareCount = mineSweeper.GetMineFields()[0].GetAdjacentMineCount(0, 0);

            Assert::AreEqual(1, nAdjacentSquareCount);
        }

        TEST_METHOD(Test2AdjacentSquaresAreMines)
        {
            const std::string sMineFieldSpec =
                "3 3\n"
                "...\n"
                "*.*\n"
                "...\n"
                "0 0\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            int nAdjacentSquareCount = mineSweeper.GetMineFields()[0].GetAdjacentMineCount(1, 1);

            Assert::AreEqual(2, nAdjacentSquareCount);
        }

        TEST_METHOD(TestSurroundedByMines)
        {
            const std::string sMineFieldSpec =
                "3 3\n"
                "***\n"
                "*.*\n"
                "***\n"
                "0 0\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            int nAdjacentSquareCount = mineSweeper.GetMineFields()[0].GetAdjacentMineCount(1, 1);

            Assert::AreEqual(8, nAdjacentSquareCount);
        }

        TEST_METHOD(TestSelfNotCounted)
        {
            const std::string sMineFieldSpec =
                "3 3\n"
                ".*.\n"
                ".*.\n"
                ".*.\n"
                "0 0\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            int nAdjacentSquareCount = mineSweeper.GetMineFields()[0].GetAdjacentMineCount(1, 1);

            Assert::AreEqual(2, nAdjacentSquareCount);
        }

        TEST_METHOD(TestMineOnLeftEdgeIsCountedCorrectly)
        {
            const std::string sMineFieldSpec =
                "3 3\n"
                "*..\n"
                "*..\n"
                "*..\n"
                "0 0\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            int nAdjacentSquareCount = mineSweeper.GetMineFields()[0].GetAdjacentMineCount(0, 1);

            Assert::AreEqual(2, nAdjacentSquareCount);
        }

        TEST_METHOD(TestMineOnRightEdgeIsCountedCorrectly)
        {
            const std::string sMineFieldSpec =
                "2 5\n"
                ".*\n"
                ".*\n"
                ".*\n"
                ".*\n"
                ".*\n"
                "0 0\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            int nAdjacentSquareCount = mineSweeper.GetMineFields()[0].GetAdjacentMineCount(1, 2);

            Assert::AreEqual(2, nAdjacentSquareCount);
        }

        TEST_METHOD(TestMineOnTopEdgeIsCountedCorrectly)
        {
            const std::string sMineFieldSpec =
                "7 4\n"
                "*******\n"
                "*......\n"
                "*......\n"
                "*......\n"
                "0 0\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            int nAdjacentSquareCount = mineSweeper.GetMineFields()[0].GetAdjacentMineCount(5, 0);

            Assert::AreEqual(2, nAdjacentSquareCount);
        }

        TEST_METHOD(TestMineOnBottomEdgeIsCountedCorrectly)
        {
            const std::string sMineFieldSpec =
                "7 4\n"
                ".......\n"
                ".......\n"
                "*******\n"
                "*****.*\n"
                "0 0\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            int nAdjacentSquareCount = mineSweeper.GetMineFields()[0].GetAdjacentMineCount(5, 3);

            Assert::AreEqual(5, nAdjacentSquareCount);
        }

        TEST_METHOD(TestMineInTLCornerIsCountedCorrectly)
        {
            const std::string sMineFieldSpec =
                "2 2\n"
                "*.\n"
                "*.\n"
                "0 0\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            int nAdjacentSquareCount = mineSweeper.GetMineFields()[0].GetAdjacentMineCount(0, 0);

            Assert::AreEqual(1, nAdjacentSquareCount);
        }

        TEST_METHOD(TestMineInTRCornerIsCountedCorrectly)
        {
            const std::string sMineFieldSpec =
                "2 2\n"
                ".*\n"
                ".*\n"
                "0 0\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            int nAdjacentSquareCount = mineSweeper.GetMineFields()[0].GetAdjacentMineCount(1, 0);

            Assert::AreEqual(1, nAdjacentSquareCount);
        }

        TEST_METHOD(TestMineInBLCornerIsCountedCorrectly)
        {
            const std::string sMineFieldSpec =
                "2 2\n"
                "*.\n"
                "*.\n"
                "0 0\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            int nAdjacentSquareCount = mineSweeper.GetMineFields()[0].GetAdjacentMineCount(0, 1);

            Assert::AreEqual(1, nAdjacentSquareCount);
        }

        TEST_METHOD(TestMineInBRCornerIsCountedCorrectly)
        {
            const std::string sMineFieldSpec =
                "2 2\n"
                ".*\n"
                ".*\n"
                "0 0\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            int nAdjacentSquareCount = mineSweeper.GetMineFields()[0].GetAdjacentMineCount(1, 1);

            Assert::AreEqual(1, nAdjacentSquareCount);
        }

        TEST_METHOD(TestZeroLengthListParsesCorrectly)
        {
            const std::string sMineFieldSpec =
                "0 0\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            std::size_t nMineFieldCount = mineSweeper.GetMineFields().size();

            Assert::AreEqual(static_cast<std::size_t>(0), nMineFieldCount);
        }

        TEST_METHOD(TestMultipleMineFieldsAreCountedCorrectly)
        {
            const std::string sMineFieldSpec =
                "3 3\n"
                "...\n"
                ".*.\n"
                "...\n"
                "3 3\n"
                "...\n"
                ".*.\n"
                "...\n"
                "3 3\n"
                "...\n"
                ".*.\n"
                "...\n"
                "3 3\n"
                "...\n"
                ".*.\n"
                "...\n"
                "0 0\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            std::size_t nMineFieldCount = mineSweeper.GetMineFields().size();

            Assert::AreEqual(static_cast<std::size_t>(4), nMineFieldCount);
        }

        TEST_METHOD(TestSingleMineInCentreIsOutputtedCorrectly)
        {
            const std::string sMineFieldSpec =
                "3 3\n"
                "...\n"
                ".*.\n"
                "...\n"
                "0 0\n";

            const std::string sExpectedOutput =
                "    111\n"
                "    1*1\n"
                "    111\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            std::string sActualOutput = mineSweeper.GetMineFields()[0].GenerateOutput();

            Assert::AreEqual(sExpectedOutput, sActualOutput);
        }

        TEST_METHOD(TestListOf1MineFieldIsOutputtedCorrectly)
        {
            const std::string sMineFieldSpec =
                "2 2\n"
                ".*\n"
                ".*\n"
                "0 0\n";

            const std::string sExpectedOutput =
                "Field #1:\n"
                "    2*\n"
                "    2*\n"
                "\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            std::string sActualOutput = mineSweeper.GenerateOutput();

            Assert::AreEqual(sExpectedOutput, sActualOutput);
        }

        TEST_METHOD(TestMultipleMineFieldsAreOutputtedCorrectly)
        {
            const std::string sMineFieldSpec =
                "3 3\n"
                "...\n"
                ".*.\n"
                "...\n"
                "2 2\n"
                "..\n"
                "..\n"
                "7 7\n"
                "*......\n"
                ".*.....\n"
                "..*....\n"
                "...*...\n"
                "....*..\n"
                ".....*.\n"
                "......*\n"
                "10 1\n"
                "..*.......\n"
                "0 0\n";

            const std::string sExpectedOutput =
                "Field #1:\n"
                "    111\n"
                "    1*1\n"
                "    111\n"
                "\n"
                "Field #2:\n"
                "    00\n"
                "    00\n"
                "\n"
                "Field #3:\n"
                "    *210000\n"
                "    2*21000\n"
                "    12*2100\n"
                "    012*210\n"
                "    0012*21\n"
                "    00012*2\n"
                "    000012*\n"
                "\n"
                "Field #4:\n"
                "    01*1000000\n"
                "\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            std::string sActualOutput = mineSweeper.GenerateOutput();

            Assert::AreEqual(sExpectedOutput, sActualOutput);
        }

        TEST_METHOD(TestSuggestedTestCasesFromTddExercise)
        {
            const std::string sMineFieldSpec =
                "4 4\n"
                "*...\n"
                "....\n"
                ".*..\n"
                "....\n"
                "5 3\n"
                "**...\n"
                ".....\n"
                ".*...\n"
                "0 0\n";

            const std::string sExpectedOutput =
                "Field #1:\n"
                "    *100\n"
                "    2210\n"
                "    1*10\n"
                "    1110\n"
                "\n"
                "Field #2:\n"
                "    **100\n"
                "    33200\n"
                "    1*100\n"
                "\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            std::string sActualOutput = mineSweeper.GenerateOutput();

            Assert::AreEqual(sExpectedOutput, sActualOutput);
        }

        TEST_METHOD(TestSquareIsOpened)
        {
            const std::string sMineFieldSpec =
                "7 3\n"
                "...*...\n"
                "***..*.\n"
                "....*..\n"
                "0 0\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            mineSweeper.GetMineFields()[0].OpenSquare(1, 2);
            bool bIsSquareOpened = mineSweeper.GetMineFields()[0].IsSquareOpened(1, 2);

            Assert::IsTrue(bIsSquareOpened);
        }

        TEST_METHOD(TestSquareIsNotOpenedAtGameStart)
        {
            const std::string sMineFieldSpec =
                "7 5\n"
                ".*.....\n"
                "..*....\n"
                "...*...\n"
                "....*..\n"
                ".....*.\n"
                "0 0\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            bool bIsSquareOpened = mineSweeper.GetMineFields()[0].IsSquareOpened(4, 3);

            Assert::IsFalse(bIsSquareOpened);
        }

        TEST_METHOD(TestSquareCanBeMarkedAsMine)
        {
            const std::string sMineFieldSpec =
                "7 3\n"
                "...*...\n"
                "***..*.\n"
                "....*..\n"
                "0 0\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            mineSweeper.GetMineFields()[0].MarkSquareAsMine(5, 2);
            bool bIsSquareOpened = mineSweeper.GetMineFields()[0].IsSquareMarkedAsMine(5, 2);

            Assert::IsTrue(bIsSquareOpened);
        }

        TEST_METHOD(TestSquareCanBeMarkedAsUnknown)
        {
            const std::string sMineFieldSpec =
                "7 3\n"
                "...*...\n"
                "***..*.\n"
                "....*..\n"
                "0 0\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            mineSweeper.GetMineFields()[0].MarkSquareAsUnknown(5, 2);
            bool bIsSquareOpened = mineSweeper.GetMineFields()[0].IsSquareMarkedAsUnknown(5, 2);

            Assert::IsTrue(bIsSquareOpened);
        }

        TEST_METHOD(TestSquareCanBeUnmarked)
        {
            const std::string sMineFieldSpec =
                "7 3\n"
                "...*...\n"
                "***..*.\n"
                "....*..\n"
                "0 0\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            mineSweeper.GetMineFields()[0].MarkSquareAsMine(3, 0);
            mineSweeper.GetMineFields()[0].UnmarkSquare(3, 0);
            bool bIsSquareOpened = mineSweeper.GetMineFields()[0].IsSquareUnmarked(3, 0);

            Assert::IsTrue(bIsSquareOpened);
        }

        TEST_METHOD(TestSquareIsUnmarkedAtGameStart)
        {
            const std::string sMineFieldSpec =
                "7 3\n"
                "...*...\n"
                "***..*.\n"
                "....*..\n"
                "0 0\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            bool bIsSquareOpened = mineSweeper.GetMineFields()[0].IsSquareUnmarked(5, 2);

            Assert::IsTrue(bIsSquareOpened);
        }

        TEST_METHOD(TestOpenAndRecurseWorksCorrectly)
        {
            const std::string sMineFieldSpec =
                "5 8\n"
                ".....\n"
                ".....\n"
                "..*..\n"
                "..*..\n"
                "..*..\n"
                "..*..\n"
                ".....\n"
                ".....\n"
                "0 0\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            MineField& mineField = mineSweeper.GetMineFields()[0];
            mineField.OpenSquare(0, 0);

            bool bFoundUnOpenedSquare = false;
            for (std::int32_t nColumn = 0; nColumn < 5 && !bFoundUnOpenedSquare; ++nColumn)
                bFoundUnOpenedSquare = !mineField.IsSquareOpened(nColumn, 0);
            Assert::IsFalse(bFoundUnOpenedSquare);

            for (std::int32_t nColumn = 0; nColumn < 5 && !bFoundUnOpenedSquare; ++nColumn)
                bFoundUnOpenedSquare = !mineField.IsSquareOpened(nColumn, 7);
            Assert::IsFalse(bFoundUnOpenedSquare);

            for (std::int32_t nLine = 0; nLine < 8 && !bFoundUnOpenedSquare; ++nLine)
                bFoundUnOpenedSquare = !mineField.IsSquareOpened(0, nLine);
            Assert::IsFalse(bFoundUnOpenedSquare);

            for (std::int32_t nLine = 0; nLine < 8 && !bFoundUnOpenedSquare; ++nLine)
                bFoundUnOpenedSquare = !mineField.IsSquareOpened(4, nLine);
            Assert::IsFalse(bFoundUnOpenedSquare);
        }

        TEST_METHOD(TestAutoOpenAdjacentSquaresWorksCorrectly)
        {
            const std::string sMineFieldSpec =
                "2 3\n"
                "..\n"
                ".*\n"
                "..\n"
                "0 0\n";

            MineSweeper mineSweeper = MineSweeperFactory::CreateFromText(sMineFieldSpec);
            MineField& mineField = mineSweeper.GetMineFields()[0];

            mineField.OpenSquare(0, 1);
            mineField.MarkSquareAsMine(1, 1);
            mineField.AutoOpenAdjacentSquares(0, 1);

            Assert::IsTrue(mineField.IsSquareOpened(0, 0));
            Assert::IsTrue(mineField.IsSquareOpened(1, 0));
            Assert::IsTrue(mineField.IsSquareOpened(0, 2));
            Assert::IsTrue(mineField.IsSquareOpened(1, 2));
        }
    };
}
