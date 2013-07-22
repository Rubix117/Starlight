#include <vector>
#include <fstream>

#include <allegro5\allegro_native_dialog.h>

#include "Game.h"
#include "Input.h"
#include "Enemy.h"
#include "GameObjectFactory.h"
#include "GameMath.h"

namespace starlight
{
	Game::CAMERA Game::camera;

	//set in constructor body
	int Game::FPS;
	int Game::WIDTH;
	int Game::HEIGHT;

	bool Game::gamePausedOn = false;
	bool Game::showStatsOn = false;
	bool Game::showWallsOn = false;
	bool Game::showBoundsOn = false;
	bool Game::showPathsOn = false;
	bool Game::showInventoryOn = false;

	Game::Game(const int &MAP_ROWS, const int &MAP_COLS, const int &WIDTH, const int &HEIGHT, const int &FPS)
		: currentState(IN_GAME),
		done(false),
		redraw(false),
		worldMap(new World(MAP_ROWS, MAP_COLS)),
		currentMap(worldMap),

		mapSpriteSheet(NULL),
		playerSpriteSheet(NULL),
		spiderSpriteSheet(NULL),
		lavaTrollSpriteSheet(NULL),		
		barbarianSpriteSheet(NULL),
		skeletonSpriteSheet(NULL),
		zombieSpriteSheet(NULL),
		ninjaSpriteSheet(NULL),

		weaponsSpriteSheet(NULL),
		armourSpriteSheet(NULL),
		potionSpriteSheet(NULL),

		voice(NULL),
		mixer(NULL),
		stream(NULL),

		mediumFont(NULL),
		bigFont(NULL),
		display(NULL),
		eventQueue(NULL),
		eventTimer(NULL),
		player(NULL),
		hud(NULL),
		selectedEnemy(NULL)
	{
		Game::WIDTH = WIDTH;
		Game::HEIGHT = HEIGHT;
		Game::FPS = FPS;

		if(!Initialise())
		{
			return;
		}

		DrawLoadingScreen();

		if(!LoadContent())
		{
			return;
		}

		StartGame();
	}

	Game::~Game()
	{
		Save();
		CleanUp();
	}

	bool Game::Initialise()
	{
		for(int i = 0; i < 3; i++)
			isMouseButtonDown[i] = false;

		for(int i = 0; i < ALLEGRO_KEY_MAX; i++)
			isKeyDown[i] = false;

		//**Allegro Initialisation**//
		if(!al_init()) 
		{
			al_show_native_message_box(NULL, "Fatal Error!", "Allegro: Failed to initialise", "", NULL, ALLEGRO_MESSAGEBOX_ERROR);
			return false;
		}

		display = al_create_display(Game::WIDTH, Game::HEIGHT);
		if(!display)
		{
			al_show_native_message_box(NULL, "Fatal Error!", "Display: Failed to initialise", "", NULL, ALLEGRO_MESSAGEBOX_ERROR);
			return false;
		}

		al_set_window_title(display, "Starlight");

		//**Addons Initialisation**//
		al_install_keyboard();			//Enable keyboard input.
		al_install_mouse();				//Enable mouse input.
		al_init_primitives_addon();		//Enables drawing of primitive shapes.
		al_init_image_addon();			//Enables support for BMP, PCX, TGA image formats. JPEG and PNG are already supported on all systems via external dependencies.
		al_init_font_addon();			//Enables support for processing fonts.
		al_init_ttf_addon();			//Enables support for TrueType, OpenType, Type1, CID, CFF, Windows FON/FNT, X11 PCF, and other bigFont font formats.
		al_install_audio();				//Enables support for processing audio.
		al_init_acodec_addon();			//Enables support for WAV, FLAC, OGG, IT, MOD, S3M, XM audio formats.

		//**Allegro Variable Initialisation**//
		eventQueue = al_create_event_queue();
		eventTimer = al_create_timer(1.0/FPS);

		//**Register Event Sources To Event Queue**//
		al_register_event_source(eventQueue, al_get_display_event_source(display));
		al_register_event_source(eventQueue, al_get_mouse_event_source());
		al_register_event_source(eventQueue, al_get_keyboard_event_source());
		al_register_event_source(eventQueue, al_get_timer_event_source(eventTimer));

		return true;
	}

	void Game::DrawLoadingScreen()
	{
		titleImage = LoadImage("Content/Sprite Sheets/starlight_logo.png");
		bigFont = LoadFont("Content/Fonts/Ubuntu_Bold.ttf", 30);
		Vector2 titleSize(al_get_bitmap_width(titleImage), al_get_bitmap_height(titleImage));
		Vector2 titlePosition = (Vector2(Game::WIDTH, Game::HEIGHT) - titleSize) / 2;
		titlePosition.y -= 100;

		al_draw_filled_rectangle(0, 0, Game::WIDTH, Game::HEIGHT, al_map_rgb(0, 0, 0));
		al_draw_bitmap(titleImage, titlePosition.x, titlePosition.y, NULL);
		al_draw_text(bigFont, al_map_rgb_f(1.0, 1.0, 1.0), Game::WIDTH/2, titlePosition.y + titleSize.y + 100, ALLEGRO_ALIGN_CENTRE, "Loading");
		al_flip_display();
	}

	bool Game::LoadContent()
	{
		//**Images**//
		mapSpriteSheet = LoadImage("Content/Sprite Sheets/map_sheet_small.bmp");
		treeSpriteSheet = LoadImage("Content/Sprite Sheets/trees.bmp", al_map_rgb(255, 0, 255));
		playerSpriteSheet = LoadImage("Content/Sprite Sheets/knight.bmp", al_map_rgb(106, 76, 48));
		weaponsSpriteSheet = LoadImage("Content/Sprite Sheets/weapons.bmp", al_map_rgb(191, 123, 199));
		armourSpriteSheet = LoadImage("Content/Sprite Sheets/armour.bmp", al_map_rgb(191, 123, 199));
		potionSpriteSheet = LoadImage("Content/Sprite Sheets/potions.bmp", al_map_rgb(181, 101, 181));

		//--Enemy Images--//
		spiderSpriteSheet		= LoadImage("Content/Sprite Sheets/spider.bmp",		al_map_rgb(97, 68, 43));
		lavaTrollSpriteSheet	= LoadImage("Content/Sprite Sheets/lava_troll.bmp", al_map_rgb(106, 76, 48));
		barbarianSpriteSheet	= LoadImage("Content/Sprite Sheets/barbarian.bmp",	al_map_rgb(106, 76, 48));
		skeletonSpriteSheet		= LoadImage("Content/Sprite Sheets/skeleton.bmp",	al_map_rgb(106, 76, 48));
		zombieSpriteSheet		= LoadImage("Content/Sprite Sheets/zombie.bmp",		al_map_rgb(106, 76, 48));
		ninjaSpriteSheet		= LoadImage("Content/Sprite Sheets/ninja.bmp",		al_map_rgb(106, 76, 48));

		//--Projectiles--//
		spiderProjectileSpriteSheet = LoadImage("Content/Sprite Sheets/web_projectile.bmp", al_map_rgb(128, 128, 128));

		GameObjectFactory *factory = GameObjectFactory::GetInstance();
		factory->SetSpriteSheet(GameObject::RANDOM_WEAPON,	weaponsSpriteSheet);
		factory->SetSpriteSheet(GameObject::RANDOM_ARMOUR,	armourSpriteSheet);
		factory->SetSpriteSheet(GameObject::RANDOM_POTION,	potionSpriteSheet);
		factory->SetSpriteSheet(GameObject::PLAYER_MELEE,	playerSpriteSheet);
		factory->SetSpriteSheet(GameObject::SPIDER,			spiderSpriteSheet);
		factory->SetSpriteSheet(GameObject::LAVA_TROLL,		lavaTrollSpriteSheet);
		factory->SetSpriteSheet(GameObject::BARBARIAN,		barbarianSpriteSheet);
		factory->SetSpriteSheet(GameObject::SKELETON,		skeletonSpriteSheet);
		factory->SetSpriteSheet(GameObject::ZOMBIE,			zombieSpriteSheet);
		factory->SetSpriteSheet(GameObject::NINJA,			ninjaSpriteSheet);

		factory->SetProjectileSpriteSheet(GameObject::SPIDER, spiderProjectileSpriteSheet);

		if(factory->AreAllSpriteSheetsSet() == false)
		{
			al_show_native_message_box(display, "Fatal Error!", "Game Class: Not all sprites have been set", "", NULL, ALLEGRO_MESSAGEBOX_ERROR);
			return false;
		}

		voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
		mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
		stream = LoadAudioStream("Content/Music/overworld.ogg");

		if(stream)
		{
			al_set_audio_stream_playmode(stream, ALLEGRO_PLAYMODE_LOOP);
			al_attach_mixer_to_voice(mixer, voice);
			al_attach_audio_stream_to_mixer(stream, mixer);
		}

		//--Fonts--//
		mediumFont = LoadFont("Content/Fonts/Ubuntu_Bold.ttf", 22);

		// Initialise content dependent objects
		player = factory->CreatePlayer(GameObject::PLAYER_MELEE, *currentMap->spawnCoords, 1);

		worldMap->GenerateEnemies(*player);
		Tile::treeSpriteSheet = treeSpriteSheet;

		hud = new HUD(mediumFont, bigFont);

		Load();

		return true;
	}

	void Game::StartGame()
	{
		PrintGameControls();

		//**Start Timer**//
		al_start_timer(eventTimer);

		//**Game Loop**//
		while(!done)
		{
			//**Block Until Event Is Fired**//
			ALLEGRO_EVENT ev;
			al_wait_for_event(eventQueue, &ev);

			switch(ev.type)
			{
			case ALLEGRO_EVENT_TIMER:
				Update();
				break;
			case ALLEGRO_EVENT_KEY_DOWN:
				KeyPressed(ev.keyboard);
				break;
			case ALLEGRO_EVENT_KEY_UP:
				KeyReleased(ev.keyboard);
				break;
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
				MousePressed(ev.mouse);	//mouse buttons are LeftClick = 1, RightClick = 2, MiddleClick = 3 so it must be offset by one for the array
				break;
			case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
				MouseReleased(ev.mouse);
				break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				done = true;
				break;
			}

			//**Rendering Section**//
			if(redraw && al_is_event_queue_empty(eventQueue))
			{
				Draw();
			}
		}
	}

	void Game::CleanUp()
	{
		//**Clean Up**//
		//--Images--//
		al_destroy_bitmap(mapSpriteSheet);
		al_destroy_bitmap(playerSpriteSheet);
		al_destroy_bitmap(spiderSpriteSheet);
		al_destroy_bitmap(weaponsSpriteSheet);

		//--Audio--//
		al_destroy_audio_stream(stream);
		al_destroy_mixer(mixer);
		al_destroy_voice(voice);

		//--Fonts--//
		al_destroy_font(mediumFont);
		al_destroy_font(bigFont);

		//--Allegro Variables--//
		al_destroy_display(display);
		al_destroy_event_queue(eventQueue);
		al_destroy_timer(eventTimer);

		//--Game Objects--//
		delete player;
		delete hud;
	}

	ALLEGRO_BITMAP* Game::LoadImage(const char *filename)
	{
		ALLEGRO_BITMAP *image = al_load_bitmap(filename); 

		if(!image)  //Check if image loaded correctly
		{
			std::stringstream sstream;
			sstream << "Image: Failed to load " << filename;
			al_show_native_message_box(al_get_current_display(), "Failed to Load Image!", sstream.str().c_str(), "", NULL, ALLEGRO_MESSAGEBOX_ERROR);
			return NULL;
		}

		return image;
	}

	ALLEGRO_BITMAP* Game::LoadImage(const char *filename, ALLEGRO_COLOR maskColour)
	{
		ALLEGRO_BITMAP *image = LoadImage(filename);
		al_convert_mask_to_alpha(image, maskColour);  //Remove mask colour from image background

		return image;
	}

	ALLEGRO_AUDIO_STREAM* Game::LoadAudioStream(const char *filename)
	{
		ALLEGRO_AUDIO_STREAM *stream = al_load_audio_stream(filename, 4, 2048);

		if(!stream)  //Check if image loaded correctly
		{
			std::stringstream sstream;
			sstream << "Audio Stream: Failed to load " << filename;
			al_show_native_message_box(al_get_current_display(), "Failed to Load Audio Stream!", sstream.str().c_str(), "", NULL, ALLEGRO_MESSAGEBOX_ERROR);
			return NULL;
		}

		return stream;
	}

	ALLEGRO_FONT* Game::LoadFont(const char *filename, const int &fontSize)
	{
		ALLEGRO_FONT *bigFont = al_load_font(filename, fontSize, NULL);

		if(!bigFont)  //Check if bigFont loaded correctly
		{
			std::stringstream sstream;
			sstream << "Font: Failed to load " << filename;
			al_show_native_message_box(al_get_current_display(), "Failed to Load Font!", sstream.str().c_str(), "", NULL, ALLEGRO_MESSAGEBOX_ERROR);
			return NULL;
		}

		return bigFont;
	}

	void Game::Update()
	{
		if(isKeyDown[KEY_ESC])
		{
			done = true;
			return;
		}

		if(!Game::gamePausedOn)
		{
			if(player->state == Character::DEAD) // If dead, respawn at last entry point into the map
			{
				player->state = Character::IDLE;
				player->position = (*currentMap->spawnCoords * Tile::TILE_SIZE).toV2F();
				player->previousCoords = *currentMap->spawnCoords;
				player->health = player->maxHealth;
				player->Rest();
			}
			else if(player->hasMoved)
			{
				Vector2 playerCoords = player->GetCoords();
				if(currentMap == worldMap)
				{
					if(worldMap->At(playerCoords).type == Tile::DOOR) // Enter dungeon from world map
					{
						int tileID = playerCoords.y*currentMap->COLS + playerCoords.x;

						if(maps[tileID] == NULL) // if no dungeon is associated with this tileID then create one.
						{
							maps[tileID] = new Dungeon(50, 50, 10);
							maps[tileID]->upperLevel.map = worldMap;

							maps[tileID]->GenerateEnemies(*player);
						}

						*worldMap->spawnCoords = playerCoords;
						player->previousCoords = *maps[tileID]->upperLevel.coords;
						player->position = maps[tileID]->upperLevel.coords->toV2F() * Tile::TILE_SIZE;

						currentMap = maps[tileID];
					}
				}
				else if(playerCoords == *dynamic_cast<Dungeon*>(currentMap)->upperLevel.coords)
				{
					Dungeon *currentDungeon = dynamic_cast<Dungeon*>(currentMap);

					if(currentDungeon->upperLevel.map == worldMap) // Go up a level to the world map from the dungeon
					{
						Vector2 dungeonCoords;
						for(std::map<int, Dungeon*>::iterator it = maps.begin(); it != maps.end(); ++it)
						{
							if(it->second == currentMap)
							{
								dungeonCoords.x = GameMath::Mod(it->first, worldMap->COLS);
								dungeonCoords.y = it->first / worldMap->COLS;
								break;
							}
						}

						player->previousCoords = dungeonCoords;
						player->position = dungeonCoords.toV2F() * Tile::TILE_SIZE;

						currentMap = worldMap;
					}
					else // Go up a level from one dungeon to another
					{
						currentMap = currentDungeon->upperLevel.map;
						Dungeon *nextDungeon = dynamic_cast<Dungeon*>(currentDungeon->upperLevel.map);

						player->previousCoords = *nextDungeon->lowerLevel.coords;
						player->position = nextDungeon->lowerLevel.coords->toV2F() * Tile::TILE_SIZE;					
					}
				}
				else if(playerCoords == *dynamic_cast<Dungeon*>(currentMap)->lowerLevel.coords) // Go down a level from one to another dungeon
				{
					Dungeon *currentDungeon = dynamic_cast<Dungeon*>(currentMap);
					Dungeon *nextDungeon;
					if(currentDungeon->lowerLevel.map == NULL)
					{
						currentDungeon->lowerLevel.map = new Dungeon(50, 50, 10);
						nextDungeon = dynamic_cast<Dungeon*>(currentDungeon->lowerLevel.map);
						nextDungeon->upperLevel.map = currentDungeon;

						nextDungeon->GenerateEnemies(*player);
					}
					else
					{
						nextDungeon = dynamic_cast<Dungeon*>(currentDungeon->lowerLevel.map);
					}

					player->previousCoords = *nextDungeon->upperLevel.coords;
					player->position = nextDungeon->upperLevel.coords->toV2F() * Tile::TILE_SIZE;

					currentMap = nextDungeon;
				}
			}

			player->Update(isKeyDown, *currentMap);
			currentMap->Update(*player);
		}

		redraw = true;
	}

	void Game::Draw()
	{
		redraw = false;

		Game::camera.x = -(player->position.x - (Game::WIDTH - player->size.x)/2);
		Game::camera.y = -(player->position.y - (Game::HEIGHT - player->size.y)/2);

		al_identity_transform(&camera.transform);
		al_translate_transform(&camera.transform, Game::camera.x, Game::camera.y);
		al_use_transform(&camera.transform);

		switch(currentState)
		{
		case TITLE_MENU:
			break;
		case IN_GAME:
			currentMap->Draw(mapSpriteSheet, *player);
			player->Draw();
			hud->Draw(*player, selectedEnemy);
			break;
		case PAUSED:
			break;
		case GAME_OVER:
			break;
		}

		al_flip_display();
		al_clear_to_color(al_map_rgb(0,0,0));
	}

	void Game::KeyPressed(const ALLEGRO_KEYBOARD_EVENT &ev)
	{
		isKeyDown[ev.keycode] = true;
	}

	void Game::KeyReleased(const ALLEGRO_KEYBOARD_EVENT &ev)
	{
		isKeyDown[ev.keycode] = false;

		// Toggle options On/Off
		switch(ev.keycode) 
		{
		case KEY_C:
			showStatsOn = !showStatsOn;
			std::cout << "Show Stats " << (showStatsOn ? "ON" : "OFF") << std::endl;
			break;
		case KEY_I:
			showInventoryOn = !showInventoryOn;
			std::cout << "Show Inventory " << (showInventoryOn ? "ON" : "OFF") << std::endl;
			break;
		case KEY_M:
			std::cout << *currentMap << std::endl; //Print map to console
			break;
		case KEY_P:
			gamePausedOn = !gamePausedOn;
			std::cout << "Game Paused " << (gamePausedOn ? "ON" : "OFF") << std::endl;
			break;
		case KEY_F1:
			showWallsOn = !showWallsOn;
			std::cout << "Show Walls " << (showWallsOn ? "ON" : "OFF") << std::endl;
			break;
		case KEY_F2:
			showBoundsOn = !showBoundsOn;
			std::cout << "Show Bounds " << (showBoundsOn ? "ON" : "OFF") << std::endl;
			break;
		case KEY_F3:
			showPathsOn = !showPathsOn;
			std::cout << "Show Paths " << (showPathsOn ? "ON" : "OFF") << std::endl;
			break;
		}
	}

	void Game::MousePressed(const ALLEGRO_MOUSE_EVENT &ev)
	{
		isMouseButtonDown[ev.button - 1] = true;
	}

	void Game::MouseReleased(const ALLEGRO_MOUSE_EVENT &ev)
	{
		isMouseButtonDown[ev.button - 1] = false;

		ALLEGRO_MOUSE_STATE mouseState;
		al_get_mouse_state(&mouseState);

		int x = mouseState.x - camera.x;
		int y = mouseState.y - camera.y;

		if(ev.button == MOUSE_BUTTON_LEFT)
		{
			if(player->inventory.size() < player->GetInventoryMaxSize())
			{
				for(std::vector<Weapon*>::iterator it = currentMap->weapons.begin(); it != currentMap->weapons.end(); ++it)
				{
					Rectangle weaponBounds = (*it)->GetBounds();
					if(weaponBounds.Contains(x, y))
					{
						std::cout <<"Added to Inventory: "<< (*it)->name << std::endl;
						Weapon *toAddToInventory = *it;
						it = currentMap->weapons.erase(it);
						player->inventory.push_back(toAddToInventory);

						break;
					}
				}

				for(std::vector<Potion*>::iterator it = currentMap->potions.begin(); it != currentMap->potions.end(); ++it)
				{
					Rectangle potionBounds = (*it)->GetBounds();
					if(potionBounds.Contains(x, y))
					{
						std::cout <<"Added to Inventory: "<< (*it)->name << std::endl;
						Potion *toAddToInventory = *it;
						it = currentMap->potions.erase(it);
						player->inventory.push_back(toAddToInventory);

						break;
					}
				}

				for(std::vector<Armour*>::iterator it = currentMap->armour.begin(); it != currentMap->armour.end(); ++it)
				{
					Rectangle armourBounds = (*it)->GetBounds();
					if(armourBounds.Contains(x, y))
					{
						std::cout <<"Added to Inventory: "<< (*it)->name << std::endl;
						Armour *toAddToInventory = *it;
						it = currentMap->armour.erase(it);
						player->inventory.push_back(toAddToInventory);

						break;
					}
				}
			}

			selectedEnemy = NULL;
			for(std::vector<Enemy*>::iterator it = currentMap->enemies.begin(); it != currentMap->enemies.end(); ++it)
			{
				Rectangle enemyBounds = (*it)->GetBounds();
				if(enemyBounds.Contains(x, y))
				{
					selectedEnemy = *it;
					break;
				}
			}
		}
		else if(ev.button == MOUSE_BUTTON_RIGHT)
		{
			if(Game::showInventoryOn)
			{
				for(std::vector<Item*>::iterator it = player->inventory.begin(); it != player->inventory.end(); ++it)
				{
					Rectangle itemBounds = (*it)->GetBounds();

					if(itemBounds.Contains(x, y))
					{
						std::cout <<"Equipping "<< (*it)->name << std::endl; 

						if((*it)->mainType == Item::WEAPON_ITEM)
						{
							player->EquipWeapon(dynamic_cast<Weapon*>(*it));
						}
						else if((*it)->mainType == Item::ARMOUR_ITEM)
						{
							if((*it)->subtype == Item::BREASTPLATE)
							{
								player->EquipBreastPlate(dynamic_cast<Armour*>(*it));
							}
							else if((*it)->subtype == Item::HELMET)
							{
								player->EquipHelmet(dynamic_cast<Armour*>(*it));
							}
						}
						else if((*it)->mainType == Item::POTION_ITEM && ((*it)->subtype == Item::SMALL_HEALTH || (*it)->subtype == Item::BIG_HEALTH))
						{
							player->UseHealth(dynamic_cast<Potion*>(*it));
						}
						else if((*it)->mainType == Item::POTION_ITEM && ((*it)->subtype == Item::SMALL_MANA || (*it)->subtype == Item::BIG_MANA))
						{
							player->UseMana(dynamic_cast<Potion*>(*it));
						}
					}
				}
			}
		}
	}

	void Game::Load()
	{
		std::ifstream fileIn("starlight.sav");

		if(fileIn.good())
		{
			player->Load(fileIn);
		}

		fileIn.close();
	}

	void Game::Save()
	{
		std::ofstream fileOut("starlight.sav");

		if(fileOut.good())
		{
			player->Save(fileOut);
		}

		fileOut.close();
	}

	void Game::PrintGameControls()
	{
		std::cout <<"Controls"<< std::endl
			<<"------------"<< std::endl
			<<"W or UP    |  move up"<< std::endl
			<<"S or DOWN  |  move down"<< std::endl
			<<"A or LEFT  |  move left"<< std::endl
			<<"D or RIGHT |  move right"<< std::endl
			<< std::endl
			<<"Space      |  attack"<< std::endl
			<<"Left Click |  pickup item; show enemy stats"<< std::endl
			<<"Esc        |  save + exit game"<< std::endl
			<< std::endl
			<<"C          |  show player stats"<< std::endl
			<<"I          |  show inventory"<< std::endl
			<<"M          |  print the map to the console"<< std::endl
			<<"P          |  pause/unpause game"<< std::endl
			<< std::endl
			<<"F1         |  highlight obstacles"<< std::endl
			<<"F2         |  show bounds boxes"<< std::endl
			<<"F3         |show enemy paths"<< std::endl;
	}
}