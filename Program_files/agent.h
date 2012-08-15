#ifndef agent_h
#define agent_h

#include <string.h>
#include <stdbool.h>

#include "typedefs.h"

/*******************************************************************************
*                                                                              *
*                       AGENT FUNCTIONS                                        *
*                                                                              *
*******************************************************************************/

/*******************************************************************************
                        NODE MUTATE
*******************************************************************************/
void agentNodeMutate(ProgramSpace* space,
                unsigned int agentIndex,
                unsigned int mutationProbability,
                unsigned int numberOfElementsPerList);

/*******************************************************************************
                        REPLACE NODE
*******************************************************************************/
void agentReplaceNode(ProgramSpace* space,
                      unsigned int agentIndex,
                      unsigned int nodeIndexToReplace,
                      unsigned int numberOfNeurons,
                      unsigned int numberOfElementsPerList);

/*******************************************************************************
                        ADD NODE
*******************************************************************************/
void agentAddNode(ProgramSpace* space,
             unsigned int agentIndex,
             unsigned int numberOfNeurons,
             unsigned int numberOfElementsPerList);

/*******************************************************************************
                        SET ELEMENT
*******************************************************************************/
void setElement(ProgramSpace* space,
                unsigned int agentDataIndex,
                unsigned int nodeIndex,
                unsigned int elementIndex,
                BASE_TYPE value);

/*******************************************************************************
                        GET TOTAL NODES
*******************************************************************************/
unsigned int agentGetTotalNodes(ProgramSpace* space,
                           unsigned int agentIndex);

/*******************************************************************************
                        INIT ARRAY NODES
*******************************************************************************/
void agentInitAgentArrayNodes(ProgramSpace* space,
                              unsigned int numberOfNeurons,
                              unsigned int numberOfElements);

/*******************************************************************************
                        PRINT AGENT (FOR DEBUGGING)
*******************************************************************************/
void agentPrintAgent(ProgramSpace* space, int index);

/*******************************************************************************
*                                                                              *
*                           MISC, DISREGARD                                    *
*                                                                              *
*******************************************************************************/
/*******************************************************************************
                        DELETE NODE
                        (not currently in program, needs debugging)
*******************************************************************************/
//void agentDeleteNode(ProgramSpace* space,
//                     unsigned int agentIndex,
//                     unsigned int nodeIndexToDelete);

//void agentLinkAngents(Agent agentArray[], ProgramSpace *space);

#endif
