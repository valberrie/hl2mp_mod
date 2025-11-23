// valberrie 2025.11.22

#pragma once

#include "basegrenade_shared.h"
#include "weapon_hl2mpbasehlmpcombatweapon.h"

#if defined( CLIENT_DLL )
	#define CWeaponThrowable C_WeaponThrowable
#endif

class CWeaponThrowable : public CBaseHL2MPCombatWeapon
{
public:
    DECLARE_CLASS( CWeaponThrowable, CBaseHL2MPCombatWeapon );
    DECLARE_NETWORKCLASS();
    DECLARE_PREDICTABLE();

    void Spawn() OVERRIDE;
    void Precache() OVERRIDE;
    void PrimaryAttack() OVERRIDE;
    void SecondaryAttack() OVERRIDE;
    void WeaponIdle() OVERRIDE;
    void ItemPostFrame() OVERRIDE;
    bool Reload() OVERRIDE;
    bool Deploy() OVERRIDE;
    bool Holster( CBaseCombatWeapon *pTo = nullptr ) OVERRIDE;

    CWeaponThrowable();

    CNetworkVar( bool, m_bThrowing );
    CNetworkVar( bool, m_bNeedReload );

#ifndef CLIENT_DLL
    DECLARE_ACTTABLE();
    DECLARE_DATADESC();
#endif

private:
    CWeaponThrowable( const CWeaponThrowable & );

    void Throw();
    void BeginThrow();
};