#include "agent.h"


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
                unsigned int numberOfElementsPerList)
{
    AgentData* agentData = space;
    agentData = &agentData[agentIndex];
///this could be changed if the number of nodes is kept static in future programs
    unsigned int numberOfNeurons = agentGetTotalNodes(space, agentIndex)
                                    -AGENT_NON_NEURON_NODES;
    if(rand()%100000+1<=mutationProbability)
    {
        unsigned int nodeToReplace =
                rand()%(AGENT_NON_NEURON_NODES-AGENT_NUMBER_OF_SENSORS+
                         numberOfNeurons)+AGENT_NUMBER_OF_SENSORS;
        agentReplaceNode(space,
                         agentIndex,
                         nodeToReplace,
                         numberOfNeurons,
                         numberOfElementsPerList);
    }
}

/*******************************************************************************
                        REPLACE NODE
*******************************************************************************/
void agentReplaceNode(ProgramSpace* space,
                      unsigned int agentIndex,
                      unsigned int nodeIndexToReplace,
                      unsigned int numberOfNeurons,
                      unsigned int numberOfElementsPerList)
{
    AgentData* agentData = space;
    agentData = &agentData[agentIndex];
    Node* node = agentData;
    BASE_TYPE* element;
    unsigned int totalNodes = agentGetTotalNodes(space, agentIndex);
    node = &node[nodeIndexToReplace];
    element = node;

    ///this logic could be move to a function
    ///combined w/add node logic
    element[NODE_STATE] = 0;
    element[NODE_TRUE_CONTROL] = rand()%totalNodes;
    element[NODE_FALSE_CONTROL] = rand()%totalNodes;

    int base = 0;
    int k,l;
    for(k=0; k<NODE_ELEMENT_LISTS; k++)
    {
        base = k*NODE_ELEMENTS_PER_LIST;
        for(l=0; l<numberOfElementsPerList; l++)
        {
            element[base+l] = rand()%(AGENT_NON_NEURON_NODES-AGENT_NUMBER_OF_SENSORS+
                                    numberOfNeurons)+AGENT_NUMBER_OF_SENSORS;
        }
    }
}

/*******************************************************************************
                        ADD NODE
*******************************************************************************/
void agentAddNode(ProgramSpace* space,
             unsigned int agentIndex,
             unsigned int numberOfNeurons,
             unsigned int numberOfElementsPerList)
{
    AgentData* agentData = space;
    agentData = &agentData[agentIndex];
    Node* node = agentData;
    BASE_TYPE* element;

    unsigned int totalNodes = agentGetTotalNodes(space, agentIndex);
    //unsigned int totalNodes = ;
    if(totalNodes>=AGENT_NUMBER_OF_NODES){return;}
    node = &node[totalNodes];
    element = node;

    element[NODE_STATE] = 0;
    element[NODE_TRUE_CONTROL] = rand()%totalNodes;
    element[NODE_FALSE_CONTROL] = rand()%totalNodes;

    int base = 0;
    int k,l;
    for(k=0; k<NODE_ELEMENT_LISTS; k++)
    {
        base = k*NODE_ELEMENTS_PER_LIST;
        for(l=0; l<numberOfElementsPerList; l++)
        {
            element[base+l] = rand()%(AGENT_NON_NEURON_NODES-AGENT_NUMBER_OF_SENSORS+
                                    numberOfNeurons)+AGENT_NUMBER_OF_SENSORS;
        }
    }
}
/*******************************************************************************
                        SET ELEMENT
*******************************************************************************/
void setElement(ProgramSpace* space,
                unsigned int agentDataIndex,
                unsigned int nodeIndex,
                unsigned int elementIndex,
                BASE_TYPE value)
{
    BASE_TYPE* element = space;
    unsigned int absoluteElementPosition =
        (agentDataIndex*AGENT_NUMBER_OF_NODES*NODE_TOTAL_ELEMENTS)+
        (nodeIndex*NODE_TOTAL_ELEMENTS)+
        elementIndex;

    element[absoluteElementPosition] = value;
}

/*******************************************************************************
                        GET TOTAL NODES
*******************************************************************************/
unsigned int agentGetTotalNodes(ProgramSpace* space,
                           unsigned int agentIndex)
{
    BASE_TYPE* element = space;
    element = &element[agentIndex*AGENT_NUMBER_OF_NODES*NODE_TOTAL_ELEMENTS];
    int count = 0;
    while(element[NODE_STATE]!=NODE_NULL&&count<AGENT_NUMBER_OF_NODES)
    {
        count++;
        element = &element[NODE_TOTAL_ELEMENTS];
    }
    return count;
}

void agentInitAgentArrayNodes(//Agent agentArray[],
                              ProgramSpace* space,
                              unsigned int numberOfNeurons,
                              unsigned int numberOfElements)
{
    if((numberOfNeurons>AGENT_MAX_NUMBER_OF_NEURONS)||
       (numberOfElements>NODE_ELEMENTS_PER_LIST))
       {
           printf("invalid value to initAgentArrayNodes in agent.c\n");
           exit(-1);
       }

    AgentData* agent = space;
    Node* node;
    BASE_TYPE* element;

    int i, j, k, l;
    for(i=0; i<(COMMUNITY_NUMBER_OF_AGENTS*3); i++)
    {
        node = &agent[i];
        for(j=0; j<(AGENT_NON_NEURON_NODES+numberOfNeurons); j++)
        {
            element = &node[j];
            element[NODE_STATE] = 0;
            element[NODE_TRUE_CONTROL] = rand()%(AGENT_NON_NEURON_NODES+numberOfNeurons);
            element[NODE_FALSE_CONTROL] = rand()%(AGENT_NON_NEURON_NODES+numberOfNeurons);

            int base = 0;
            for(k=0; k<NODE_ELEMENT_LISTS; k++)
            {
                base = k*NODE_ELEMENTS_PER_LIST;
                for(l=0; l<numberOfElements; l++)
                {
                    element[base+l] = rand()%(AGENT_NON_NEURON_NODES-AGENT_NUMBER_OF_SENSORS+
                                            numberOfNeurons)+AGENT_NUMBER_OF_SENSORS;
                }
            }
        }
    }
}

/*******************************************************************************
                        PRINT AGENT (FOR DEBUGGING)
*******************************************************************************/
void agentPrintAgent(ProgramSpace* space, int index)
{
    AgentData* agent = space;
    Node* node = &agent[index];
    BASE_TYPE* element;

    int i, j;
    for(i=0; i<AGENT_NUMBER_OF_NODES; i++)
    {
        printf("************************************************************\n");
        printf("%d:  ",i);
        element = &node[i];
        for(j=0; j<NODE_TOTAL_ELEMENTS; j++)
        {
            printf("%d ", element[j]);
        }
         printf("\n");
    }
}

/*******************************************************************************
*                                                                              *
*                           MISC NOTES ETC., DISREGARD                         *
*                                                                              *
*******************************************************************************/
/*******************************************************************************
                        DELETE NODE
                        (not currently in program, needs debugguin)
*******************************************************************************/
//void agentDeleteNode(ProgramSpace* space,
//                     unsigned int agentIndex,
//                     unsigned int nodeIndexToDelete)
//{
//    ///error check if can delete, not delete essent nodes
//    ///what happens if we delete the last ellement?  easiest to mandate
//    ///always one neuro
//    ///still need guard against deleting the last node
//    ///what about nodes still pointing to last node?
//    AgentData* agentData = space;
//    agentData = &agentData[agentIndex];
//    Node* node = agentData;
//    BASE_TYPE* element;
//
//    unsigned int numberOfNodes = agentGetTotalNodes(space, agentIndex);
//
//    unsigned int toMove = numberOfNodes-1;
//
//    //re"point" array elements "pointing" to last node to node about to
//    //be deleted (last node is swapped)
//    int i, j;
//    for(i = 0; i < numberOfNodes; i++)
//    {
//        element = &node[i];
//        for(j = 0; j<NODE_TOTAL_ELEMENTS; j++)
//        {
//            if(element[j]==toMove)
//            {
//                element[j] = nodeIndexToDelete;
//            }
//        }
//    }
//
//    //copy last node to overwrite deleted node
//    memcpy(&node[nodeIndexToDelete],&node[toMove],(sizeof(BASE_TYPE))*NODE_TOTAL_ELEMENTS);
//
//    //erase last element
//    element = node[toMove];
//    for(i=0; i<NODE_TOTAL_ELEMENTS; i++)
//    {
//        element[i] = NODE_NULL;
//    }
//}


//void agentLinkAngents(Agent agentArray[], ProgramSpace *space)
//{
//    AgentData* agent = space;
//
//    int i;
//    for(i=0; i<(COMMUNITY_NUMBER_OF_AGENTS*3); i++)
//    {
//        agentArray[i].data = &agent[i];
//        agentArray[i].node = &agent[i];///this may be unecessary
//    }
//}


//*******************************************************************************
//                        NODE MUTATE
//*******************************************************************************/
//void agentNodeMutate(ProgramSpace* space,
//                unsigned int agentIndex,
//                unsigned int mutationProbability,
//                unsigned int numberOfElementsPerList)
//{
//    AgentData* agentData = space;
//    agentData = &agentData[agentIndex];
//    unsigned int numberOfNeurons = agentGetTotalNodes(space, agentIndex)
//                                    -AGENT_NON_NEURON_NODES;
//    if(rand()%100000+1<=mutationProbability)
//    {
//        int val = rand()%3;
//        if(val==0)
//        {
//            agentAddNode(space,
//                         agentIndex,
//                         numberOfNeurons,
//                         numberOfElementsPerList);
//        }
////        else if(val==1)
////        //delete node
////        {
////            ///need to debug
//////            unsigned int toDelete = rand()%(AGENT_NON_NEURON_NODES-AGENT_NUMBER_OF_SENSORS+
//////                                                numberOfNeurons)+AGENT_NUMBER_OF_SENSORS;
//////            agentDeleteNode(space,
//////                            agentIndex,
//////                            toDelete);
////        }
//        else
//        {
//            ///replace
//            unsigned int nodeToReplace = rand()%(AGENT_NON_NEURON_NODES-AGENT_NUMBER_OF_SENSORS+
//                                                numberOfNeurons)+AGENT_NUMBER_OF_SENSORS;
//            agentReplaceNode(space,
//                             agentIndex,
//                             nodeToReplace,
//                             numberOfNeurons,
//                             numberOfElementsPerList);
//        }
//    }
//}
