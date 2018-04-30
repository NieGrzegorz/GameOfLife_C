#include <stdio.h>
#include <stdlib.h>

typedef struct _Population
{
    unsigned int size;
    unsigned int numberOfIndividuals;
    char type;
    char* name;
} Population;

typedef struct _Individual
{
    char state;
    int* position;
    Population population;
} Individual;

typedef struct _GameBoard
{
    char** board;
    unsigned int boardSize;
} GameBoard;

typedef struct _ClimateZone
{
    unsigned int startRow;
    unsigned int startCol;
    unsigned int width;
    unsigned int height;
    char zoneType;
} ClimateZone;

void createBoard(GameBoard *gameBoard, unsigned int size)
{
      gameBoard->boardSize = size;
      gameBoard->board =malloc(size*sizeof(char*));

      for(unsigned int i = 0; i < size; ++i)
      {
          gameBoard->board[i] = malloc(size*sizeof(char));
      }
}

void emptyGameBoard(GameBoard *gameBoard)
{
    for(unsigned int i = 0; i < gameBoard->boardSize; ++i)
    {
        for(unsigned int j = 0; j < gameBoard->boardSize; ++j)
        {
            gameBoard->board[i][j] = '.';
        }
    }
}

void generatePopulations(unsigned int size);
void placeClimateZones(unsigned int boardSize);
void placePopulations(GameBoard gameBoard, unsigned int number);
void evolve(Individual currentIndividual);
void nextGeneration(GameBoard gameBoard);
int countNeighbours(Individual currentIndividual);
void printBoard(GameBoard *gameBoard)
{
    for(unsigned int i = 0; i < gameBoard->boardSize; ++i)
    {
        for(unsigned int j = 0; j < gameBoard->boardSize; ++j)
        {
            printf("%c ",gameBoard->board[i][j]);
        }
        printf("\n");
    }
}

int main()
{

    unsigned int boardSize = 15;
    unsigned int populationSize;
    unsigned int numberOfPopulations;

    GameBoard *board = malloc(sizeof(GameBoard));
    createBoard(board, boardSize);
    emptyGameBoard(board);
    printBoard(board);

    return 0;
}
