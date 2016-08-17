// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>

using namespace std; 

double  g_dElapsedTime;
double  g_dDeltaTime;
bool    g_abKeyPressed[K_COUNT];
bool	mapExtract = false;
bool	mapRender = false;

//string one[1] = {" _____                           _   _                                    "};
//string two[1] = { "|  ___|                         | | | |                                   " };
//string three[1] = { "| |__ ___  ___ __ _ _ __   ___  | |_| |__   ___   _ __ ___   __ _ _______ " };
//string four[1] = { "|  __/ __|/ __/ _` | '_ \ / _ \ | __| '_ \ / _ \ | '_ ` _ \ / _` |_  / _ \ " };
//string five[1] = { "| |__\__ \ (_| (_| | |_) |  __/ | |_| | | |  __/ | | | | | | (_| |/ /  __/" };
//string six[1] = { "\____/___/\___\__,_| .__/ \___|  \__|_| |_|\___| |_| |_| |_|\__,_/___\___|" };
//string seven[1] = { "                   | |                                                    " };
//string eight[1] = { "                   |_" };

char	txt[61][21];
char	wall = 178;
unsigned char direction;
unsigned char ground = 176;
string	teleport;
int stage;

// Game specific variables here
SGameChar	g_sChar;
EGAMESTATES g_eGameState = S_SPLASHSCREEN;
double  g_dBounceTime; // this is to prevent key bouncing, so we won't trigger keypresses more than once

// Console object
Console g_Console(100, 25, "SP1 Framework");

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

    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;

    g_sChar.m_cLocation.X = 7;
    g_sChar.m_cLocation.Y = 6;
    g_sChar.m_bActive = true;
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
    g_abKeyPressed[K_UP]     = isKeyPressed(VK_UP);
    g_abKeyPressed[K_DOWN]   = isKeyPressed(VK_DOWN);
    g_abKeyPressed[K_LEFT]   = isKeyPressed(VK_LEFT);
    g_abKeyPressed[K_RIGHT]  = isKeyPressed(VK_RIGHT);
    g_abKeyPressed[K_SPACE]  = isKeyPressed(VK_SPACE);
    g_abKeyPressed[K_ESCAPE] = isKeyPressed(VK_ESCAPE);
	g_abKeyPressed[K_RETURN] = isKeyPressed(VK_RETURN);
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
        case S_GAME: renderGame();
            break;
    }
    renderFramerate();  // renders debug information, frame rate, elapsed time, etc
    renderToScreen();   // dump the contents of the buffer to the screen, one frame worth of game
}

void splashScreenWait()    // waits for time to pass in splash screen
{
	if (g_abKeyPressed[K_RETURN]) // press enter to start game
	{
		stage = 0;
		g_eGameState = S_GAME;
	}
}

void gameplay()            // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
                        // sound can be played here too.
}

void teleportation()
{
	for (int i = 0; i < teleport.size() / 4; i++)
	{
		int j = 4;
		if (g_sChar.m_cLocation.X == teleport[i * j] && g_sChar.m_cLocation.Y == teleport[i * j + 1])
		{
			g_sChar.m_cLocation.X = teleport[i * j + 2];
			g_sChar.m_cLocation.Y = teleport[i * j + 3];
		}
		else if (g_sChar.m_cLocation.X == teleport[i * j + 2] && g_sChar.m_cLocation.Y == teleport[i * j + 3])
		{
			g_sChar.m_cLocation.X = teleport[i * j];
			g_sChar.m_cLocation.Y = teleport[i * j + 1];
		}
		j++;
	}
}

void moveCharacter()
{
	bool bSomethingHappened = false;
	if (g_dBounceTime > g_dElapsedTime)
		return;

	// Updating the location of the character based on the key press
	// providing a beep sound whenver we shift the character
	if (g_abKeyPressed[K_UP] && g_sChar.m_cLocation.Y > 0)
	{
		direction = 'u';
		//Beep(1440, 30);
		if (txt[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y - 2] != 'x')
		{
			g_sChar.m_cLocation.Y--;
			bSomethingHappened = true;
		}
	}
	if (g_abKeyPressed[K_LEFT] && g_sChar.m_cLocation.X > 0)
	{
		direction = 'l';
		//Beep(1440, 30);
		if (txt[g_sChar.m_cLocation.X - 1][g_sChar.m_cLocation.Y - 1] != 'x')
		{
			g_sChar.m_cLocation.X--;
			bSomethingHappened = true;
		}
	}
	if (g_abKeyPressed[K_DOWN] && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
	{
		direction = 'd';
		//Beep(1440, 30);
		if (txt[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y] != 'x')
		{
			g_sChar.m_cLocation.Y++;
			bSomethingHappened = true;
		}
	}
	if (g_abKeyPressed[K_RIGHT] && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 1)
	{
		direction = 'r';
		//Beep(1440, 30);
		if (txt[g_sChar.m_cLocation.X + 1][g_sChar.m_cLocation.Y - 1] != 'x')
		{
			g_sChar.m_cLocation.X++;
			bSomethingHappened = true;
		}
	}
	if (g_abKeyPressed[K_SPACE])
	{
		g_sChar.m_bActive = !g_sChar.m_bActive;
		bSomethingHappened = true;
	}

	if (bSomethingHappened)
	{
		teleportation();
		// set the bounce time to some time in the future to prevent accidental triggers
		g_dBounceTime = g_dElapsedTime; // 125ms should not be enough
		g_dBounceTime = g_dElapsedTime + 0.1; // 125ms should be enough
	}
	if (g_sChar.m_cLocation.X == 60 && g_sChar.m_cLocation.Y == 19)
	{
		stage++;
		mapExtract = false;
		mapRender = false;
	}
}

void processUserInput()
{
    // quits the game if player hits the escape key
    if (g_abKeyPressed[K_ESCAPE])
        g_bQuitGame = true;    
}

void clearScreen()
{
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(0x00);
}

void renderSplashScreen()  // renders the splash screen
{
	int i = 0;
	int j = 0;
	char splash[74][8];
	ifstream file("title.txt");
	COORD c;
	if (file.is_open())
	{
		while (j <= 7)
		{
			while (i <= 73)
			{
				file >> splash[i][j];
				i++;
			}
			i = 0;
			j++;
		}
		file.close();
	}
	for (int y = 0; y <= 7; y++)
	{
		c.Y = y + 4;
		for (int x = 0; x <= 73; x++)
		{
			c.X = x + 3;
			if (splash[x][y] != '~')
			{
				g_Console.writeToBuffer(c, splash[x][y], 0x09);
			}
		}
	}
	c = g_Console.getConsoleSize();
	c.Y /= 3;
	c.X = c.X / 2 - 35;
	c.Y += 5;
	c.X = g_Console.getConsoleSize().X / 2 - 15;
    g_Console.writeToBuffer(c, "Press <Enter> to continue", 0x03);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 22;
    g_Console.writeToBuffer(c, "Press <Space> to change character colour", 0x07);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 13;
    g_Console.writeToBuffer(c, "Press 'Esc' to quit", 0x07);
}

void renderGame()
{
	if (mapRender == false)
	{
		renderMap();// renders the map to the buffer first
		mapRender = true;
	}
    renderCharacter();  // renders the character into the buffer
}

void extractMap(int level)
{
	int i = 0;
	int j = 0;
	if (level == 0)
	{
		ifstream file("test.txt");
		if (file.is_open())
		{
			while (j <= 19)
			{
				while (i <= 59)
				{
					file >> txt[i][j];
					if (txt[i][j] == 'p')
					{
						teleport += i;
						teleport += j + 1;
					}
					i++;
				}
				i = 0;
				j++;
			}
			file.close();
		}
		mapExtract = true;
	}
	if (level == 1)
	{
		ifstream file("sampleLevel.txt");
		if (file.is_open())
		{
			while (j <= 19)
			{
				while (i <= 59)
				{
					file >> txt[i][j];
					if (txt[i][j] == 'p')
					{
						teleport += i;
						teleport += j + 1;
					}
					i++;
				}
				i = 0;
				j++;
			}
			file.close();
		}
		g_sChar.m_cLocation.X = 0;
		g_sChar.m_cLocation.Y = 19;
		mapExtract = true;
	}
}

void renderMap()
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
	if (mapExtract == false)
	{
		extractMap(stage);
	}
	COORD c;
	for (int y = 0; y <= 19; y++)
	{
		c.Y = y + 1;
		for (int x = 0; x <= 59; x++)
		{
			c.X = x;
			if (txt[x][y] == '-')
			{
				g_Console.writeToBuffer(c, ground, 0x1A);
			}
			if (txt[x][y] == 'x')
			{
				g_Console.writeToBuffer(c, wall);
			}
			if (txt[x][y] == 'p')
			{
				g_Console.writeToBuffer(c, wall, 0x2B);
			}
		}
	}
}

void renderCharacter()
{
    // Draw the location of the character
	if (direction == 'u')
	{
		WORD charColor = 0x0C;
		if (g_sChar.m_bActive)
		{
			charColor = 0x0A;
		}
		g_Console.writeToBuffer(g_sChar.m_cLocation, (unsigned char)202, charColor);
	}
	else if (direction == 'd')
	{
		WORD charColor = 0x0C;
		if (g_sChar.m_bActive)
		{
			charColor = 0x0A;
		}
		g_Console.writeToBuffer(g_sChar.m_cLocation, (unsigned char)203, charColor);
	}
	else if (direction == 'l')
	{
		WORD charColor = 0x0C;
		if (g_sChar.m_bActive)
		{
			charColor = 0x0A;
		}
		g_Console.writeToBuffer(g_sChar.m_cLocation, (unsigned char)185, charColor);
	}
	else if (direction == 'r')
	{
		WORD charColor = 0x0C;
		if (g_sChar.m_bActive)
		{
			charColor = 0x0A;
		}
		g_Console.writeToBuffer(g_sChar.m_cLocation, (unsigned char)204, charColor);
	}
	else
	{
		WORD charColor = 0x0C;
		if (g_sChar.m_bActive)
		{
			charColor = 0x0A;
		}
		g_Console.writeToBuffer(g_sChar.m_cLocation, (unsigned char)202, charColor);
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
