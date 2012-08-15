#include "world.h"

/*******************************************************************************
*                                                                              *
*                           AGENT DRIVER FUNCTIONS                             *
*                                                                              *
*******************************************************************************/

/*******************************************************************************
                        AGENT DRIVER
*******************************************************************************/
void worldAgentDriver(ProgramSpace* space,
                      Agent* world[][WORLD_SIZE],
                      Agent agentArray[],
                      unsigned int numberOfCycles,
                      unsigned int numberOfGenerations,
                      unsigned int maximumCallsPerAgent,
                      unsigned int mutationProbability,
                      unsigned int numberOfElementsPerList,
                      AppState* app_state)
{
    unsigned int shuffledIndex[PROGRAM_SPACE_TOTAL_AGENTS];

    int h, i, j;
    for(h = 0; h < numberOfGenerations; h++)
    {
        if(app_state->visibleCheck==TRUE && h == numberOfGenerations-1)
        {

            app_state->makeVisible = 2;
        }
        else
        {
            app_state->makeVisible = 0;
        }
        for(i = 0; i < PROGRAM_SPACE_TOTAL_AGENTS; i++)
        {
            shuffledIndex[i] = i;
        }

        worldArrayShuffle(shuffledIndex, PROGRAM_SPACE_TOTAL_AGENTS);

        for(i = 0; i < numberOfCycles; i++)
        {
            for(j = 0; j < PROGRAM_SPACE_TOTAL_AGENTS; j++)
            {
                int nextAgentIndex = shuffledIndex[j];
                if(agentArray[nextAgentIndex].isAlive==true)
                {
                    worldExecuteAgent(space,
                                      world,
                                      agentArray,
                                      nextAgentIndex,
                                      maximumCallsPerAgent,
                                      app_state);
                }
            }
            //display agents
            if(app_state->makeVisible == 2)
            {
                clear_grid(app_state);
                for(h=0;
                    h < PROGRAM_SPACE_NUMBER_OF_COMMUNITIES*COMMUNITY_NUMBER_OF_AGENTS;
                    h++)
                {
                    if(agentArray[h].isAlive == 1)
                    {
                        set_cell_color(app_state,
                                       agentArray[h].xLocation,
                                       agentArray[h].yLocation,
                                       agentArray[h].type);
                    }
                }
                gtk_widget_show(app_state->window);
                while (gtk_events_pending())
                {
                    gtk_main_iteration();
                }
                sleep(1);
            }
        }
        //reproduce agents
        worldReproduceAgents(space,
                             agentArray,
                             mutationProbability,
                             numberOfElementsPerList);
        //reset agent data & world
        init_world(space,
                   world,
                   agentArray);
    }
    if(app_state->saveResults == TRUE)
    {
        saveToFile(space,
                   app_state);
    }
}

/*******************************************************************************
                        EXECUTE AGENT
*******************************************************************************/
int worldExecuteAgent(ProgramSpace* space,
                      Agent* world[][WORLD_SIZE],
                      Agent agentArray[],
                      unsigned int agentIndex,
                      unsigned int numberOfCalls,
                      AppState* app_state)
{
    AgentData* agentData = space;
    agentData = agentData[agentIndex];
    Node* node = agentData;
    Node* controlNode = &node[AGENT_START_NODE_INDEX];
    Node* tmpNode;
    BASE_TYPE* element;
    unsigned int nodeIndexToChange;
    unsigned int newState;
    int base;
    bool foundNull;
    BASE_TYPE* tmpElement;
    setAgentStateSensors(space,
                         world,
                         agentArray,
                         agentIndex);

    int i, j, k, l;
    for(i=0; i<numberOfCalls; i++)
    {
        element = controlNode; 
        if(element[NODE_STATE]==1)
        {
            base = 0;
        }
        else{base = 2*NODE_ELEMENTS_PER_LIST;}
        for(j=0; j<2; j++)
        {
            base = base + j*NODE_ELEMENTS_PER_LIST;
            foundNull = false;
            for(k=0; k<NODE_ELEMENTS_PER_LIST; k++)
            {
                tmpElement = controlNode;
                nodeIndexToChange = tmpElement[base+k];
                if((nodeIndexToChange!=NODE_NULL)&&
                   (nodeIndexToChange>=AGENT_NUMBER_OF_SENSORS))
                {
                    if(nodeIndexToChange==AGENT_MOVE_NODE_INDEX)
                    {
                        if(j==1)
                        {
                            tmpNode = &node[nodeIndexToChange];
                            tmpElement = tmpNode;
                            tmpElement[NODE_STATE] = j;
                            worldMoveAgent(space,
                                           world,
                                           agentArray,
                                           agentIndex,
                                           app_state);

                            return 1;
                        }

                    }
                    else if((nodeIndexToChange>=AGENT_DIRECTION_INDEX_FIRST)&&
                            (nodeIndexToChange<=AGENT_DIRECTION_INDEX_LAST))
                    {
                        if(j==1)
                        {
                            for(l=AGENT_DIRECTION_INDEX_FIRST;
                                l<=AGENT_DIRECTION_INDEX_LAST; l++)
                            {
                                tmpNode = &node[l];
                                tmpElement = tmpNode;
                                tmpElement[NODE_STATE] = 0;
                            }
                        }
                        tmpNode = &node[nodeIndexToChange];
                        tmpElement = tmpNode;
                        tmpElement[NODE_STATE] = j;
                    }
                    else
                    {
                        tmpNode = &node[nodeIndexToChange];
                        tmpElement = tmpNode;
                        tmpElement[NODE_STATE] = j;
                    }
                }
                else{
                    foundNull=true;}
            }
        }
/**
    i think there is an error in this if else block.  what should happen is
    something like this:

    node = agent;
    contolNode = node[controlNode[nodeTrue/falseControl]];
*/
        if(element[NODE_STATE]==1)
        {
            tmpElement = controlNode;
            controlNode = &node[tmpElement[NODE_TRUE_CONTROL]];
        }
        else{
            tmpElement = controlNode;
            controlNode = &node[tmpElement[NODE_FALSE_CONTROL]];
        }
    }
    worldMoveAgent(space,
                   world,
                   agentArray,
                   agentIndex,
                   app_state);
}

/*******************************************************************************
                        MOVE AGENTS
*******************************************************************************/
void worldMoveAgent(ProgramSpace* space,
                    Agent* world[][WORLD_SIZE],
                    Agent agentArray[],
                    unsigned int agentIndex,
                    AppState* app_state)
{
    AgentData* agentData = space;
    agentData = &agentData[agentIndex];
    Node* node = agentData;
    Node* tmpNode;
    BASE_TYPE* tmpElement;

    Agent* agentAtNext;
    Agent* agentToMove = &agentArray[agentIndex];

    int direction = -1;
    int direction_array_width = 3;
    int direction_array_center = 5;
    int x, y;

    int i;
    int count = 0;
    for(i=AGENT_DIRECTION_INDEX_FIRST; i<=AGENT_DIRECTION_INDEX_LAST; i++)
    {
        tmpNode = &node[i];
        tmpElement = tmpNode;
        if(tmpElement[NODE_STATE] == 1)
        {
            //redundant check
            //can remove check an exit loop here once error checked
            if(direction != -1)
            {
                printf("error in worldMoveAgent\n");
                exit(-1);
            }
            direction = i-AGENT_DIRECTION_INDEX_FIRST;
        }
    }
    if(direction!=-1&&direction!=direction_array_center)
    {
        //get x and y relative to agent
        x = direction/direction_array_width - direction_array_width/2;
        y = direction%direction_array_width - direction_array_width/2;
        //get new absolute x and y position for agent
        ///x = absolutePosition((agentToMove->xLocation+x), WORLD_SIZE);
x = absolutePosition((agentToMove->xLocation+x-1), WORLD_SIZE); ///POSITION FUCTIONS NEEDS FURTHER TESTING
        ///y = absolutePosition((agentToMove->yLocation+y), WORLD_SIZE);
y = absolutePosition((agentToMove->yLocation+y), WORLD_SIZE);
        //determine agent type and location
        agentAtNext = world[x][y];
        //if no agent is at next
        if(agentAtNext==NULL)
        {
            //reset world array where agent was to NULL
            world[agentToMove->xLocation][agentToMove->yLocation] = NULL;
            //move agent
            world[x][y] = agentToMove;
            //update display
            if(app_state->makeVisible == 1)
            {
                //paint previous square gray
                set_cell_color(app_state,
                               agentToMove->xLocation,
                               agentToMove->yLocation,
                               gray);
                //paint next square agent's color
                set_cell_color(app_state,
                               x,
                               y,
                               agentToMove->type);
                while (gtk_events_pending())
                {
                    gtk_main_iteration();
                }
            }
            //update agent's position
            agentToMove->xLocation = x;
            agentToMove->yLocation = y;
            //update agent's lifeLength
            agentToMove->lifeLength++;
        }
        //if agent at next is of same type as agent
        else if(agentAtNext->type==agentArray[agentIndex].type)
        {
            //don't move agent
            //update agent's lifeLength
            agentToMove->lifeLength++;
        }
        //if agent at next is prey
        else if((agentAtNext->type - agentToMove->type == 1)||
                (agentAtNext->type == 0 && agentToMove->type == 2))
        {
            //kill agentAtNext
            agentAtNext->isAlive = false;
            //increment agent's capture total
            agentToMove->captures++;
            //reset world array where agent was to NULL
            world[agentToMove->xLocation][agentToMove->yLocation] = NULL;
            world[x][y] = agentToMove;
            //update display
            if(app_state->makeVisible == 1)
            {
                //paint previous square gray
                set_cell_color(app_state,
                               agentToMove->xLocation,
                               agentToMove->yLocation,
                               gray);
                //paint next square agent's color
                set_cell_color(app_state,
                               x,
                               y,
                               agentToMove->type);
                while (gtk_events_pending())
                {
                    gtk_main_iteration();
                }
            }
            //update agent's position
            agentToMove->xLocation = x;
            agentToMove->yLocation = y;
            //update agent's lifeLength
            agentToMove->lifeLength++;
        }
        //if agent at next is predator
        else
        {
            //kill agentToMove
            agentToMove->isAlive = false;
            //increment agentAtNext's capture total
            agentAtNext->captures++;
            //reset world array where agent was to NULL
            world[agentToMove->xLocation][agentToMove->yLocation] = NULL;
            //change display
            if(app_state->makeVisible == 1)
            {
                //change cell to gray
                set_cell_color(app_state,
                               agentToMove->xLocation,
                               agentToMove->yLocation,
                               gray);
                while (gtk_events_pending())
                {
                    gtk_main_iteration();
                }
            }
        }
    }
    else
    {
        agentArray[agentIndex].lifeLength++;
    }
}



/*******************************************************************************
                        REPRODUCE AGENTS
*******************************************************************************/
void worldReproduceAgents(ProgramSpace* space,
                          Agent agentArray[],
                          unsigned int mutationProbability,
                          unsigned int numberOfElementsPerList)
{
    if(NUMBER_OF_AGENTS_TO_REPRODUCE > 0)
    {
        AgentData* tmpSpace = malloc((sizeof(AgentData))
                                     *NUMBER_OF_AGENTS_TO_REPRODUCE);
        AgentData* agentData = space;
        unsigned int toReproduce[NUMBER_OF_AGENTS_TO_REPRODUCE];

        int i, j, k;
        //cacluate Agent's points
        for(i = 0; i < PROGRAM_SPACE_TOTAL_AGENTS; i++)
        {
            agentArray[i].points = agentArray[i].captures * CAPTURE_WEIGHT +
                                   agentArray[i].lifeLength * LIFE_LENGTH_WEIGHT;
        }

        for(i = 0; i < PROGRAM_SPACE_NUMBER_OF_COMMUNITIES; i++)
        {
            int highScore;
            int highIndex;
            int thisIndex;
            //int previousHighIndex;
            //bool isFull = false;

            //find first high scare
            highScore = 0;
            for(j = 0; j < COMMUNITY_NUMBER_OF_AGENTS; j++)
            {
                int thisIndex = COMMUNITY_NUMBER_OF_AGENTS * i + j;
                if(agentArray[thisIndex].points > highScore)
                {
                    highIndex = thisIndex;
                    highScore = agentArray[thisIndex].points;
                }
            }
            toReproduce[0] = highIndex;
///printf("\n%d\t%d\t%d\n", 0, agentArray[highIndex].points, highIndex);//FOR DEBUGGING
            for(j = 1; j < NUMBER_OF_AGENTS_TO_REPRODUCE; j++)
            {
                int nextHigh = 0;
                for(k = 0; k < COMMUNITY_NUMBER_OF_AGENTS; k++)
                {
                    int thisIndex = COMMUNITY_NUMBER_OF_AGENTS * i + k;
                    if(agentArray[thisIndex].points > nextHigh)
                    {
                        if(agentArray[thisIndex].points <=
                           agentArray[toReproduce[j-1]].points)
                        {
                            if(agentArray[thisIndex].points ==
                               agentArray[toReproduce[j-1]].points)
                            {
                                if(thisIndex > toReproduce[j-1])
                                {
                                    highIndex = thisIndex;
                                    nextHigh = agentArray[thisIndex].points;
                                }
                            }
                            else
                            {
                                highIndex = thisIndex;
                                nextHigh = agentArray[thisIndex].points;
                            }
                        }
                    }
                }
///printf("%d\t%d\t%d\n", j, agentArray[highIndex].points, highIndex);//FOR DEBUGGING
                toReproduce[j] = highIndex;
            }
            //shuffle agent array to prevent favoritism of position
            worldArrayShuffle(toReproduce, NUMBER_OF_AGENTS_TO_REPRODUCE);

            //copy agents into tmp space
            for(j = 0; j < NUMBER_OF_AGENTS_TO_REPRODUCE; j++)
            {
                memcpy(&tmpSpace[j],
                       &agentData[toReproduce[j]],
                       sizeof(AgentData));
            }
            for(j = 0; j < NUMBER_OF_AGENTS_TO_REPRODUCE; j++)
            {
                for(k = 0; k < NUMBER_OF_REPRODUCTIONS; k++)
                {
                    memcpy(&agentData[(i*COMMUNITY_NUMBER_OF_AGENTS)+
                                      (j*NUMBER_OF_REPRODUCTIONS)+
                                       k],
                           &tmpSpace[j],
                           sizeof(AgentData));
                 }
            }
        }
        //mutate all agents
        for(i = 0; i < PROGRAM_SPACE_TOTAL_AGENTS; i++)
        {
            agentNodeMutate(space,
                            i,
                            mutationProbability,
                            numberOfElementsPerList);
        }
        //free array
        free(tmpSpace);
    }
}

/*******************************************************************************
                        SET AGENT STATE SENSORS
*******************************************************************************/
void setAgentStateSensors(ProgramSpace* space,
                          Agent* world[][WORLD_SIZE],
                          Agent agentArray[],
                          unsigned int agentIndex)
{
    AgentData* agentData = space;
    agentData = &agentData[agentIndex];
    int state;

    unsigned int sensorArrayWidth = sqrt(AGENT_NUMBER_OF_SENSORS
                                         /AGENT_NUMBER_OF_SENSOR_TYPES);
    int upperX = absolutePosition(agentArray[agentIndex].xLocation
                                  - sensorArrayWidth/2,
                                  sensorArrayWidth);
    int upperY = absolutePosition(agentArray[agentIndex].yLocation
                                  - sensorArrayWidth/2,
                                  sensorArrayWidth);
    int i,j;
    int tmpX, tmpY;
    for(i=0; i<AGENT_NUMBER_OF_SENSOR_TYPES; i++)
    {
        for(j=0; j<AGENT_SENSORS_PER_TYPE; j++)
        {
            tmpX = absolutePosition(j/sensorArrayWidth+upperX,
                                    sensorArrayWidth);
            tmpY = absolutePosition(j%sensorArrayWidth+upperY,
                                    sensorArrayWidth);
            if((world[tmpX][tmpY]!=NULL)&&(world[tmpX][tmpY]->type == i))
            {
                state = 1;
            }
            else{state = 0;}
            setElement(space,
                       agentIndex,
                       i*AGENT_SENSORS_PER_TYPE+j,
                       NODE_STATE,
                       state);
        }
    }
}

/*******************************************************************************
                        GET ABSOLUTE POSITION
*******************************************************************************/
unsigned int absolutePosition(int positionIn, unsigned int arrayWidth)
{
    if(positionIn<0)
    {
        return(arrayWidth+positionIn);
    }
    else if(positionIn>=arrayWidth)
    {
        return(positionIn-arrayWidth);
    }
    return positionIn;
}

/*******************************************************************************
                        ARRAY AGENT SET PLACE AND TYPE
*******************************************************************************/
void worldAgentArraySetPlaceAndType(ProgramSpace* space,
                                    Agent* world[][WORLD_SIZE],
                                    Agent agentArray[])
{
    int i,j,x,y;
    for(i=0; i<PROGRAM_SPACE_NUMBER_OF_COMMUNITIES; i++)
    {
        for(j=0; j<COMMUNITY_NUMBER_OF_AGENTS; j++)
        {
            bool check = true;
            while(check)
            {
                x = rand()%WORLD_SIZE;
                y = rand()%WORLD_SIZE;
                if(world[x][y] == NULL)
                {
                    unsigned int agentIndex = i*COMMUNITY_NUMBER_OF_AGENTS+j;
                    agentArray[agentIndex].xLocation = x;
                    agentArray[agentIndex].yLocation = y;
                    agentArray[agentIndex].type = i;
                    world[x][y] = &agentArray[agentIndex];
                    check = false;
                }
            }
        }
    }
}

/*******************************************************************************
*                                                                              *
*                           AGENT DRIVER HELPER FUNCTOINS                      *
*                                                                              *
*******************************************************************************/

/*******************************************************************************
                        INIT PROGRAM SPACE
*******************************************************************************/
void programSpaceInit(ProgramSpace *space)
{
    BASE_TYPE* element = space;
    unsigned int i;
    for(i = 0; i<TOTAL_BASE_UNITS; i++)
    {
        element[i] = NODE_NULL;
    }
}

/*******************************************************************************
                        INITIALISE WORLD
*******************************************************************************/
void init_world(ProgramSpace* space,
                Agent* world[][WORLD_SIZE],
                Agent agentArray[])
{
    int x,y;
    for(x=0; x<WORLD_SIZE; x++)
    {
        for(y=0; y<WORLD_SIZE; y++)
        {
            world[x][y] = NULL;
        }
    }
    worldResetAgentArray(agentArray);
    worldAgentArraySetPlaceAndType(space,
                                   world,
                                   agentArray);
}

/*******************************************************************************
                        RESET AGENT ARRAY
*******************************************************************************/
void worldResetAgentArray(Agent agentArray[])
{
    unsigned short int type;
    int i;
    for(i=0; i<(COMMUNITY_NUMBER_OF_AGENTS*3); i++)
    {
        if(i<COMMUNITY_NUMBER_OF_AGENTS){type = red;}
        else if(i<COMMUNITY_NUMBER_OF_AGENTS*2){type = blue;}
        else{type = green;}
        agentArray[i].isAlive = true;
        agentArray[i].type = type;
        agentArray[i].captures = 0;
        agentArray[i].lifeLength = 0;
    }
}

/*******************************************************************************
                        PRINT WORLD
*******************************************************************************/
///FOR DEBUGGING
void print_world(Agent* world[][WORLD_SIZE])
{
    int x,y;
    for(x=0; x<WORLD_SIZE; x++)
    {
        printf("\n %d:  ",x);
        for(y=0; y<WORLD_SIZE; y++)
        {
            if(world[x][y]==NULL){printf("   ");}
            else{printf("%d  ", world[x][y]->type);}
        }
    }
}

/*******************************************************************************
                        LINK AGENTS
*******************************************************************************/
///this may not be necessary since the data and array contain same index
void worldLinkAgents(ProgramSpace* space,
                     Agent agentArray[])
{
    AgentData* agent = space;
    int i;
    for(i=0; i<(COMMUNITY_NUMBER_OF_AGENTS*3); i++)
    {
        agentArray[i].data = &agent[i];
    }
}
/*******************************************************************************
*                                                                              *
*                           DISPLAY FUNCTOINS                                  *
*                                                                              *
*******************************************************************************/
/*******************************************************************************
                        INITIALISE GRID
*******************************************************************************/
void init_grid(AppState* app_state)
{
    app_state->pixbuf_red = gdk_pixbuf_new_from_file("red.jpg", NULL);
    app_state->pixbuf_green = gdk_pixbuf_new_from_file("green.jpg", NULL);
    app_state->pixbuf_blue = gdk_pixbuf_new_from_file("blue.jpg", NULL);
    app_state->pixbuf_gray = gdk_pixbuf_new_from_file("gray.jpg", NULL);

    int i, j;
    for(i = 0; i < TABLE_WIDTH; i++)
    {
        for(j = 0; j < TABLE_HEIGHT; j++)
        {
            app_state->grid_array[i][j] = gtk_image_new_from_pixbuf(app_state->pixbuf_gray);
            gtk_table_attach(app_state->table,
                             (GtkWidget*)app_state->grid_array[i][j],
                             i, i+1, j, j+1,
                             GTK_EXPAND, GTK_EXPAND,
                             1, 1);
        }
    }
}

/*******************************************************************************
                        CLEAR GRID
*******************************************************************************/
void clear_grid(AppState* app_state)
{
    int i, j;
    for(i = 0; i < TABLE_WIDTH; i++)
    {
        for(j = 0; j < TABLE_HEIGHT; j++)
        {
            gtk_image_set_from_pixbuf(app_state->grid_array[i][j],
                                      app_state->pixbuf_gray);
                                      ///(GtkImage*) app_state->pixbuf_gray);
        }
    }
}

/*******************************************************************************
                        SET CELL COLOR
*******************************************************************************/
void set_cell_color(AppState* app_state, int x_pos, int y_pos, int color_in)
{
    GdkPixbuf* new_color;
    switch(color_in)
    {
        case red:
            new_color = app_state->pixbuf_red;
            break;
        case green:
            new_color = app_state->pixbuf_green;
            break;
        case blue:
            new_color = app_state->pixbuf_blue;
            break;
        case gray:
            new_color = app_state->pixbuf_gray;
            break;
        default:
            printf("error in main, set_cell_color\n");
            exit(-1);
    }
    gtk_image_set_from_pixbuf(app_state->grid_array[x_pos][y_pos],
                              new_color);
}

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
                  char* fileName)
{
    BASE_TYPE* element = space;
    FILE *readFile;
    readFile = fopen(fileName, "r");
    if (fread(space, sizeof(BASE_TYPE), TOTAL_BASE_UNITS+META_DATA, readFile)
       !=TOTAL_BASE_UNITS+META_DATA)
    {
        g_print("error opening file");
        exit(-1);
    }
    fclose(readFile);

    gtk_spin_button_set_value((GtkSpinButton*)app_state->node_spin_button,
                              (element[TOTAL_BASE_UNITS+1]));
    gtk_spin_button_set_value((GtkSpinButton*)app_state->links_spin_button,
                              (element[TOTAL_BASE_UNITS+2]));
    gtk_spin_button_set_value((GtkSpinButton*)app_state->life_length_spin_button,
                              (element[TOTAL_BASE_UNITS+3]));
    gtk_spin_button_set_value((GtkSpinButton*)app_state->generations_spin_button,
                              1);
    gtk_spin_button_set_value((GtkSpinButton*)app_state->mutate_spin_button,
                              (element[TOTAL_BASE_UNITS+5]));
    gtk_spin_button_set_value((GtkSpinButton*)app_state->max_calls_spin_button,
                              (element[TOTAL_BASE_UNITS+6]));
}

/*******************************************************************************
                        SAVE TO FILE
*******************************************************************************/
void saveToFile(ProgramSpace* space,
                AppState* app_state)
{

    BASE_TYPE* element = space;

    char currentTime[20];
    sprintf(currentTime, "%d", time(NULL));
    strncat(currentTime, ".evo", 4);
    FILE *writeFile;
    writeFile = fopen(currentTime, "w");
    element[TOTAL_BASE_UNITS + 1] = (BASE_TYPE)gtk_adjustment_get_value
                            (app_state->node_spin_button_adjustment);
    element[TOTAL_BASE_UNITS + 2] = (BASE_TYPE)gtk_adjustment_get_value
                            (app_state->links_spin_button_adjustment);
    element[TOTAL_BASE_UNITS + 3] = (BASE_TYPE)gtk_adjustment_get_value
                            (app_state->life_length_spin_button_adjustment);
    element[TOTAL_BASE_UNITS + 4] = (BASE_TYPE)gtk_adjustment_get_value
                            (app_state->generations_spin_button_adjustment);
    element[TOTAL_BASE_UNITS + 5] = (BASE_TYPE)gtk_adjustment_get_value
                            (app_state->mutate_spin_button_adjustment);
    element[TOTAL_BASE_UNITS + 6] = (BASE_TYPE)gtk_adjustment_get_value
                            (app_state->max_calls_spin_button_adjustment);
    if(fwrite(space, sizeof(BASE_TYPE), TOTAL_BASE_UNITS + META_DATA, writeFile)
       !=TOTAL_BASE_UNITS+META_DATA)
    {
        g_print("error saving file");
        exit(-1);
    }
    fclose(writeFile);
}

/*******************************************************************************
*                                                                              *
*                           HELPER FUNCTOINS                                   *
*                                                                              *
*******************************************************************************/
/*******************************************************************************
                            WOLRD ARRAY SHUFFLE
*******************************************************************************/
void worldArrayShuffle(unsigned int array[], unsigned int size)
{
    unsigned int tmpIndex;
    unsigned int tmpValue;

    int i;
    for(i = 0; i < size-1; i++)
    {
        tmpIndex = rand()%(size-i)+i;
        tmpValue = array[tmpIndex];
        array[tmpIndex] = array[i];
        array[i] = tmpValue;
    }
}

//*******************************************************************************
//                        FOR DEBUGGING
//*******************************************************************************/
//void worldTestWorldSet(Agent* world[][WORLD_SIZE], int type)
//{
//    int x,y;
//    for(x=0; x<WORLD_SIZE; x++)
//    {
//        for(y=0; y<WORLD_SIZE; y++)
//        {
//            Agent* newAgent = malloc(sizeof(Agent));
//            newAgent->type = type;
//            world[x][y] = newAgent;
//        }
//    }
//}
