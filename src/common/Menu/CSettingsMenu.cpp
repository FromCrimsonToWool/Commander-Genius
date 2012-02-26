/*
 * CSettingsMenu.cpp
 *
 *  Created on: 19.05.2010
 *      Author: gerstrong
 */

#include "CSettingsMenu.h"
#include "CVideoSettings.h"
#include "CAudioSettings.h"
#include "COptions.h"
#include "CControlsettings.h"
#include "CProfilesMenu.h"
#include "sdl/CTimer.h"
#include "sdl/CVideoDriver.h"
#include "gui/CGUIButton.h"

#include "core/CGameControl.h"

#define SAFE_DELETE(x)	if(x) { delete x; x = NULL; }

CSettingsMenu::CSettingsMenu( const Uint8 dlgtheme ) :
CBaseMenu(dlgtheme)
{
	mpMenuDialog = new CGUIDialog(CRect<float>(0.25f, 0.24f, 0.5f, 0.5f));
	mpMenuDialog->setBackground(CGUIDialog::VORTICON);

	mpMenuDialog->addControl(new CGUIButton( "Video",
									new OpenMenuEvent( new CVideoSettings(dlgtheme) ),
									CGUIButton::VORTICON ),
								CRect<float>(0.05f, 0.08f, 0.9f, 0.07f) );

	mpMenuDialog->addControl(new CGUIButton( "Audio",
									new OpenMenuEvent( new CAudioSettings(dlgtheme) ),
									CGUIButton::VORTICON ),
								CRect<float>(0.05f, 0.16f, 0.9f, 0.07f) );

	mpMenuDialog->addControl(new CGUIButton( "Options",
									new OpenMenuEvent( new COptions(dlgtheme) ),
									CGUIButton::VORTICON ),
								CRect<float>(0.05f, 0.24f, 0.9f, 0.07f) );

	mpMenuDialog->addControl(new CGUIButton( "Controls",
									new OpenMenuEvent( new CControlsettings(dlgtheme) ),
									CGUIButton::VORTICON ),
								CRect<float>(0.05f, 0.32f, 0.9f, 0.07f) );

	mpMenuDialog->addControl(new CGUIButton( "Profile",
									new OpenMenuEvent( new CProfilesMenu(dlgtheme) ),
									CGUIButton::VORTICON ),
								CRect<float>(0.05f, 0.40f, 0.9f, 0.07f) );

}


