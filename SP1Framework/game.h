#ifndef _GAME_H
#define _GAME_H

#include "Framework\timer.h"
#include "extract.h"

extern CStopWatch g_swTimer;
extern bool g_bQuitGame;

// Enumeration to store the control keys that your game will have
enum EKEYS
{
	PLAYER_1_K_UP,
	PLAYER_1_K_DOWN,
	PLAYER_1_K_LEFT,
	PLAYER_1_K_RIGHT,
	PLAYER_2_K_UP,
	PLAYER_2_K_DOWN,
	PLAYER_2_K_LEFT,
	PLAYER_2_K_RIGHT,
	K_ESCAPE,
	K_SPACE,
	K_RETURN,
	K_BACK,
	K_ONE,
	K_TWO,
	K_THREE,
	K_FOUR,
	K_FIVE,
	K_E,
	K_R,
	K_K,
	K_L,
	K_COUNT
};

// Enumeration for the different screen states
enum EGAMESTATES
{
	S_SPLASHSCREEN,
	S_SPLASH2,
	S_INSTRUCT,
	S_GAME,
	S_MAINMENU,
	S_LEVELONE,
	S_LEVELTWO,
	S_LEVELTHREE,
	S_LEVELFOUR,
	S_LEVELFIVE,
	S_GAMEOVER,
	S_CLEARGAME,
	S_COUNT
};

// struct for the game character
struct SGameChar
{
    COORD m_cLocation;
    bool  m_bActive;
};
struct SGameChar2
{
	COORD m_cLocation;
	bool  m_bActive;
};

struct portalgun
{
	unsigned char portal = 79;
	unsigned char UpDownProjectile = 179;
	unsigned char LeftRightProjectile = 196;
};

struct Direction
{
	unsigned char Up = 94;
	unsigned char Down = 118;
	unsigned char Left = 60;
	unsigned char Right = 62;
};

void init        ( void );      // initialize your variables, allocate memory, etc
void getInput    ( void );      // get input from player
void update      ( double dt ); // update the game and the state of the game
void render      ( void );      // renders the current state of the game to the console
void shutdown    ( void );      // do clean up, free memory

void splashScreenWait();    // waits for time to pass in splash screen

void gameplay();            // gameplay logic
void moveCharacter();
// moves the character, collision detection, physics, etc
void processUserInput();    // checks if you should change states or do something else with the game, e.g. pause, exit

void clearScreen();         // clears the current screen and draw from scratch 
void renderSplashScreen();	// renders the splash screen
void splashscreenextract();
void splashscreenextract2();

void renderGame();          // renders the game stuff
int load_game(int level);
void renderMap();           // renders the map to the buffer first
void renderCharacter();     // renders the character into the buffer

void renderFramerate();     // renders debug information, frame rate, elapsed time, etc
void renderToScreen();      // dump the contents of the buffer to the screen, one frame worth of game

void mainmenuchoice();
void renderToMainMenu();

void renderhealth();
void renderGameOver();
void gameoverwait();
void extractover();

void renderinstruct();
void instructwait();
void renderPortalgun();
void renderPortalgun(COORD playerLocation, char playerDirection, COORD& portal, bool& portalActive);

void rendercleargame();
void cleargamewait();

#endif // _GAME_H