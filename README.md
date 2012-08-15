Evolve is a program to simulate genetic evolution. In evolve, a 40x40 2D field exists on which 100
each, red, blue and green agents pass through life cycles of a user defined length. At the end of each
life cycle, the agents in the 75th percentile are selected by the program. Each agent which is selected is
then replicated 4 times. After all agents have been replicated, the program enters a mutation cycle. For
each node in the agent, a mutate functions is called. These nodes are randomly deleted and replaced
with a user defined probability. The new set of agents are then run again through a life cycle. This
process continues for a number or generations defined by the user.
The user is able to define several parameters regarding the agent and the evolutionary cycle but
to better understand these settings and their effects, it is helpful to have an overview of the structure of
the agent. The agent, and in fact all agents exist in the program as a monolithic 1D array of unsigned
integers. This array is divided into several subsections. The first division is into three sections for each
red, green and blue “agent community”. These communities are divided into 100 segments for each
individual agent. Each agent is further divided into several subsections for the given agent's “nodes”.
The nodes themselves are subdivided into 4 lists plus 3 special elements.
When a life cycle is being processed, the main program successively calls each individual agent.
When the agent is called, the agent driver begins an executions of various commands within the agent's
nodes. The agent begins by calling a special “start node”. The start node has an element representing
its state. This state can be either true or false. Depending on the state of the agent's state node, two
things can happen. If the state node is true, the nodes pointed to by list 1 are set to true and the nodes
pointed to by list 2 are set to false. If the state node is set to false, list 3 nodes are set to true, and list 4
nodes are set to false.
In addition to the 4 lists, and state element, each node also contains two “control next” elements
with the index of two nodes. After the node has processed its lists, it passes control to one of the two
control next elements. Which element it passes control to is determined by its state (if state is true, it
passes control to control next 1, else it passes control to control next 2).
Of the nodes that each agent contains, there are two types. The first are static and have special
purposes, the second are dynamic and variable. Of the static nodes, the first “start control node” has
already been discussed. This node is always the first node called by the agent. The second major type
of node is the sensor node.
If one imagines that surrounding the agent is a virtual 5x5 2D array, each element of that array
is tied to a “sensor node”. There are actually three sets of sensor nodes (one for each agent type).
When an agent's turn begins, each surrounding nodes is checked for the presence of an agent of each
given type. If presence is detected, a given sensor node, corresponding to that agent's type and location
is set. The state of these nodes is static. Unlike dynamic “neuro nodes” their state can not be changed
by other nodes, only by the presence or lack of presence of agents.
There is also a special set of nodes called direction nodes. There are nine of these representing
8 cardinal directions plus one representing no direction. These nodes can have their states set, however
only one direction node can be set at a time. After the end of the agent's turn, it is moved if one of the
cardinal direction nodes is set.
The final special node is a “move node”. There is only one “move node”. At the start of the
agent's turn, its state is set to false. If it is set to true during the agent's turn, execution of that agent's
turn ceases, and the agent is immediately moved if it has a direction node set.
Of the user defined functions in the GUI. The first is for “Neurons”. This sets the number of
dynamic nodes that each agent can have. The maximum number is 100. This is the number of nodes
each agent can have in addition to the static nodes (which are constant).
The second user defined input “Links” defines the number of elements in each of the 4 node
element lists. Again, these lists define the nodes whos states the given nodes is able to set. The range
is from 0-3. Resulting in 0-12 possible total links per node.
The life length widget defines the number of cycles which pass for each given generation.
The generations widget defines how generation cycles are passed through.
The mutation rate widget defines the probability of mutation expressed as a fraction of
input/100000. This mutation probability is the probability that each individual node will be mutated
not the probability that an agent will be mutated. The probability of an agent mutation is the user
defined probability of node mutation * the number of total nodes.
The max call widget defines the maximum allowable calls to individual nodes an agent is able
to make during its turn.
There are three check boxes allowing for visualization and saving visualizations to file. This is
useful since it allows the user to record the agent data and replay it. Running high numbers of
generations can take a long time. This allows the user to run a long cycle and then replay the results
once it has finished.
The visualize from file widget allows the user to replay these save files. Evolve files have a
.evo extension.
The visualize last cycle widget allows the user to observe only the running of the last generation
of a given cycle.
The save to file widget saves the program data to file after the execution of the last cycle for
later replay.

