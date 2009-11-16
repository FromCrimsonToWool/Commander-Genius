/*
 * CMap.cpp
 *
 *  Created on: 21.09.2009
 *      Author: gerstrong
 */

#include "../keen.h"
#include "CMap.h"
#include <iostream>
#include <fstream>
#include "../FindFile.h"
#include "../CLogFile.h"
#include "../include/fileio/rle.h"
#include "../sdl/CVideoDriver.h"

std::string formatPathString(const std::string& path);

CMap::CMap(SDL_Surface *p_scrollsurface, CTilemap *p_Tilemap) {
	m_width = m_height = 0;
	m_worldmap = false;
	
	resetScrolls();
	mp_scrollsurface = p_scrollsurface;
	mp_Tilemap = p_Tilemap;
	mp_data = NULL;
	memset(m_objectlayer, 0, sizeof(m_objectlayer));
}

////////////////////////////
// Initialization Routine //
////////////////////////////

void CMap::resetScrolls()
{
	m_scrollx = m_scrolly = 0;

	m_scrollx_buf = m_scrolly_buf = 0;

	m_scrollpix = m_scrollpixy = 0;
	m_mapx = m_mapy = 0;           // map X location shown at scrollbuffer row 0
	m_mapxstripepos = m_mapystripepos = 0;  // X pixel position of next stripe row
}

/////////////////////////
// Getters and Setters //
/////////////////////////
// returns the tile which is set at the given coordinates
Uint16 CMap::at(Uint16 x, Uint16 y)
{
	return mp_data[y*m_width + x];
}

//////////////////////////
// returns the object/sprite/level which is set at the given coordinates
Uint16 CMap::getObjectat(Uint16 x, Uint16 y)
{
	return m_objectlayer[x][y];
}

// searches the map's object layer for object OBJ.
// if it is found returns nonzero and places the
// coordinates of the first occurance of the object
// in (xout,yout)
bool CMap::findObject(unsigned int obj, int *xout, int *yout)
{
	unsigned int x,y;
	
	for(y=2;y<m_height-2;y++)
	{
		for(x=2;x<m_width-2;x++)
		{
			if (m_objectlayer[x][y]==obj)
			{
				*xout = x;
				*yout = y;
				return true;
			}
		}
	}
	return false;
}

// searches the map's tile layer for tile TILE.
// if it is found returns nonzero and places the
// coordinates of the first occurance of the tile
// in (xout,yout)
bool CMap::findTile(unsigned int tile, int *xout, int *yout)
{
	unsigned int x,y;

	for(y=2;y<m_height-2;y++)
	{
		for(x=2;x<m_width-2;x++)
		{
			if (mp_data[y*m_width + x]==tile)
			{
				*xout = x;
				*yout = y;
				return true;
			}
		}
	}
	return false;
}

bool CMap::setTile(Uint16 x, Uint16 y, Uint16 t)
{
	if( x<m_width && y<m_height )
	{
		mp_data[y*m_width + x] = t;
		return true;
	}
	else
		return false;
}

bool CMap::setTile(Uint16 x, Uint16 y, Uint16 t, bool update)
{
	if(setTile( x, y, t))
	{
		if(update) redrawAt(x,y);
		return true;
	}
	else return false;
}

// Called in level. This function does the same as setTile, but also draws directly to the scrollsurface
// used normally, when items are picked up
bool CMap::changeTile(Uint16 x, Uint16 y, Uint16 t)
{
	if(setTile( x, y, t))
	{
		mp_Tilemap->drawTile(mp_scrollsurface, (x<<4)&511, (y<<4)&511, t);
		mp_Tilemap->registerAnimation( (x<<4)&511, (y<<4)&511, t );
		return true;
	}
	return false;
}

////
// Scrolling Routines
////
bool CMap::gotoPos(int x, int y)
{
	int dx,dy;
	bool retval = false;
	dx = x - m_scrollx;
	dy = y - m_scrolly;
	
	if( dx > 0 )
		for( int scrollx=0 ; scrollx<dx ; scrollx++) scrollRight();
	else retval = true;
	
	if( dx < 0 )
		for( int scrollx=0 ; scrollx<-dx ; scrollx++) scrollLeft();
	else retval = true;
	
	if( dy > 0 )
		for( int scrolly=0 ; scrolly<dy ; scrolly++) scrollDown();
	else retval = true;
	
	if( dy < 0 )
		for( int scrolly=0 ; scrolly<-dy ; scrolly++) scrollUp();
	else retval = true;

	return retval;
}

// scrolls the map one pixel right
void CMap::scrollRight(void)
{
	m_scrollx++;
	if(m_scrollx_buf>=511) m_scrollx_buf=0; else m_scrollx_buf++;
	
	m_scrollpix++;
	if (m_scrollpix>=16)
	{  // need to draw a new stripe
		drawVstripe(m_mapxstripepos, m_mapx + 32);
		m_mapx++;
		m_mapxstripepos += 16;
		if (m_mapxstripepos >= 512) m_mapxstripepos = 0;
		m_scrollpix = 0;
	}
}

// scrolls the map one pixel left
void CMap::scrollLeft(void)
{
	m_scrollx--;
	if(m_scrollx_buf==0) m_scrollx_buf=511; else m_scrollx_buf--;
	
	if (m_scrollpix==0)
	{  // need to draw a new stripe
		m_mapx--;
		if (m_mapxstripepos == 0)
		{
			m_mapxstripepos = (512 - 16);
		}
		else
		{
			m_mapxstripepos -= 16;
		}
		drawVstripe(m_mapxstripepos, m_mapx);
		
		m_scrollpix = 15;
	} else m_scrollpix--;
}

void CMap::scrollDown(void)
{
	m_scrolly++;
	if(m_scrolly_buf>=511) m_scrolly_buf=0; else m_scrolly_buf++;
	
	m_scrollpixy++;
	if (m_scrollpixy>=16)
	{  // need to draw a new stripe
		drawHstripe(m_mapystripepos, m_mapy + 32);
		m_mapy++;
		m_mapystripepos += 16;
		if (m_mapystripepos >= 512) m_mapystripepos = 0;
		m_scrollpixy = 0;
	}
}

void CMap::scrollUp(void)
{
	m_scrolly--;
	if(m_scrolly_buf==0) m_scrolly_buf=511; else m_scrolly_buf--;
	
	if (m_scrollpixy==0)
	{  // need to draw a new stripe
		m_mapy--;
		if (m_mapystripepos == 0)
		{
			m_mapystripepos = (512 - 16);
		}
		else
		{
			m_mapystripepos -= 16;
		}
		drawHstripe(m_mapystripepos, m_mapy);
		
		m_scrollpixy = 15;
	} else m_scrollpixy--;
}

//////////////////////
// Drawing Routines //
//////////////////////
// Draws the entire map to the scroll buffer
// called at start of level to draw the upper-left corner of the map
// onto the scrollbuffer...from then on the map will only be drawn
// in stripes as it scrolls around.
void CMap::redrawAt(int mx, int my)
{
	int c = mp_data[my*m_width + mx];
	mp_Tilemap->drawTile(mp_scrollsurface, (mx<<4)&511, (my<<4)&511, c);
}

// redraws all the map area. This is used for the title screen, when game starts and other passive scenes.
// Don't use it, when the game is scrolling. Use redrawAt instead,
// for the correct and fast update of tiles
void CMap::drawAll()
{
	int y;
	int num_h_tiles= mp_scrollsurface->h/16;
	
    for(y=0;y<num_h_tiles;y++)
    {
		drawHstripe(y<<4, y);
    }
}

// draw a horizontal stripe, for vertical scrolling
void CMap::drawHstripe(unsigned int y, unsigned int mpy)
{
	int x,c;
	int num_v_tiles= mp_scrollsurface->w/16;
	
	for(x=0;x<num_v_tiles;x++)
	{
		c = mp_data[mpy*m_width + x+m_mapx];
		
		mp_Tilemap->drawTile(mp_scrollsurface, ((x<<4)+m_mapxstripepos)&511, y, c);
		mp_Tilemap->registerAnimation( ((x<<4)+m_mapxstripepos)&511, y, c );
	}
}

// draws a vertical stripe from map position mapx to scrollbuffer position x
void CMap::drawVstripe(unsigned int x, unsigned int mpx)
{
	int y,c;
	int num_h_tiles= mp_scrollsurface->h/16;
	for(y=0;y<num_h_tiles;y++)
	{
		c = mp_data[(y+m_mapy)*m_width + mpx];
		mp_Tilemap->drawTile(mp_scrollsurface, x, ((y<<4)+m_mapystripepos)&511, c);
		mp_Tilemap->registerAnimation( x, ((y<<4)+m_mapystripepos)&511, c );
	}
}

///
// Animation functions
///
// searches for animated tiles at the map position (X,Y) and
// unregisters them from animtiles
void CMap::deAnimate(int x, int y)
{
	int px,py;
	// figure out pixel position of map tile (x,y)
    px = ((m_mapxstripepos+((x-m_mapx)<<4))&511);
    py = ((m_mapystripepos+((y-m_mapy)<<4))&511);
	
    mp_Tilemap->deAnimateAt(px , py);
}

CMap::~CMap() {
	if(mp_data) delete[] mp_data;
}



