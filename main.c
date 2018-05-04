#include <stdio.h>
#include <stdlib.h>

#define CLIMATEZONENUMBER 4
#define MINIMALPOPSIZE 4

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
    Population *population;
} Individual;

typedef struct _GameBoard
{
    Individual **board;
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
      gameBoard->board =malloc(size*sizeof(Individual*));

      for(unsigned int i = 0; i < size; ++i)
      {
          gameBoard->board[i] = malloc(size*sizeof(Individual));
      }
}

void emptyGameBoard(GameBoard *gameBoard)
{
    for(unsigned int i = 0; i < gameBoard->boardSize; ++i)
    {
        for(unsigned int j = 0; j < gameBoard->boardSize; ++j)
        {
           gameBoard->board[i][j].state = 'D';
        }
    }
}

Population generatePopulation(unsigned int size, char type, char* name)
{
    Population *retValue = NULL;
    if(size < MINIMALPOPSIZE)
    {
        printf("Minimal population size is 4");
    }
    else
    {
        Population *newPopulation = malloc(sizeof(Population));
        newPopulation->size = size;
        newPopulation->numberOfIndividuals = (size*size)/2;
        newPopulation->type = type;
        newPopulation->name = name;

        for(unsigned int i = 0; i < newPopulation->numberOfIndividuals; ++i)
        {
            Individual *newIndi = malloc(sizeof(Individual));
            newIndi->state = 'A';
            newIndi->population = newPopulation;
        }

        retValue = newPopulation;
    }
    return *retValue;
}

void placeClimateZones(unsigned int boardSize);
void placePopulation(GameBoard *gameBoard, Population *population)
{
    for(unsigned int i = 0; i < gameBoard->boardSize; ++i)
    {
        for(unsigned int j = 0; j < gameBoard->boardSize; ++j)
        {
            if()
            {

            }
        }
    }
}
void evolve(Individual currentIndividual);
void nextGeneration(GameBoard *gameBoard);
int countNeighbours(Individual currentIndividual);
void executeClimateInflu(GameBoard *gameBoard);
void printBoard(GameBoard *gameBoard)
{
    for(unsigned int i = 0; i < gameBoard->boardSize; ++i)
    {
        for(unsigned int j = 0; j < gameBoard->boardSize; ++j)
        {


           printf("%c ",gameBoard->board[i][j].state);

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
