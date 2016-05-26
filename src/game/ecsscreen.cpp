#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <alligator/input/input.h>
#include <iostream>

#include "ecsscreen.h"
#include "phackmangame.h"
#include "assets.h"
#include <alligator/util/matrix.h>
#include "../map/mapgen.h"
#include "../ai/pfmap.h"

ECSScreen::ECSScreen( PhackmanGame* g )
	: m_game(g)
{

}

ECSScreen::~ECSScreen()
{

}

void ECSScreen::show()
{
	m_map = LayoutBuilder().generate(ShapeStorage().makeSample());
	m_tileMap = convolute3x3(*m_map, draw_map_tiles_convolutor);
	m_navmap.reset(new NavigationMap(m_map));

	auto start_node = m_navmap->nodes()[0];
	//m_player.reset(new Player((start_node->x()) * 16, (start_node->y()) * 16, m_navmap));
}

void ECSScreen::update(double delta)
{
	//m_player->update(delta);
	if( Input::IsKeyDown(ALLEGRO_KEY_ESCAPE) )
	{
		m_game->close();
	}
}

void ECSScreen::render()
{
	al_clear_to_color(al_map_rgb(20,20,20));
	al_set_target_bitmap(al_get_backbuffer(m_game->display()));

	//debugRender();
	tilesRender();
	//m_player->render();
}

void ECSScreen::hide()
{

}

void ECSScreen::tilesRender()
{
	for( int r = 0; r < m_tileMap->rows(); r++ )
	{
		for( int c = 0; c < m_tileMap->cols(); c++ )
		{
			int x1, y1, x2, y2;
			x1 = c * 16; y1 = r * 16;
			x2 = (c+1) * 16; y2 = (r+1) * 16;

			ALLEGRO_BITMAP* bm = Assets::instance->maptilesSheet->getFrame(m_tileMap->get(c, r));
			al_draw_bitmap(bm, x1, y1, 0);
		}
	}
}

void ECSScreen::debugRender()
{
	for( int r = 0; r < m_map->rows(); r++ )
	{
		for( int c = 0; c < m_map->cols(); c++ )
		{
			int x1, y1, x2, y2;
			x1 = c * 16; y1 = r * 16;
			x2 = (c+1) * 16; y2 = (r+1) * 16;

			if( m_map->get(c, r) == 1 )
			{
				al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(0, 255, 0));
			}
			else
			{
				al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(0, 0, 255));
			}
		}
	}

	for( PathNode::SharedPtr node : m_navmap->nodes() )
	{
		float cx, cy;
		cx = (node->x()+1) * 16;
		cy = (node->y()+1) * 16;

		for( PathNode::SharedPtr neighboor : node->neighboors() )
		{
			int ncx, ncy;
			ncx = (neighboor->x()+1) * 16;
			ncy = (neighboor->y()+1) * 16;

			al_draw_line(cx, cy, ncx, ncy, al_map_rgba(0, 255, 255, 2), 1);
		}

		al_draw_filled_circle(cx, cy, 3, al_map_rgb(255, 0, 0));
	}
}