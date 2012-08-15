#ifndef typedefs_h
#define typedefs_h

#include <limits.h>
#include <gtk/gtk.h>



/*******************************************************************************
*                                                                              *
*                       GTK DATA                                               *
*                                                                              *
*******************************************************************************/
/*******************************************************************************
                        MACROS
*******************************************************************************/
#define WINWIDTH        400
#define WINHEIGHT       400
#define TABLE_WIDTH     40
#define TABLE_HEIGHT    40
/*******************************************************************************
                        APP STATE
*******************************************************************************/
typedef struct _AppState
{
    GtkWidget *window;
    GtkWidget *vbox;
    GtkWidget *hbox_top;
    GtkTable *table;
    GtkWidget *node_spin_button;
    GtkAdjustment *node_spin_button_adjustment;
    GtkWidget *links_spin_button;
    GtkAdjustment *links_spin_button_adjustment;
    GtkWidget *life_length_spin_button;
    GtkAdjustment *life_length_spin_button_adjustment;
    GtkWidget *generations_spin_button;
    GtkAdjustment *generations_spin_button_adjustment;
    GtkWidget *mutate_spin_button;
    GtkAdjustment *mutate_spin_button_adjustment;
    GtkWidget *max_calls_spin_button;
    GtkAdjustment *max_calls_spin_button_adjustment;
    GtkWidget* from_file_check_button;
    GtkWidget* save_to_file_check_button;
    GtkWidget* visualize_last_generation_check_button;
    GtkWidget* generic_label;
    GtkWidget *wrapper_vbox;
    GtkImage* grid_array[TABLE_WIDTH][TABLE_HEIGHT];
    GdkPixbuf* pixbuf_red;
    GdkPixbuf* pixbuf_green;
    GdkPixbuf* pixbuf_blue;
    GdkPixbuf* pixbuf_gray;
    GtkWidget* bottom_hbox;
    GtkWidget* bottom_hbox2;
    GtkWidget* bottom_hbox3;
    GtkWidget* start_button;
    gboolean visibleCheck;
    gboolean saveResults;
    gboolean visualise_from_file;
    int delayTime;
    int makeVisible;
} AppState;


/*******************************************************************************
*                                                                              *
*                       AGENT DATA                                             *
*                                                                              *
*******************************************************************************/
/*******************************************************************************
                        PRIMITIVES
*******************************************************************************/
#define NODE_NULL                   UINT_MAX
#define BASE_TYPE                   unsigned int
/*******************************************************************************
                        NODES
*******************************************************************************/

///If any elements are changed, NODE_TOTAL_ELEMENTS must be changed to reflect
///new sum.
///NODE_STATE                   1 //last elem is sate
///NODE_CONTROL                 2 //last 2 elem are control
#define NODE_MAX_LINKS          12 ///must be evenly divisible by 4
#define NODE_TOTAL_ELEMENTS     15 ///SUM OF NODE ELEMENTS

/** Each list is a set of data members which point to other nodes.  When a
    node is called, if its state is true, it sets list one nodes to true and
    list 2 nodes to false.  If its state is false, it sets nodes in list 3
    to true and nodes in list 4 to be false.
*/
#define NODE_ELEMENT_LISTS      4
#define NODE_ELEMENTS_PER_LIST  NODE_MAX_LINKS/NODE_ELEMENT_LISTS

///node's sate is last element
#define NODE_STATE              NODE_TOTAL_ELEMENTS - 1
///if node's state is true, control is passed to node pointed to here
#define NODE_TRUE_CONTROL       NODE_TOTAL_ELEMENTS - 2
///if node's state is false, control is passed to node pointed to here
#define NODE_FALSE_CONTROL      NODE_TOTAL_ELEMENTS - 3

typedef BASE_TYPE Node[NODE_TOTAL_ELEMENTS];


/*******************************************************************************
                        AGENTS
*******************************************************************************/

///If any elements are changed, AGENT_NUMBER_OF_NODES must be changed to reflect
///new sum.

#define AGENT_NUMBER_OF_SENSORS          27// 9 each red, green, blue
///AGENT_START                           1
///AGENT_MOVE                            1
///AGENT_DIRECTIONS                      9
#define AGENT_MAX_NUMBER_OF_NEURONS      100
///sum of all nodes
#define AGENT_NUMBER_OF_NODES            138
///sum of all non-neuron nodes
#define AGENT_NON_NEURON_NODES           38

#define AGENT_NUMBER_OF_SENSOR_TYPES     3
#define AGENT_SENSORS_PER_TYPE           AGENT_NUMBER_OF_SENSORS/AGENT_NUMBER_OF_SENSOR_TYPES
#define AGENT_START_NODE_INDEX           AGENT_NUMBER_OF_SENSORS
#define AGENT_MOVE_NODE_INDEX            AGENT_NUMBER_OF_SENSORS+1
#define AGENT_DIRECTION_INDEX_FIRST      AGENT_NUMBER_OF_SENSORS+2
#define AGENT_DIRECTION_INDEX_LAST       AGENT_NUMBER_OF_SENSORS+10

typedef BASE_TYPE AgentData[AGENT_NUMBER_OF_NODES*NODE_TOTAL_ELEMENTS];

/*******************************************************************************
                        COMMUNITIES
*******************************************************************************/
///number of agents per community
#define COMMUNITY_NUMBER_OF_AGENTS   100
typedef BASE_TYPE Community[COMMUNITY_NUMBER_OF_AGENTS*AGENT_NUMBER_OF_NODES
                            *NODE_TOTAL_ELEMENTS];
///types of agent and non-agent type
enum {red, blue, green, gray};

/*******************************************************************************
                        PROGRAM_SPACE
*******************************************************************************/
#define PROGRAM_SPACE_NUMBER_OF_COMMUNITIES     3

///total number of data ellements in program
#define TOTAL_BASE_UNITS                        PROGRAM_SPACE_NUMBER_OF_COMMUNITIES*COMMUNITY_NUMBER_OF_AGENTS*AGENT_NUMBER_OF_NODES*NODE_TOTAL_ELEMENTS
#define PROGRAM_SPACE_TOTAL_AGENTS              PROGRAM_SPACE_NUMBER_OF_COMMUNITIES*COMMUNITY_NUMBER_OF_AGENTS

///space reserved for program meta-data
#define META_DATA                               10

typedef BASE_TYPE ProgramSpace[TOTAL_BASE_UNITS + META_DATA];




/*******************************************************************************
*                                                                              *
*                       MISC PROGRAM MACROS                                    *
*                                                                              *
*******************************************************************************/
/*******************************************************************************
                WORLD_SIZE
*******************************************************************************/
///World is always square, this defines the x and y dimension of the space the
///agents exist in.  Deined relative to GTK specifications for table size above.
#define WORLD_SIZE    TABLE_WIDTH

/*******************************************************************************
                WORLD_REPRODUCE_AGENTS CONSTANTS
                (should be made to be definable in GUI.
*******************************************************************************/
///points added when agent captues another agent
#define CAPTURE_WEIGHT                  100
///points added when agent survivies a cycle
#define LIFE_LENGTH_WEIGHT              1
///number of agents to reproduce at end of cycle
#define NUMBER_OF_AGENTS_TO_REPRODUCE   COMMUNITY_NUMBER_OF_AGENTS / 4
///number of copies to make of each reproduced agent
#define NUMBER_OF_REPRODUCTIONS         4

#endif
