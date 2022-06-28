/*
    Developed by : Giovanni Tshibangu & Dejah Murray 
    BB Team : 1
    Project name: Snake Part1
    Date: 6/28/2022

Part objective:
    Intermediate deliverable (due 06/30/2022) needs to have the following functionality:
    * The game starts with the snake of size 5 moving right;
    * Snake movement can be controlled in all directions;
    * Snake does not grow;
    * Snake pit border is visible;
    No trophies.

*/
// Libraies needed for this project
#include <curses.h> 
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>   

// Define const settings for the gameplay
#define WIDTH 70
#define HEIGHT 30
#define SPEED 300000
#define LENGTH 5 

/*  Created a snake Struct 
 This is the snake object   */
struct Snake{
    int x,y;    // Snake position 
    struct Snake *next;
};

// Enum directions. Use the arrow keys from keyboard to guide the snake
enum Direction { LEFT, RIGHT, UP, DOWN };

// Global variables
int speed, snakeLength, numNewBodies;
float speedRate = 0.98;
bool gameOver = false, exitGame = false;
enum Direction currDir;
struct Snake *head;

// Declaring all my function Prototypes 
void buildGame();       // This function set the game and init snake position ect...
void updateDirection(int);      // Change the snake position based on the user input.
void guideSnake();      // More snake control, such as head movement and collision hanlder
void buildScreen();     // Build the pit and overall Gameplay Area
void gameOverMessageScreen();  // This function is called when you kill the snake.

// ################ MAIN FUNCTION #################

int main(void){
    buildGame();        // Start by building the game first.

    while(!exitGame){
        if(gameOver){
            gameOverMessageScreen("YOU LOSE !!!!!... \n");
        }
        int keypress = getch();
        updateDirection(keypress);
        guideSnake();
        buildScreen();
  
	    usleep(speed);      // add delay between movement, speed is proportional to its length.
                            // The longer the snake, the faster it goes 
    }
    endwin();    // end the curses
    return 0;   
}
// ################ buildGame FUNCTION #################
/*
 Function : buildGame()
 Name: Giovanni Tshibangu.
 functionality  : This function set the game and init snake position and Direction ect...
*/
void buildGame(){
    clear();    // clear the screen/ terminal

    // Initialize 
    initscr();  // start curses or init curse
    keypad(stdscr, TRUE);   // let us use arrow keys from keyboard
    timeout(0);     // dont stop. this prevent getchar from waiting for input

    noecho();
    curs_set(0);    // disable cursor, So it does not interfer with gamepaly

    raw();  //disables line buffering and erase/kill character-processing 

    // Initialise Snake 
    currDir =RIGHT;     // Start the snake movement to  RIGHT
    gameOver = false;   // Game is not Over. This is because there is a while loop that's checking
    speed = SPEED;      // Set the initial speed to the defined constant.
    snakeLength = LENGTH;   // Set the initial LENGTH to the defined constant

    // Snake starts at the middle of the screen 
    int x = WIDTH/2;
    int y = HEIGHT/2;

    // Create the snake, This snake is basically a Linked LIST
    struct Snake *currBody;
    currBody = (struct Snake*) malloc(sizeof(struct Snake));
    currBody->next = NULL;
    currBody->x = x - (LENGTH - 1);
    currBody->y = y;

    for (int i = LENGTH - 2; i >= 0; i--) {
        head = (struct Snake*) malloc(sizeof(struct Snake));
        head->next = currBody;
        head->x = x - i;
        head->y = y;
        currBody = head;    
    }
}

// ################ updateDirection FUNCTION #################
/*
 Function : updateDirection()
 Name: 
 functionality  : This function takes in the user key inputs to update the snake position 
*/
void updateDirection(int keypress){
    // Switch case based on the keypressed by the user..
    switch (keypress) {     
        case KEY_UP:    
            if(currDir ==DOWN){
                // reverse
                gameOverMessageScreen("You lose! Snakes do not go BACKWARDS...\n");
                break;
            }
            currDir = UP;
            break;

        case KEY_DOWN:
            if(currDir == UP){
                    // reverse
                    gameOverMessageScreen("You lose! Snakes do not go BACKWARDS...\n");
                    break;
                }
                currDir = DOWN;
                break;

        case KEY_LEFT:
            if(currDir == RIGHT){
                // reverse
                    gameOverMessageScreen("You lose! Snakes do not go BACKWARDS...\n");
                    break;
            }
            currDir =LEFT;
            break;
        
        case KEY_RIGHT:
            if(currDir ==LEFT){
                 // reverse
                    gameOverMessageScreen("You lose! Snakes do not go BACKWARDS...\n");
                    break;
            }
            currDir = RIGHT;
            break;
            // ENd the game if X or x  key is pressed.
            case 'x':
            case 'X':
                exitGame = true;
    }
}
// ################ guideSnake FUNCTION #################
/*
 Function : guideSnake()
 Name: Giovanni Tshibangu
 functionality  : More snake control, such as head movement and collision hanlder 
*/
void guideSnake(){
     // new snake head
     struct Snake *newHead;
     newHead = (struct Snake*) malloc(sizeof(struct Snake));
     newHead->next = head;
     newHead->x = head->x;
     newHead->y = head->y;
     head = newHead;   // Set head to newHead

    // Snake head movement 
     if(currDir == UP)  head->y--;
     else if(currDir == DOWN)  head->y++;
     else if(currDir == LEFT)  head->x--;
     else if(currDir == RIGHT) head->x++;

    // End the game if the snake hit the walls
        
    if (head->x >= WIDTH || head->x <= 0 || head->y >= HEIGHT || head->y <= 0) {
        /* gamover, this will end the game. 
        The while loop checking for it will break out of the loop once value is eqaul to 1*/
        gameOver = true;    
        gameOverMessageScreen("Game over!  You hit a wall... Try not to next time. \n");
    }
    // Game ends when the snake hit itself
    struct Snake *currBody;
    currBody = head;
    while (currBody->next->next != NULL) {
        // if collide with body, gameover
        if (currBody->next->x == head->x && currBody->next->y == head->y) {
            /* gamover, this will end the game. 
            The while loop checking for it will break out of the loop once value is eqaul to 1*/
            gameOver = true;
            gameOverMessageScreen("You lose! Stop Trying to eat Yourself!!!...\n");
        }
        currBody = currBody->next;
    }

    // if there is no new body left to add, remove tail
    if (numNewBodies <= 0) {
        mvaddch(currBody->next->y, currBody->next->x, ' '); // clear that body from screen
        free(currBody->next);
        currBody->next = NULL;
    }
    else {
        numNewBodies--;
        speed = (int)(speed * speedRate);
    }
     }

// ################ gameOverMessageScreen FUNCTION #################
/*
 Function : buildScreen()
 Name: Giovanni Tshibangu
 functionality  :Build the pit and overall Gameplay Area 
*/

void gameOverMessageScreen(char* result) {
    clear();    // clear terminal 
    mvprintw(10, 50, result);    // print the scoreboard
    mvprintw(12, 42, "Game Over! Hit any key to close game");   // print text to terminal
    refresh();
    sleep(4);   // Wait 4 second
    getchar();  // get any key input from user keyboard to Quit the Screen
    endwin();   // end 
    exit(0);    // QUIT the program or KILL IT
}

// ################ buildScreen FUNCTION #################
/*
 Function : buildScreen()
 Name: Giovanni Tshibangu
 functionality  :Build the pit and overall Gameplay Area 
*/
void buildScreen() {
    // Draw Border
    move(0, 0); // move cursor to the uppder left corner so border will be draw in the right position
    for (int i = 0; i <= HEIGHT; i++) {
        for (int j = 0; j <= WIDTH; j++) {
            // only print hashtages for firsts and lasts, which creates a border
            if (i == 0 || i == HEIGHT || j == 0 || j == WIDTH)
                mvaddch(i, j, '#'); // The border is made of '#' Symbol
        }
        addch('\n');    // add new line
    }
    // Draw Snake
    struct Snake *currBody;
    currBody = head;
    mvaddch(currBody->y, currBody->x, 'O');
    while (currBody->next != NULL) {
        mvaddch(currBody->next->y, currBody->next->x, 'O');
        currBody = currBody->next;
    }
    // game progress
    // User Wins when The user wins the game if the snake's length grows to the length equal to half the perimeter of the border.
    float halfPerimeter = HEIGHT + WIDTH;
    float length = snakeLength;
    float progress = (length / halfPerimeter) * 100;
    mvprintw(4, WIDTH + 2, "Progress: %d %%", (int)progress);
    if ((int)progress >= 100) {
        gameOverMessageScreen("You won!!!\n");     // User Won!!
    }
   // render
    move(HEIGHT + 1, 0); // move cursor out of the game screen 
    refresh(); // render screen, screen buffer -> real screen
}
