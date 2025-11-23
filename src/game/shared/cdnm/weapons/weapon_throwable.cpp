// valberire 2025.11.22

#include "cbase.h"

#include "gamerules.h"
#include "npcevent.h"
#include "in_buttons.h"
#include "engine/IEngineSound.h"

#if defined( CLIENT_DLL )
	#include "c_hl2mp_player.h"
#else
	#include "hl2mp_player.h"
	#include "entitylist.h"
	#include "eventqueue.h"
    #include "props.h"
    #include "util.h"
#endif

#include "weapon_throwable.h"

#include "tier0/memdbgon.h"

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponThrowable, DT_WeaponThrowable );

// clang-format off
BEGIN_NETWORK_TABLE( CWeaponThrowable, DT_WeaponThrowable )
#ifdef CLIENT_DLL
    RecvPropBool( RECVINFO( m_bThrowing ) ),
    RecvPropBool( RECVINFO( m_bNeedReload ) ),
#else
    SendPropBool( SENDINFO( m_bThrowing ) ),
    SendPropBool( SENDINFO( m_bNeedReload ) ),
#endif
END_NETWORK_TABLE()

#ifdef CLIENT_DLL

BEGIN_PREDICTION_DATA( CWeaponThrowable )
    DEFINE_PRED_FIELD( m_bThrowing, FIELD_BOOLEAN, FTYPEDESC_INSENDTABLE ),
    DEFINE_PRED_FIELD( m_bNeedReload, FIELD_BOOLEAN, FTYPEDESC_INSENDTABLE ),
END_PREDICTION_DATA()

#endif

LINK_ENTITY_TO_CLASS( weapon_throwable, CWeaponThrowable );
PRECACHE_WEAPON_REGISTER( weapon_throwable );

#ifndef CLIENT_DLL

BEGIN_DATADESC( CWeaponThrowable )
    DEFINE_FIELD( m_bThrowing, FIELD_BOOLEAN ),
    DEFINE_FIELD( m_bNeedReload, FIELD_BOOLEAN ),
END_DATADESC()

acttable_t CWeaponThrowable::m_acttable[] = {
    { ACT_RANGE_ATTACK1, ACT_RANGE_ATTACK_SLAM, true },
    { ACT_HL2MP_IDLE, ACT_HL2MP_IDLE_SLAM, false },
    { ACT_HL2MP_RUN, ACT_HL2MP_RUN_SLAM, false },
    { ACT_HL2MP_IDLE_CROUCH, ACT_HL2MP_IDLE_CROUCH_SLAM, false },
    { ACT_HL2MP_WALK_CROUCH, ACT_HL2MP_WALK_CROUCH_SLAM, false },
    { ACT_HL2MP_GESTURE_RANGE_ATTACK, ACT_HL2MP_GESTURE_RANGE_ATTACK_SLAM, false },
    { ACT_HL2MP_GESTURE_RELOAD, ACT_HL2MP_GESTURE_RELOAD_SLAM, false },
    { ACT_HL2MP_JUMP, ACT_HL2MP_JUMP_SLAM, false },
};

IMPLEMENT_ACTTABLE( CWeaponThrowable );
// clang-format on

#endif

CWeaponThrowable::CWeaponThrowable()
{
    m_bThrowing = false;
    m_bNeedReload = true;
}

void CWeaponThrowable::Spawn()
{
    BaseClass::Spawn();
    
    Precache();
    
    FallInit();

    m_iClip1 = 1;
}

void CWeaponThrowable::Precache()
{
    BaseClass::Precache();
}

bool CWeaponThrowable::Holster( CBaseCombatWeapon *pTo /* = nullptr */ )
{
    SetThink( nullptr );
    return BaseClass::Holster( pTo );
}

bool CWeaponThrowable::Reload()
{
    Assert( m_bNeedReload );
    WeaponIdle();
    m_bNeedReload = false;
    return true;
}

void CWeaponThrowable::PrimaryAttack()
{
    CBaseCombatCharacter *pOwner = GetOwner();
    Assert( pOwner );

    if ( pOwner->GetAmmoCount(m_iPrimaryAmmoType) <= 0 )
    {
        return;
    }

    BeginThrow();
}

void CWeaponThrowable::SecondaryAttack()
{
    return;
}

void CWeaponThrowable::Throw()
{   
#ifndef CLIENT_DLL
    m_bThrowing = false;

    CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
    
    Vector vecSrc = pPlayer->WorldSpaceCenter();
    Vector vecDir = pPlayer->BodyDirection3D();
    vecSrc += vecDir * 18.0;
    vecSrc.z += 24.0f;

    Vector vecThrow{};
    GetOwner()->GetVelocity( &vecThrow, NULL );
    vecThrow += vecDir * 500;
    
    CPhysicsProp *pProp = static_cast< CPhysicsProp * >( CreateEntityByName( "prop_physics" ) );
    Assert( pProp != nullptr );

    pProp->SetModel( GetWorldModel() );
    pProp->ApplyAbsVelocityImpulse( vecThrow );
    pProp->SetLocalAngularVelocity( QAngle( 0, 400, 0 ) );

    pPlayer->RemoveAmmo( 1, m_iPrimaryAmmoType );
    pPlayer->SetAnimation( PLAYER_ATTACK1 );    
#endif

    EmitSound( "Weapon_SLAM.SatchelThrow" );
}

void CWeaponThrowable::BeginThrow()
{
    SendWeaponAnim( ACT_SLAM_THROW_THROW_ND );    

    m_bNeedReload = true;
    m_bThrowing = true;
    m_flNextPrimaryAttack = gpGlobals->curtime + SequenceDuration();
    m_flNextSecondaryAttack = gpGlobals->curtime + SequenceDuration();
}

void CWeaponThrowable::ItemPostFrame()
{
    CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
    if ( pPlayer == nullptr )
    {
        return;
    }

    if ( ( pPlayer->m_nButtons & IN_ATTACK ) && ( m_flNextPrimaryAttack <= gpGlobals->curtime ) )
    {
        PrimaryAttack();
        return;
    }

    WeaponIdle();
}

void CWeaponThrowable::WeaponIdle()
{
    if ( !HasWeaponIdleTimeElapsed() )
    {
        return;   
    }
    
    auto *pOwner = GetOwner();
    Assert( pOwner );

    int iAnim = 0;
    if (m_bThrowing)
    {
        Throw();
        iAnim = ACT_SLAM_THROW_THROW_ND2;
    }
    else if (m_bNeedReload)
    {
        if ( pOwner->GetAmmoCount( m_iPrimaryAmmoType ) <= 0 )
        {
#ifndef CLIENT_DLL
            pOwner->Weapon_Drop( this );
            UTIL_Remove( this );
#endif
            return;
        }

        // TODO
        iAnim = ACT_SLAM_THROW_ND_DRAW;   
        m_bNeedReload = false;
    }
    else
    {
        iAnim = ACT_SLAM_THROW_ND_IDLE;
    }

    SendWeaponAnim( iAnim );
}

bool CWeaponThrowable::Deploy()
{
    auto *pOwner = GetOwner();
    Assert( pOwner );
    
    SetModel( GetViewModel() );
    
    if ( pOwner->GetAmmoCount( m_iPrimaryAmmoType) <= 0 )
    {
        m_bNeedReload = true;
    }
    
    int iAnim = ACT_SLAM_THROW_ND_DRAW;
    return DefaultDeploy( ( char * )GetViewModel(), ( char * )GetWorldModel(), iAnim, ( char * )GetAnimPrefix() );
}