#ifndef STARLIGHT_HUD_H_
#define STARLIGHT_HUD_H_

#include <allegro5\allegro_font.h>

#include "Player.h"
#include "Enemy.h"

namespace starlight
{
	class HUD
	{
	public:
		HUD(ALLEGRO_FONT *mediumFont, ALLEGRO_FONT *bigFont);
		~HUD();

		void Update();
		void Draw(const Player &player, Enemy *selectedEnemy);
		void DrawPlayerStats(const Player &player);
		void DrawInventory(const Player &player);
		void HUD::DrawEnemyStats(const Enemy &enemy);
		void DrawPausedOverlay();
		
		const int MEDIUM_TEXT_HEIGHT;
		const int BIG_TEXT_HEIGHT;
		ALLEGRO_FONT *mediumFont;
		ALLEGRO_FONT *bigFont;

	private:
		static const int PADDING;
	};
}

#endif // STARLIGHT_HUD_H_