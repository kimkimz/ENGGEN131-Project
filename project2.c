/* ENGGEN131 C Project 2023 - Battleships */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAP_SIZE 6
#define HIT 1000

// Prototype declarations of required functions
void PrintArray(int values[MAP_SIZE][MAP_SIZE]);
void InitialiseMap(int map[MAP_SIZE][MAP_SIZE]);
void AddRandomShip(int size, int map[MAP_SIZE][MAP_SIZE]);
int CountValues(int value, int map[MAP_SIZE][MAP_SIZE]);
int TopLeftPosition(int size, int *row, int *col, int map[MAP_SIZE][MAP_SIZE]);
int IsShipValid(int size, int map[MAP_SIZE][MAP_SIZE]);
void InitialiseRandomMap(int map[MAP_SIZE][MAP_SIZE]);
void FireShot(int shots[MAP_SIZE][MAP_SIZE], int map[MAP_SIZE][MAP_SIZE], int row, int col);
int CheckGameOver(int shots[MAP_SIZE][MAP_SIZE], int map[MAP_SIZE][MAP_SIZE]);


/******************************************************************************
*******************************************************************************
*******************************************************************************
// Implementations of the required functions should go below
*******************************************************************************
*******************************************************************************
******************************************************************************/

void PrintArray(int values[MAP_SIZE][MAP_SIZE])
{
	int i, j = 0;

	for (i = 0; i < MAP_SIZE; i++) {
		for (j = 0; j < MAP_SIZE; j++) {
			printf("%d ", values[i][j]);
		}
		printf("\n");
	}
}

void InitialiseMap(int map[MAP_SIZE][MAP_SIZE])
{
	//initialise map with zeros:
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			map[i][j] = 0;
		}
	}
	// Placing down ship size 2:
	for (int i = 0; i < 2; i++) {
		map[i][0] = 2;
	}

	// Placing down ship size 3:
	for (int j = 1; j < 4; j++) {
		map[2][j] = 3;
	}

	// Placing down ship size 4:
	for (int j = 0; j < 4; j++) {
		map[MAP_SIZE - 1][j] = 4;
	}

	// Placing down ship size 5:
	for (int i = MAP_SIZE-1; i >= MAP_SIZE-5; i--) {
		map[i][MAP_SIZE - 1] = 5;
	}
}


void AddRandomShip(int size, int map[MAP_SIZE][MAP_SIZE])
{
	int isVertical = rand() % 2; //returns 1 if vertical is chosen

	if (isVertical == 1) {
		int row = rand() % (MAP_SIZE - size + 1);
		int col = rand() % MAP_SIZE;
		for (int i = 0; i < size; i++) {
			map[row + i][col] = size;
		}
	}
	else { //placing ship horizontally
		int row = rand() % MAP_SIZE;
		int col = rand() % (MAP_SIZE - size + 1);
		for (int j = 0; j < size; j++) {
			map[row][col + j] = size;
		}
	}
}

int CountValues(int value, int map[MAP_SIZE][MAP_SIZE])
{
	int count = 0;
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			if (map[i][j] == value) {
				count++;
			}
		}
	}
	return count;
}

int TopLeftPosition(int size, int *row, int *col, int map[MAP_SIZE][MAP_SIZE])
{
	*row = MAP_SIZE;
	*col = MAP_SIZE;

	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			if (map[i][j] == size) {
				if (i < *row || j < *col) {
					*row = i;
					*col = j;
				}
				break;
			}
		}
	}

	if (map[*row][*col + 1] == size) {
		return 1;
	}
	else if (map[*row][*col] == size) {
		return 2;
	}
	else {
		return -1;
	}
}

int IsShipValid(int size, int map[MAP_SIZE][MAP_SIZE])
{
	int row, col, possible, direction;
	
	possible = 0;
	direction = TopLeftPosition(size, &row, &col, map);
	

	if (direction == 1) {// if horizontal
		possible = 1;
		
		//checking left and right of the horizontal ship
		if ((col - 1 >= 0) && (map[row][col - 1] != 0) || (col + size < MAP_SIZE) && (map[row][col + size] != 0)) {
			 possible = 0;
		}

		for (int a = row - 1; a <= row+1; a++) {
			for (int b = col; b <= col + size - 1; b++) {
				//checking above and below the horizontal ship
				if ((0<=a)&& (a<MAP_SIZE) && (0<=b) && (b<MAP_SIZE)){
					if ((map[a][b] != size) && (map[a][b] != 0)) {
						possible = 0;
					}
				}
			}
		}
		
	}
	else { // if vertical (direction == 2)
		possible = 1;

		//checking above and below the vertical ship
		if ((row - 1 >= 0) && (map[row - 1][col] != 0) || (row + size < MAP_SIZE) && (map[row + size][col] != 0)) {
			possible = 0;
		}

		for (int a = row; a <= row + size - 1; a++) {
			for (int b = col - 1; b <= col + 1; b++) {
				//checking left and right of the vertical ship
				if ((0 <= a) && (a < MAP_SIZE) && (0 <= b) && (b < MAP_SIZE)) {
					if ((map[a][b] != size) && (map[a][b] != 0)) {
						possible = 0;
					}
				}
			}
		}
		
	}
	//determining if there is overlap:
	if (CountValues(size, map) != size) {
		possible = 0;
	}

	return possible;
}

void InitialiseRandomMap(int map[MAP_SIZE][MAP_SIZE])
{
	int valid = 0;
	while (!valid) { //while ship is invalid:
		 valid = 1;

		//placing each ship down in decending order:
		 for (int size = 5; size >= 2; size--) {

			 while (!IsShipValid(size, map)) {
				 // adding first ship and checking if valid:
				 AddRandomShip(size, map);

				 if (!IsShipValid(size, map)) {
					 //remove the ship + try again
					 for (int j = 0; j < MAP_SIZE; j++) {
						 for (int k = 0; k < MAP_SIZE; k++) {
							 if (map[j][k] == size) {
								 map[j][k] = 0; // remove invalid ship with zeros
							 }
						 }
					 }
				 }
			 }
		 }

		 //once all ships are placed down:
		 for (int i = 2; i <=5; i++) {
			 if (!IsShipValid(i, map)) {
				 valid = 0;
			 }
		 }
		 // if invalid clear map:
		 if(valid == 0){
			 for (int k = 0; k < MAP_SIZE; k++) {
				 for (int j = 0; j < MAP_SIZE; j++) {
					 map[k][j] = 0;
				 }
			 }
		 }
	}	
}

void FireShot(int shots[MAP_SIZE][MAP_SIZE], int map[MAP_SIZE][MAP_SIZE], int row, int col)
{
	//extracting the last shot number:
	int last_number = 0;
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			if (shots[i][j] % 1000 > last_number) {
				last_number = shots[i][j] % 1000;
			}
		}
	}

	if (shots[row][col] == 0) {//if shot is in a new position:
		
		//new shot number:
		int shotNumber = last_number + 1;
		//shots[row][col] = shotNumber; //new order of shot added to shots array

		//if it is a miss:
		if (map[row][col] == 0) {
			shots[row][col] = shotNumber;
		}

		//if a shot is a hit:
		if (map[row][col] != 0) {

			int shipSize = map[row][col];
			int destroyed = 1;
			shots[row][col] = shotNumber + 1000;

			//checking if ship is destroyed:
			for (int i = 0; i < MAP_SIZE; i++) {
				for (int j = 0; j < MAP_SIZE; j++) {
					if ((map[i][j] == shipSize) && (shots[i][j] == 0)) {
						destroyed = 0;
					}
				}
			}				

			if (destroyed == 1) {
				for (int i = 0; i < MAP_SIZE; i++) {
					for (int j = 0; j < MAP_SIZE; j++) {
						if (map[i][j] == shipSize) {
							shots[i][j] = (shipSize * 1000) + (shots[i][j]%1000);
						}
					}
				}
			}
		}
	}
}

// Example: a test function for Task Eight
void TestFireShot(void)
{
	int map[MAP_SIZE][MAP_SIZE] = {
	{0, 4, 4, 4, 4, 0},
	{0, 0, 0, 0, 0, 0},
	{5, 5, 5, 5, 5, 0},
	{0, 0, 0, 0, 0, 0},
	{0, 0, 0, 3, 3, 3},
	{0, 2, 2, 0, 0, 0}
	};

	int shots[MAP_SIZE][MAP_SIZE] = { 0 };
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE - 1; j++) {
			FireShot(shots, map, i, j);
		}
	}

	PrintArray(shots);


	/*int map[MAP_SIZE][MAP_SIZE] = {0};
	int shots[MAP_SIZE][MAP_SIZE] = { 0 };
	InitialiseMap(map);
	printf("TEST FIRE SHOT Map:\n");
	PrintArray(map);
	printf("Shots:\n");
	PrintArray(shots);
	FireShot(shots, map, 2, 0);
	printf("Shots:\n");
	PrintArray(shots);
	FireShot(shots, map, 2, 1);
	printf("Shots:\n");
	PrintArray(shots);
	FireShot(shots, map, 2, 2);
	printf("Shots:\n");
	PrintArray(shots);
	FireShot(shots, map, 2, 3);
	printf("Shots:\n");
	PrintArray(shots);
	FireShot(shots, map, 0, 0);
	FireShot(shots, map, 1, 0);
	FireShot(shots, map, 2, 0);
	FireShot(shots, map, 3, 0);
	FireShot(shots, map, 4, 0);
	FireShot(shots, map, 6, 0);
	FireShot(shots, map, 5, 0);
	FireShot(shots, map, 6, 1);
	FireShot(shots, map, 6, 2);
	FireShot(shots, map, 6, 3);
	printf("Shots:\n");
	PrintArray(shots);*/
}

int CheckGameOver(int shots[MAP_SIZE][MAP_SIZE], int map[MAP_SIZE][MAP_SIZE])
{
	int gameOver = 1;

	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			if (map[i][j] != 0) {
				if (shots[i][j] < 1000) {
					gameOver = 0;
					break;
				}
			}
		}
		if (!gameOver) {
			break;
		}
	}
	if (gameOver == 0) {
		return 0;
	}
	else {
		return 1;
	}
}

/******************************************************************************
*******************************************************************************
*******************************************************************************
// Implementations of the required functions should go above
*******************************************************************************
*******************************************************************************
******************************************************************************/

/*************************************************************************
** You should define your own test functions below
*************************************************************************/

// Example: a test function for Task One
/*void TestPrintArray(void)
{
	int map1[MAP_SIZE][MAP_SIZE] = {0};
	printf("Map 1: \n");
	PrintArray(map1);

	int map2[MAP_SIZE][MAP_SIZE] = {0};
	for (int i = 0; i < MAP_SIZE; i++) {
		map2[MAP_SIZE-i-1][i] = i;
	}
	printf("\nMap 2:\n");
	PrintArray(map2);
}

// Example: a test function for Task Two
void TestInitialiseMap(void)
{
	int map1[MAP_SIZE][MAP_SIZE] = {0};
	InitialiseMap(map1);
	printf("\nInitialiseMap: \n");
	PrintArray(map1);
}

// Example: a test function for Task Three
void TestAddRandomShip(void)
{
	int map1[MAP_SIZE][MAP_SIZE] = {0};
	int map2[MAP_SIZE][MAP_SIZE] = {0};

	AddRandomShip(5, map1);

	printf("\nAddRandomShip: \n");
	PrintArray(map1);

	AddRandomShip(2, map2);
	AddRandomShip(3, map2);
	AddRandomShip(4, map2);
	AddRandomShip(5, map2);

	printf("\nAddRandomFOURShipsMap: \n");
	PrintArray(map2);
}

// Example: a test function for Task Four
void TestCountValues(void)
{
	int map1[MAP_SIZE][MAP_SIZE];
	// Initialise the array
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			map1[i][j] = i + j;
		}
	}
	printf("\nTestCountValues1: \n");
	PrintArray(map1);
	int count1 = CountValues(5, map1);
	printf("Count = %d\n", count1);


	
	int map[MAP_SIZE][MAP_SIZE] = { 0 };
	int count, shipSize;
	InitialiseMap(map);
	printf("\nTestCountValues: \n");
	PrintArray(map);
	for (shipSize = 2; shipSize <= 5; shipSize++) {
		count = CountValues(shipSize, map);
		printf("The value %d appears %d times\n", shipSize,count);
	}
}

// Example: a test function for Task Five
void TestTopLeftPosition(void)
{
	int map[MAP_SIZE][MAP_SIZE] = {0};
	int row, col, direction, shipSize;
	InitialiseMap(map);
	printf("\nTestTopLeftPosition: \n");
	PrintArray(map);
	for (shipSize = 2; shipSize <= 5; shipSize++) {
		direction = TopLeftPosition(shipSize, &row, &col, map);
		printf("Ship %d is at (%d, %d) facing %d\n", shipSize,row, col, direction);
	}

	/*int row1, col1, direction1;
	int map1[MAP_SIZE][MAP_SIZE] = {
	  {0, 0, 0, 3, 3, 3, 0},
	  {0, 5, 0, 0, 0, 0, 0},
	  {0, 5, 0, 0, 4, 0, 0},
	  {0, 5, 0, 0, 4, 0, 0},
	  {0, 5, 0, 0, 4, 0, 0},
	  {0, 5, 0, 0, 4, 0, 0},
	  {0, 0, 2, 2, 0, 0, 0}
	};
	for (int shipSize1 = 2; shipSize1 <= 5; shipSize1++) {
		direction1 = TopLeftPosition(shipSize1, &row1, &col1, map1);
		printf("Ship %d is at (%d, %d) facing %d\n", shipSize1, row1, col1, direction1);
	}
}*/

// Example: a test function for Task Six
/*void TestIsShipValid(void)
{
	int map[MAP_SIZE][MAP_SIZE] = {0};
	int valid, shipSize;

	InitialiseMap(map);
	printf("\nTestIsShipValid: \n");
	PrintArray(map);

	for (shipSize = 2; shipSize <= 5; shipSize++) {
		valid = IsShipValid(shipSize, map);
		printf("Is ship %d valid? %d\n", shipSize, valid);
	}

	//(Using map size 7)
	// Move Ship 3 to an invalid position 
	map[2][3] = 0;
	map[2][0] = 3;

	//moving ship 2 to invalid position
	map[0][0] = 0;
	map[1][0] = 0;
	map[0][5] = 2;
	map[1][5] = 2;
	// Move Ship 4 to an invalid position 
	map[6][0] = 0;
	map[6][1] = 0;
	map[6][2] = 0;
	map[6][3] = 0;
	map[4][3] = 4;
	map[4][4] = 4;
	map[4][5] = 4;
	map[4][6] = 4;
	PrintArray(map);

	for (shipSize = 2; shipSize <= 5; shipSize++) {
		valid = IsShipValid(shipSize, map);
		printf("Is ship %d valid? %d\n", shipSize, valid);
	}
	
	//Test 2:
	// FOR THIS TEST, MAP_SIZE WILL BE 6
	int map[MAP_SIZE][MAP_SIZE];
	int valid, shipSize;
	int output = 0;

	// Initialise a random (and valid) map 1000 times
	for (int i = 0; i < 1000; i++) {
		InitialiseRandomMap(map); //OMG U CANT TEST THIS BECAUSE YOUR FUNCTION DOESNT WORK LOL
		// All ships are in valid positions
		// Check that IsShipValid() returns true
		for (shipSize = 2; shipSize <= 5; shipSize++) {
			valid = IsShipValid(shipSize, map);
			if (!valid) {
				if (!output) {
					printf("Reporting an invalid ship!\n");
					PrintArray(map);
					output = 1;
				}
			}
		}
	}
	
	printf("All ships are valid");
	}
	// Test 3:
	// FOR THIS TEST, MAP_SIZE WILL BE 7
	int valid, shipSize;
	int map[7][7] = {
	  {0, 0, 0, 0, 0, 0, 4},
	  {0, 0, 0, 0, 0, 0, 4},
	  {0, 0, 0, 0, 0, 0, 4},
	  {0, 0, 0, 2, 0, 0, 4},
	  {0, 0, 0, 2, 0, 0, 0},
	  {5, 5, 5, 3, 5, 0, 0},
	  {0, 0, 0, 0, 0, 0, 0}
	};
	for (shipSize = 2; shipSize <= 5; shipSize++) {
		valid = IsShipValid(shipSize, map);
		printf("Is ship %d valid? %d\n", shipSize, valid);
	}
}
*/
// Example: a test function for Task Seven
void TestInitialiseRandomMap(void)
{
	int map[MAP_SIZE][MAP_SIZE] = { 0 };
	InitialiseRandomMap(map);

	printf("\nTestInitialiseRandomMap: \n");
	PrintArray(map);
}

// Example: a test function for Task Nine
/*void TestCheckGameOver(void)
{
	int map[MAP_SIZE][MAP_SIZE] = {
	  {0, 4, 4, 4, 4, 0, 2},
	  {0, 0, 0, 0, 0, 0, 2},
	  {3, 3, 3, 0, 0, 0, 0},
	  {0, 0, 0, 0, 0, 0, 0},
	  {0, 6, 6, 6, 6, 6, 6},
	  {0, 0, 0, 0, 0, 0, 0},
	  {0, 0, 5, 5, 5, 5, 5}
	};
	int shots[MAP_SIZE][MAP_SIZE] = {
	  {17, 4007, 4002, 4047, 4011, 31, 2025},
	  {5, 12, 32, 43, 41, 10, 2036},
	  {3013, 3033, 3023, 42, 30, 3, 21},
	  {34, 15, 27, 1, 40, 49, 28},
	  {24, 6037, 6035, 6038, 6029, 6014, 6046},
	  {16, 8, 20, 45, 39, 26, 22},
	  {9, 44, 5006, 5018, 5019, 5048, 5004}
	};
	printf("Game over? %d\n", CheckGameOver(shots, map));
}*/



int main(void)
{
	// Initialise the seed for the random number generator
	srand((unsigned int)time(NULL));
	TestFireShot();
	TestInitialiseRandomMap();


	/*TestPrintArray();
	TestInitialiseMap();
	TestAddRandomShip();
	TestCountValues();
	TestTopLeftPosition();
	TestIsShipValid();
	TestCheckGameOver();
*/

	return 0;
}


