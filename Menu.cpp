#include "Menu.h"



Menu::Menu(){
    std::cout << "WELCOME TO QWIRKLE!\n-------------------" << std::endl;
    std::cout << "Enter 'help' at any time for a list of commands\n-------------------" << std::endl;
}

Menu::~Menu(){
}

//-------- Start the Menu ---------//
void Menu::startMenu(){
    printMenu();
    int sel = getMenuSelection();
    executeOption(sel);
}

//-------- Display Menu ---------//
void Menu::printMenu(){
    std::cout << "Menu\n----" << std::endl;
    std::cout << "1. New Game" << std::endl;
    std::cout << "2. Load Game" << std::endl;
    std::cout << "3. Show Student Information" << std::endl;
    std::cout << "4. Quit" << std::endl;
}

//-------- Initial Menu Selection ---------//
int Menu::getMenuSelection(){
    std::string selectionString = "";
    int selectionInt = 0;
    bool validSelection = false;

    /* Loops until the user makes a valid menu choice */
    while(!validSelection){         
        selectionString = getInputStr();

        if (std::cin.fail()){
            std::cout << "Error" << std::endl;
        }

        /* Help command */
        if((selectionString == "help") || (selectionString == "HELP")){
            std::cout << "Select a menu option from 1-4 and press Enter." << std::endl;
        }

        /* Try to interpret input at integer selection */
        else try{
            selectionInt = std::stoi(selectionString);
            if (selectionInt < 1 || selectionInt > 4) {
                    std::cout << "Error -- Please enter a valid menu item." << std::endl;
            }
            else{
                validSelection = true;
            }
        }
        catch(...){
            std::cout << "Error -- Please enter an integer" << std::endl;
        }
        std::cin.clear();           // clear error flag
        std::cin.ignore();
    }
    return selectionInt;
}


//-------- Executing Menu Choice ---------//
void Menu::executeOption(int selection){
    if (selection == 1){
        newGame();
    }

    else if (selection == 2){
        loadGame();
    }

    else if (selection == 3){
        showStudents();
    }

    else{
        quitGame();
    }
}


//-------- New Game Initialization ---------//
void Menu::newGame(){
    std::cout << "Starting a New Game" << std::endl;
    std::cout << "Enter Player Amount" << '\n';

    while (this->numPlayers == 0) {
      try {
        this->numPlayers = std::stoi(getInputStr());
      }
    catch (...) {
      std::cout << "Invalid Input..." << '\n';
      this->numPlayers = 0;
      }
    }

    std::cout << "Enter Board Size" << '\n';

    while (this->boardSize == 0) {
      try {
        this->boardSize = std::stoi(getInputStr());
      }
    catch (...) {
      std::cout << "Invalid Input..." << '\n';
      this->boardSize = 0;
      }
    }

    for(int i = 0; i < this->numPlayers; i++) {
      std::cout << "Player " << i + 1 << ", enter your name (uppercase characters only): " << std::endl;
      players[i] = new Player(getPlayerName(),0);
    }

    std::cout << "\nLet's Play!" << std::endl;

    //Initalizes the bag and board
    this->bag = new Bag();
    this->board = new Board(this->boardSize,this->boardSize);
    this->bag->fillBag();

    //Places the first tile
    board->firstTile(bag->getTile());

    //Players draw the first tile
    for(int i = 0; i < this->numPlayers; i++) {
        players[i]->Draw(bag, 6);
    }
    gameplayLoop();
}

//-------- Main Gameplay Function ---------//
void Menu::gameplayLoop() {
  bool loop = true;
  bool bagIsEmpty = false;
  bool playersHaveTiles = true;
  //The gameplay loop
  while(loop) {

    for(int i = 0; i < numPlayers; i++) {

      bool loopPlayer = true;


      while (loopPlayer) {
        std::string response = "";
        bool hasPlacedTile = false;
        bool hasReplacedTile = false;
        Tile* tileToPlace = nullptr;
        int yPos = 0;
        int xPos = 0;
        int score = 0;

        //Gets the player command
        while (response != "place" && !(response == "replace" && bag->getLength() > 0) && response != "tilehint") {
          std::cout << players[i]->getName() << "'s turn" << '\n';
          std::cout << "Score for " << players[i]->getName() << ": " << players[i]->getScore() << '\n';
          std::cout << "Your hand is:" << '\n';
          std::cout << players[i]->handToString() << '\n';
          std::cout << board->toString() << '\n';

          response = getGameloopInputStr();
          std::cout << response << '\n';
        }

        //Prompts the player to select the tile he wants to replace
        if (response == "replace" && bag->getLength() > 0) {
          Tile* tiletoReplace = NULL;
          while (tiletoReplace == NULL) {
            std::cout << "Chose tile to replace..." << '\n';
            response = getGameloopInputStr();

            if (response.size() == 2) {
              char color = response.at(0);
              int shape = response.at(1) - '0';
              //Checks if the player has the tile he wants to replace
              tiletoReplace = players[i]->hasTile(color, shape);
            }

            if (tiletoReplace == NULL) {
              std::cout << "Invalid Input..." << '\n';
            }
          }

          //Deletes the tile he wants to replace
          std::cout << tiletoReplace->getColour() << '\n';
          players[i]->deleteTile(tiletoReplace->getColour(), tiletoReplace->getShape());
          //draws a new tile
          if (bag->getLength() > 0) {
            players[i]->Draw(bag, 1);

          }
          hasReplacedTile = true;
          hasPlacedTile = true;
        }
        //Gets a hint on where to place a tile
        else if (response == "tilehint") {
          Tile* tileHint = NULL;
          while (tileHint == NULL) {
            std::cout << "Chose tile..." << '\n';
            response = getGameloopInputStr();

            if (response.size() == 2) {
              char color = response.at(0);
              int shape = response.at(1) - '0';
              //Checks if the player has the tile he wants to replace
              tileHint = players[i]->hasTile(color, shape);
            }

            if (tileHint == NULL) {
              std::cout << "Invalid Input..." << '\n';
            }
          }

          board->tileHint(tileHint);
          hasPlacedTile = true;
        }

        //Prompts the player to chose which tile to place
        while (tileToPlace == nullptr && !hasPlacedTile && players[i]->hasTilesLeft()) {
          std::cout << "Chose tile to place..." << '\n';
          response = getGameloopInputStr();

          if (response.size() == 2) {
            char color = response.at(0);
            int shape = response.at(1) - '0';

            //Validates that the player has the tile
            tileToPlace = players[i]->hasTile(color, shape);
            //Deletes the tile after use
            //Draws a new tile if bag has tiles


          }
          else if (response.size() == 3) {
            yPos = board->getIndexOfChar(response.at(0));

            std::string x1(1, response.at(1));
            std::string x2(1, response.at(2));
            std::string x3 = x1 + x2;
            xPos = std::stoi(x3);
            std::cout << xPos << '\n';
            score = board->placeTile(xPos - 1, yPos, tileToPlace);

          }

          if (tileToPlace == nullptr) {
            std::cout << "Invalid Input..." << '\n';
          }

        }

        while (!hasPlacedTile && players[i]->hasTilesLeft()) {

          //Prompts the player to give the position he wants to place the tile on
          std::cout << "Chose position..." << '\n';
          response = getGameloopInputStr();

          if (response.size() == 2) {
            //Gets the tile position
            yPos = board->getIndexOfChar(response.at(0));
            xPos = response.at(1) - '0';
            score = board->placeTile(xPos - 1, yPos, tileToPlace);
          }
          else if (response.size() == 3) {
          yPos = board->getIndexOfChar(response.at(0));

          std::string x1(1, response.at(1));
          std::string x2(1, response.at(2));
          std::string x3 = x1 + x2;
          xPos = std::stoi(x3);
          std::cout << xPos << '\n';
          score = board->placeTile(xPos - 1, yPos, tileToPlace);
        }

          //Gives score if placement is valid
          if (score > 0) {
            players[i]->deleteTile(tileToPlace->getColour(), tileToPlace->getShape());

            if (bag->getLength() > 0) {
              players[i]->Draw(bag, 1);
            }
            hasPlacedTile = true;
            players[i]->updateScore(score);
            loopPlayer = false;
          }
          else if (score < 0) {
            hasPlacedTile = true;
          }

        }

        if (hasReplacedTile) {
          std::cout << "hasReplaced" << '\n';
          loopPlayer = false;
        }

        if (players[i]->hasTilesLeft() == false) {
          playersHaveTiles = false;
        }
      }

      if (bag->getLength() == 0) {
        bagIsEmpty = true;
      }

      if (bagIsEmpty && !playersHaveTiles) {
        std::cout << "Game Over!" << '\n';

        int winnerIndex = 0;
        int maxScore = 0;

        for(int y = 0; y < numPlayers; y++) {
          if (players[y]->getScore() > maxScore) {
            winnerIndex = y;
          }
          std::cout << "Score for " << players[y]->getName()<<  ": " << players[y]->getScore() << '\n';
        }

        std::cout << "Player " << players[winnerIndex]->getName() << " Won!" << '\n';
        return;
      }
      }

  }
}

//-------- Get a user input as a name and validate ---------//
std::string Menu::getPlayerName(){
    bool validName = false;
    std::string name = "";

    /* Loops until a valid name is entered */
    while(!validName){ 
        name = getInputStr();

        if (std::cin.fail()){
            std::cout << "Error" << std::endl;
        }

        for (unsigned int i = 0; i < name.length(); i++) {
            if (std::isalpha(static_cast<unsigned char>(name[i])) && std::isupper(static_cast<unsigned char>(name[i]))){
                validName = true;
            }

            else{
                validName = false;
            }
        }

        if(!validName){
            std::cout << "Error -- Ensure you only enter uppercase characters" << std::endl;
        }
        std::cin.clear();           // clear error flag
        std::cin.ignore();
    }
    return name;
}


//-------- Load Game ---------//
void Menu::loadGame(){
    bool validFormat = true;
    std::cout << "Please enter the path to the Save Game file" << std::endl;
    std::ifstream savedGame;
    std::string saveLocation = getInputStr();
    savedGame.open(saveLocation);

    /* check to see if save exists */
    if (savedGame.fail()) {
        std::cout << "Error -- File does not exist or is inaccessible" << std::endl;
    }
    savedGame.clear();      //clear error flag

    std::string numPlayers = "";
    std::getline(savedGame, numPlayers);
    numPlayers = deleteReturnChar(numPlayers);
    this->numPlayers = stoi(numPlayers);
    std::string boardSize = "";
    std::getline(savedGame, boardSize);
    boardSize = deleteReturnChar(boardSize);
    this->boardSize = stoi(boardSize);

    std::vector<std::string> playerNames;
    std::vector<int> playerScores;
    std::vector<std::vector<Tile*>> playerHands;
    std::string tile = "";
    char color = 0;
    int shape = 0;

    /* Loading Players */
    for (int i = 0; i < this->numPlayers; i++){
        /* Loading Names */
        std::string playerName = "";
        std::getline(savedGame, playerName, '\n');
        playerName = deleteReturnChar(playerName);
        for (unsigned int i = 0; i < playerName.length(); i++) {
            if (!std::isalpha(static_cast<unsigned char>(playerName[i])) || !std::isupper(static_cast<unsigned char>(playerName[i]))){
                validFormat = false;
            }
        }
        /* Loading Scores */
        std::string playerScore = "";
        std::getline(savedGame, playerScore);
        playerScore = deleteReturnChar(playerScore);
        for (unsigned int i = 0; i < playerScore.length(); i++) {
            if (!std::isdigit(playerScore[i])){
                validFormat = false;
            }
        }
        /* Loading Hands */
        std::string playerHandLine = "";
        std::getline(savedGame, playerHandLine, '\n');
        playerHandLine = deleteReturnChar(playerHandLine);
        std::stringstream handStream(playerHandLine);
        tile = "";
        std::vector<Tile*> tileVector;
        while(getline(handStream, tile,','))
        {
            if (!(tile.length() == 2) || !(tile[0] == 'R' || tile[0] == 'O' || tile[0] == 'Y' || tile[0] == 'G' || tile[0] == 'B' || tile[0] == 'P') || !(tile[1] == '1' || tile[1] == '2' || tile[1] == '3' || tile[1] == '4' || tile[1] == '5' || tile[1] == '6')){
                validFormat = false;
            }
            else{
                color = tile.at(0);
                shape = tile.at(1) - '0';
                tileVector.push_back(new Tile(color,shape));
            }
        }

        if(validFormat){
            playerNames.push_back(playerName);
            playerScores.push_back(stoi(playerScore));
            playerHands.push_back(tileVector);
        }
    }

    /* Loading Board */
    std::string boardLine = "";
    std::getline(savedGame, boardLine, '\n');
    boardLine = deleteReturnChar(boardLine);
    tile = "";
    color = 0;
    shape = 0;
    this->board = new Board(this->boardSize,this->boardSize);               //ToDo store board load in a temporary value and don't store until validation passes
    for (int i = 0; i < (int)this->boardSize; i++){
        std::getline(savedGame, boardLine, '\n');
        boardLine = deleteReturnChar(boardLine);
        std::stringstream boardStream(boardLine);
        for (int j = 0; j < (int)this->boardSize + 1; j++){
            getline(boardStream, tile, '|');
            if (!(tile.length() == 2) || !(tile.at(0) == 'R' || tile.at(0) == 'O' || tile.at(0) == 'Y' || tile.at(0) == 'G' || tile.at(0) == 'B' || tile.at(0) == 'P') || !(tile.at(1) == '1' || tile.at(1) == '2' || tile.at(1) == '3' || tile.at(1) == '4' || tile.at(1) == '5' || tile.at(1) == '6')){
                //validFormat = false;
            }
            else{
                //std::cout << tile << std::endl;
                color = tile.at(0);
                shape = tile.at(1) - '0';
                this->board->loadTile(j-1,i, new Tile(color, shape));
            }
        }
    }


    /* Loading Bag */
    std::string bagLine = "";
    std::getline(savedGame, bagLine, '\n');
    bagLine = deleteReturnChar(bagLine);
    std::stringstream bagStream(bagLine);
    tile = "";
    color = 0;
    shape = 0;
    std::vector<Tile*> bagVector;
    while(getline(bagStream, tile,','))
    {
        if (!(tile.length() == 2) || !(tile.at(0) == 'R' || tile.at(0) == 'O' || tile.at(0) == 'Y' || tile.at(0) == 'G' || tile.at(0) == 'B' || tile.at(0) == 'P') || !(tile.at(1) == '1' || tile.at(1) == '2' || tile.at(1) == '3' || tile.at(1) == '4' || tile.at(1) == '5' || tile.at(1) == '6')){
            validFormat = false;
        }
        else{
            color = tile.at(0);
            shape = tile.at(1) - '0';
            bagVector.push_back(new Tile(color, shape));
        }
    }


    /* Loading Current Player */
    std::string currentPlayer = "";
    std::getline(savedGame, currentPlayer, '\n');
    currentPlayer = deleteReturnChar(currentPlayer);

    if (validFormat){
        //validation success, set all the variable and start the game

        /* Set the Current Player */
        std::string tempName = "";
        int tempScore = 0;
        std::vector<Tile*> tempHand;
        for (int i = 0; i < this->numPlayers; i++){
            if (playerNames[i] == currentPlayer){
                tempName = playerNames[i];
                tempScore = playerScores[i];
                tempHand = playerHands[i];
                playerNames[i] = playerNames[0];
                playerScores[i] = playerScores[0];
                playerHands[i] = playerHands[0];
                playerNames[0] = tempName;
                playerScores[0] = tempScore;
                playerHands[0] = tempHand;
            }
        }

        /* Set all the Players */
        for (int i = 0; i < this->numPlayers; i++){
            this->players[i] = new Player(playerNames[i],playerScores[i]);
            for (unsigned int j = 0; j < playerHands[i].size(); j++){
                this->players[i]->addToHand(playerHands[i][j]);
            }
        }
        /* ToDo Move actual Board initialization down here */
            //Board* board = new Board(boardSize,boardSize);
            //Loop to populate board

        /* Set the Bag */
        this->bag = new Bag();
        for (unsigned int i = 0; i < bagVector.size(); i++){
            this->bag->addToBag(bagVector[i]);
        }
        savedGame.clear();
        savedGame.close();

        /* Start Game*/
        gameplayLoop();
    }
    /* If the format is incorrect */
    else{
        std::cout << "Error -- Loading Game File" << std::endl;
        savedGame.clear();
        savedGame.close();
        quitGame();
    }
}



//-------- Print Team Members ---------//
void Menu::showStudents(){
    std::cout << "-------------------\n" << std::endl;
    for(int i = 0; i < 3; i++){
        std::cout << "Name: " << this->students[i][0] <<std::endl;
        std::cout << "Student ID: " << this->students[i][1]<< std::endl;
        std::cout << "Email: "<< this->students[i][2] << "\n" << std::endl;
    }

    std::cout << "-------------------\n Press enter to return to Menu..." << std::endl;
    std::cin.ignore(100, '\n');         // For the press enter to continue effect
    startMenu();
}


//-------- Quit ---------//
void Menu::quitGame(){
    std::cout << "Goodbye!" << std::endl;
    std::exit(0);
}

//-------- Get a string input (and check for EoF). Error handling is done where this is called ---------//
std::string Menu::getInputStr(){
    std::cout << "> ";      //User Input Prompt

    std::string inputVar = "";

    std::cin >> inputVar;

    if (std::cin.eof()){
        quitGame();         //quit if EoF is entered
    }
    return inputVar;
}

//Getting input for the gameplay loop, has to be different so the players dont save the game when in the main menu//
std::string Menu::getGameloopInputStr() {
    std::string response = getInputStr();

    //-------- Saving ---------//
    if (response == "save" || response == "SAVE" ){
        std::cout << "Enter Filename" << std::endl;
        std::string filename = getInputStr();
        std::ofstream saveGame;
        saveGame.open(filename);
        saveGame << this->numPlayers << std::endl;
        saveGame << this->boardSize << std::endl;
        for (int i = 0; i < this->numPlayers; i++){
            saveGame << this->players[i]->getName() << std::endl;
            saveGame << this->players[i]->getScore() << std::endl;
            saveGame << this->players[i]->handToSaveString() << std::endl;
        }
        saveGame << this->board->toSaveString() << std::endl;
        saveGame << this->bag->bagToSaveString() << std::endl;
        saveGame << this->currentPlayer << std::endl;
        saveGame.close();
    }

    //-------- Help Commands ---------//
    if (response == "help" || response == "HELP" ){
        std::cout << "-------------------\n Commands:"<< std::endl;
        std::cout << "<place> | Use to place a tile on the board. E.g place R4 at B2"<< std::endl;
        std::cout << "<replace> | Use to replace a tile in your hand with a new tile from the bag. E.g replace R4"<< std::endl;
        std::cout << "<tilehint> | Get a helping hint telling you valid tile placements. E.g tilehint R4"<< std::endl;
        std::cout << "<help> | Displays this handy list of commands!"<< std::endl;
        
    }
    return response;
}

//-------- Function to strip return characters from strings ---------//
std::string Menu::deleteReturnChar(std::string string){

   std::string returnString = "";

   for(unsigned int i=0; i<string.length(); i++){
      if(string[i] != '\r'){
         returnString += string[i];
      }
   }
   return returnString;
}
