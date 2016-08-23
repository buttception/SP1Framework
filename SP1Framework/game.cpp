	// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include "mapInteract.h"
#include "extract.h"
#include "levelTransition.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>

using namespace std; 

double  g_dElapsedTime;
double  g_dDeltaTime;
bool    g_abKeyPressed[K_COUNT], teleporter = false, gateOpen = false, shotPortal = false, shotPortal2 = false, PortActive1 = false, PortActive2 = false;
bool transisted, shotPortal3 = false, shotPortal4 = false, PortActive3 = false, PortActive4;

char	map[61][21];
int level;
char health = 53;
unsigned char wall = 178;
unsigned char direction, direction2;
unsigned char ground = 176;
unsigned char destination = 177;
string	teleport;
string	null = { '\0', };

int teledel = 0;
COORD door;
COORD lever1;
COORD lever2;
COORD cord1;
COORD cord2;
COORD cord3;
COORD cord4;
COORD portal1;
COORD portal2;
COORD portal3;
COORD portal4;


// Game specific variables here
SGameChar	g_sChar;
SGameChar2   g_sChar2;
EGAMESTATES g_eGameState = S_SPLASHSCREEN;
double  g_dBounceTime, g_dBounceTime2;// this is to prevent key bouncing, so we won't trigger keypresses more than once

// Console object
Console g_Console(80, 25, "SP1 Framework");

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init( void )
{
    // Set precision for floating point output
    g_dElapsedTime = 0.0;
    g_dBounceTime = 0.0;
	g_dBounceTime2 = 0.0;

    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;

    g_sChar.m_cLocation.X = 7;
    g_sChar.m_cLocation.Y = 6;
    g_sChar.m_bActive = true;
	g_sChar2.m_cLocation.X = 7;
	g_sChar2.m_cLocation.Y = 6;
	g_sChar2.m_bActive = true;
    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 16, L"Consolas");
}

//--------------------------------------------------------------
// Purpose  : Reset before exiting the program
//            Do your clean up of memory here
//            This is called once just before the game exits
// Input    : Void
// Output   : void
//--------------------------------------------------------------

void shutdown( void )
{
    // Reset to white text on black background
	colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    g_Console.clearBuffer();
}

//--------------------------------------------------------------
// Purpose  : Getting all the key press states
//            This function checks if any key had been pressed since the last time we checked
//            If a key is pressed, the value for that particular key will be true
//
//            Add more keys to the enum in game.h if you need to detect more keys
//            To get other VK key defines, right click on the VK define (e.g. VK_UP) and choose "Go To Definition" 
//            For Alphanumeric keys, the values are their ascii values (uppercase).
// Input    : Void
// Output   : void
//--------------------------------------------------------------

void getInput( void )
{    
	//g_abKeyPressed[K_UP] = isKeyPressed(VK_UP);
	//g_abKeyPressed[K_DOWN] = isKeyPressed(VK_DOWN);
	//g_abKeyPressed[K_LEFT] = isKeyPressed(VK_LEFT);
	//g_abKeyPressed[K_RIGHT] = isKeyPressed(VK_RIGHT);
	g_abKeyPressed[PLAYER_1_K_UP] = isKeyPressed(0x57); //player 1 control
	g_abKeyPressed[PLAYER_1_K_DOWN] = isKeyPressed(0x53);
	g_abKeyPressed[PLAYER_1_K_LEFT] = isKeyPressed(0x41);
	g_abKeyPressed[PLAYER_1_K_RIGHT] = isKeyPressed(0x44);
	g_abKeyPressed[PLAYER_2_K_UP] = isKeyPressed(VK_UP); //PLAYER 2 CONTROL
	g_abKeyPressed[PLAYER_2_K_DOWN] = isKeyPressed(VK_DOWN);
	g_abKeyPressed[PLAYER_2_K_LEFT] = isKeyPressed(VK_LEFT);
	g_abKeyPressed[PLAYER_2_K_RIGHT] = isKeyPressed(VK_RIGHT);
	g_abKeyPressed[K_SPACE] = isKeyPressed(VK_SPACE);
	g_abKeyPressed[K_ESCAPE] = isKeyPressed(VK_ESCAPE);
	g_abKeyPressed[K_RETURN] = isKeyPressed(VK_RETURN);
	g_abKeyPressed[K_ONE] = isKeyPressed(0x31); // assign the one key
	g_abKeyPressed[K_TWO] = isKeyPressed(0x32); // assign the two key
	g_abKeyPressed[K_THREE] = isKeyPressed(0x33); // assign the three key
	g_abKeyPressed[K_FOUR] = isKeyPressed(0x34); // assign the four key
	g_abKeyPressed[K_FIVE] = isKeyPressed(0x35); // assign the five key
	g_abKeyPressed[K_E] = isKeyPressed(0x45); // assign 'E' key for Player 1 first portal
	g_abKeyPressed[K_R] = isKeyPressed(0x52); // assign 'R' key for Player 1 second portal
	g_abKeyPressed[K_K] = isKeyPressed(0x4B); // assign 'K' key for Player 2 first portal
	g_abKeyPressed[K_L] = isKeyPressed(0x4C); // assign 'L' key for player 2 second portal

}

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function
//            double dt - This is the amount of time in seconds since the previous call was made
//
//            Game logic should be done here.
//            Such as collision checks, determining the position of your game characters, status updates, etc
//            If there are any calls to write to the console here, then you are doing it wrong.
//
//            If your game has multiple states, you should determine the current state, and call the relevant function here.
//
// Input    : dt = deltatime
// Output   : void
//--------------------------------------------------------------

void update(double dt)
{
    // get the delta time
    g_dElapsedTime += dt;
    g_dDeltaTime = dt;

    switch (g_eGameState)
    {
        case S_SPLASHSCREEN : splashScreenWait(); // game logic for the splash screen
            break;
        case S_GAME: gameplay(); // gameplay logic when we are in the game
            break;
    }
}

//--------------------------------------------------------------
// Purpose  : Render function is to update the console screen
//            At this point, you should know exactly what to draw onto the screen.
//            Just draw it!
//            To get an idea of the values for colours, look at console.h and the URL listed there
// Input    : void
// Output   : void
//--------------------------------------------------------------

void render()
{
    clearScreen();      // clears the current screen and draw from scratch 
	switch (g_eGameState)
	{
	case S_SPLASHSCREEN: renderSplashScreen();
		break;
	case S_MAINMENU: renderToMainMenu();
		break;
	case S_MAINMENU2: renderToMainMenu2();
		break;
	case S_MAINMENU3: renderToMainMenu3();
		break;
	case S_MAINMENU4: renderToMainMenu4();
		break;
	case S_MAINMENU5: renderToMainMenu5();
		break;
	case S_GAME: renderGame();
		break;
	case S_GAMEOVER: renderGameOver();
		break;
	}
    renderFramerate();  // renders debug information, frame rate, elapsed time, etc
    renderToScreen();   // dump the contents of the buffer to the screen, one frame worth of game
}

void splashScreenWait()    // waits for time to pass in splash screen
{
	if (g_abKeyPressed[K_SPACE]) // press space to start game
	{
		g_eGameState = S_MAINMENU;
	}

	if (g_abKeyPressed[K_ESCAPE])
		g_bQuitGame = true;
}

void gameplay()            // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter_1(); 
	moveCharacter_2();// moves the character, collision detection, physics, etc
                        // sound can be played here too.
}

void moveCharacter_1()
{
	COORD xy;
	bool bSomethingHappened = false;
	if (g_dBounceTime > g_dElapsedTime)
		return;

	// Updating the location of the character based on the key press
	// providing a beep sound whenver we shift the character
	if (g_abKeyPressed[PLAYER_1_K_UP] && g_sChar.m_cLocation.Y > 0)
	{
		//Beep(1440, 30);	
		//only move if player is facing in the direction he wants to move
		if (map[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y - 2] != 'x' && map[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y - 2] != 'd' && direction == 'u')
		{
			if (g_sChar2.m_cLocation.X != g_sChar.m_cLocation.X || g_sChar.m_cLocation.Y - 1 != g_sChar2.m_cLocation.Y)
			{
				g_sChar.m_cLocation.Y--;
				bSomethingHappened = true;
			}
		}
		direction = 'u';
	}
	if (g_abKeyPressed[PLAYER_1_K_LEFT] && g_sChar.m_cLocation.X > 0)
	{
		//Beep(1440, 30);
		if (map[g_sChar.m_cLocation.X - 1][g_sChar.m_cLocation.Y - 1] != 'x' && map[g_sChar.m_cLocation.X - 1][g_sChar.m_cLocation.Y - 1] != 'd' && direction == 'l')
		{
			if (g_sChar2.m_cLocation.X != g_sChar.m_cLocation.X - 1 || g_sChar.m_cLocation.Y != g_sChar2.m_cLocation.Y)
			{
				g_sChar.m_cLocation.X--;
				bSomethingHappened = true;
			}
		}
		direction = 'l';
	}
	if (g_abKeyPressed[PLAYER_1_K_DOWN] && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
	{
		//Beep(1440, 30);
		if (map[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y] != 'x' && map[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y] != 'd' && direction == 'd')
		{
			if (g_sChar2.m_cLocation.X != g_sChar.m_cLocation.X || g_sChar.m_cLocation.Y + 1 != g_sChar2.m_cLocation.Y)
			{
				g_sChar.m_cLocation.Y++;
				bSomethingHappened = true;
			}
		}
		direction = 'd';
	}
	if (g_abKeyPressed[PLAYER_1_K_RIGHT] && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 1)
	{
		//Beep(1440, 30);
		if (map[g_sChar.m_cLocation.X + 1][g_sChar.m_cLocation.Y - 1] != 'x' && map[g_sChar.m_cLocation.X + 1][g_sChar.m_cLocation.Y - 1] != 'd' && direction == 'r')
		{
			if (g_sChar2.m_cLocation.X != g_sChar.m_cLocation.X + 1 || g_sChar.m_cLocation.Y  != g_sChar2.m_cLocation.Y)
			{
				g_sChar.m_cLocation.X++;
				bSomethingHappened = true;
			}
		}
		direction = 'r';
	}

	if (bSomethingHappened == true)
	{
		
		
		if (map[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y - 1] == 'f')
		{
			g_sChar.m_cLocation.X = 5;
			g_sChar.m_cLocation.Y = 10;
			direction = 'u';
			health--;

		}

		//check if playr moved into a telporter
		xy = teleportation(teleport, g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y);
		if (xy.X != 0 && xy.Y != 0)
		{
			// move player to other teleporter
			g_sChar.m_cLocation = xy;
		}
		//if player location is l
		//if (map[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y - 1] == 'l')
		//{
		//	//set door to open
		//	gateOpen = true;
		//}
		gateOpen = dooring(lever1, lever2, g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y - 1, g_sChar2.m_cLocation.X, g_sChar2.m_cLocation.Y - 1);
		if (gateOpen == true)
		{
			map[door.X][door.Y] = '-';
		}
		// set the bounce time to some time in the future to prevent accidental triggers
		g_dBounceTime = g_dElapsedTime + 0.010; // 125ms should not be enough
										// 125ms should be enough
										//if player reaches exit for stage 0, move to next map
		g_sChar.m_cLocation = mapTransition(g_sChar.m_cLocation, g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y, &level, &transisted);
		if (transisted == true)
		{
			load_game(level);
			g_sChar2.m_cLocation.X = g_sChar.m_cLocation.X + 1;
			g_sChar2.m_cLocation.Y = g_sChar.m_cLocation.Y;
			transisted = false;
		}
		teleport.erase(0, teledel);
		teledel = 0;

		if (g_sChar.m_cLocation.X == portal1.X && g_sChar.m_cLocation.Y == portal1.Y && PortActive1 && PortActive2)
		{
			g_sChar.m_cLocation.X = portal2.X;
			g_sChar.m_cLocation.Y = portal2.Y;
		}
		else if(g_sChar.m_cLocation.X == portal2.X && g_sChar.m_cLocation.Y == portal2.Y && PortActive1 && PortActive2)
		{
			g_sChar.m_cLocation.X = portal1.X;
			g_sChar.m_cLocation.Y = portal1.Y;
		}
		if (g_sChar.m_cLocation.X == portal3.X && g_sChar.m_cLocation.Y == portal3.Y && PortActive3 && PortActive4)
		{
			g_sChar.m_cLocation.X = portal4.X;
			g_sChar.m_cLocation.Y = portal4.Y;
		}
		else if (g_sChar.m_cLocation.X == portal4.X && g_sChar.m_cLocation.Y == portal4.Y && PortActive3 && PortActive4)
		{
			g_sChar.m_cLocation.X = portal3.X;
			g_sChar.m_cLocation.Y = portal3.Y;
		}


	}
}

void moveCharacter_2()
{
	COORD xy;
	bool bSomethingHappened_2 = false;
	if (g_dBounceTime2 > g_dElapsedTime)
		return;
	if (g_abKeyPressed[PLAYER_2_K_UP] && g_sChar2.m_cLocation.Y > 0)
	{
		//Beep(1440, 30);
		//only move if player is facing in the direction he wants to move
		if (map[g_sChar2.m_cLocation.X][g_sChar2.m_cLocation.Y - 2] != 'x' && map[g_sChar2.m_cLocation.X][g_sChar2.m_cLocation.Y - 2] != 'd' && direction2 == 't')
		{
			if (g_sChar.m_cLocation.X  != g_sChar2.m_cLocation.X || g_sChar2.m_cLocation.Y - 1 != g_sChar.m_cLocation.Y)
			{
				g_sChar2.m_cLocation.Y--;
				bSomethingHappened_2 = true;
			}
		}
		direction2 = 't';
	}
	if (g_abKeyPressed[PLAYER_2_K_LEFT] && g_sChar2.m_cLocation.X > 0)
	{
		//Beep(1440, 30);
		if (map[g_sChar2.m_cLocation.X - 1][g_sChar2.m_cLocation.Y - 1] != 'x' && map[g_sChar2.m_cLocation.X - 1][g_sChar2.m_cLocation.Y - 1] != 'd' && direction2 == 'f')
		{
			if (g_sChar.m_cLocation.X != g_sChar2.m_cLocation.X - 1 || g_sChar2.m_cLocation.Y != g_sChar.m_cLocation.Y)
			{
				g_sChar2.m_cLocation.X--;
				bSomethingHappened_2 = true;
			}
		}
		direction2 = 'f';
	}
	if (g_abKeyPressed[PLAYER_2_K_DOWN] && g_sChar2.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
	{
		//Beep(1440, 30);
		if (map[g_sChar2.m_cLocation.X][g_sChar2.m_cLocation.Y] != 'x' && map[g_sChar2.m_cLocation.X][g_sChar2.m_cLocation.Y] != 'd' && direction2 == 'g')
		{
			if (g_sChar.m_cLocation.X != g_sChar2.m_cLocation.X || g_sChar2.m_cLocation.Y + 1 != g_sChar.m_cLocation.Y)
			{
				g_sChar2.m_cLocation.Y++;
				bSomethingHappened_2 = true;
			}
		}
		direction2 = 'g';
	}
	if (g_abKeyPressed[PLAYER_2_K_RIGHT] && g_sChar2.m_cLocation.X < g_Console.getConsoleSize().X - 1)
	{
		//Beep(1440, 30);
		if (map[g_sChar2.m_cLocation.X + 1][g_sChar2.m_cLocation.Y - 1] != 'x' && map[g_sChar2.m_cLocation.X + 1][g_sChar2.m_cLocation.Y - 1] != 'd' && direction2 == 'h')
		{
			if (g_sChar.m_cLocation.X != g_sChar2.m_cLocation.X + 1 || g_sChar2.m_cLocation.Y != g_sChar.m_cLocation.Y)
			{
				g_sChar2.m_cLocation.X++;
				bSomethingHappened_2 = true;
			}
		}
		direction2 = 'h';
	}


	if (bSomethingHappened_2 == true)
	{
		//restart player at start point(sample level)
		

		if (map[g_sChar2.m_cLocation.X][g_sChar2.m_cLocation.Y - 1] == 'f')
		{
			g_sChar2.m_cLocation.X = 7;
			g_sChar2.m_cLocation.Y = 8;
			direction2 = 't';
			health--;
		}

		//check if playr moved into a telporter
		xy = teleportation(teleport, g_sChar2.m_cLocation.X, g_sChar2.m_cLocation.Y);
		if (xy.X != 0 && xy.Y != 0)
		{
			// move player to other teleporter
			g_sChar2.m_cLocation = xy;
		}
		//if player location is l
		//if (map[g_sChar2.m_cLocation.X][g_sChar2.m_cLocation.Y - 1] == 'l')
		//{
		//	//set door to open
		//	gateOpen = true;
		//}
		// set the bounce time to some time in the future to prevent accidental triggers
		gateOpen = dooring(lever1, lever2, g_sChar.m_cLocation.X, g_sChar.m_cLocation.Y - 1, g_sChar2.m_cLocation.X, g_sChar2.m_cLocation.Y - 1);
		if (gateOpen == true)
		{
			map[door.X][door.Y] = '-';
		}
		g_dBounceTime2 = g_dElapsedTime + 0.005; // 125ms should not be enough
										// 125ms should be enough
										//if player reaches exit for stage 0, move to next map
		g_sChar2.m_cLocation = mapTransition(g_sChar2.m_cLocation, g_sChar2.m_cLocation.X, g_sChar2.m_cLocation.Y, &level, &transisted);
		if (transisted == true)
		{
			load_game(level);
			g_sChar.m_cLocation.X = g_sChar2.m_cLocation.X + 1;
			g_sChar.m_cLocation.Y = g_sChar2.m_cLocation.Y;
			transisted = false;
		}
		teleport.erase(0, teledel);
		teledel = 0;

		if (g_sChar2.m_cLocation.X == portal3.X && g_sChar2.m_cLocation.Y == portal3.Y && PortActive3 && PortActive4)
		{
			g_sChar2.m_cLocation.X = portal4.X;
			g_sChar2.m_cLocation.Y = portal4.Y;
		}
		else if (g_sChar2.m_cLocation.X == portal4.X && g_sChar2.m_cLocation.Y == portal4.Y && PortActive3 && PortActive4)
		{
			g_sChar2.m_cLocation.X = portal3.X;
			g_sChar2.m_cLocation.Y = portal3.Y;
		}
		if (g_sChar2.m_cLocation.X == portal1.X && g_sChar2.m_cLocation.Y == portal1.Y && PortActive1 && PortActive2)
		{
			g_sChar2.m_cLocation.X = portal2.X;
			g_sChar2.m_cLocation.Y = portal2.Y;
		}
		else if (g_sChar2.m_cLocation.X == portal2.X && g_sChar2.m_cLocation.Y == portal2.Y && PortActive1 && PortActive2)
		{
			g_sChar2.m_cLocation.X = portal1.X;
			g_sChar2.m_cLocation.Y = portal1.Y;
		}
	}
}

void processUserInput()
{
    // quits the game if player hits the escape key
    if (g_abKeyPressed[K_ESCAPE])
        g_bQuitGame = true; 
	if (g_abKeyPressed[K_E] && (g_sChar.m_cLocation.Y - 1) != 'x')
		shotPortal = true;
	if (g_abKeyPressed[K_R] && (g_sChar.m_cLocation.Y - 1) != 'x')
		shotPortal2 = true;
	if (g_abKeyPressed[K_E] && (g_sChar.m_cLocation.Y + 1) != 'x')
		shotPortal = true;
	if (g_abKeyPressed[K_R] && (g_sChar.m_cLocation.Y + 1) != 'x')
		shotPortal2 = true;
	if (g_abKeyPressed[K_E] && (g_sChar.m_cLocation.X + 1) != 'x')
		shotPortal = true;
	if (g_abKeyPressed[K_R] && (g_sChar.m_cLocation.X + 1) != 'x')
		shotPortal2 = true;
	if (g_abKeyPressed[K_E] && (g_sChar.m_cLocation.X - 1) != 'x')
		shotPortal = true;
	if (g_abKeyPressed[K_R] && (g_sChar.m_cLocation.X - 1) != 'x')
		shotPortal2 = true;

	if (g_abKeyPressed[K_K] && (g_sChar2.m_cLocation.Y - 1) != 'x')
		shotPortal3 = true;
	if (g_abKeyPressed[K_L] && (g_sChar2.m_cLocation.Y - 1) != 'x')
		shotPortal4 = true;
	if (g_abKeyPressed[K_K] && (g_sChar2.m_cLocation.Y + 1) != 'x')
		shotPortal3 = true;
	if (g_abKeyPressed[K_L] && (g_sChar2.m_cLocation.Y + 1) != 'x')
		shotPortal4 = true;
	if (g_abKeyPressed[K_K] && (g_sChar2.m_cLocation.X + 1) != 'x')
		shotPortal3 = true;
	if (g_abKeyPressed[K_L] && (g_sChar2.m_cLocation.X + 1) != 'x')
		shotPortal4 = true;
	if (g_abKeyPressed[K_K] && (g_sChar2.m_cLocation.X - 1) != 'x')
		shotPortal3 = true;
	if (g_abKeyPressed[K_L] && (g_sChar2.m_cLocation.X - 1) != 'x')
		shotPortal4 = true;

}

void clearScreen()
{
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(0x00);
}

void renderSplashScreen()  // renders the splash screen
{
	if ((int)(g_dElapsedTime) % 2 == 0)
	{
		int i = 0;
		int j = 0;
		char splash[59][21];
		ifstream file("title.txt"); // read from title.txt to print the ascii art
		COORD c;
		if (file.is_open())
		{
			while (j <= 20)
			{
				while (i <= 58)
				{
					file >> splash[i][j];
					i++;
				}
				i = 0;
				j++;
			}
			file.close();
		}
		for (int y = 0; y <= 20; y++)
		{
			c.Y = y + 4;
			for (int x = 0; x <= 58; x++)
			{
				c.X = x + 10;
				if (splash[x][y] != '~' && x < 11)
				{
					g_Console.writeToBuffer(c, splash[x][y], 0x0B);
				}
				else if (splash[x][y] != '~' && x > 47)
				{
					g_Console.writeToBuffer(c, splash[x][y], 0x0B);
				}
				else if (splash[x][y] != '~' && y > 5)
				{
					g_Console.writeToBuffer(c, splash[x][y], 0x0B);
				}
				else if (splash[x][y] != '~' && y > 10)
				{
					g_Console.writeToBuffer(c, splash[x][y], 0x0B);
				}
				else if (splash[x][y] != '~')
				{
					g_Console.writeToBuffer(c, splash[x][y], 0x07);
				}
			}
		}

		c = g_Console.getConsoleSize();
		c.Y /= 3;
		c.X = c.X / 2 - 35;
		c.Y += 4;
		c.X = g_Console.getConsoleSize().X / 2 - 15;
		g_Console.writeToBuffer(c, "Press <Space> to continue", 0x03);
		c.Y += 1;
		c.X = g_Console.getConsoleSize().X / 2 - 13;
		g_Console.writeToBuffer(c, "Press 'Esc' to quit", 0x07);
	}
	else
	{
		int i = 0;
		int j = 0;
		char splash[59][21];
		ifstream file("title2.txt"); // read from title.txt to print the ascii art
		COORD c;
		if (file.is_open())
		{
			while (j <= 20)
			{
				while (i <= 58)
				{
					file >> splash[i][j];
					i++;
				}
				i = 0;
				j++;
			}
			file.close();
		}
		for (int y = 0; y <= 20; y++)
		{
			c.Y = y + 4;
			for (int x = 0; x <= 58; x++)
			{
				c.X = x + 10;
				if (splash[x][y] != '~' && x < 11)
				{
					g_Console.writeToBuffer(c, splash[x][y], 0x0B);
				}
				else if (splash[x][y] != '~' && x > 47)
				{
					g_Console.writeToBuffer(c, splash[x][y], 0x0B);
				}
				else if (splash[x][y] != '~' && y > 5)
				{
					g_Console.writeToBuffer(c, splash[x][y], 0x0B);
				}
				else if (splash[x][y] != '~')
				{
					g_Console.writeToBuffer(c, splash[x][y], 0x07);
				}
			}
		}

		c = g_Console.getConsoleSize();
		c.Y /= 3;
		c.X = c.X / 2 - 35;
		c.Y += 4;
		c.X = g_Console.getConsoleSize().X / 2 - 15;
		g_Console.writeToBuffer(c, "Press <Space> to continue", 0x03);
		c.Y += 1;
		c.X = g_Console.getConsoleSize().X / 2 - 13;
		g_Console.writeToBuffer(c, "Press 'Esc' to quit", 0x07);
	}
}

void renderGameOver()
{
	health = 53;
	int i = 0;
	int j = 0;
	char main[63][6];
	ifstream file("GameOver.txt"); // read file from PickALevel.txt to print the ascii art
	COORD c;
	if (file.is_open())
	{
		while (j <= 5)
		{
			while (i <= 62)
			{
				file >> main[i][j];
				i++;
			}
			i = 0;
			j++;
		}
		file.close();
	}
	for (int y = 0; y <= 5; y++)
	{
		c.Y = y + 4;
		for (int x = 0; x <= 62; x++)
		{
			c.X = x + 10;
			if (main[x][y] != '~')
			{
				g_Console.writeToBuffer(c, main[x][y], 0x0C);
			}
		}
	}
	c = g_Console.getConsoleSize();
	c.Y /= 3 + 10;
	c.X = c.X / 2 - 35;
	c.Y += 10;
	c.X = g_Console.getConsoleSize().X / 2 - 20;
	g_Console.writeToBuffer(c, "Press enter to go back to Starting Screen.", 0x03);

	if (g_abKeyPressed[K_RETURN])
	{
		g_eGameState = S_SPLASHSCREEN;
	}
	if (g_abKeyPressed[K_ESCAPE])
	{
		g_bQuitGame = true;
	}
}

void renderhealth()
{
	COORD c;
	c = g_Console.getConsoleSize();
	c.Y /= 3 + 10;
	c.X = c.X / 2;
	c.Y += 5;
	c.X = g_Console.getConsoleSize().X / 2 + 30;
	g_Console.writeToBuffer(c, "Health", 0x07);
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 + 30;
	g_Console.writeToBuffer(c, health, 0x07);
	if (health == 48)
	{
		g_eGameState = S_GAMEOVER;
	}

}

void rendermap()
{
	/*// Set up sample colours, and output shadings
	const WORD colors[] = {
	0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
	0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
	};

	COORD c;
	for (int i = 0; i < 12; ++i)
	{
	c.X = 5 * i;
	c.Y = i + 1;
	colour(colors[i]);
	g_Console.writeToBuffer(c, " ����", colors[i]);
	}*/
	//write everything in txt to stupid
	//string stupid = extractMap(&level);
	//map[b][a] = stupid[c];
	int c = 0;
	//int d = 0;
	//int e = 0;
	//int f = 0;
	//int g = 0;
	for (int a = 0; a < 20; a++)
	{
		for (int b = 0; b < 60; b++)
		{
			//transfer it to an array
			//map[b][a] = stupid[c];
			//record where is the teleporter to the 
			if (map[b][a] == 'p')
			{
				teledel += 2;
				teleport += b;
				teleport += a + 1;
			}
			if (map[b][a] == 'd')
			{
				door.X = b;
				door.Y = a;
			}
			if (map[b][a] == 'l')
			{
				lever1.X = b;
				lever1.Y = a;
			}
			if (map[b][a] == 'L')
			{
				lever2.X = b;
				lever2.Y = a;
			}
			//record where door is, only when door not opened
			//if (map[b][a] == 'd' && gateOpen == false)
			//{
			//	door[d][e] = b;
			//	d++;
			//	door[d][e] = a;
			//}
			////if player stepped on lever, treat door as open tile
			//else if (map[b][a] == 'd')
			//{
			//	map[b][a] = '-';
			//}
			c++;
		}
		c = (a + 1) * 60;
	}
	//buffer the shit
	COORD coord;
	for (int y = 0; y <= 19; y++)
	{
		coord.Y = y + 1;
		for (int x = 0; x <= 59; x++)
		{
			coord.X = x;
			//buffer ground
			if (map[x][y] == '-')
			{
				if ((g_sChar.m_cLocation.X + 7) >= x && x >= (g_sChar.m_cLocation.X - 7) && (g_sChar.m_cLocation.Y - 5) <= (y - 1) && (g_sChar.m_cLocation.Y + 5) >= (y + 3))
				{
					g_Console.writeToBuffer(coord, ground, 0x88);
				}
				if ((g_sChar2.m_cLocation.X + 7) >= x && x >= (g_sChar2.m_cLocation.X - 7) && (g_sChar2.m_cLocation.Y - 5) <= (y - 1) && (g_sChar2.m_cLocation.Y + 5) >= (y + 3))
				{
					g_Console.writeToBuffer(coord, ground, 0x88);
				}
			}
			//buffer wall
			if (map[x][y] == 'x')
			//buffer teleportal
			{
				if ((g_sChar.m_cLocation.X + 5) >= x && x >= (g_sChar.m_cLocation.X - 5) && (g_sChar.m_cLocation.Y - 5) <= (y + 1) && (g_sChar.m_cLocation.Y + 5) >= (y + 1))
				{
					g_Console.writeToBuffer(coord, wall, 0x80);
				}
				if ((g_sChar2.m_cLocation.X + 5) >= x && x >= (g_sChar2.m_cLocation.X - 5) && (g_sChar2.m_cLocation.Y - 5) <= (y + 1) && (g_sChar2.m_cLocation.Y + 5) >= (y + 1))
				{
					g_Console.writeToBuffer(coord, wall, 0x80);
				}
			}
			if (map[x][y] == 'p')
			{
				if ((g_sChar.m_cLocation.X + 5) >= x && x >= (g_sChar.m_cLocation.X - 5) && (g_sChar.m_cLocation.Y - 5) <= (y + 1) && (g_sChar.m_cLocation.Y + 5) >= (y + 1))
				{
					g_Console.writeToBuffer(coord, wall, 0x2B);
				}
				if ((g_sChar2.m_cLocation.X + 5) >= x && x >= (g_sChar2.m_cLocation.X - 5) && (g_sChar2.m_cLocation.Y - 5) <= (y + 1) && (g_sChar2.m_cLocation.Y + 5) >= (y + 1))
				{
					g_Console.writeToBuffer(coord, wall, 0x2B);
				}
			}
			//buffer exit
			if (map[x][y] == 'e')
			{
				g_Console.writeToBuffer(coord, destination, 0xA0);
			}
			if (map[x][y] == 'd')
			{
				if ((g_sChar.m_cLocation.X + 5) >= x && x >= (g_sChar.m_cLocation.X - 5) && (g_sChar.m_cLocation.Y - 5) <= (y + 1) && (g_sChar.m_cLocation.Y + 5) >= (y + 1))
				{
					g_Console.writeToBuffer(coord, wall, 0x11);
				}
				if ((g_sChar2.m_cLocation.X + 5) >= x && x >= (g_sChar2.m_cLocation.X - 5) && (g_sChar2.m_cLocation.Y - 5) <= (y + 1) && (g_sChar2.m_cLocation.Y + 5) >= (y + 1))
				{
					g_Console.writeToBuffer(coord, wall, 0x11);
				}
			}
			if (map[x][y] == 'l' || map[x][y] == 'L')
			{
				if ((g_sChar.m_cLocation.X + 5) >= x && x >= (g_sChar.m_cLocation.X - 5) && (g_sChar.m_cLocation.Y - 5) <= (y + 1) && (g_sChar.m_cLocation.Y + 5) >= (y + 1))
				{
					g_Console.writeToBuffer(coord, destination, 0x1F);
				}
				if ((g_sChar2.m_cLocation.X + 5) >= x && x >= (g_sChar2.m_cLocation.X - 5) && (g_sChar2.m_cLocation.Y - 5) <= (y + 1) && (g_sChar2.m_cLocation.Y + 5) >= (y + 1))
				{
					g_Console.writeToBuffer(coord, destination, 0x1F);
				}
			}
			if (map[x][y] == 'f')
			{
				if ((g_sChar.m_cLocation.X + 5) >= x && x >= (g_sChar.m_cLocation.X - 5) && (g_sChar.m_cLocation.Y - 5) <= (y + 1) && (g_sChar.m_cLocation.Y + 5) >= (y + 1))
				{
					g_Console.writeToBuffer(coord, ground, 0xC3);
				}
				if ((g_sChar2.m_cLocation.X + 5) >= x && x >= (g_sChar2.m_cLocation.X - 5) && (g_sChar2.m_cLocation.Y - 5) <= (y + 1) && (g_sChar2.m_cLocation.Y + 5) >= (y + 1))
				{
					g_Console.writeToBuffer(coord, ground, 0xC3);
				}

			}
			if (shotPortal == false)
			{
				g_Console.writeToBuffer(portal1, 'O', 0x8C);
			}
			if (shotPortal2 == false)
			{
				g_Console.writeToBuffer(portal2, 'O', 0x81);
			}
			if (shotPortal3 == false)
			{
				g_Console.writeToBuffer(portal3, 'O', 0x8C);
			}
			if (shotPortal4 == false)
			{
				g_Console.writeToBuffer(portal4, 'O', 0x81);
			}

		}
	}
}

void renderGame()
{
	rendermap();// renders the map to the buffer first	
	renderCharacter(); 
	renderCharacter_2();// renders the character into the buffer
	renderhealth();

}

void renderCharacter()
{
	WORD charColor = 0x89;
    // Draw the location of the character
	//change player direction
	
	if (direction == 'u')
	{
		g_Console.writeToBuffer(g_sChar.m_cLocation, '^', charColor);
		cord1.X = g_sChar.m_cLocation.X;
		cord1.Y = g_sChar.m_cLocation.Y;
		cord2.X = g_sChar.m_cLocation.X;
		cord2.Y = g_sChar.m_cLocation.Y;
		while (shotPortal)
		{
			if ((map[cord1.X][cord1.Y - 2] != 'x') && (map[cord1.X][cord1.Y - 2] != 'e') && (map[cord1.X][cord1.Y - 2] != 'd'))
			{
				cord1.Y--;
				g_Console.writeToBuffer(cord1, '|', 0x8C);
			}
			else
			{
				portal1.X = cord1.X;
				portal1.Y = cord1.Y;
				shotPortal = false;
				PortActive1 = true;
			}
		}
		while (shotPortal2)
		{
			if ((map[cord2.X][cord2.Y - 2] != 'x') && (map[cord2.X][cord2.Y - 2] != 'e') && (map[cord2.X][cord2.Y - 2] != 'd'))
			{
				cord2.Y--;
				g_Console.writeToBuffer(cord2, '|', 0x81);
			}
			else
			{
				portal2.X = cord2.X;
				portal2.Y = cord2.Y;
				shotPortal2 = false;
				PortActive2 = true;
			}
		}
	}
	else if (direction == 'd')
	{
		g_Console.writeToBuffer(g_sChar.m_cLocation, 'v', charColor);
		cord1.X = g_sChar.m_cLocation.X;
		cord1.Y = g_sChar.m_cLocation.Y;
		cord2.X = g_sChar.m_cLocation.X;
		cord2.Y = g_sChar.m_cLocation.Y;
		while (shotPortal)
		{
			if ((map[cord1.X][cord1.Y] != 'x') && (map[cord1.X][cord1.Y] != 'e') && (map[cord1.X][cord1.Y] != 'd'))
			{
				cord1.Y++;
				g_Console.writeToBuffer(cord1, '|', 0x8C);
			}
			else
			{
				portal1.X = cord1.X;
				portal1.Y = cord1.Y;
				shotPortal = false;
				PortActive1 = true;
			}
		}
		while (shotPortal2)
		{
			if ((map[cord2.X][cord2.Y] != 'x') && (map[cord2.X][cord2.Y] != 'e') && (map[cord2.X][cord2.Y] != 'd'))
			{
				cord2.Y++;
				g_Console.writeToBuffer(cord2, '|', 0x81);
			}
			else
			{
				portal2.X = cord2.X;
				portal2.Y = cord2.Y;
				shotPortal2 = false;
				PortActive2 = true;
			}
		}
	}
	else if (direction == 'l')
	{
		g_Console.writeToBuffer(g_sChar.m_cLocation, '<', charColor);
		cord1.X = g_sChar.m_cLocation.X;
		cord1.Y = g_sChar.m_cLocation.Y;
		cord2.X = g_sChar.m_cLocation.X;
		cord2.Y = g_sChar.m_cLocation.Y;
		while (shotPortal)
		{
			if ((map[cord1.X-1][cord1.Y-1] != 'x') && (map[cord1.X - 1][cord1.Y - 1] != 'e') && (map[cord1.X - 1][cord1.Y - 1] != 'd'))
			{
				cord1.X--;
				g_Console.writeToBuffer(cord1, '-', 0x8C);
			}
			else
			{
				portal1.X = cord1.X;
				portal1.Y = cord1.Y;
				shotPortal = false;
				PortActive1 = true;
			}
		}
		while (shotPortal2)
		{
			if ((map[cord2.X-1][cord2.Y-1] != 'x') && (map[cord2.X - 1][cord2.Y - 1] != 'e') && (map[cord2.X - 1][cord2.Y - 1] != 'd'))
			{
				cord2.X--;
				g_Console.writeToBuffer(cord2, '-', 0x81);
			}
			else
			{
				portal2.X = cord2.X;
				portal2.Y = cord2.Y;
				shotPortal2 = false;
				PortActive2 = true;
			}
		}
	}
	else if (direction == 'r')
	{
		g_Console.writeToBuffer(g_sChar.m_cLocation, '>', charColor);
		cord1.X = g_sChar.m_cLocation.X;
		cord1.Y = g_sChar.m_cLocation.Y;
		cord2.X = g_sChar.m_cLocation.X;
		cord2.Y = g_sChar.m_cLocation.Y;
		while (shotPortal)
		{
			if ((map[cord1.X + 1][cord1.Y-1] != 'x') && (map[cord1.X + 1][cord1.Y - 1] != 'e') && (map[cord1.X + 1][cord1.Y - 1] != 'd'))
			{
				cord1.X++;
				g_Console.writeToBuffer(cord1, '-', 0x8C);
			}
			else
			{
				portal1.X = cord1.X;
				portal1.Y = cord1.Y;
				shotPortal = false;
				PortActive1 = true;
			}
		}
		while (shotPortal2)
		{
			if ((map[cord2.X + 1][cord2.Y-1] != 'x') && (map[cord2.X + 1][cord2.Y - 1] != 'e') && (map[cord2.X + 1][cord2.Y - 1] != 'd'))
			{
				cord2.X++;
				g_Console.writeToBuffer(cord2, '-', 0x81);
			}
			else
			{
				portal2.X = cord2.X;
				portal2.Y = cord2.Y;
				shotPortal2 = false;
				PortActive2 = true;
			}
		}
	}
	else
	{
		g_Console.writeToBuffer(g_sChar.m_cLocation, '^', charColor);
	}
}

void renderCharacter_2()
{
	WORD charColor2 = 0x89;
	// Draw the location of the character
	//change player direction

	if (direction2 == 't')
	{
		g_Console.writeToBuffer(g_sChar2.m_cLocation, '^', charColor2);
		cord3.X = g_sChar2.m_cLocation.X;
		cord3.Y = g_sChar2.m_cLocation.Y;
		cord4.X = g_sChar2.m_cLocation.X;
		cord4.Y = g_sChar2.m_cLocation.Y;
		while (shotPortal3)
		{
			if ((map[cord3.X][cord3.Y - 2] != 'x') && (map[cord3.X][cord3.Y - 2] != 'e') && (map[cord3.X][cord3.Y - 2] != 'd'))
			{
				cord3.Y--;
				g_Console.writeToBuffer(cord3, '|', 0x8C);
			}
			else
			{
				portal3.X = cord3.X;
				portal3.Y = cord3.Y;
				shotPortal3 = false;
				PortActive3 = true;
			}
		}
		while (shotPortal4)
		{
			if ((map[cord4.X][cord4.Y - 2] != 'x') && (map[cord4.X][cord4.Y - 2] != 'e') && (map[cord4.X][cord4.Y - 2] != 'd'))
			{
				cord4.Y--;
				g_Console.writeToBuffer(cord4, '|', 0x81);
			}
			else
			{
				portal4.X = cord4.X;
				portal4.Y = cord4.Y;
				shotPortal4 = false;
				PortActive4 = true;
			}
		}
	}
	else if (direction2 == 'g')
	{
		g_Console.writeToBuffer(g_sChar2.m_cLocation, 'v', charColor2);
		cord3.X = g_sChar2.m_cLocation.X;
		cord3.Y = g_sChar2.m_cLocation.Y;
		cord4.X = g_sChar2.m_cLocation.X;
		cord4.Y = g_sChar2.m_cLocation.Y;
		while (shotPortal3)
		{
			if ((map[cord3.X][cord3.Y] != 'x') && (map[cord3.X][cord3.Y] != 'e') && (map[cord3.X][cord3.Y] != 'd'))
			{
				cord3.Y++;
				g_Console.writeToBuffer(cord3, '|', 0x8C);
			}
			else
			{
				portal3.X = cord3.X;
				portal3.Y = cord3.Y;
				shotPortal3 = false;
				PortActive3 = true;
			}
		}
		while (shotPortal4)
		{
			if ((map[cord4.X][cord4.Y] != 'x') && (map[cord4.X][cord4.Y] != 'e') && (map[cord4.X][cord4.Y] != 'd'))
			{
				cord4.Y++;
				g_Console.writeToBuffer(cord4, '|', 0x81);
			}
			else
			{
				portal4.X = cord4.X;
				portal4.Y = cord4.Y;
				shotPortal4 = false;
				PortActive4 = true;
			}
		}
	}
	else if (direction2 == 'f')
	{
		g_Console.writeToBuffer(g_sChar2.m_cLocation, '<', charColor2);
		cord3.X = g_sChar2.m_cLocation.X;
		cord3.Y = g_sChar2.m_cLocation.Y;
		cord4.X = g_sChar2.m_cLocation.X;
		cord4.Y = g_sChar2.m_cLocation.Y;
		while (shotPortal3)
		{
			if ((map[cord3.X - 1][cord3.Y - 1] != 'x') && (map[cord3.X - 1][cord3.Y - 1] != 'e') && (map[cord3.X - 1][cord3.Y - 1] != 'd'))
			{
				cord3.X--;
				g_Console.writeToBuffer(cord3, '-', 0x8C);
			}
			else
			{
				portal3.X = cord3.X;
				portal3.Y = cord3.Y;
				shotPortal3 = false;
				PortActive3 = true;
			}
		}
		while (shotPortal4)
		{
			if ((map[cord4.X - 1][cord4.Y - 1] != 'x') && (map[cord4.X - 1][cord4.Y - 1] != 'e') && (map[cord4.X - 1][cord4.Y - 1] != 'd'))
			{
				cord4.X--;
				g_Console.writeToBuffer(cord4, '-', 0x81);
			}
			else
			{
				portal4.X = cord4.X;
				portal4.Y = cord4.Y;
				shotPortal4 = false;
				PortActive4 = true;
			}
		}
	}
	else if (direction2 == 'h')
	{
		g_Console.writeToBuffer(g_sChar2.m_cLocation, '>', charColor2);
		cord3.X = g_sChar2.m_cLocation.X;
		cord3.Y = g_sChar2.m_cLocation.Y;
		cord4.X = g_sChar2.m_cLocation.X;
		cord4.Y = g_sChar2.m_cLocation.Y;
		while (shotPortal3)
		{
			if ((map[cord3.X + 1][cord3.Y - 1] != 'x') && (map[cord3.X + 1][cord3.Y - 1] != 'e') && (map[cord3.X + 1][cord3.Y - 1] != 'd'))
			{
				cord3.X++;
				g_Console.writeToBuffer(cord3, '-', 0x8C);
			}
			else
			{
				portal3.X = cord3.X;
				portal3.Y = cord3.Y;
				shotPortal3 = false;
				PortActive3 = true;
			}
		}
		while (shotPortal4)
		{
			if ((map[cord4.X + 1][cord4.Y - 1] != 'x') && (map[cord4.X + 1][cord4.Y - 1] != 'e') && (map[cord4.X + 1][cord4.Y - 1] != 'd'))
			{
				cord4.X++;
				g_Console.writeToBuffer(cord4, '-', 0x81);
			}
			else
			{
				portal4.X = cord4.X;
				portal4.Y = cord4.Y;
				shotPortal4 = false;
				PortActive4 = true;
			}
		}
	}
	else
	{
		g_Console.writeToBuffer(g_sChar2.m_cLocation, '^', charColor2);
	}
}

void renderFramerate()
{
    COORD c;
    // displays the framerate
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << 1.0 / g_dDeltaTime << "fps";
    c.X = g_Console.getConsoleSize().X - 9;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());

    // displays the elapsed time
    ss.str("");
    ss << g_dElapsedTime << "secs";
    c.X = 0;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());
}

void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}

void renderToMainMenu()
{
	int i = 0;
	int j = 0;
	char main[73][12];

	int count = 0;

	ifstream file("PickALevel.txt"); // read file from PickALevel.txt to print the ascii art
	COORD c;
	if (file.is_open())
	{
		while (j <= 11)
		{
			while (i <= 72)
			{
				file >> main[i][j];
				i++;
			}
			i = 0;
			j++;
		}
		file.close();
	}
	for (int y = 0; y <= 11; y++)
	{
		c.Y = y + 4;
		for (int x = 0; x <= 72; x++)
		{
			c.X = x + 3;
			if (main[x][y] != '~')
			{
				g_Console.writeToBuffer(c, main[x][y], 0x09);
			}
		}
	}
	c = g_Console.getConsoleSize();
	c.Y /= 3 + 10;
	c.X = c.X / 2 - 35;
	c.Y += 15;
	c.X = g_Console.getConsoleSize().X / 2 - 20;
	g_Console.writeToBuffer(c, "Press enter to choose your level (1-5).", 0x03);

	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 7;
	g_Console.writeToBuffer(c, "Level 1", 0x73);
	c.Y += 1;
	g_Console.writeToBuffer(c, "Level 2", 0x03);
	c.Y += 1;
	g_Console.writeToBuffer(c, "Level 3", 0x03);
	c.Y += 1;
	g_Console.writeToBuffer(c, "Level 4", 0x03);
	c.Y += 1;
	g_Console.writeToBuffer(c, "Level 5", 0x03);
	
	if (g_abKeyPressed[PLAYER_2_K_DOWN])
	{
		g_eGameState = S_MAINMENU2;
	}

	if (g_abKeyPressed[K_RETURN])
	{

		g_sChar.m_cLocation.X = 5;
		g_sChar.m_cLocation.Y = 10;
		g_sChar2.m_cLocation.X = 7;
		g_sChar2.m_cLocation.Y = 8;
		level = 0;

		g_eGameState = S_GAME;
	}

	if (g_abKeyPressed[K_ESCAPE])
		g_bQuitGame = true;
}

void renderToMainMenu2()
{
	int i = 0;
	int j = 0;
	char main[73][12];
	ifstream file("PickALevel.txt"); // read file from PickALevel.txt to print the ascii art
	COORD c;
	if (file.is_open())
	{
		while (j <= 11)
		{
			while (i <= 72)
			{
				file >> main[i][j];
				i++;
			}
			i = 0;
			j++;
		}
		file.close();
	}
	for (int y = 0; y <= 11; y++)
	{
		c.Y = y + 4;
		for (int x = 0; x <= 72; x++)
		{
			c.X = x + 3;
			if (main[x][y] != '~')
			{
				g_Console.writeToBuffer(c, main[x][y], 0x09);
			}
		}
	}
	c = g_Console.getConsoleSize();
	c.Y /= 3 + 10;
	c.X = c.X / 2 - 35;
	c.Y += 15;
	c.X = g_Console.getConsoleSize().X / 2 - 20;
	g_Console.writeToBuffer(c, "Press enter to choose your level (1-5).", 0x03);
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 7;
	g_Console.writeToBuffer(c, "Level 1", 0x03);
	c.Y += 1;
	g_Console.writeToBuffer(c, "Level 2", 0x73);
	c.Y += 1;
	g_Console.writeToBuffer(c, "Level 3", 0x03);
	c.Y += 1;
	g_Console.writeToBuffer(c, "Level 4", 0x03);
	c.Y += 1;
	g_Console.writeToBuffer(c, "Level 5", 0x03);

	if (g_abKeyPressed[PLAYER_2_K_UP])
	{
		g_eGameState = S_MAINMENU;
	}

	if (g_abKeyPressed[PLAYER_2_K_DOWN])
	{
		g_eGameState = S_MAINMENU3;
	}

	if (g_abKeyPressed[K_ESCAPE])
		g_bQuitGame = true;
}

void renderToMainMenu3()
{
	int i = 0;
	int j = 0;
	char main[73][12];
	ifstream file("PickALevel.txt"); // read file from PickALevel.txt to print the ascii art
	COORD c;
	if (file.is_open())
	{
		while (j <= 11)
		{
			while (i <= 72)
			{
				file >> main[i][j];
				i++;
			}
			i = 0;
			j++;
		}
		file.close();
	}
	for (int y = 0; y <= 11; y++)
	{
		c.Y = y + 4;
		for (int x = 0; x <= 72; x++)
		{
			c.X = x + 3;
			if (main[x][y] != '~')
			{
				g_Console.writeToBuffer(c, main[x][y], 0x09);
			}
		}
	}
	c = g_Console.getConsoleSize();
	c.Y /= 3 + 10;
	c.X = c.X / 2 - 35;
	c.Y += 15;
	c.X = g_Console.getConsoleSize().X / 2 - 20;
	g_Console.writeToBuffer(c, "Press enter to choose your level (1-5).", 0x03);
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 7;
	g_Console.writeToBuffer(c, "Level 1", 0x03);
	c.Y += 1;
	g_Console.writeToBuffer(c, "Level 2", 0x03);
	c.Y += 1;
	g_Console.writeToBuffer(c, "Level 3", 0x73);
	c.Y += 1;
	g_Console.writeToBuffer(c, "Level 4", 0x03);
	c.Y += 1;
	g_Console.writeToBuffer(c, "Level 5", 0x03);

	if (g_abKeyPressed[PLAYER_2_K_UP])
	{
		g_eGameState = S_MAINMENU2;
	}

	if (g_abKeyPressed[PLAYER_2_K_DOWN])
	{
		g_eGameState = S_MAINMENU4;
	}

	if (g_abKeyPressed[K_ESCAPE])
		g_bQuitGame = true;
}

void renderToMainMenu4()
{
	int i = 0;
	int j = 0;
	char main[73][12];
	ifstream file("PickALevel.txt"); // read file from PickALevel.txt to print the ascii art
	COORD c;
	if (file.is_open())
	{
		while (j <= 11)
		{
			while (i <= 72)
			{
				file >> main[i][j];
				i++;
			}
			i = 0;
			j++;
		}
		file.close();
	}
	for (int y = 0; y <= 11; y++)
	{
		c.Y = y + 4;
		for (int x = 0; x <= 72; x++)
		{
			c.X = x + 3;
			if (main[x][y] != '~')
			{
				g_Console.writeToBuffer(c, main[x][y], 0x09);
			}
		}
	}
	c = g_Console.getConsoleSize();
	c.Y /= 3 + 10;
	c.X = c.X / 2 - 35;
	c.Y += 15;
	c.X = g_Console.getConsoleSize().X / 2 - 20;
	g_Console.writeToBuffer(c, "Press enter to choose your level (1-5).", 0x03);
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 7;
	g_Console.writeToBuffer(c, "Level 1", 0x03);
	c.Y += 1;
	g_Console.writeToBuffer(c, "Level 2", 0x03);
	c.Y += 1;
	g_Console.writeToBuffer(c, "Level 3", 0x03);
	c.Y += 1;
	g_Console.writeToBuffer(c, "Level 4", 0x73);
	c.Y += 1;
	g_Console.writeToBuffer(c, "Level 5", 0x03);

	if (g_abKeyPressed[PLAYER_2_K_UP])
	{
		g_eGameState = S_MAINMENU3;
	}

	if (g_abKeyPressed[PLAYER_2_K_DOWN])
	{
		g_eGameState = S_MAINMENU5;
	}

	if (g_abKeyPressed[K_ESCAPE])
		g_bQuitGame = true;
}

void renderToMainMenu5()
{
	int i = 0;
	int j = 0;
	char main[73][12];
	ifstream file("PickALevel.txt"); // read file from PickALevel.txt to print the ascii art
	COORD c;
	if (file.is_open())
	{
		while (j <= 11)
		{
			while (i <= 72)
			{
				file >> main[i][j];
				i++;
			}
			i = 0;
			j++;
		}
		file.close();
	}
	for (int y = 0; y <= 11; y++)
	{
		c.Y = y + 4;
		for (int x = 0; x <= 72; x++)
		{
			c.X = x + 3;
			if (main[x][y] != '~')
			{
				g_Console.writeToBuffer(c, main[x][y], 0x09);
			}
		}
	}
	c = g_Console.getConsoleSize();
	c.Y /= 3 + 10;
	c.X = c.X / 2 - 35;
	c.Y += 15;
	c.X = g_Console.getConsoleSize().X / 2 - 20;
	g_Console.writeToBuffer(c, "Press enter to choose your level (1-5).", 0x03);
	c.Y += 1;
	c.X = g_Console.getConsoleSize().X / 2 - 7;
	g_Console.writeToBuffer(c, "Level 1", 0x03);
	c.Y += 1;
	g_Console.writeToBuffer(c, "Level 2", 0x03);
	c.Y += 1;
	g_Console.writeToBuffer(c, "Level 3", 0x03);
	c.Y += 1;
	g_Console.writeToBuffer(c, "Level 4", 0x03);
	c.Y += 1;
	g_Console.writeToBuffer(c, "Level 5", 0x73);

	if (g_abKeyPressed[PLAYER_2_K_UP])
	{
		g_eGameState = S_MAINMENU4;
	}

	if (g_abKeyPressed[K_ESCAPE])
		g_bQuitGame = true;
}