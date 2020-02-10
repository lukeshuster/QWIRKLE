#include <string>
#include "LinkedList.h"
#include "Tile.h"

class Bag {
    public:
        Bag();
        // gets a random tile from the bag
        Tile* getTile();
        LinkedList* getLinkedList();
        //checks if the tile is in the bag
        bool hasTile(char color, int shape);
        void fillBag();
        // gets the amount of tiles left in the bag
        int getLength();
        // places a tile back into the bag
        void addToBag(Tile* tile);

        std::string bagToSaveString();

    private:
        LinkedList* tiles = new LinkedList();
};
