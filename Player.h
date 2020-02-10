#include <string>
#include "LinkedList.h"
#include "Bag.h"

class Player {

    public:
        Player(std::string name, int score);

        //Draws a Tile from the linked List Bag
        void Draw(Bag* bag, int tiles);

        //Adds a Tile to the hand
        void addToHand(Tile* tile);

        //Prints all nodes in hand in a savable way
        std::string handToSaveString();

        //Prints all nodes in hand
        std::string handToString();

        std::string getName();
        //checks if tile is in the players hand
        Tile* hasTile(char color, int shape);
        //deletes tile from players hand
        void deleteTile(char color, int shape);
        //increases users score
        void updateScore(int score);
        //hand size
        bool hasTilesLeft();
        
        void setName(std::string name);

        int getScore();

    private:
        std::string name, studentNumber, Email;
        int score;
        LinkedList* hand = new LinkedList();
};
