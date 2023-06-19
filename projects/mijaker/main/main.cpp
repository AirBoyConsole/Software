#include <iostream>
#include <cstring>
#include <string>
#include <cmath>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/i2c.h"

#include "engine.hpp"
#include "input.hpp"
#include "textures.h"

#define APP_TAG "App"

#define TASKNAME "main"

#define LCD_WIDTH 320
#define LCD_HEIGHT 240
#define LCD_DEPTH 2

#define SWAP_ENDIAN_16(value)		( (((value) & 0xFFu) << 8u) | ((value) >> 8u) )

#define mapWidth 24
#define mapHeight 24
#define mapSize 64

#define texWidth 64
#define texHeight 64

const int worldmap[mapWidth][mapHeight] =
{
  {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
  {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
  {4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
  {4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,1},
  {4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,7,7,1},
  {4,0,7,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,1},
  {4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
  {4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
  {6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
  {6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
  {4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,4,4,4,4,4,4,4,4,4,1,0,1,2,2,2,2,2,2,3,3,3,3,3},
};

const int floormap[mapWidth][mapHeight]=
{
  {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,},
  {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,6,6,6,6,6,6,3,},
  {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,6,3,3,3,3,6,3,},
  {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,6,3,3,3,3,6,3,},
  {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,6,6,6,6,6,6,3,},
  {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,},
  {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,},
  {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,},
  {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,},
  {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,},
  {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,},
  {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,},
  {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,},
  {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,},
  {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,},
  {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,},
  {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,},
  {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,},
  {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,},
  {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,},
  {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,},
  {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,},
  {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,},
  {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,},
};

const int ceilingmap[mapWidth][mapHeight]=
{
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,},
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,9,9,9,9,9,9,7,},
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,9,9,9,9,9,9,7,},
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,9,9,9,9,9,9,7,},
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,9,9,9,9,9,9,7,},
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,},
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,},
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,},
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,},
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,},
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,},
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,},
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,},
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,},
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,},
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,},
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,},
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,},
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,},
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,},
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,},
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,},
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,},
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,},
};

typedef struct {
    int x,
        y,
        width,
        height;
    uint16_t color;
} Rectangle;

const Rectangle ceiling_tex = {0, 0, LCD_WIDTH, LCD_HEIGHT / 2, 0xB6B5};
const Rectangle floor_tex = {0, LCD_HEIGHT / 2, LCD_WIDTH, LCD_HEIGHT / 2, 0x0842};

uint16_t *textures[8];

float playerx = 22,
	   playery = 12,
	   playerdeltax = 0,
	   playerdeltay = 0,
	   playerdirx = -1,
	   playerdiry = 0,
	   playerleftx = 0,
	   playerlefty = -1,
	   playercplanex = 0,
	   playercplaney = 0.66,
	   playerrotspeed = 0.16 * 3.0,
	   playermovespeed = 0.16 * 5.0;

void load_textures()
{
	for (int i = 0; i < 8; i++)
		textures[i] = new uint16_t[texHeight * texWidth * 2];
	memcpy(textures[0], image_data_bluestone,   texHeight * texWidth * sizeof(uint16_t));
	memcpy(textures[1], image_data_colorstone,  texHeight * texWidth * sizeof(uint16_t));
	memcpy(textures[2], image_data_eagle,       texHeight * texWidth * sizeof(uint16_t));
	memcpy(textures[3], image_data_greystone,   texHeight * texWidth * sizeof(uint16_t));
	memcpy(textures[4], image_data_mossy,       texHeight * texWidth * sizeof(uint16_t));
	memcpy(textures[5], image_data_purplestone, texHeight * texWidth * sizeof(uint16_t));
	memcpy(textures[6], image_data_redbrick,    texHeight * texWidth * sizeof(uint16_t));
	memcpy(textures[7], image_data_wood,        texHeight * texWidth * sizeof(uint16_t));
}

void handle_input(airboy::Input *input)
{
	if (input->is_pressed(airboy::Buttons::BUTTON_DPAD_LEFT)) {
		float oldplayerdirx = playerdirx;
		playerdirx = playerdirx * cos(-playerrotspeed) - playerdiry * sin(-playerrotspeed);
		playerdiry = oldplayerdirx * sin(-playerrotspeed) + playerdiry * cos(-playerrotspeed);
		float oldplayercplanex = playercplanex;
		playercplanex = playercplanex * cos(-playerrotspeed) - playercplaney * sin(-playerrotspeed);
		playercplaney = oldplayercplanex * sin(-playerrotspeed) + playercplaney * cos(-playerrotspeed);

		playerleftx = -playerdiry;
		playerlefty = playerdirx;
	}
	if (input->is_pressed(airboy::Buttons::BUTTON_DPAD_RIGHT)) {
		float oldplayerdirx = playerdirx;
		playerdirx = playerdirx * cos(playerrotspeed) - playerdiry * sin(playerrotspeed);
		playerdiry = oldplayerdirx * sin(playerrotspeed) + playerdiry * cos(playerrotspeed);
		float oldplayercplanex = playercplanex;
		playercplanex = playercplanex * cos(playerrotspeed) - playercplaney * sin(playerrotspeed);
		playercplaney = oldplayercplanex * sin(playerrotspeed) + playercplaney * cos(playerrotspeed);

		playerleftx = -playerdiry;
		playerlefty = playerdirx;
	}
	if (input->is_pressed(airboy::Buttons::BUTTON_BUMPER_LEFT)) {
		if (!worldmap[(int) (playerx + playerleftx * playermovespeed)][(int) playery])
			playerx += playerleftx * playermovespeed;
		if (!worldmap[(int) playerx][(int) (playery + playerlefty * playermovespeed)])
			playery += playerlefty * playermovespeed;
	}
	if (input->is_pressed(airboy::Buttons::BUTTON_BUMPER_RIGHT)) {
		if (!worldmap[(int) (playerx - playerleftx * playermovespeed)][(int) playery])
			playerx -= playerleftx * playermovespeed;
		if (!worldmap[(int) playerx][(int) (playery - playerlefty * playermovespeed)])
			playery -= playerlefty * playermovespeed;
	}
	if (input->is_pressed(airboy::Buttons::BUTTON_DPAD_UP)) {
		if (!worldmap[(int) (playerx + playerdirx * playermovespeed)][(int) playery])
			playerx += playerdirx * playermovespeed;
		if (!worldmap[(int) playerx][(int) (playery + playerdiry * playermovespeed)])
			playery += playerdiry * playermovespeed;
	}
	if (input->is_pressed(airboy::Buttons::BUTTON_DPAD_DOWN)) {
		if (!worldmap[(int) (playerx - playerdirx * playermovespeed)][(int) playery])
			playerx -= playerdirx * playermovespeed;
		if (!worldmap[(int) playerx][(int) (playery - playerdiry * playermovespeed)])
			playery -= playerdiry * playermovespeed;
	}
}

void draw_player_view(airboy::Display *display)
{
	float raydirx0 = playerdirx - playercplanex,
		  raydiry0 = playerdiry - playercplaney,
		  raydirx1 = playerdirx + playercplanex,
		  raydiry1 = playerdiry + playercplaney;

	for (int y = LCD_HEIGHT / 2 + 1; y < LCD_HEIGHT; y++) {
		int p = y - LCD_HEIGHT / 2;
		float rowdistance = LCD_HEIGHT / (2.0 * p),
			  floorstepx = rowdistance * (raydirx1 - raydirx0) / LCD_WIDTH,
			  floorstepy = rowdistance * (raydiry1 - raydiry0) / LCD_WIDTH,
			  floorx = playerx + rowdistance * raydirx0,
			  floory = playery + rowdistance * raydiry0;

		for (int x = 0; x < LCD_WIDTH; x++) {
			int cellx = (int) floorx,
				celly = (int) floory,
				texx = (int) (texWidth * (floorx - cellx)) & (texWidth - 1),
				texy = (int) (texHeight * (floory - celly)) & (texHeight - 1);

			floorx += floorstepx;
			floory += floorstepy;

			/* int floortexture = floormap[abs(cellx & (mapWidth - 1))][abs(celly & (mapHeight - 1))], */
			int floortexture = 3,
				  ceilingtexture = 9;
			/* if (texindexx < 0 || texindexx >= mapWidth || texindexy < 0 || texindexy >= mapHeight) */
			/* 	floortexture = 3; */
			/* if (texindexx >= 0 && texindexx < mapWidth && texindexy >= 0 && texindexy < mapHeight) */
			/* 	floortexture = floormap[texindexx][texindexy]; */
			if (cellx >= 0 && cellx < mapWidth && celly >= 0 && celly < mapHeight) {
				floortexture = floormap[cellx][celly];
				ceilingtexture = ceilingmap[cellx][celly];
			}
			/* int floortexture = floormap[(int)(playerx + cellx) & (mapWidth - 1)][(int)(playery + celly) & (mapHeight - 1)], */
			/* 	ceilingtexture = 6; */
			uint16_t pixelcolor;
			/* if (floortexture > 7 || floortexture < 0) */
				/* ESP_LOGI(TASKNAME, "cellx: %d celly: %d", cellx, celly); */

			pixelcolor = SWAP_ENDIAN_16(textures[floortexture][texWidth * texy + texx]);
			pixelcolor = (pixelcolor >> 1) & 0xEF7B;
			//display->set_pixel_fast(x, y, pixelcolor);
			display->frame_buffer[y * LCD_WIDTH + x] = pixelcolor;

			if (ceilingtexture != 9) {
				pixelcolor = SWAP_ENDIAN_16(textures[ceilingtexture][texWidth * texy + texx]);
				pixelcolor = (pixelcolor >> 1) & 0xEF7B;
				//display->set_pixel_fast(x, (LCD_HEIGHT - y - 1), pixelcolor);
				display->frame_buffer[(LCD_HEIGHT - y - 1) * LCD_WIDTH + x] = pixelcolor;
			}
		}
	}

	for (int x = 0; x < LCD_WIDTH; x++) {
		float camerax = 2 * x / (float) LCD_WIDTH - 1,
			   raydirx = playerdirx + playercplanex * camerax,
			   raydiry = playerdiry + playercplaney * camerax,
			   deltadistx = (raydirx == 0) ? 1e30 : fabs(1 / raydirx),
			   deltadisty = (raydiry == 0) ? 1e30 : fabs(1 / raydiry),
			   sidedistx,
			   sidedisty,
			   perpwalldist;

		int mapx = (int) playerx,
			mapy = (int) playery,
			stepx = 0,
			stepy = 0,
			hit = 0,
			side = 0; // 0 = NS, 1 = EW

		if (raydirx < 0) {
			stepx = -1;
			sidedistx = (playerx - mapx) * deltadistx;
		} else {
			stepx = 1;
			sidedistx = (mapx + 1.0 - playerx) * deltadistx;
		}
		if (raydiry < 0) {
			stepy = -1;
			sidedisty = (playery - mapy) * deltadisty;
		} else {
			stepy = 1;
			sidedisty = (mapy + 1.0 - playery) * deltadisty;
		}

		int dof = 0;
		while (!hit && dof < 255) {
			if (sidedistx < sidedisty) {
				sidedistx += deltadistx;
				mapx += stepx;
				side = 0;
			} else {
				sidedisty += deltadisty;
				mapy += stepy;
				side = 1;
			}

			if (worldmap[mapx][mapy] > 0)
				hit = 1;
			dof++;
		}
		if (dof == 255 || mapx >= mapWidth || mapx < 0 || mapy < 0 || mapy >= mapHeight)
			continue;

		if (!side)
			perpwalldist = (sidedistx - deltadistx);
		else
			perpwalldist = (sidedisty - deltadisty);

		int lineheight = (int)(LCD_HEIGHT / perpwalldist),
			linestart = (LCD_HEIGHT - lineheight) / 2,
			lineend = (LCD_HEIGHT + lineheight) / 2;

		if (linestart < 0)
			linestart = 0;
		if (lineend >= LCD_HEIGHT)
			lineend = LCD_HEIGHT - 1;

		/* uint16_t *texture = (uint16_t *)malloc(texHeight * texWidth * sizeof(uint16_t)); */
		/* if (texture == NULL) */
		/* 	ESP_LOGI(TASKNAME, "dupa"); */
		/* switch (worldmap[mapx][mapy]) { */
		/* 	default: memcpy(texture, image_data_bluestone,   texHeight * texWidth * sizeof(uint16_t)); break; */
		/* } */
		int texindex = worldmap[mapx][mapy] - 1;

		float wallx = (!side) ?
			playery + perpwalldist * raydiry :
			playerx + perpwalldist * raydirx;
		wallx -= floor(wallx);

		int texx = (int) (wallx * (float) texWidth);
		if (!side && raydirx > 0)
			texx = texWidth - texx - 1;
		if (side && raydiry < 0)
			texx = texWidth - texx - 1;

		float step = 1.0 * texHeight / lineheight,
			   texposition = (linestart - LCD_HEIGHT / 2 + lineheight / 2) * step; // TODO: check what pitch does

		for (int y = linestart; y < lineend; y++) {
			int texy = (int) texposition & (texHeight - 1);
			texposition += step;

			uint16_t pixelcolor = SWAP_ENDIAN_16(textures[texindex][texHeight * texy + texx]);
			if (side)
				pixelcolor = (pixelcolor >> 1) & 0xEF7B;

			display->frame_buffer[y * LCD_WIDTH + x] = pixelcolor;
			//display->set_pixel_fast(x, y, pixelcolor);
		}
		/* free(texture); */

		/* flat color renderer
		uint16_t wallcolor;
		switch (worldmap[mapx][mapy]) {
			case 1:  wallcolor = 0x00F8; break; // red
			case 2:  wallcolor = 0xE007; break; // green
			case 3:  wallcolor = 0x1F00; break; // blue
			case 4:  wallcolor = 0xFFFE; break; // white
			default: wallcolor = 0xE0FF; break; // yellow
		}
		if (side)
			//wallcolor = (wallcolor & 0x00F8 >> 1 & 0x00F8) | (wallcolor & 0xE007 >> 1 & 0xE007) | (wallcolor & 0x1F00 >> 1 & 0x1F00);
			wallcolor = (wallcolor >> 1) & 0xEF7B;
		Line line = {x, linestart, x, lineend, wallcolor};
		drawLine(line, framebuffer);
		*/
	}
}

class Example : public airboy::Engine
{
public:
	void setup() override
	{
        ESP_LOGI(TASKNAME, "Hello, starting up");
		load_textures();
	}

	void update(float delta) override
	{
		this->display->clear_buffer();
        draw_player_view(this->display);
        playermovespeed = delta * 6.0;
		playerrotspeed = delta * 3.0;
        handle_input(this->input);

		std::string str;
		str = std::to_string(1 / delta);
		const char * c = str.c_str();

		this->renderer->draw_text(airboy::Vector2i(110, 220), 2, 0xFFFF, c);
	}
};

extern "C" void app_main(void)
{
    Example demo;
	if (demo.construct())
		demo.run();
	else ESP_LOGE(APP_TAG, "Construct error");
}