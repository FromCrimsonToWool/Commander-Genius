/*
 * CMessageBoxGalaxy.cpp
 *
 *  Created on: 30.03.2011
 *      Author: gerstrong
 *
 *  This
 */

#include "CMessageBoxGalaxy.h"
#include "sdl/CVideoDriver.h"
#include "sdl/CInput.h"
#include "graphics/CGfxEngine.h"

CMessageBoxGalaxy::CMessageBoxGalaxy(const std::string& Text) :
CMessageBox(Text, false, false, false)
{
	// Center that dialog box
	m_gamerect = g_pVideoDriver->getGameResolution();

	m_gamerect.x = m_gamerect.w/2;
	m_gamerect.y = m_gamerect.h/2;

	// TODO: Those hardcoded are only temporary. They need to be removed.
	m_gamerect.h = 100;
	m_gamerect.w = 240;

	m_gamerect.x -= m_gamerect.w/2;
	m_gamerect.y -= m_gamerect.h/2;

	m_gamerect.h /= 8;
	m_gamerect.w /= 8;

	mp_DlgFrame = new CDlgFrame(m_gamerect.x, m_gamerect.y,
								m_gamerect.w, m_gamerect.h, DLG_THEME_GALAXY);
}

void CMessageBoxGalaxy::process()
{
	SDL_Surface *sfc = g_pVideoDriver->mp_VideoEngine->getFGLayerSurface();

	// Look, if somebody pressed a button, and close this dialog!
	if(g_pInput->getPressedAnyCommand()) {
		m_mustclose = true;
		return;
	}

	// Draw the empty Dialog Box
	mp_DlgFrame->draw(sfc);

	// Set the proper Font colors
	g_pGfxEngine->getFont(0).setBGColour(sfc->format, 0xFFFFFFFF);
	g_pGfxEngine->getFont(0).setFGColour(sfc->format, 0xFF000000);

	// Draw the Text on our surface
	for( size_t i=0 ; i<m_Lines.size() ; i++)
		g_pGfxEngine->getFont(0).drawFont(sfc, m_Lines[i], m_gamerect.x+8, m_gamerect.y+((i+1)*8) );
}
