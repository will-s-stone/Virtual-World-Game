#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

int respect = 40;


typedef struct{
    //creatureType location (two integers; creatureType is 0 for the PC,
    // 1 for an animal, 2 for an NPC.
    // Location is the number of the room the creature is in. Numbering starts at 0)
    struct Room* currentRoom;
    int type;
    int ID;
}Creature;



struct Room {
    int state;//0 = clean, 1 = half-dirty, 2 = dirty.
    int number;
    Creature* creatures[10];
    struct Room* north; //Pointers to neighboring rooms, we do this because we can't call on struct Room in itself.
    struct Room* south;
    struct Room* east;
    struct Room* west;
    int lastAdded; //Keeps track of the index we are at in the room's creatures array. Make sure to decrement when a creature moves.

};


int numberCreatures(struct Room *room){
    //Function that returns the number of creatures in a room
    int n = 0;
    for (int i = 0; i < 10; ++i) {
        if(room->creatures[i] != NULL){
            n++;
        }
    }
    return n;
}

void removeCreature(struct Room *room, Creature *creature){
    //Get the array of creatures, find the creature using the ID, remove the creature.
    for (int i = 0; i < numberCreatures(room); ++i) {
        if (room->creatures[i] == NULL){
            //Do nothing
        } else if(room->creatures[i] == creature){
            room->creatures[i] = NULL;
        }
    }
    printf("%s", "\n");
    printf("%d", creature->ID);
    printf("%s", " has drilled a hole in the ceiling and left the house\narggghh \nargghhh");
}


char* look(struct Room *room){
    //We take in a pointer to a room and then return the pointer to the description of the room.
    char* description = malloc(1024); //malloc = memory allocate.
    int num_creatures = 0;
    //CHANGED BODY OF FOR LOOP HERE*****
    for (int i = 0; i < 10 && room->creatures[i] != NULL; i++) {
        if (room->creatures[i] != NULL) {
            num_creatures++;
        }
    }
    if(room ==  NULL){
        sprintf(description, "You are not in a room.\n"); //sprintf, allow us to append strings.
    }else{
        sprintf(description, "You are in room %d.\n", room->number ); //(pointer_name)->(variable_name)
        sprintf(description + strlen(description), "The state of this room is: ");
        //description + strlen(description) is just string concatenation.
        switch(room->state) {
            case 0 :
                sprintf(description + strlen(description), "clean\n");
                break;
            case 1 :
                sprintf(description + strlen(description), "half-dirty\n");
                break;
            case 2 :
                sprintf(description + strlen(description), "dirty\n");
                break;
            default:
                sprintf(description + strlen(description), "State not known.\n");
                break;
        }
        sprintf(description + strlen(description), "Neighbors:\n");
        if (room->north != NULL) {
            sprintf(description + strlen(description), "- To the north is room %d.\n", room->north->number);
        } else {
            sprintf(description + strlen(description), "- There is no room to the north.\n");
        }
        if (room->south != NULL) {
            sprintf(description + strlen(description), "- To the south is room %d.\n", room->south->number);
        } else {
            sprintf(description + strlen(description), "- There is no room to the south.\n");
        }
        if (room->east != NULL) {
            sprintf(description + strlen(description), "- To the east is room %d.\n", room->east->number);
        } else {
            sprintf(description + strlen(description), "- There is no room to the east.\n");
        }
        if (room->west != NULL) {
            sprintf(description + strlen(description), "- To the west is room %d.\n", room->west->number);
        } else {
            sprintf(description + strlen(description), "- There is no room to the west.\n");
        }
        sprintf(description + strlen(description), "Creatures in the room: \n");

        for (int i = 0; i < 10; ++i) {
            if(room->creatures[i] == NULL){

            }else {
                switch (room->creatures[i]->type) {
                    case 0:
                        sprintf(description + strlen(description), "%d", room->creatures[i]->ID);
                        sprintf(description + strlen(description), ": PC\n");

                        break;
                    case 1:
                        sprintf(description + strlen(description), "%d", room->creatures[i]->ID);
                        sprintf(description + strlen(description), ": Animal\n");
                        break;
                    case 2:
                        sprintf(description + strlen(description), "%d", room->creatures[i]->ID);
                        sprintf(description + strlen(description), ": NPC\n");
                        break;
                }
            }
        }
    }

    //free(description);
    return description;
}


int moveCreature(struct Room* currentRoom, Creature *creature, int direction) {
    //Return 0 if works, return 1 otherwise
    struct Room* destinationRoom = NULL;
    switch(direction) {
        case 1 :
            destinationRoom = currentRoom->north;
            if(destinationRoom == NULL) {
                    printf("%s", "Can't go there");
                    return 1;
            }

            break;
        case 2 :
            destinationRoom = currentRoom->south;
            if(destinationRoom == NULL){
                printf("%s", "Can't go there");
                return 1;
            }
            break;
        case 3 :
            destinationRoom = currentRoom->east;
            if(destinationRoom == NULL){
                printf("%s", "Can't go there");
                return 1;
            }
            break;
        case 4 :
            destinationRoom = currentRoom->west;
            if(destinationRoom == NULL){
                printf("%s", "Can't go there");
                return 1;
            }
            break;
        default:
            printf("%s", "That is not a valid direction");
            return 1;
    }
    if (destinationRoom->lastAdded >= 10) {
        return 1; //Full room
    }
    //Remove creature from current room then add to destination room.
    //for (int i = 0; i < numberCreatures(currentRoom); ++i) {
    for(int i = 0; i < 10; ++i){
        if(currentRoom->creatures[i]==NULL){
            //Do nothing
        } else if(currentRoom->creatures[i]->ID == creature->ID){
            currentRoom->creatures[i] = NULL;
            //May or may not need the following...
            currentRoom->lastAdded--;
            break;
        }
    }
    //Add
    for (int i = 0; i < 10; i++) {
        if (destinationRoom->creatures[i] == NULL) {
            destinationRoom->creatures[i] = creature;
            //May or may not need the following...
            destinationRoom->lastAdded++;
            break;
        }
    }
    creature->currentRoom = destinationRoom;
    if(creature->type != 0) {
        printf("%d", creature->ID);
        printf("%s", ": has moved to room ");
    }
    printf("%s", "\nYou have moved to room ");
    printf("%d", creature->currentRoom->number);
    return 0;
}

struct Room* roomGivenDirection(struct Room *room, int direction){
    switch(direction){
        case 1:return room->north;
        case 2:return room->south;
        case 3:return room->east;
        case 4:return room->west;
    }
}

void cleanOrDirty(struct Room *room, int i, int multiplier){
    //0 means no multiplier, 1 means 3x multiplier;
    //0, decrease/clean, 1 increase/dirty
    // 0 clean, 1 half-dirty,
    //The int that we take in signifies whether we are trying to clean the room, decrement,
    //or dirty in which case we increment.
    //Depending on the type of animal we may order certain creatures to leave or drill hole

    if(i == 1){
        //If the room already isn't dirty, we increment it's state.

        if (room->state != 2){

            room->state = room->state + 1;
            //If the room is dirty, we get all the creatures in the room and if they are
            //Animals want their rooms to be clean;
            //NPCs want their rooms to be dirty;
            //1 for Animal, 2 for NPC;
            if(room->state == 1){
                //When we go from clean to dirty.
                //We want animals to growl and NPCs to smile
                for (int j = 0; j < 10; ++j) {
                    if(room->creatures[j] == NULL){
                        //Do nothing
                    } else if(room->creatures[j]->type == 1) {
                        if (multiplier == 1) {
                            //Growl 3 times
                            printf("%d", room->creatures[j]->ID);
                            printf("%s", " growls\n");
                            printf("%d", room->creatures[j]->ID);
                            printf("%s", " growls\n");
                            printf("%d", room->creatures[j]->ID);
                            printf("%s", " growls\n");
                            respect = respect - 3;
                            printf("%s", "respect level is now ");
                            printf("%d", respect);
                            printf("%s", "\n");
                        }
                        if (multiplier == 0) {
                            printf("%d", room->creatures[j]->ID);
                            printf("%s", " growls\n");
                            respect = respect - 1;
                            printf("%s", "respect level is now ");
                            printf("%d", respect);
                            printf("%s", "\n");
                        }
                    }else if(room->creatures[j]->type == 2) {
                        if (multiplier == 1) {
                            //Smile 3 times
                            printf("%d", room->creatures[j]->ID);
                            printf("%s", " smiles\n");
                            printf("%d", room->creatures[j]->ID);
                            printf("%s", " smiles\n");
                            printf("%d", room->creatures[j]->ID);
                            printf("%s", " smiles\n");
                            respect = respect + 3;
                            printf("%s", "respect level is now ");
                            printf("%d", respect);
                            printf("%s", "\n");
                        }
                        if (multiplier == 0) {
                            printf("%d", room->creatures[j]->ID);
                            printf("%s", " smiles\n");
                            respect = respect + 1;
                            printf("%s", "respect level is now ");
                            printf("%d", respect);
                            printf("%s", "\n");
                        }
                    }
                }

            }

            if(room->state == 2){
                //Animals will leave, expressing their discontent with a growl.
                //NPC's will stay and smile.

                for (int j = 0; j < 10 && room->creatures[j] != NULL; ++j) {
                    if(room->creatures[j]->type == 2){
                        if(multiplier == 1){
                            printf("%d", room->creatures[j]->ID);
                            printf("%s", " smiles\n");
                            printf("%d", room->creatures[j]->ID);
                            printf("%s", " smiles\n");
                            printf("%d", room->creatures[j]->ID);
                            printf("%s", " smiles\n");
                            respect = respect + 3;
                            printf("%s", "respect level is now ");
                            printf("%d", respect);
                            printf("%s", "\n");
                        }
                        if(multiplier == 0){
                            printf("%d", room->creatures[j]->ID);
                            printf("%s", " smiles\n");
                            respect = respect + 1;
                            printf("%s", "respect level is now ");
                            printf("%d", respect);
                            printf("%s", "\n");

                        }
                    }
                    if(room->creatures[j]->type == 1 ){
                        struct Room *newRoom = NULL;
                        int randomDir;
                        int direction;
                        do{
                            randomDir = rand() % 4; //Random int/direction 0-3
                            switch(randomDir){
                                case 0:
                                    newRoom = room->north;
                                    direction = 1;
                                    break;
                                case 1:
                                    newRoom = room->south;
                                    direction = 2;
                                    break;
                                case 2:
                                    newRoom = room->east;
                                    direction = 3;
                                    break;
                                case 3:
                                    newRoom = room->west;
                                    direction = 4;
                                    break;
                            }
                        } while (newRoom == NULL);
                        if(multiplier == 1){
                            printf("%d", room->creatures[j]->ID);
                            printf("%s", " growls\n");
                            printf("%d", room->creatures[j]->ID);
                            printf("%s", " growls\n");
                            printf("%d", room->creatures[j]->ID);
                            printf("%s", " growls\n");
                            respect = respect - 3;
                            printf("%s", "respect level is now ");
                            printf("%d", respect);
                            printf("%s", "\n");
                        }
                        if(multiplier == 0){
                            printf("%d", room->creatures[j]->ID);
                            printf("%s", " growls\n");
                            respect = respect - 1;
                            printf("%s", "respect level is now ");
                            printf("%d", respect);
                            printf("%s", "\n");
                        }

                        moveCreature(room, room->creatures[j], direction);


                        //If the room we move to is dirty, we clean it.
                        if(roomGivenDirection(room, direction)->state == 2){
                            roomGivenDirection(room, direction)->state = 1;
                        }
                    }
                }
            }
            //Now lets get the creatures and check to see what their preference is, and change the respect value, respectively.
            return;
        }
        printf("%s", "Room is already dirty");
        return;
    }

    if(i == 0){
        //If the room already isn't clean, we decrement it's state.
        if(room->state != 0){
            room->state = room->state - 1;
            //If the room is dirty, we get all the creatures in the room and if they are
            //Animals want their rooms to be clean;
            //NPCs want their rooms to be dirty;
            //1 for Animal, 2 for NPC;
            //Clean to dirty
            if(room->state == 1){
                if(room->state == 1){
                    //When we go from dirty to clean.
                    //We want animals to growl and NPCs to smile
                    for (int j = 0; j < 10; ++j) {
                        if(room->creatures[j] == NULL){
                            //Do nothing
                        } else if(room->creatures[j]->type == 1) {
                            if (multiplier == 1) {
                                //Lick face 3 times
                                printf("%d", room->creatures[j]->ID);
                                printf("%s", " lick face\n");
                                printf("%d", room->creatures[j]->ID);
                                printf("%s", " lick face\n");
                                printf("%d", room->creatures[j]->ID);
                                printf("%s", " lick face\n");
                                respect = respect + 3;
                                printf("%s", "respect level is now ");
                                printf("%d", respect);
                                printf("%s", "\n");
                            }
                            if (multiplier == 0) {
                                printf("%d", room->creatures[j]->ID);
                                printf("%s", " lick face\n");
                                respect = respect + 1;
                                printf("%s", "respect level is now ");
                                printf("%d", respect);
                                printf("%s", "\n");
                            }
                        }else if(room->creatures[j]->type == 2) {
                            if (multiplier == 1) {
                                //Smile 3 times
                                printf("%d", room->creatures[j]->ID);
                                printf("%s", " grumbles\n");
                                printf("%d", room->creatures[j]->ID);
                                printf("%s", " grumbles\n");
                                printf("%d", room->creatures[j]->ID);
                                printf("%s", " grumbles\n");
                                respect = respect - 3;
                                printf("%s", "respect level is now ");
                                printf("%d", respect);
                                printf("%s", "\n");
                            }
                            if (multiplier == 0) {
                                printf("%d", room->creatures[j]->ID);
                                printf("%s", " grumbles\n");
                                respect = respect - 1;
                                printf("%s", "respect level is now ");
                                printf("%d", respect);
                                printf("%s", "\n");
                            }
                        }
                    }

                }
            }
            if(room->state == 0){
                //Animals will leave, expressing their discontent with a growl.
                //NPC's will stay and smile.
                for (int j = 0; j < 10 && room->creatures[j] != NULL; ++j) {

                    if(room->creatures[j]->type == 1){
                        if(multiplier == 1){
                            printf("%d", room->creatures[j]->ID);
                            printf("%s", " licks face\n");
                            printf("%d", room->creatures[j]->ID);
                            printf("%s", " licks face\n");
                            printf("%d", room->creatures[j]->ID);
                            printf("%s", " licks face\n");
                            respect = respect + 3;
                            printf("%s", "respect level is now ");
                            printf("%d", respect);
                            printf("%s", "\n");
                        }
                        if(multiplier == 0){
                            printf("%d", room->creatures[j]->ID);
                            printf("%s", " licks face\n");
                            respect = respect + 1;
                            printf("%s", "respect level is now ");
                            printf("%d", respect);
                            printf("%s", "\n");
                        }
                    }
                    if(room->creatures[j]->type == 2 ){
                        struct Room *newRoom = NULL;
                        int randomDir;
                        int direction;
                        do{
                            randomDir = rand() % 4; //Random int/direction 0-3
                            switch(randomDir){
                                case 0:
                                    newRoom = room->north;
                                    direction = 1;
                                    break;
                                case 1:
                                    newRoom = room->south;
                                    direction = 2;
                                    break;
                                case 2:
                                    newRoom = room->east;
                                    direction = 3;
                                    break;
                                case 3:
                                    newRoom = room->west;
                                    direction = 4;
                                    break;
                            }
                        } while (newRoom == NULL);
                        if(room->creatures[j]->type == 2){
                            if(multiplier == 1){
                                printf("%d", room->creatures[j]->ID);
                                printf("%s", " grumbles\n");
                                printf("%d", room->creatures[j]->ID);
                                printf("%s", " grumbles\n");
                                printf("%d", room->creatures[j]->ID);
                                printf("%s", " grumbles\n");
                                respect = respect - 3;
                                printf("%s", "respect level is now ");
                                printf("%d", respect);
                                printf("%s", "\n");
                            }
                            if(multiplier == 0){
                                printf("%d", room->creatures[j]->ID);
                                printf("%s", " grumbles\n");
                                respect = respect - 1;
                                printf("%s", "respect level is now ");
                                printf("%d", respect);
                                printf("%s", "\n");
                            }
                        }

                        if(roomGivenDirection(room, direction)->lastAdded>=10){
                            removeCreature(room, room->creatures[j]);
                        }
                        moveCreature(room, room->creatures[j], direction);

                        //If the room we move to is dirty, we clean it.
                        if(roomGivenDirection(room, direction)->state == 0){
                            roomGivenDirection(room, direction)->state = 1;
                        }
                        printf("%s", "\n");
                    }
                }
            }
            return;
        }
        printf("%s", "Room is already clean");
        return;

    }else{
        printf("%s", "Darn");
    }

}


int main() {

    /*
     * n (integer, the number of rooms, 1 < n < 100)
     * The next n lines are the following:
      state north south east west (five integers, signifying the cleanliness of the room and the numbers
      of the respective neighbors. State can be 0 for clean, 1 for half-dirty, 2 for dirty. -1 signifies no neighbor.
      Numbering of the rooms starts at 0)
     * The next line is:
      m (integer, the number of creatures, 1 < m < 100)
      The next m lines are:
      creatureType location (two integers; creatureType is 0 for the PC, 1 for an animal, 2 for an NPC. Location is the number of the room the creature is in. Numbering starts at 0)
     */
    int maxRooms = 100;
    int maxCreatures = 100;
    int numberOfRooms;
    int numberOfCreatures;
    int uniqueID = 100;
    Creature player;
    //Room array
    struct Room rooms[maxRooms];
    Creature creatures[maxCreatures];

    /*
     * scanf() notes:
     * %c = single character
     * %d = decimal int
     * %s = String of characters
     */
    //Taking in the number of rooms given by the first line of input.
    scanf("%d", &numberOfRooms); //

    //Now we use a loop to iterate over each line and store the respective values in the Room struct.
    for (int i = 0; i < numberOfRooms; i++) {
        scanf("%d", &rooms[i].state);
        rooms[i].number = i;

        //MIGHT HAVE TO CHANGE BACK!!!!
        for (int j = 0; j < 10; ++j) {
            rooms[i].creatures[i] = NULL;
        }

        // 'state' 'north' 'south' 'east' 'west'
        int north, south, east, west;
        scanf("%d %d %d %d", &north, &south, &east, &west);
        //If north is non-negative, indicating it has a neighbor room in that direction,
        //we set the room we're at in the index's north pointer to point to the room that
        //matches up with that neighbor, essentially just making a connective between the two.
        if(north >= 0){
            rooms[i].north = &rooms[north];
        } else rooms[i].north = NULL;

        if(south>=0){
            rooms[i].south= &rooms[south];
        } else rooms[i].south = NULL;

        if(east >= 0){
            rooms[i].east = &rooms[east];
        } else rooms[i].east = NULL;

        if(west >= 0){
            rooms[i].west = &rooms[west];
        } else rooms[i].west = NULL;


    }

    //Fill in the creatures array.
    scanf("%d", &numberOfCreatures);
    for (int i = 0; i < numberOfCreatures; ++i) {
        scanf("%d", &creatures[i].type);

        creatures[i].ID = uniqueID;
        uniqueID++;
        int tempRoom;
        scanf("%d", &tempRoom);
        for (int j = 0; j < numberOfRooms; ++j) {
            if(rooms[j].number == tempRoom){
                //Matching the room number to the creatures then setting the creatures current room to that.
                creatures[i].currentRoom = &rooms[j];
                //The room also has an array of creatures, so we want to fill this in as well.
                //We use the index of the room we currently have, go to its creature's array, and add the creature that we currently have at i.
                rooms[j].creatures[rooms[j].lastAdded] = &creatures[i];
                //Don't forget to increment lastAdded.
                rooms[j].lastAdded++;
            }
        }
        if(creatures[i].type == 0){
            player = creatures[i];
        }
    }

    do {
        char input[100];
        scanf("%s", input);
        int creatureID;
        char task[10];


        if (sscanf(input, "%d:%s", &creatureID, task) == 2) {
            if (strcmp(task, "clean") == 0) {
                for (int i = 0; i < 10 && player.currentRoom->creatures[i] != NULL; ++i) {
                    cleanOrDirty(player.currentRoom, 0, 1);
                    // Adjust respect here. Could add in parameter to the cleanOrDirty function to represent the multiplier.
                    // aka, cleanOrDirty(room, 0 or 1, 0 or 1) 0 being respect level changing at normal rate or 1 for a 3x multiplier
                    break;
                }
            }
            else if (strcmp(task, "dirty") == 0) {
                for (int i = 0; i < 10 && player.currentRoom->creatures[i] != NULL; ++i) {
                    if (player.currentRoom->creatures[i]->type != 0) {
                        cleanOrDirty(player.currentRoom, 1, 1);
                        //Adjust respect here.
                        break;
                    }
                }
            } else if(strcmp(task, "north") == 0){
                //Find creatureID
                for (int i = 0; i < 10; ++i) {
                    if (player.currentRoom->creatures[i] == NULL){
                        //do nothing
                    } else if (player.currentRoom->creatures[i]->ID == creatureID){
                        moveCreature(player.currentRoom, player.currentRoom->creatures[i], 1);
                    }
                }
            } else if(strcmp(task, "south") == 0){
                //Find creatureID
                for (int i = 0; i < 10; ++i) {
                    if (player.currentRoom->creatures[i] == NULL){
                        //do nothing
                    } else if (player.currentRoom->creatures[i]->ID == creatureID){
                        moveCreature(player.currentRoom, player.currentRoom->creatures[i], 2);
                    }
                }
            } else if(strcmp(task, "east") == 0){
                //Find creatureID
                for (int i = 0; i < 10; ++i) {
                    if (player.currentRoom->creatures[i] == NULL){
                        //do nothing
                    } else if (player.currentRoom->creatures[i]->ID == creatureID){
                        moveCreature(player.currentRoom, player.currentRoom->creatures[i], 3);
                    }
                }
            } else if(strcmp(task, "west") == 0){
                //Find creatureID
                for (int i = 0; i < 10; ++i) {
                    if (player.currentRoom->creatures[i] == NULL){
                        //do nothing
                    } else if (player.currentRoom->creatures[i]->ID == creatureID){
                        moveCreature(player.currentRoom, player.currentRoom->creatures[i], 4);
                    }
                }
            }

            else {
                printf("Invalid task: %s\n", task);
            }
        }

        if(strcmp(input, "look") == 0){
            printf("%s", look(player.currentRoom));
        }

        if(strcmp(input, "north") == 0){
            moveCreature(player.currentRoom, &player, 1);
        }

        if(strcmp(input, "south") == 0){
            moveCreature(player.currentRoom, &player, 2);
        }

        if(strcmp(input, "east") == 0){
            moveCreature(player.currentRoom, &player, 3);
        }

        if(strcmp(input, "west") == 0){
            moveCreature(player.currentRoom, &player, 4);
        }
        if(strcmp(input, "exit") == 0) {
            printf("%s", "Goodbye");
            respect = -1;
            exit(0);
        }
        if(strcmp(input, "clean") == 0) {
            cleanOrDirty(player.currentRoom, 0, 0);
        }
        if(strcmp(input, "dirty") == 0) {
            cleanOrDirty(player.currentRoom, 1, 0);
        }

    } while( !(respect >= 80 || respect < 0));
    if (respect>=80){
        printf("%s", "\nCongrats!!!");
    }
    if(respect<=0){
        printf("%s", "\nBetter luck next time");
    }
    return 0;
}