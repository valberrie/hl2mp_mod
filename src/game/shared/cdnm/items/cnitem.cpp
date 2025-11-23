// valberrie 2025.11.22

#include "cbase.h"
#include <KeyValues.h>
#include "filesystem.h"

#include "cnitem.h"

#include "tier0/memdbgon.h"

#define ITEM_SCRIPT_FILE_EXT "vkv"
#define ITEM_SCRIPT_DIR "scripts/cn"
#define ITEM_SCRIPT_MANIFEST_NAME ITEM_SCRIPT_DIR "/item_manifest." ITEM_SCRIPT_FILE_EXT

CnItemInfo_t &CnItemInfo_t::LazyLoad( const char *pszClassName )
{
    if ( !s_Database.HasElement( pszClassName ) )
    {
        Assert( CreateDatabase() );
    }

    IndexType_t nIndex = s_Database.Find( pszClassName );
    Assert( nIndex != s_Database.InvalidIndex() );
    return s_Database.Element( nIndex );
}

bool CnItemInfo_t::CreateDatabase() 
{
    KeyValues *pManifest = new KeyValues( "ItemManifest" );
    if ( !pManifest->LoadFromFile( filesystem, ITEM_SCRIPT_MANIFEST_NAME, "GAME" ) )
    {
        AssertMsg( false, "Failed to load item manifest file %s", ITEM_SCRIPT_MANIFEST_NAME );
        pManifest->deleteThis();
        return false;
    }

    for ( KeyValues *sub = pManifest->GetFirstSubKey(); sub != nullptr; sub = sub->GetNextKey() )
    {
        if ( Q_stricmp( sub->GetName(), "file" ) )
        {
            Error( "In item manifest: expected 'file', got '%s'\n", sub->GetName() );
            pManifest->deleteThis();
            return false;
        }

        const char *pszName = sub->GetString();
        Assert( pszName != nullptr );
        
        Assert( !s_Database.HasElement(pszName) );

        
        char szFileName[256]{};
        Q_snprintf( szFileName, sizeof( szFileName ), ITEM_SCRIPT_DIR "/%s." ITEM_SCRIPT_FILE_EXT, pszName );
        
        KeyValues *pDataFile = new KeyValues( "ItemDataFile" );
        if ( !pDataFile->LoadFromFile( filesystem, szFileName, "GAME" ) )
        {
            Error( "File '%s' listed in item manifest couldn't be loaded.\n", szFileName );
            pDataFile->deleteThis();
            pManifest->deleteThis();
            return false;
        }

        CnItemInfo_t Info = ParseInfoFile( pszName, pDataFile );
        s_Database.Insert( pszName, Info );

        pDataFile->deleteThis();
    }

    pManifest->deleteThis();
    return true;
}

static const char *KVGetStringChecked( KeyValues *pKV, const char *pszName )
{
    const char *str = pKV->GetString( pszName, { '\0' } );
    Assert( str[0] != '\0' );
    if ( str[0] == '\0' )
    {
        Error( "Failed to parse checked string '%s' from KeyValues\n", pszName );
        return nullptr;
    }

    return str;
}

static int KVGetIntChecked( KeyValues *pKV, const char *pszName )
{
    int i = pKV->GetInt( pszName, INT_MAX );
    Assert( i != INT_MAX );
    if ( i == INT_MAX )
    {
        Error( "Failed to parse checked int '%s' from KeyValues\n", pszName );
        return 0;
    }

    return i;
}


CnItemInfo_t CnItemInfo_t::ParseInfoFile( const char *pszName, KeyValues *pDataFile )
{
    CnItemInfo_t outInfo{};
    Q_strncpy( outInfo.szClassName, pszName, CN_ITEM_STRING_LENGTH );
    Q_strncpy( outInfo.szPrettyName, KVGetStringChecked( pDataFile, "PrintName" ), CN_ITEM_STRING_LENGTH );
    int iMaxStack = KVGetIntChecked( pDataFile, "MaxStack" );
    Assert( iMaxStack >= 0 );
    outInfo.nMaxStack = iMaxStack;

    outInfo.eFlags = CN_ITEM_FLAGS_NONE;
    if ( pDataFile->GetBool( "Stackable" ) )
    {
        outInfo.eFlags = (CnItemFlags_t)( outInfo.eFlags | CN_ITEM_FLAGS_STACKABLE );
    }

    if ( pDataFile->GetBool( "Equipable" ) )
    {
        outInfo.eFlags = ( CnItemFlags_t )( outInfo.eFlags | CN_ITEM_FLAGS_EQUIPABLE );
    }

    outInfo.m_bInitialized = true;
    return outInfo;
}

CUtlMap< const char *, CnItemInfo_t, CnItemInfo_t::IndexType_t > CnItemInfo_t::s_Database{};



IMPLEMENT_NETWORKCLASS_ALIASED( CnInventory, DT_CnInventory );

// clang-format off

BEGIN_NETWORK_TABLE( CCnInventory, DT_CnInventory )
#ifdef CLIENT_DLL
    RecvPropEHandle( RECVINFO( m_hOwner ) ),
    RecvPropInt( RECVINFO( m_nNextValidID ) ),
#else
    SendPropEHandle( SENDINFO( m_hOwner ) ),
    SendPropInt( SENDINFO( m_nNextValidID ), 32, SPROP_UNSIGNED ),
    
#endif
END_NETWORK_TABLE()

#ifdef CLIENT_DLL
// prediction
#endif

LINK_ENTITY_TO_CLASS( cn_inventory, CCnInventory );
PRECACHE_REGISTER( cn_inventory );

// clang-format on

CCnInventory::CCnInventory()
{
}

CCnInventory::~CCnInventory()
{
}

void CCnInventory::Spawn()
{
    BaseClass::Spawn();
    Precache();
}

void CCnInventory::Precache()
{
    BaseClass::Precache();
}

const CnItemInfo_t &CCnInventory::GetItem( CnInventoryItemID_t nID ) const
{
    Assert( IsValidID( nID ) );
    IndexType_t nIndex = GetIndexFromID( nID );
    return m_Items.Element( nIndex );
}

// NOTE: i don't actually know if this works.
//       with any good compiler NRVO should kick in 
//       but it's possible that the vector dies as it's returned.
const CUtlVector< CnInventoryItemID_t > &CCnInventory::GetAllItems() const
{
    CUtlVector< CnInventoryItemID_t > Items;
    Items.SetSize( m_Items.Count() );
    
    for ( IndexType_t nIndex = 0; nIndex < m_Items.Count(); ++nIndex )
    {
        CnInventoryItemID_t nID = m_Items.Key( nIndex );
        Items.AddToTail( nID );
    }
    
    return Items;
}

CnInventoryItemID_t CCnInventory::AddItem( CnItemInfo_t &&Info )
{
    CnInventoryItemID_t nID = m_nNextValidID++;
    Assert( !IsValidID( nID ) );
    m_Items.Insert( nID, Info );
    return nID;
}

bool CCnInventory::RemoveItem( CnInventoryItemID_t nID )
{
    Assert( IsValidID( nID ) );
    if ( !IsValidID( nID ) )
    {
        Warning( "CCnInventory: Attempted to remove invalid ID %lu\n", nID );
        return false;
    }

    m_Items.Remove( nID );

    // extra insurance
    Assert( !IsValidID( nID ) );
    Assert( !IsValidID( m_nNextValidID ) );
    
    return true;
}