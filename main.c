#include <stdio.h>
#include <time.h>
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
    int xPosition;
    int yPosition;
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
           gameBoard->board[i][j].state = '.';
        }
    }
}

Population* generatePopulation(unsigned int size, char type, char* name)
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

        retValue = newPopulation;
    }
    return retValue;
}

int isFreeSpace(GameBoard *gameBoard, unsigned int xPos, unsigned int yPos, unsigned int populationSize)
{
    if((xPos+populationSize <= gameBoard->boardSize) && (yPos+populationSize <= gameBoard->boardSize))
    {
        for(unsigned int i = xPos; i < (xPos+populationSize); ++i)
        {
            for(unsigned int j = yPos; j < (yPos+populationSize); ++j)
            {
                if(gameBoard->board[i][j].state != '.')
                {
                    return -1;
                }
            }
        }
    }
    else
    {
        return -1;
    }
    return 0;
}

void placePopulation(GameBoard *gameBoard, Population *population, unsigned int xPos, unsigned int yPos)
{
    int x, y;
    unsigned int i = 0;
    while( i < population->numberOfIndividuals)
    {
        x = (rand()%population->size)+ xPos;
        y = (rand()%population->size)+ yPos;

        if(gameBoard->board[x][y].state != 'A')
        {
            gameBoard->board[x][y].state ='A';
            gameBoard->board[x][y].population = population;
            gameBoard->board[x][y].xPosition = x;
            gameBoard->board[x][y].yPosition = y;
            ++i;
        }
    }
}

void placeClimateZones(unsigned int boardSize);
int findSpot(GameBoard *gameBoard, Population *population)
{
    for(unsigned int i = 0; i < gameBoard->boardSize; ++i)
    {
        for(unsigned int j = 0; j < gameBoard->boardSize; ++j)
        {
            if(isFreeSpace(gameBoard, i, j, population->size) == 0)
            {
                placePopulation(gameBoard, population, i, j);
                return 0;
            }
        }
    }
    return -1;
}

void evolve(Individual *currentIndividual, GameBoard *gameBoard)
{
    unsigned int sameTypeNeighbours, differentTypeNeighbours;
    sameTypeNeigbours = countNeighbours(currentIndividual, gameBoard, currentIndividual->population->type);

    if((currentIndividual->state == '.') && (numberOfNeighbours == 3))
    {

    }
}

void nextGeneration(GameBoard *gameBoard);
int countNeighbours(Individual *currentIndividual, GameBoard *gameBoard, char type)
{
    unsigned int x = currentIndividual->xPosition;
    unsigned int y = currentIndividual->yPosition;
    int numberOfNeighbours = 0;
    if((currentIndividual->xPosition == 0) && ((currentIndividual->yPosition != 0) && (currentIndividual->yPosition != gameBoard->boardSize)))
    {
        // First row
        if((gameBoard->board[x][y+1].state == 'A') && (gameBoard->board[x][y+1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x][y-1].state == 'A') && (gameBoard->board[x][y-1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x+1][y-1].state == 'A') && (gameBoard->board[x+1][y-1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x+1][y].state == 'A') && (gameBoard->board[x+1][y].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x+1][y+1].state == 'A') && (gameBoard->board[x+1][y+1].population->type == type)) numberOfNeighbours++;
    }
    else if(((currentIndividual->xPosition != 0) && (currentIndividual->xPosition != gameBoard->boardSize)) && (currentIndividual->yPosition = 0))
    {
        if((gameBoard->board[x-1][y].state == 'A') && (gameBoard->board[x-1][y].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x+1][y].state == 'A') && (gameBoard->board[x+1][y].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x-1][y+1].state == 'A') && (gameBoard->board[x-1][y+1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x][y+1].state == 'A') && (gameBoard->board[x][y+1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x+1][y+1].state == 'A') (gameBoard->board[x+1][y+1].population->type == type)) numberOfNeighbours++;
    }
    else if((currentIndividual->xPosition == gameBoard->boardSize) && ((currentIndividual->yPosition != 0) && (currentIndividual->yPosition != gameBoard->boardSize)))
    {
        if((gameBoard->board[x][y-1].state == 'A') && (gameBoard->board[x][y-1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x][y+1].state == 'A') && (gameBoard->board[x][y+1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x-1][y-1].state == 'A') && (gameBoard->board[x-1][y-1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x-1][y].state == 'A') && (gameBoard->board[x-1][y].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x-1][y+1].state == 'A') && (gameBoard->board[x-1][y+1].population->type == type)) numberOfNeighbours++;
    }
    else if(((currentIndividual->xPosition != 0) && (currentIndividual->xPosition != gameBoard->boardSize)) && (currentIndividual->yPosition == gameBoard->boardSize))
    {
        if((gameBoard->board[x-1][y].state == 'A') && (gameBoard->board[x-1][y].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x+1][y].state == 'A') && (gameBoard->board[x+1][y].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x-1][y-1].state == 'A') && (gameBoard->board[x-1][y-1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x][y-1].state == 'A') && (gameBoard->board[x][y-1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x+1][y-1].state == 'A') && (gameBoard->board[x+1][y-1].population->type == type)) numberOfNeighbours++;
    }
    else if((currentIndividual->xPosition == gameBoard->boardSize) && (currentIndividual->yPosition == gameBoard->boardSize))
    {
        if((gameBoard->board[x-1][y].state == 'A') && (gameBoard->board[x-1][y].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x-1][y-1].state == 'A') && (gameBoard->board[x-1][y-1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x][y-1].state == 'A') && (gameBoard->board[x][y-1].population->type == type)) numberOfNeighbours++;
    }
    else if((currentIndividual->xPosition = 0) && (currentIndividual->yPosition == 0))
    {
        if((gameBoard->board[x+1][y].state == 'A') && (gameBoard->board[x+1][y].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x+1][y+1].state == 'A') && (gameBoard->board[x+1][y+1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x][y+1].state == 'A') && (gameBoard->board[x][y+1].population->type == type)) numberOfNeighbours++;
    }
    else
    {
        if((gameBoard->board[x-1][y-1].state == 'A')&& (gameBoard->board[x-1][y-1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x-1][y].state == 'A') && (gameBoard->board[x-1][y].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x-1][y+1].state == 'A') && (gameBoard->board[x-1][y+1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x][y-1].state == 'A') && (gameBoard->board[x][y-1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x][y+1].state == 'A') && (gameBoard->board[x][y+1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x+1][y-1].state == 'A') && (gameBoard->board[x+1][y-1].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x+1][y].state == 'A') && (gameBoard->board[x+1][y].population->type == type)) numberOfNeighbours++;
        if((gameBoard->board[x+1][y+1].state == 'A') && (gameBoard->board[x+1][y+1].population->type == type)) numberOfNeighbours++;
    }
    return numberOfNeighbours;
}

void executeClimateImpact(GameBoard *gameBoard);
void printBoard(GameBoard *gameBoard)
{
    printf("\n");
    printf("-----------------------------------------\n");
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
    srand(time(NULL));
    unsigned int boardSize = 15;
    unsigned int populationSize = 7;
    unsigned int numberOfPopulations = 3;

    Population *no1, *no2, *no3, *no4;
    no1 = generatePopulation(populationSize, 'G', "No1");
    no2 = generatePopulation(populationSize, 'G', "No2");
    no3 = generatePopulation(populationSize, 'F', "No3");
    no4 = generatePopulation(populationSize, 'F', "No4");
    GameBoard *board = malloc(sizeof(GameBoard));
    createBoard(board, boardSize);
    emptyGameBoard(board);
    printBoard(board);


    findSpot(board, no1);
    findSpot(board, no2);
    findSpot(board, no3);
 //   findSpot(board, no4);
    printBoard(board);

    return 0;
}
