// valberrie 2025.11.23

#pragma once

#include "hudelement.h"
#include <vgui_controls/Panel.h>

// FIXME: this should be the same as CN_ITEM_NAME_MAX for posterity
constexpr auto HUD_INTERACT_HINT_TEXT_MAX = 64;

namespace vgui
{
	class IScheme;
}

class CHudInteractHint : public CHudElement, public vgui::Panel
{
    DECLARE_CLASS_SIMPLE( CHudInteractHint, vgui::Panel );
public:
    CHudInteractHint( const char *pElementName );
    virtual ~CHudInteractHint();

    bool ShouldDraw() OVERRIDE;

    void HideHint();
    void ShowHint( const char *pszDisplayName );
    void UpdateHint( int iX, int iY );

protected:
    void ApplySchemeSettings( vgui::IScheme *pScheme ) OVERRIDE;
    void Paint() OVERRIDE;

    char m_szText[HUD_INTERACT_HINT_TEXT_MAX]; 
    Color m_clrBase;
    int m_iX = 0, m_iY = 0;
    bool m_bShow;
};

extern ConVar cn_hud_draw_interact_hint;