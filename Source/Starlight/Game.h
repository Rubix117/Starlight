#ifndef STARLIGHT_GAME_H_
#define STARLIGHT_GAME_H_

#include <sstream>
#include <iostream>
#include <map>

#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>

#include "Player.h"
#include "World.h"
#include "Dungeon.h"
#include "HUD.h"

namespace starlight
{
	class Game
	{
	public:
		static struct CAMERA
		{
			float x;
			float y;
			ALLEGRO_TRANSFORM transform;	// Transformation matrix
		} camera;
		
		static int FPS;		// Draw speed in Frames Per Second.
		static int WIDTH;	// Width of the game window in pixels.
		static int HEIGHT;	// Height of the game window in pixels.
		
		static bool gamePausedOn;	// If true, the game is rendered but not
		static bool showStatsOn;	// If true, the player's stats are displayed.
		static bool showWallsOn;	// If true, all obstacle tiles are marked with hideous magenta.
		static bool showBoundsOn;	// If true, the bounds of objects are displayed.
		static bool showPathsOn;	// If true, the paths of enemies are displayed.
		static bool showInventoryOn;// If true, the player's inventory is displayed.

		Game(const int &MAP_ROWS, const int &MAP_COLS, const int &WIDTH = 1024, const int &HEIGHT = 768, const int &FPS = 60);
		~Game();

	private:
		enum STATE
		{
			TITLE_MENU = 0, 
			IN_GAME, 
			PAUSED, 
			GAME_OVER
		};

		bool Initialise();
		void DrawLoadingScreen();
		bool LoadContent();
		void StartGame();
		void CleanUp();
		void Update();
		void Draw();
		void KeyPressed(const ALLEGRO_KEYBOARD_EVENT &ev);
		void KeyReleased(const ALLEGRO_KEYBOARD_EVENT &ev);
		void MousePressed(const ALLEGRO_MOUSE_EVENT &ev);
		void MouseReleased(const ALLEGRO_MOUSE_EVENT &ev);
		void Load();
		void Save();
		void PrintGameControls();

		ALLEGRO_BITMAP* LoadImage(const char *filename);
		ALLEGRO_BITMAP* LoadImage(const char *filename, ALLEGRO_COLOR maskColour);
		ALLEGRO_AUDIO_STREAM* LoadAudioStream(const char *filename);
		ALLEGRO_FONT* LoadFont(const char *filename, const int &fontSize);

		STATE currentState;				//Enum stating the current game state.
		bool done;						//True if game loop should stop.
		bool redraw;					//True if rendering should be done on the back buffer before drawing the back buffer to the screen.
		bool isMouseButtonDown[3];		//Stores the state of the left, right and middle mouse buttons. mouseButton[i] is true if the mouse button is held down, false otherwise.
		bool isKeyDown[ALLEGRO_KEY_MAX];//Stores the state of all keyboard keys. isKeyDown[i] is true if the isKeyDown is held down, false otherwise.
		World *worldMap;				//Stores a randomly generated world map.
		Map *currentMap;				//map currently in use by player.
		std::map<int, Dungeon*> maps;	//stores all maps entered by player and their coordinates relative to the world map.
		Enemy* selectedEnemy;			//Stores pointer to enemy clicked on which is passed to HUD to show stats of.

		//**Content Variables**//
		ALLEGRO_BITMAP *titleImage;
		ALLEGRO_BITMAP *mapSpriteSheet;
		ALLEGRO_BITMAP *treeSpriteSheet;
		ALLEGRO_BITMAP *playerSpriteSheet;
		ALLEGRO_BITMAP *spiderSpriteSheet;
		ALLEGRO_BITMAP *lavaTrollSpriteSheet;
		ALLEGRO_BITMAP *spiderProjectileSpriteSheet;
		ALLEGRO_BITMAP *barbarianSpriteSheet;
		ALLEGRO_BITMAP *skeletonSpriteSheet;
		ALLEGRO_BITMAP *zombieSpriteSheet;
		ALLEGRO_BITMAP *ninjaSpriteSheet;
        ALLEGRO_BITMAP *weaponsSpriteSheet;
        ALLEGRO_BITMAP *armourSpriteSheet;
        ALLEGRO_BITMAP *potionSpriteSheet;

		ALLEGRO_VOICE *voice;
		ALLEGRO_MIXER *mixer;
		ALLEGRO_AUDIO_STREAM *stream;

		ALLEGRO_FONT *mediumFont;
		ALLEGRO_FONT *bigFont;

		//**Allegro Variables**//
		ALLEGRO_DISPLAY *display;
		ALLEGRO_EVENT_QUEUE *eventQueue;
		ALLEGRO_TIMER *eventTimer;

		//**Game Objects**//
		Player *player;
		HUD *hud;
	};
}

#endif // STARLIGHT_GAME_H_