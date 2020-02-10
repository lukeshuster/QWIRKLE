
#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include "Node.h"
#include "Tile.h"
#include "iostream"
#include <time.h>

class LinkedList {
public:

   LinkedList();
   ~LinkedList();
   // adds to the front of the linked list
   void addToFront(Tile *tile);
   //adds to the end of the linked list
   void addToEnd(Tile *tile);
   //deletes node from the linked list
   void deleteNode(char color, int shape);
   //returns a string of all tiles in the linked list
   std::string print();
   //returns a string of all tiles in the linked list for saving
   std::string savePrint();
   Tile* hasTile(char color, int shape);
   //returns random tile
   Tile* getRandomTile();
   //gets tile from the front of the linked list
   Tile* getHead();
   int getLength();

//private:
   Node* head;
   int length;
};

#endif // ASSIGN2_LINKEDLIST_H
