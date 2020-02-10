#include "Board.h"

Board::Board(int y_size, int x_size){
    this->y_size = y_size;
    this->x_size = x_size;
}

int Board::placeTile(int x, int y, Tile* tile){
    if (map[y][x] == NULL){
        int score = allDirections(x, y, tile);
         if (score > 0){ //ONLY allows tile to be placed if it has 1 or greater score meaning it is connected to atleast 1 valid tile
             map[y][x] = tile;
         }
        return score;
    }
    return 0;
}

int Board::allDirections(int x, int y, Tile* tile){
     int score = 0;

        //IF result equals -1 the move is invalid and therefore score does not count
        //IF result equals 0 there is no connected tile in that direction and the next if statement is run to find a connecting tile

         int result = isValid(y, x, tile, -1, 0); //Checks if there are any connecting tiles UP
         score += result;
         if(result == -1){
             return -1;
         }
         result = isValid(y, x, tile, 1, 0); //Checks if there are any connecting tiles DOWN
         score += result;
         if(result == -1){
              return -1;
         }
         result = isValid(y, x, tile, 0, -1);  //Checks if there are any connecting tiles RIGHT
         score += result;
         if(result == -1){
              return -1;
         }
         result = isValid(y, x, tile, 0, 1); //Checks if there are any connecting tiles LEFT
         score += result;
         if(result == -1){
              return -1;
         }
    return score;
}

void Board::tileHint(Tile* tile){
    for(int y = 0; y < y_size;y++){
        for(int x = 0; x < x_size;x++){
            if(nullCheck(y, x) == true){
                int score = allDirections(x, y, tile);
                if (score > 0){

                    std::cout << "Valid Tile Placement at: " << letters[y] << x  + 1 << " for " << score << " points \n";
                    return;
                }

            }
        }
    }
     std::cout << "There is no valid location to place this tile";
}

void Board::loadTile(int x, int y, Tile* tile){
    if (map[y][x] == NULL){
        map[y][x] = tile;
    }
}

int Board::isValid(int y, int x, Tile* tile, int y_change, int x_change){
   if(nullCheck(y + y_change, x + x_change) == true){
        return 0;

   }
   if(y + y_change > y_size || x + x_change > x_size || y + y_change < 0 || x + x_change < 0){
       return 0;
   }
   //CHECKS that either the Tile matches the connected tiles colour or shape
   if(map[y + y_change][x + x_change]->getShape() == tile->getShape() || map[y + y_change][x + x_change]->getColour() == tile->getColour()){
       int colourScore = straightColourLine(y, x, x_change, y_change, 0, 0, tile->getColour());
       int shapeScore = straightShapeLine(y, x, x_change, y_change, 0, 0, tile->getShape());
       if(colourScore != -1 && shapeScore != -1){
           if(colourScore > 1 && shapeScore > 1){//scoring rules
               return colourScore + shapeScore;
           }
            if(colourScore >= shapeScore){
                return colourScore;
            }
            return shapeScore;
       }
       return -1;
   }
    return -1;
}

int Board::straightColourLine(int y_start, int x_start, int x_change, int y_change, int length, int score, Colour colour){
    if(length > 5){ //checks to see if the line is larger then 6
        return -1;
    }
    if(length == 5){ // if length equals 6 the user gets a qwirkle
        score += 6;
         std::cout << "QWIRKLE!!!";
    }
      if(y_start + y_change > y_size || x_start + x_change > x_size){
       return score;
   }
   if(y_start + y_change < 0 || x_start + x_change < 0){
       return score;
   }
   //recursion when a tile is in the next location to check if it is valid and add to increase the score
    if(nullCheck(y_start + y_change, x_start + x_change) == false){ //CHECKS if tile is in this direction
        if(map[y_start + y_change][x_start + x_change]->getColour() == colour){ //checks if the tile is valid
            if(y_change > 0){ // Up LINE
                score++;
                length++;
                y_change++;
                return straightColourLine(y_start, x_start, x_change, y_change, length, score, colour);
            }
            if(y_change < 0){ //DOWN LINE
                score++;
                length++;
                y_change--;
                return straightColourLine(y_start, x_start, x_change, y_change, length, score, colour);
            }
            if(x_change > 0){ // RIGHT LINE
                score++;
                length++;
                x_change++;
                return straightColourLine(y_start, x_start, x_change, y_change, length, score, colour);
            }
            if(x_change < 0){ // LEFT LINE
                score++;
                length++;
                x_change--;
                return straightColourLine(y_start, x_start, x_change, y_change, length, score, colour);
            }
    }
     if(length == 1 && score == 1){ //duplicate tile
         return 0;
     }
     if(length > 0){
         return -1;
     }

    }
    return score;
}
//same as straightColourLine buy checks if the shape is matching
int Board::straightShapeLine(int y_start, int x_start, int x_change, int y_change, int length, int score, Shape shape){
     if(length > 5){
        return -1;
    }
    if(length == 5){
        score += 6;
         std::cout << "QWIRKLE!!!";
    }
      if(y_start + y_change > y_size || x_start + x_change > x_size){
       return score;
   }
   if(y_start + y_change < 0 || x_start + x_change < 0){
       return score;
   }
    if(nullCheck(y_start + y_change, x_start + x_change) == false){
        if(map[y_start + y_change][x_start + x_change]->getShape() == shape){
            if(y_change > 0){ //UP LINE
                score++;
                length++;
                y_change++;
                return straightShapeLine(y_start, x_start, x_change, y_change, length, score, shape);
            }
            if(y_change < 0){ //DOWN LINE
                score++;
                length++;
                y_change--;
                return straightShapeLine(y_start, x_start, x_change, y_change, length, score, shape);
            }
            if(x_change > 0){ // RIGHT LINE
                score++;
                length++;
                x_change++;
                return straightShapeLine(y_start, x_start, x_change, y_change, length, score, shape);
            }
            if(x_change < 0){ //LEFT LINE
                score++;
                length++;
                x_change--;
                return straightShapeLine(y_start, x_start, x_change, y_change, length, score, shape);
            }
    }
    if(length == 1 && score == 1){ //duplicate tile
         return 0;
     }
     if(length > 0){
         return -1;
     }
    }
    return score;
}

bool Board::nullCheck(int y, int x){
    if(map[y][x] == NULL){
        return true;
    }
    return false;
}

int Board::getX(){
    return x_size;
}

int Board::getY(){
    return y_size;
}

int Board::getIndexOfChar(char ch) {
  int index = 0;
  for(int i = 0; i < 26; i++) {
    if (letters[i].at(0) == ch) {
      index = i;
    }
  }

  return index;
}
void Board::firstTile(Tile* tile){
    int x = (x_size - 1) /2;
    int y = (y_size - 1) /2;
    map[y][x] = tile;
}

std::string Board::toString(){
    std::string text = "  ";
    for(int top = 0; top < x_size;top++){
        text += "  " + numbers[top];
    }

    for(int y = 0; y < y_size;y++){
        for(int x = 0; x < x_size;x++){
            if(x == 0 ){
                text += "\n" + letters[y] + " |";
            }
            if(map[y][x] == NULL){
                    text += "  |";
            }
            else{
                text += map[y][x]->toString() + "|";
            }
        }
    }

    return text;
}

std::string Board::toSaveString(){
    std::string text = "  ";
    for(int top = 0; top < x_size;top++){
        text += "  " + numbers[top];
    }

    for(int y = 0; y < y_size;y++){
        for(int x = 0; x < x_size;x++){
            if(x == 0 ){
                text += "\n" + letters[y] + " |";
            }
            if(map[y][x] == NULL){
                    text += "  |";
            }
            else{
                text += map[y][x]->toSaveString() + "|";
            }
        }
    }

    return text;
}
