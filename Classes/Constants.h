#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

// resource name
# define R_BG_FILENAME (std::string)"bg.png"
# define R_OUTBOX_FILENAME (std::string)"outbox.png"
# define R_INBOX_FILENAME (std::string)"inbox.png"
# define R_HUD_LEFTARROW (std::string)"leftarrow.png"
# define R_HUD_RIGHTARROW (std::string)"rightarrow.png"

# define R_JET_FILENAME (std::string)"jet.png"
# define R_JET_LEFT_FILENAME (std::string)"jet_left.png"
# define R_JET_RIGHT_FILENAME (std::string)"jet_right.png"
# define R_JET2_FILENAME (std::string)"jet2.png"
# define R_JET2_LEFT_FILENAME (std::string)"jet2_left.png"
# define R_JET2_RIGHT_FILENAME (std::string)"jet2_right.png"
# define R_JET3_FILENAME (std::string)"jet3.png"
# define R_JET3_LEFT_FILENAME (std::string)"jet3_left.png"
# define R_JET3_RIGHT_FILENAME (std::string)"jet3_right.png"
# define R_JET4_FILENAME (std::string)"jet4.png"
# define R_JET4_LEFT_FILENAME (std::string)"jet4_left.png"
# define R_JET4_RIGHT_FILENAME (std::string)"jet4_right.png"

# define R_ENEMY_FILENAME (std::string)"enemy.png"
# define R_SENEMY_SPEED_FILENAME (std::string)"senemy_speed.png"
# define R_SENEMY_HEALTH_FILENAME (std::string)"senemy_health.png"
# define R_SENEMY_POWER_FILENAME (std::string)"senemy_power.png"
# define R_SENEMY_BOMB_FILENAME (std::string)"senemy_bomb.png"
# define R_BOMB_ICON_FILENAME (std::string)"bomb_icon.png"
# define R_BOMB_FILENAME (std::string)"bomb.png"
# define R_BOMBWAVE_FILENAME (std::string)"bombwave.png"
# define R_JBULLET_FILENAME (std::string)"jet_bullet.png"
# define R_MJBULLET_FILENAME (std::string)"magicjet_bullet.png"
# define R_JBULLET_SMALL_FILENAME (std::string)"jet_bullet_small.png"
# define R_EBULLET_FILENAME (std::string)"enemy_bullet.png"
# define R_EHBULLET_FILENAME (std::string)"barrier.png"

// custom type
# define DIREC unsigned char  // represent direction
// eight directions
# define DIREC_NONE (DIREC)0x0
# define DIREC_UP (DIREC)0x8
# define DIREC_DOWN (DIREC)0x4
# define DIREC_LEFT (DIREC)0x2
# define DIREC_RIGHT (DIREC)0x1
# define DIREC_UPRIGHT (DIREC)0x9
# define DIREC_UPLEFT (DIREC)0xA
# define DIREC_DOWNRIGHT (DIREC)0x5 
# define DIREC_DOWNLEFT (DIREC)0x6

// bases
# define BG_SCROLL_SPEED 30
# define SPEED_BASE 180
# define POWER_BASE 20
# define HONOR_BASE 50

# define JET_SHOOT_TAG "jet_shoot_counter"
# define JET_SHOOT_DURATION 0.25f
# define JET2_SHOOT_DURATION 0.45f
# define JET3_SHOOT_DURATION 0.15f
# define JET4_SHOOT_DURATION 1.15f
# define JET_SHADOW_TAG "jet_shadow_counter"
# define JET_SHADOW_DURATION 0.2f
# define ENEMY_SHOOT_TAG "enemy_shoot_counter"
# define ENEMY_SHOOT_DURATION 0.5f
# define JET_POWER_TRAIT_TAG "jet_power_trait_counter"
# define JET_SPEED_TRAIT_TAG "jet_shoot_trait_counter"
# define JET_POWER_TRAIT_DURATION 12.0f
# define JET_SPEED_TRAIT_DURATION 15.0f
# define JET_ULTIMATE_TAG "jet_ultimate_counter"
# define JET_ULTIMATE_DURATION 1.0f
# define JET2_ULTIMATE_DURATION 8.0f
# define JET3_ULTIMATE_DURATION 8.0f
# define JET4_ULTIMATE_DURATION 2.0f

# define JET_INIT_SPEED 0.8f
# define JET_INIT_POWER 50
# define JET_INIT_HEALTH 350
# define JET2_INIT_SPEED 0.4f
# define JET2_INIT_POWER 50
# define JET2_INIT_HEALTH 600
# define JET3_INIT_SPEED 1.2f
# define JET3_INIT_POWER 50
# define JET3_INIT_HEALTH 200
# define JET4_INIT_SPEED 0.9f
# define JET4_INIT_POWER 50
# define JET4_INIT_HEALTH 350
# define ENEMY_INIT_SPEED_BASE 0.3f
# define ENEMY_INIT_POWER_BASE 1
# define ENEMY_INIT_HEALTH_BASE 50
# define BULLET_INIT_SPEED 1.5f
# define BULLET_INIT_POWER 1
# define BULLET_INIT_HEALTH 1
# define BULLET_POWER_LV1 1
# define BULLET_POWER_LV2 2
# define BULLET_POWER_LV3 3

# define COLLISIONTYPE_AIRCRAFT 1
# define COLLISIONTYPE_BULLET 2
# define COLLISIONTYPE_ALL 3

// enemy constants
# define ENEMY_SPAWN_LIKELYHOOD 3
# define ENEMY_SHOOT_LIKELYHOOD_EASY 2
# define ENEMY_SHOOT_LIKELYHOOD_NORMAL 1
# define ENEMY_SHOOT_LIKELYHOOD_HARD 3
# define ENEMY_SHOOT_LIKELYHOOD_SPECIAL 2
# define ENEMY_DIFFICULTY_EASY 1
# define ENEMY_DIFFICULTY_NORMAL 2
# define ENEMY_DIFFICULTY_HARD 3
# define ENEMY_DIFFICULTY_SPECIAL 4
# define ENEMY_DIFFICULTY_SPECIAL_BOMB 5
# define ENEMY_DIFFICULTY_SPECIAL_HEALTH 6
# define ENEMY_DIFFICULTY_SPECIAL_POWER 7
# define ENEMY_DIFFICULTY_SPECIAL_SPEED 8
# define NEXT_ENEMY_SPAWN_BASE 2.4f
# define SPECIAL_ENEMY_SPAWN_BASE 10.0f

// explosion animation constants
# define FRAME_BOOM_1_NAME (std::string)"boom1.png"
# define FRAME_BOOM_2_NAME (std::string)"boom2.png"
# define FRAME_BOOM_3_NAME (std::string)"boom3.png"
# define FRAME_BOOM_4_NAME (std::string)"boom4.png"
# define FRAME_BOOM_5_NAME (std::string)"boom5.png"
# define FRAME_BOOM_1_WIDTH 65
# define FRAME_BOOM_1_HEIGHT 36
# define FRAME_BOOM_2_WIDTH 84
# define FRAME_BOOM_2_HEIGHT 46
# define FRAME_BOOM_3_WIDTH 107
# define FRAME_BOOM_3_HEIGHT 58
# define FRAME_BOOM_4_WIDTH 134
# define FRAME_BOOM_4_HEIGHT 73
# define FRAME_BOOM_5_WIDTH 156
# define FRAME_BOOM_5_HEIGHT 85

# define HONOR_DISPLAY_LENGTH 8
# define LEVEL_NORMAL_THRESHOLD 4000
# define LEVEL_HARD_THRESHOLD 15000

# define JET_LEVEL1_THRESHOLD 10000
# define JET_LEVEL2_THRESHOLD 20000
# define JET_LEVEL3_THRESHOLD 30000

#endif