#include <iostream>
#include <stdlib.h>
#include <time.h>

uint8_t singleDigitRand(uint16_t mask) {
	uint8_t tryNumber = rand() % 9 + 1;
	while (mask & (1 << tryNumber)) {
		tryNumber++;
		// tryNumber = tryNumber > 9;
		if (tryNumber > 9) {
			tryNumber = 1;
		}
	}
	return tryNumber;
}

void initArray(uint8_t(&arr)[9][9]) {
	for (uint8_t i = 0; i < 1; i++) {
		uint16_t bitMask = 0;
		for (uint8_t j = 0; j < 9; j++) {
			uint8_t digit = singleDigitRand(bitMask);
			arr[i][j] = digit;
			bitMask |= (1 << digit);
		}
	}
}

void printArray(uint8_t(&arr)[9][9]) {
	for (uint8_t i = 0; i < 9; i++) {
		for (uint8_t j = 0; j < 9; j++) {
			std::cout << (arr[i][j] ? char(arr[i][j] + 0x30) : ' ');
			std::cout << " ";
			if ((j + 1) % 3 == 0) {
				std::cout << "| ";
			}
		}
		std::cout << std::endl;
		if ((i + 1) % 3 == 0) {
			std::cout << "-----------------------" << std::endl;
		}
	}
	std::cout << std::endl;
}

uint8_t solveFor(uint8_t num, uint8_t(&arr)[9][9]) {
	uint8_t tempArray[9][9] =
	{
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0}
	};
	// Row, Col
	uint8_t trackPosition[9][3] =
	{
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0}
	};

	for (uint8_t i = 0; i < 9; i++) {
		for (uint8_t j = 0; j < 9; j++) {
			// arr[i][j] = (arr[i][j] > num)*2;
			if (arr[i][j] != num && arr[i][j] > 0) {
				tempArray[i][j] = 12;
			}
			else if (arr[i][j] == num) {
				tempArray[i][j] = 11;
				// trackPosition[i][0] = i;
				trackPosition[i][2] = 1;
				trackPosition[i][1] = j;
				trackPosition[i][0] = i;

			}
		}
	}
	for (uint8_t i = 0; i < 9; i++) {
		if (trackPosition[i][2]) {
			for (uint8_t j = 0; j < 9; j++) {
				if (tempArray[i][j] == 0) {
					tempArray[i][j] = 13;
				}
				if (tempArray[j][trackPosition[i][1]] == 0) {
					tempArray[j][trackPosition[i][1]] = 13;
				}
			}
		}
	}
	// 0110111010000000
	// 0000000101110110
	// 0,1,2 3,4,5 6,7,8
	uint16_t containsNum = 0;
	for (uint8_t i = 0; i < 9; i++) {
		if (trackPosition[i][2]) {
			containsNum |= (1 << ((trackPosition[i][1] / 3) + ((i / 3) * 3)));
		}
	}
	// ((trackPosition[i][1] / 3) + ((i / 3) * 3))
	/**/
	uint8_t solvedCells = 0;

	for (uint8_t k = 0; k < 9; k++) {
		if ((~containsNum & (1 << k)) >> k) {
			// std::cout << "1" << std::endl;
			uint8_t subCounter = 0;
			uint8_t elementSelector[2] = { 0,0 };
			for (uint8_t i = 0 + ((k / 3) * 3); i < 3 + ((k / 3) * 3); i++) {
				for (uint8_t j = 0 + ((k - ((k / 3) * 3)) * 3); j < 3 + ((k - ((k / 3) * 3)) * 3); j++) {
					std::cout << +i << ' ' << +j << std::endl;
					if (tempArray[i][j] == 0) {
						subCounter++;
						elementSelector[0] = i;
						elementSelector[1] = j;
					}
				}
			}
			if (subCounter == 1) {
				arr[elementSelector[0]][elementSelector[1]] = num;
				solvedCells++;
			}
		}
	}

	for (uint8_t i = 0; i < 16; i++) {
		std::cout << ((containsNum & (0x8000 >> i)) >> (15 - i));
	}
	std::cout << std::endl;

	printArray(tempArray);
	return solvedCells;
}

int main(void) {
	// Keep track of how many missing squares and increment each time one is solved, when solved == missing -> break

	srand(time(NULL));
	uint8_t sudokuArray[9][9] =
	{
		{0,0,0,5,0,8,6,0,1},
		{0,0,4,0,0,0,0,3,0},
		{5,0,0,0,1,3,0,0,0},
		{9,5,0,0,7,1,2,6,4},
		{4,7,0,6,0,0,0,0,0},
		{2,3,0,4,8,9,1,5,0},
		{6,0,9,0,0,0,7,1,0},
		{0,0,5,0,9,0,4,0,3},
		{1,0,0,8,0,0,0,0,0}
	};
	uint8_t unsolvedCells = 0;
	for (uint8_t i = 0; i < 9; i++) {
		for (uint8_t j = 0; j < 9; j++) {
			if (sudokuArray[i][j] == 0) {
				unsolvedCells++;
			}
		}
	}
	while (unsolvedCells) {
		for (uint8_t i = 1; i < 10; i++) {
			unsolvedCells -= solveFor(i, sudokuArray);
		}
	}

	// rand() % 9 + 1;
	// initArray(sudokuArray);
	printArray(sudokuArray);

	while (1);
}