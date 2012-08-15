/******************************************************************************
  Title          : final_breczin.c
  Author         : Dan Breciznski
  Created on     : December, 2011
  Description    : This program creates an evolutionary simulation with various
                   user defined parameters.  User is able to define rate of
                   mutations, life length of agents, etc.  User is able to save
                   program data to a file after cycle has been run and replay
                   the last generation of a given test.
  Usage          : final_breczin
  Build with     : gcc -o final_breczin main.c open_file_get_buffer.c\
                        world.c agent.c `pkg-config gtk+-2.0 --cflags --libs`

******************************************************************************/

#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <limits.h>
#include <time.h>

#include "typedefs.h"
#include "world.h"
#include "agent.h"

///contains all agent data
ProgramSpace PROGRAM_SPACE;
///2d array of pointers for keeping tack of agent locations
Agent* WORLD[WORLD_SIZE][WORLD_SIZE];
///keeps track of agent's success durring cycles
Agent AGENT_ARRAY[COMMUNITY_NUMBER_OF_AGENTS*3];


///main driver of program
///AppState defined in typedefs
void startMain(GtkWidget* button, AppState* app_state);


int main( int argc, char *argv[])

{

    srand(time(NULL));
    gtk_init(&argc, &argv);

    AppState app_state;
    app_state.makeVisible = 0;
    app_state.saveResults = 0;
    app_state.delayTime = 1;

/******************************************************************************
*                                                                             *
*                       GTK                                                   *
*                                                                             *
******************************************************************************/

/*******************************************************************************
                        CONTAINERS, TABLE, MAIN START BUTTON
*******************************************************************************/
    app_state.window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title              (GTK_WINDOW (app_state.window),
                                      "Evolve");
    gtk_window_set_default_size(GTK_WINDOW (app_state.window),
                                       WINWIDTH, WINHEIGHT);
    app_state.vbox = gtk_vbox_new (FALSE, 2);
    app_state.bottom_hbox = gtk_hbox_new (FALSE, 2);
    app_state.bottom_hbox2 = gtk_hbox_new (FALSE, 2);
    app_state.bottom_hbox3 = gtk_hbox_new (FALSE, 2);

    app_state.table = gtk_table_new(100, 100, FALSE);
    app_state.start_button = gtk_button_new_with_label("Run");

/*******************************************************************************
                        NODE SPIN BUTTON
*******************************************************************************/
    app_state.node_spin_button_adjustment =
            (GtkAdjustment *)gtk_adjustment_new(0,0,100,1,0,0);
    app_state.node_spin_button = gtk_spin_button_new
            (app_state.node_spin_button_adjustment,1,0);
    app_state.generic_label = gtk_label_new("Neurons\n0-100");
    app_state.wrapper_vbox = gtk_vbox_new(FALSE, 2);
    gtk_box_pack_start (GTK_BOX(app_state.wrapper_vbox),
                        app_state.generic_label,
                        TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(app_state.wrapper_vbox),
                        app_state.node_spin_button,
                        TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(app_state.bottom_hbox), app_state.wrapper_vbox,
                        TRUE, TRUE, 0);
/*******************************************************************************
                        LINKS SPIN BUTTON
*******************************************************************************/
    app_state.links_spin_button_adjustment =
            (GtkAdjustment *)gtk_adjustment_new(0,0,3,1,0,0);
    app_state.links_spin_button = gtk_spin_button_new
            (app_state.links_spin_button_adjustment,1,0);
    app_state.generic_label = gtk_label_new("Links\n0-3");
    app_state.wrapper_vbox = gtk_vbox_new(FALSE, 2);
    gtk_box_pack_start (GTK_BOX(app_state.wrapper_vbox),
                        app_state.generic_label,
                        TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(app_state.wrapper_vbox),
                        app_state.links_spin_button,
                        TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(app_state.bottom_hbox), app_state.wrapper_vbox,
                        TRUE, TRUE, 0);
/*******************************************************************************
                        LIFE LENGTH SPIN BUTTON
*******************************************************************************/
    app_state.life_length_spin_button_adjustment =
            (GtkAdjustment *)gtk_adjustment_new(0,0,1000,1,0,0);
    app_state.life_length_spin_button = gtk_spin_button_new
            (app_state.life_length_spin_button_adjustment,1,0);
    app_state.generic_label = gtk_label_new("Life length\n0-1000");
    app_state.wrapper_vbox = gtk_vbox_new(FALSE, 2);
    gtk_box_pack_start (GTK_BOX(app_state.wrapper_vbox),
                        app_state.generic_label,
                        TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(app_state.wrapper_vbox),
                        app_state.life_length_spin_button,
                        TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(app_state.bottom_hbox),
                        app_state.wrapper_vbox,TRUE, TRUE, 0);
/*******************************************************************************
                        GENERATIONS SPIN BUTTON
*******************************************************************************/
    app_state.generations_spin_button_adjustment =
            (GtkAdjustment *)gtk_adjustment_new(0,0,1000000,1,0,0);
    app_state.generations_spin_button =
        gtk_spin_button_new(app_state.generations_spin_button_adjustment,
                                                     1,
                                                     0);
    app_state.generic_label = gtk_label_new("Generations\n0-1000000");
    app_state.wrapper_vbox = gtk_vbox_new(FALSE, 2);
    gtk_box_pack_start (GTK_BOX(app_state.wrapper_vbox),
                        app_state.generic_label,
                        TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(app_state.wrapper_vbox),
                        app_state.generations_spin_button,
                        TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(app_state.bottom_hbox), app_state.wrapper_vbox,
                        TRUE, TRUE, 0);
/*******************************************************************************
                        MUTATE SPIN BUTTON
*******************************************************************************/
    app_state.mutate_spin_button_adjustment =
            (GtkAdjustment *)gtk_adjustment_new(0,0,100000,1,0,0);
    app_state.mutate_spin_button = gtk_spin_button_new
            (app_state.mutate_spin_button_adjustment,1,0);
    app_state.generic_label = gtk_label_new("Mutate rate\n0-100000");
    app_state.wrapper_vbox = gtk_vbox_new(FALSE, 2);
    gtk_box_pack_start (GTK_BOX(app_state.wrapper_vbox),
                        app_state.generic_label,
                        TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(app_state.wrapper_vbox),
                        app_state.mutate_spin_button,
                        TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(app_state.bottom_hbox), app_state.wrapper_vbox,
                        TRUE, TRUE, 0);
/*******************************************************************************
                        MAX CALLS SPIN BUTTON
*******************************************************************************/
    app_state.max_calls_spin_button_adjustment =
            (GtkAdjustment *)gtk_adjustment_new(0,0,10000,1,0,0);
    app_state.max_calls_spin_button =
        gtk_spin_button_new(app_state.max_calls_spin_button_adjustment,1,0);
    app_state.generic_label = gtk_label_new("Max calls / agent\n0-10000");
    app_state.wrapper_vbox = gtk_vbox_new(FALSE, 2);
    gtk_box_pack_start (GTK_BOX(app_state.wrapper_vbox),
                        app_state.generic_label,
                        TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(app_state.wrapper_vbox),
                        app_state.max_calls_spin_button,
                        TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(app_state.bottom_hbox), app_state.wrapper_vbox,
                        TRUE, TRUE, 0);

/*******************************************************************************
                        CHECK BOXES
*******************************************************************************/
    app_state.from_file_check_button =
            gtk_check_button_new_with_label("Visualise from file");
    gtk_box_pack_start (GTK_BOX(app_state.bottom_hbox2),
                        app_state.from_file_check_button,
                        TRUE, TRUE, 0);

    app_state.visualize_last_generation_check_button =
            gtk_check_button_new_with_label("Visualise last cycle");
    gtk_box_pack_start (GTK_BOX(app_state.bottom_hbox2),
                        app_state.visualize_last_generation_check_button,
                        TRUE, TRUE, 0);

    app_state.save_to_file_check_button =
            gtk_check_button_new_with_label("Save to file");
    gtk_box_pack_start (GTK_BOX(app_state.bottom_hbox2),
                        app_state.save_to_file_check_button,
                        TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(app_state.bottom_hbox3), app_state.start_button,
                     TRUE, TRUE, 0);


/*******************************************************************************
                        PACKING / SIGNAL CONNECT
*******************************************************************************/
    gtk_container_add(GTK_CONTAINER(app_state.window),app_state.vbox);
    gtk_box_pack_start (GTK_BOX(app_state.vbox), (GtkWidget*)app_state.table,
                        TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(app_state.vbox), app_state.bottom_hbox,
                        TRUE, TRUE, 0);

    gtk_box_pack_start (GTK_BOX(app_state.vbox), app_state.bottom_hbox2,
                        TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(app_state.vbox), app_state.bottom_hbox3,
                        TRUE, TRUE, 0);

    g_signal_connect (G_OBJECT(app_state.start_button),
                      "clicked",
                      G_CALLBACK (startMain),
                      &app_state);
    g_signal_connect(G_OBJECT(app_state.window), "destroy",
                 G_CALLBACK(gtk_main_quit),
                 NULL);
    init_grid(&app_state);///necessary?

    gtk_widget_show_all (app_state.window);

    gtk_main ();

    return(0);
}

/*******************************************************************************
                        START MAIN
*******************************************************************************/
void startMain(GtkWidget* button, AppState* app_state)
{
    int numberOfNeurons, numberOfElementsPerList, numberOfCycles,
        numberOfGenerations, mutationProbability, maximumCallsPerAgent;

    if(gtk_toggle_button_get_active ((GtkToggleButton*)
                                     app_state->from_file_check_button))
    {
        char* nextFile = get_file_name(app_state->window,
                                               "",
                                               "");
        if(nextFile != NULL)
        {
            openFromFile(PROGRAM_SPACE,
                         app_state,
                         nextFile);
        }
        else
        {
            return;
        }
        app_state->visibleCheck = TRUE;

        numberOfNeurons = (int)gtk_adjustment_get_value
                                (app_state->node_spin_button_adjustment);
        numberOfElementsPerList = (int)gtk_adjustment_get_value
                                (app_state->links_spin_button_adjustment);
        numberOfCycles = (int)gtk_adjustment_get_value
                                (app_state->life_length_spin_button_adjustment);

        numberOfGenerations = (int)gtk_adjustment_get_value
                                    (app_state->generations_spin_button_adjustment);

        mutationProbability = (int)gtk_adjustment_get_value
                                        (app_state->mutate_spin_button_adjustment);

        maximumCallsPerAgent = (int)gtk_adjustment_get_value
                                        (app_state->max_calls_spin_button_adjustment);

    }
    else
    {

        if(gtk_toggle_button_get_active
           ((GtkToggleButton*)app_state->visualize_last_generation_check_button))
        {
            app_state->visibleCheck = TRUE;
        }
        else
        {
            app_state->visibleCheck = FALSE;
        }
        if(gtk_toggle_button_get_active
           ((GtkToggleButton*)app_state->save_to_file_check_button))
        {
            app_state->saveResults = TRUE;
        }
        else
        {
            app_state->saveResults = FALSE;
        }

        numberOfNeurons = (int)gtk_adjustment_get_value
                                (app_state->node_spin_button_adjustment);
        numberOfElementsPerList = (int)gtk_adjustment_get_value
                                (app_state->links_spin_button_adjustment);
        numberOfCycles = (int)gtk_adjustment_get_value
                                (app_state->life_length_spin_button_adjustment);

        numberOfGenerations = (int)gtk_adjustment_get_value
                                    (app_state->generations_spin_button_adjustment);

        mutationProbability = (int)gtk_adjustment_get_value
                                        (app_state->mutate_spin_button_adjustment);

        maximumCallsPerAgent = (int)gtk_adjustment_get_value
                                        (app_state->max_calls_spin_button_adjustment);

        programSpaceInit(&PROGRAM_SPACE); ///review this
        agentInitAgentArrayNodes(&PROGRAM_SPACE,
                                 numberOfNeurons,
                                 numberOfElementsPerList);
    }

    init_world(&PROGRAM_SPACE, WORLD, AGENT_ARRAY);

    worldAgentDriver(&PROGRAM_SPACE,
                     WORLD,
                     AGENT_ARRAY,
                     numberOfCycles,
                     numberOfGenerations,
                     maximumCallsPerAgent,
                     mutationProbability,
                     numberOfElementsPerList,
                     app_state);
}
