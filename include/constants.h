#ifndef CONSTANTS_H
#define CONSTANTS_H

/* Other */
#define HEIGHT  720
#define WIDTH   1280 
#define SPEED_CHAR 150
#define WINDOW_TITLE "Royal Retribution"
#define SCORE_FILENAME  "assets/scores.txt"
#define SPRITE_PADDING_AMOUNT_X 28
#define SPRITE_PADDING_AMOUNT_Y 20
#define DELTA_TIME              0.0167

/* Maps */
#define TILES_IMG "assets/blocks_2.png"
#define TILES_TXT "maps/level_legend.txt"
#define TILES_ADD "maps/level_additions_legend.txt"
// level 1
#define LEVEL_1 "maps/level_1.txt"
#define LEVEL_1_E "maps/level_1_enemies.txt"
#define LEVEL_1_ADD "maps/level_1_additions.txt"
// level 2
#define LEVEL_2 "maps/level_2.txt"
#define LEVEL_2_E "maps/level_2_enemies.txt"
#define LEVEL_2_ADD "maps/level_2_additions.txt"

/* Fonts */
#define FONT_ARCADE "assets/arcade.ttf"
#define FONT_ROBOTO "assets/Roboto.ttf"
#define FONT_FILENAME "assets/Capture_it.ttf"

/* Audio */
#define MAX_MUSIC_VOLUME  60
#define KILL_FILENAME "assets/sword.wav"
#define INTRO_FILENAME "assets/intro.xm"
#define THEME1_FILENAME "assets/theme1.ogg"
#define COLLISION_FILENAME "assets/collision.wav"

/* Sprites */
#define NUM_ROWS  4
#define NUM_COLS  4
#define HEART "assets/Heart.png"
#define E_C_FILENAME "assets/characters/Animations.png"
#define SQUIRE_FILENAME "assets/characters/Squire_sheet.png"

/* Images */
#define BG_FILENAME "assets/space.png"
#define SHIP_FILENAME "assets/ship.png"
#define FLASH_FILENAME "assets/flash.png"
#define STAIRS_FILENAME "assets/stairs.png"
#define CASTLE_FILENAME "assets/castle.png"
#define PLANET_FILENAME "assets/planet.png"
#define BLACK_PIXEL     "assets/black.png"
#define LIGHTS_FILENAME "assets/lights.png"
#define THRONE_FILENAME "assets/throne.png"
#define COIN "assets/coin.png"
#define KEY "assets/key.png"

/* Bars */
#define BAR_BOX "assets/bar_outline.png"
#define STA_BAR "assets/stamina_bar.png"
#define EXP_BAR "assets/experience_bar.png"

/* LAYER */
#define ELE 0
#define PPL 1
#define ADD 2
#define TOP 3

#endif
