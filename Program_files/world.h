#ifndef world_h
#define world_h

#include <stdio.h>
#include <math.h>
#include <unistd.h>

#include "typedefs.h"
#include "agent.h"

typedef struct _Location
{
    unsigned int x;
    unsigned int y;
}Location;

typedef struct _Agent
{
    AgentData* data;
    bool isAlive;
    unsigned short int type;
    unsigned int xLocation;
    unsigned int yLocation;
    unsigned int captures;
    unsigned int lifeLength;
    unsigned int points;
}Agent;

/*******************************************************************************
*                                                                              *
*                           AGENT DRIVER FUNCTIONS                             *
*                                                                              *
*******************************************************************************/
/*******************************************************************************
                        AGENT DRIVER

    Main program driver.  Program space represets all the agents and their
    data.  The 2d world array keeps track of the agent's position within a 2d
    virtual space.  The agent array is an array of meta-data about the agents,
    it includes info. such as the lenght of the agent' life.

    numerOfCycles defines the number of cycles per generation.

    numberOfGenerations defines the number of virtual life cycles for the
    program's exeucution

    maxCallsPerAgent is the upper limit of calls each agent can make during
    its turn

    mutationProbability defines the probability of an individual node during
    the reproduction cycle defined as a fraction of value/100000

    numberOfElements per list defines the number of liks/4 each node can have

    app_state contains info relating to GTK and display of the life cycle

*******************************************************************************/
void worldAgentDriver(ProgramSpace* space,
                      Agent* world[][WORLD_SIZE],
                      Agent agentArray[],
                      unsigned int numberOfCycles,
                      unsigned int numberOfGenerations,
                      unsigned int maximumCallsPerAgent,
                      unsigned int mutationProbability,
                      unsigned int numberOfElementsPerList,
                      AppState* app_state);
/*******************************************************************************
                        EXECUTE AGENT

    Main execution function.  During a generation, each agent gets passed to
    this function.
*******************************************************************************/
int worldExecuteAgent(ProgramSpace* space,
                      Agent* world[][WORLD_SIZE],
                      Agent agentArray[],
                      unsigned int agentIndex,
                      unsigned int numberOfCalls,
                      AppState* app_state);
/*******************************************************************************
                        MOVE AGENTS

    Moves the agent at the end of worldExecuteAgent function.
*******************************************************************************/
void worldMoveAgent(ProgramSpace* space,
                    Agent* world[][WORLD_SIZE],
                    Agent agentArray[],
                    unsigned int agentIndex,
                    AppState* app_state);
/*******************************************************************************
                        REPRODUCE AGENTS

    Reproduces agents at end of generation.
*******************************************************************************/
void worldReproduceAgents(ProgramSpace* space,
                          Agent agentArray[],
                          unsigned int mutationProbability,
                          unsigned int numberOfElementsPerList);
/*******************************************************************************
                        SET AGENT STATE SENSORS

    Helper function, sets agent's state sensors.
*******************************************************************************/
void setAgentStateSensors(ProgramSpace* space,
                          Agent* world[][WORLD_SIZE],
                          Agent agentArray[],
                          unsigned int agentIndex);

/*******************************************************************************
*                                                                              *
*                           AGENT DRIVER HELPER FUNCTOINS                      *
*                                                                              *
*******************************************************************************/
/*******************************************************************************
                        INIT PROGRAM SPACE
*******************************************************************************/
void programSpaceInit(ProgramSpace *space);
/*******************************************************************************
                        INITIALISE WORLD
*******************************************************************************/
void init_world(ProgramSpace* space,
                Agent* world[][WORLD_SIZE],
                Agent agentArray[]);
/*******************************************************************************
                        RESET AGENT ARRAY
*******************************************************************************/
void worldResetAgentArray(Agent agentArray[]);
/*******************************************************************************
                        ARRAY AGENT SET PLACE AND TYPE
*******************************************************************************/
void worldAgentArraySetPlaceAndType(ProgramSpace* space,
                                    Agent* world[][WORLD_SIZE],
                                    Agent agentArray[]);
/*******************************************************************************
                        PRINT WORLD
*******************************************************************************/
///FOR DEBUGGING
void print_world(Agent* world[][WORLD_SIZE]);
/*******************************************************************************
                        LINK AGENTS
*******************************************************************************/
///this may not be necessary since the data and array contain same index
void worldLinkAgents(ProgramSpace* space,
                     Agent agentArray[]);



/*******************************************************************************
*                                                                              *
*                           DISPLAY FUNCTOINS                                  *
*                                                                              *
*******************************************************************************/
/*******************************************************************************
                        INITIALISE GRID
*******************************************************************************/
void init_grid(AppState* app_state);
/*******************************************************************************
                        CLEAR GRID
*******************************************************************************/
void clear_grid(AppState* app_state);
/*******************************************************************************
                        SET CELL COLOR
*******************************************************************************/
void set_cell_color(AppState* app_state, int x_pos, int y_pos, int color_in);



/*******************************************************************************
*                                                                              *
*                           FILE FUNCTOINS                                     *
*                                                                              *
*******************************************************************************/
/*******************************************************************************
                        OPEN FROM FILE
*******************************************************************************/
void openFromFile(ProgramSpace* space,
                  AppState* app_state,
                  char* fileName);
/*******************************************************************************
                        SAVE TO FILE
*******************************************************************************/
void saveToFile(ProgramSpace* space,
                AppState* app_state);



/*******************************************************************************
*                                                                              *
*                           HELPER FUNCTOINS                                   *
*                                                                              *
*******************************************************************************/
/*******************************************************************************
                            WOLRD ARRAY SHUFFLE
*******************************************************************************/
void worldArrayShuffle(unsigned int array[], unsigned int size);
/*******************************************************************************
                        GET ABSOLUTE POSITION
*******************************************************************************/
unsigned int absolutePosition(int positionIn, unsigned int arrayWidth);



/*******************************************************************************
*                                                                              *
*                           DEBUGGING                                          *
*                                                                              *
*******************************************************************************/
void worldTestWorldSet(Agent* world[][WORLD_SIZE], int type);



#endif

