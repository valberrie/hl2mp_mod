// valberrie 2025.11.23

#include "cbase.h"
#include "hud.h"
#include "iclientmode.h"
#include "view.h"
#include "hud_crosshair.h"
#include "vgui/ISurface.h"
#include "ILocalize.h"

#include "hud_interact.h"

#include "tier0/memdbgon.h"

ConVar cn_hud_draw_interact_hint{ "cn_hud_draw_interact_hint", "1", FCVAR_ARCHIVE };

#ifdef CDNM
	DECLARE_HUDELEMENT( CHudInteractHint );
#endif

CHudInteractHint::CHudInteractHint( const char *pElementName )
	: CHudElement( pElementName ), vgui::Panel( nullptr, "HudInteractHint" )
{
    auto *pParent = g_pClientMode->GetViewport();
    SetParent( pParent );

	// Kill me if the type of this turns into something else
    memset( m_szText, 0, sizeof( m_szText ) );
	m_clrBase = Color{ 255, 255, 255, 255 };

	SetHiddenBits( HIDEHUD_PLAYERDEAD );
	
	m_bShow = false;
}

CHudInteractHint::~CHudInteractHint()
{

}

void CHudInteractHint::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );
	SetPaintBackgroundEnabled(false);

	SetSize( ScreenWidth(), ScreenHeight() );
	
	SetForceStereoRenderToFrameBuffer( true );
}

bool CHudInteractHint::ShouldDraw()
{
	bool bDraw = m_bShow;

	if ( !cn_hud_draw_interact_hint.GetBool() )
    {
		return false;
	}

	return bDraw && CHudElement::ShouldDraw();
}

void CHudInteractHint::Paint()
{
	if (!IsCurrentViewAccessAllowed())
		return;

	/*float flX = 0.f, flY = 0.f;
    bool bBehindCamera = false;
    CHudCrosshair::GetDrawPosition( &flX, &flY, &bBehindCamera );
    if ( bBehindCamera )
        return;*/
	
	wchar_t wszUtfText[HUD_INTERACT_HINT_TEXT_MAX]{};
	ILocalize::ConvertANSIToUnicode( m_szText, wszUtfText, HUD_INTERACT_HINT_TEXT_MAX );

	vgui::surface()->DrawSetTextColor( m_clrBase );
    vgui::surface()->DrawSetTextFont( g_hFontTrebuchet24 );
    vgui::surface()->DrawSetTextPos( m_iX, m_iY );
    vgui::surface()->DrawPrintText( wszUtfText, wcslen( wszUtfText ) );

	/*vgui::surface()->DrawSetTextPos( m_vecBL.x, m_vecBL.y );
    vgui::surface()->DrawUnicodeChar( 'B' );

    vgui::surface()->DrawSetTextPos( m_vecTR.x, m_vecTR.y );
    vgui::surface()->DrawUnicodeChar( 'T' );*/
}


void CHudInteractHint::HideHint()
{
    memset( m_szText, 0, HUD_INTERACT_HINT_TEXT_MAX );
	m_bShow = false;
}

void CHudInteractHint::ShowHint( const char *pszDisplayName )
{
	V_strncpy( m_szText, pszDisplayName, HUD_INTERACT_HINT_TEXT_MAX );
	m_bShow = true;
}

void CHudInteractHint::UpdateHint( int iX, int iY )
{
    Assert( m_bShow );
	m_iX = iX;
	m_iY = iY;
}