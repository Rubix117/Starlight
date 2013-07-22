#include <sstream>

#include "HUD.h"
#include "Game.h"

namespace starlight
{
	const int HUD::PADDING = 10;

	HUD::HUD(ALLEGRO_FONT *mediumFont, ALLEGRO_FONT *bigFont) :
		mediumFont(mediumFont),
		MEDIUM_TEXT_HEIGHT(al_get_font_line_height(mediumFont)),
		bigFont(bigFont),
		BIG_TEXT_HEIGHT(al_get_font_line_height(mediumFont))
	{		

	}

	HUD::~HUD()
	{
	}

	void HUD::Draw(const Player &player, Enemy *selectedEnemy)
	{
		al_identity_transform(&Game::camera.transform);
		al_translate_transform(&Game::camera.transform, 0, 0);
		al_use_transform(&Game::camera.transform);

		if(!Game::gamePausedOn)
		{
			if(Game::showStatsOn)
			{
				DrawPlayerStats(player);
			}

			if(Game::showInventoryOn)
			{
				DrawInventory(player);
			}

			if(selectedEnemy != NULL && selectedEnemy->state != Character::DEAD)
			{
				DrawEnemyStats(*selectedEnemy);
			}
		}
		else
		{
			DrawPausedOverlay();
		}
	}

	void HUD::DrawPlayerStats(const Player &player)
	{
		const int NUM_TEXT_LABELS = 10;
		const Vector2 SIZE(250, NUM_TEXT_LABELS*MEDIUM_TEXT_HEIGHT + 4*PADDING);
		const Vector2F POSITION(PADDING, Game::HEIGHT - SIZE.y - PADDING);
		const Rectangle BOUNDS(POSITION, SIZE);

		int expNeededToLevel = pow(2.0, player.level + 1) - player.experience;

		std::string statText[NUM_TEXT_LABELS];
		statText[0]	= "HP: "		+ std::to_string(player.health) + "/" + std::to_string(player.maxHealth);
		statText[1] = "LVL: "		+ std::to_string(player.level);
		statText[2] = "EXP-TO-LVL: "+ std::to_string(expNeededToLevel);
		statText[3] = "";
		statText[4] = "EXP: "		+ std::to_string(player.experience);
		statText[5] = "STR: "		+ std::to_string(player.strength);
		statText[6] = "DEX: "		+ std::to_string(player.dexterity);
		statText[7] = "CON: "		+ std::to_string(player.constitution);
		statText[8] = "INT: "		+ std::to_string(player.intelligence);
		statText[9] = "WIS: "		+ std::to_string(player.wisdom);

		al_draw_filled_rectangle(BOUNDS.Left(), BOUNDS.Top() - MEDIUM_TEXT_HEIGHT, BOUNDS.Right(), BOUNDS.Bottom(), al_map_rgba_f(0, 0, 0, 0.4));
		al_draw_filled_rectangle(BOUNDS.Left() + PADDING, BOUNDS.Top() + PADDING, BOUNDS.Right() - PADDING, BOUNDS.Bottom() - PADDING, al_map_rgba_f(0, 0, 0, 0.4));
		al_draw_text(mediumFont, al_map_rgb_f(1.0, 1.0, 1.0), BOUNDS.Left() + PADDING, BOUNDS.Top() - MEDIUM_TEXT_HEIGHT, ALLEGRO_ALIGN_LEFT, "Stats");

		for (int i = 0; i < NUM_TEXT_LABELS; i++)
		{
			al_draw_text(mediumFont, al_map_rgb(255, 255, 255), BOUNDS.Left() + 2*PADDING, BOUNDS.Top() +(i*MEDIUM_TEXT_HEIGHT) + 2*PADDING, ALLEGRO_ALIGN_LEFT, statText[i].c_str());
		}
	}

	void HUD::DrawInventory(const Player &player)
	{
		const Vector2 NUM_SLOTS(player.INVENTORY_SIZE);
		const Vector2 SLOT_SIZE(80, 80);

		const Vector2 SIZE(NUM_SLOTS*SLOT_SIZE + Vector2(1, 1)*PADDING);
		const Vector2F POSITION = (Vector2(Game::WIDTH, Game::HEIGHT) - (SIZE + Vector2(1, 1)*PADDING)).toV2F();
		const Rectangle BOUNDS(POSITION, SIZE);

		al_draw_filled_rectangle(BOUNDS.Left(), BOUNDS.Top() - MEDIUM_TEXT_HEIGHT, BOUNDS.Right(), BOUNDS.Bottom(), al_map_rgba_f(0, 0, 0, 0.4));
		al_draw_text(mediumFont, al_map_rgb_f(1.0, 1.0, 1.0), BOUNDS.Left() + PADDING, BOUNDS.Top() - MEDIUM_TEXT_HEIGHT, ALLEGRO_ALIGN_LEFT, "Inventory");

		std::vector<Item*>::const_iterator it = player.inventory.begin();

		for (int y = 0; y < NUM_SLOTS.y; y++)
		{
			for (int x = 0; x < NUM_SLOTS.x; x++)
			{
				Vector2F slotPos(x, y);
				slotPos = slotPos*SLOT_SIZE.toV2F() + BOUNDS.position;

				al_draw_filled_rectangle(slotPos.x + PADDING, slotPos.y + PADDING, slotPos.x + SLOT_SIZE.x, slotPos.y + SLOT_SIZE.y, al_map_rgba_f(0, 0, 0, 0.4));

				if(it != player.inventory.end())
				{
					const Vector2F DELTA_SIZE = (SLOT_SIZE - (*it)->size).toV2F() / 2;
					Vector2 framePosition = (*it)->frameCoords * (*it)->size;
					(*it)->position = slotPos + DELTA_SIZE;

					al_draw_bitmap_region((*it)->image, framePosition.x, framePosition.y, (*it)->size.x, (*it)->size.y, (*it)->position.x, (*it)->position.y, NULL);
					++it;
				}
			}
		}
	}

	void HUD::DrawPausedOverlay()
	{
		al_draw_filled_rectangle(0, 0, Game::WIDTH, Game::HEIGHT, al_map_rgba_f(0, 0, 0, 0.3));

		const float TEXT_Y_POSITION = (Game::HEIGHT - al_get_font_line_height(bigFont)) / 2;
		al_draw_text(bigFont, al_map_rgb_f(1.0, 1.0, 1.0), Game::WIDTH/2, TEXT_Y_POSITION, ALLEGRO_ALIGN_CENTRE, "Game Paused");
	}

	void HUD::DrawEnemyStats(const Enemy &enemy)
	{
		const int NUM_TEXT_LABELS = 9;
		const Vector2 SIZE(250, NUM_TEXT_LABELS*MEDIUM_TEXT_HEIGHT + 4*PADDING);
		const Vector2F POSITION(PADDING, PADDING+MEDIUM_TEXT_HEIGHT);
		const Rectangle BOUNDS(POSITION, SIZE);

		std::string statText[NUM_TEXT_LABELS];
		statText[0]	= "HP: "		+ std::to_string(enemy.health) + "/" + std::to_string(enemy.maxHealth);
		statText[1] = "LVL: "		+ std::to_string(enemy.level);
		statText[2] = "";
		statText[3] = "EXP: "		+ std::to_string(enemy.experience);
		statText[4] = "STR: "		+ std::to_string(enemy.strength);
		statText[5] = "DEX: "		+ std::to_string(enemy.dexterity);
		statText[6] = "CON: "		+ std::to_string(enemy.constitution);
		statText[7] = "INT: "		+ std::to_string(enemy.intelligence);
		statText[8] = "WIS: "		+ std::to_string(enemy.wisdom);

		al_draw_filled_rectangle(BOUNDS.Left(), BOUNDS.Top() - MEDIUM_TEXT_HEIGHT, BOUNDS.Right(), BOUNDS.Bottom(), al_map_rgba_f(0, 0, 0, 0.4));
		al_draw_filled_rectangle(BOUNDS.Left() + PADDING, BOUNDS.Top() + PADDING, BOUNDS.Right() - PADDING, BOUNDS.Bottom() - PADDING, al_map_rgba_f(0, 0, 0, 0.4));

		std::string titleText = enemy.name + " Stats";
		al_draw_text(mediumFont, al_map_rgb_f(1.0, 1.0, 1.0), BOUNDS.Left() + PADDING, BOUNDS.Top() - MEDIUM_TEXT_HEIGHT, ALLEGRO_ALIGN_LEFT, titleText.c_str());

		for (int i = 0; i < NUM_TEXT_LABELS; i++)
		{
			al_draw_text(mediumFont, al_map_rgb(255, 255, 255), BOUNDS.Left() + 2*PADDING, BOUNDS.Top() +(i*MEDIUM_TEXT_HEIGHT) + 2*PADDING, ALLEGRO_ALIGN_LEFT, statText[i].c_str());
		}
	}
}