#include <MD_MAX72xx.h>
#include <SPI.h>

// display pins
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW   // Set display type  so that  MD_MAX72xx library treets it properly
#define MAX_DEVICES  4    // Total number display modules
#define CLK_PIN   13      // Clock pin to communicate with display
#define DATA_PIN  11      // Data pin to communicate with display
#define CS_PIN    3       // Control pin to communicate with display
#define DEBUG 0

// display dimensions
#define MAX_Y 8
#define MAX_X 32

// init game board so we can easily input a starting pattern
int board[MAX_Y][MAX_X] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};


MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);   // display object

void setup() {
  mx.begin();             // initialize display
  mx.control(MD_MAX72XX::INTENSITY, 0);     // SET DISPLAY brightness
  Serial.begin(9600);     // open the serial port at 9600 bps:

  // get noise as long as analog 1 is not in use
  randomSeed(analogRead(1));

  //  initBoard(board, 1);
}


void loop() {
  delay(1000);
  printBoard();
  displayBoard();
  gameOfLife();

  //  printBoardValues();

}


void gameOfLife() {

  // create a new board

  int nextBoard[MAX_Y][MAX_X] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  };


  int count = 0;
  // calculate the state of the next board
  //  Serial.println("printing neighbor count");
  for (int y = 0; y < MAX_Y; y++) {
    for (int x = 0; x < MAX_X; x++) {
//      count = my_count_neighbors(y, x);
      count = getNeighbors(y, x);
      // Any live cell with two or three live neighbours survives
      if (board[y][x] == 1 && (count == 2 || count == 3)) {
        nextBoard[y][x] = 1;
      }
      // Any dead cell with three live neighbours becomes a live cell.
      else if (board[y][x] == 0 && (count == 3)) {
        nextBoard[y][x] = 1;
      }
      // All other live cells die in the next generation. Similarly, all other dead cells stay dead.
      else {
        nextBoard[y][x] = 0;
      }
    }
  }
  // update the board
  copyBoard(nextBoard, board);
}

// copy the src board to the destination board
void copyBoard(int src[MAX_Y][MAX_X], int dest[MAX_Y][MAX_X]) {
  for (int y = 0; y < MAX_Y; y++) {
    for (int x = 0; x < MAX_X; x++) {
      dest[y][x] = src[y][x];
    }
  }
}


int my_count_neighbors(int y, int x) {
  int count = 0;

  // above --------------------------------------------------------
  // above left
  if (y > 0 && x > 0) count += board[y - 1][x - 1];
  // above
  if (y > 0) count += board[y - 1][x];
  // above right
  if ((x + 1) < MAX_Y) count += board[y - 1][x + 1];
  
  // current row --------------------------------------------------
  // left
  if (x > 0) count += board[y][x - 1];
  // right
  if ((x + 1) < MAX_Y) count += board[y][x + 1];
  
  // below --------------------------------------------------------
  // below left
  if ((y + 1) < MAX_Y && x > 0) count += board[y + 1][x - 1];
  // below
  if ((y + 1) < MAX_Y) count += board[y + 1][x];
  // below right
  if ((x + 1) < MAX_Y) count += board[y + 1][x + 1];



  return count;
}


// TODO this almost works, but fails to return correct count 
int getNeighbors(int y, int x) {
  int count = 0;
  // perp and vertical
  for (int j = -1; j < 2; j++) {
    for (int i = -1; i < 2; i++) {
      // don't count self
      if (i == 0 && j == 0) continue;
      // else count everything in bounds
      if ((y + j >= 0) && (y + j < MAX_Y) && (x + i >= 0) && (x + i < MAX_X)) {
          count += board[y+j][x+i];
      }
    }
  }
  return count;
}


int count_neighbors(int y, int x) {
  int count = 0;

  // -- Row above us ---
  if (y > 0) {
    // above left
    if (x > 0) {
      count += board[y - 1][x - 1];
    }
    // above
    count += board[y - 1][x];
    // above right
    if ((x + 1) < MAX_Y) {
      count += board[y - 1][x + 1];
    }
  }

  // -- Same row -------
  // left
  if (x > 0) {
    count += board[y][x - 1];
  }
  // right
  if ((x + 1) < MAX_Y) {
    count += board[y][x + 1];
  }

  // -- Row below us ---
  if ((y + 1) < MAX_Y) {
    // below left
    if (x > 0) {
      count += board[y + 1][x - 1];
    }
    // below
    count += board[y + 1][x];
    // below right
    if ((x + 1) < MAX_Y) {
      count += board[y + 1][x + 1];
    }
  }
  return count;
}



void printBoardValues() {
  for (int y = 0; y < MAX_Y; y++) {
    for (int x = 0; x < MAX_X; x++) {
      Serial.print(y);
      Serial.print(":");
      Serial.print(x);
      Serial.print(" = ");
      Serial.println(board[y][x]);
    }
  }
}



void displayBoard() {
  for (int y = 0; y < MAX_Y; y++) {
    for (int x = 0; x < MAX_X; x++) {
      mx.setPoint(y, x, board[y][x]);
    }
  }
}

// debug board
void printBoard() {
  for (int y = 0; y < MAX_Y; y++) {
    for (int x = 0; x < MAX_X; x++) {
      Serial.print(board[y][x]);
      Serial.print(" ");
    }
    Serial.println("");
  }
  Serial.println("##########################################################################################");
}


void initBoard(int board[MAX_Y][MAX_X], int value) {
  for (int y = 0; y < MAX_Y; y++) {
    for (int x = 0; x < MAX_X; x++) {
      board[y][x] = value;
    }
  }
}


// standalone method that can be called in loop
void randomDisplay() {
  set_random_point(1, 1);
  set_random_point(0, 2);
  delay(100);
}

void set_random_point(int state, int repeat) {
  uint8_t max_y = 0;
  uint16_t max_x = 0;
  for (int i = 0; i < repeat; i++) {
    max_y = random(0, 8);
    max_x = random(0, 32);
    mx.setPoint(max_y, max_x, state);
  }
}

//void printNeighbors(int x, int y, int count) {
//  Serial.print("HEY");
//  printit("i = ", i);
//  printit("j = ", j);
//  printLine(": ", count);
//}

void printLine(char* str, int value) {
  if (DEBUG) {
    Serial.print(str);
    Serial.println(value);
  }
}


void printit(char* str, int value) {
  if (DEBUG) {
    Serial.print(str);
    Serial.print(value);
  }
}
