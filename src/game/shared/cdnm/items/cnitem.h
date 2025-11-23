// valberrie 2025.11.22

#ifdef GAME_DLL
    #include "baseanimating.h"
#endif

constexpr size_t CN_ITEM_STRING_LENGTH = 100;

enum CnItemFlags_t : uint8_t
{
    CN_ITEM_FLAGS_NONE = 0,
    CN_ITEM_FLAGS_STACKABLE = 1 << 1,
    CN_ITEM_FLAGS_EQUIPABLE = 1 << 2,
};

class CnItemInfo_t
{
public:
    static CnItemInfo_t &LazyLoad( const char *pszClassName );

    CnItemInfo_t() = default;

    char szClassName[CN_ITEM_STRING_LENGTH]; // The entity to spawn when this item is equipped (typically a CWeaponSomething)
    char szPrettyName[CN_ITEM_STRING_LENGTH]; // The name to display
    uint32_t nMaxStack;
    CnItemFlags_t eFlags;

private:
    typedef size_t IndexType_t;

    static bool CreateDatabase();
    static CnItemInfo_t ParseInfoFile( const char *pszName, KeyValues *pDataFile );
    
    static CUtlMap< const char *, CnItemInfo_t, IndexType_t > s_Database;

    bool m_bInitialized{ false };
};

typedef uint32_t CnInventoryItemID_t;

#ifdef CLIENT_DLL
    #define CCnInventory C_CnInventory
#endif

class CCnInventory : public CBaseEntity
{
public:
    DECLARE_CLASS( CCnInventory, CBaseEntity );
    DECLARE_NETWORKCLASS();
    //DECLARE_PREDICTABLE();
#ifdef GAME_DLL
    //DECLARE_ENT_SCRIPTDESC();
#endif
    
    CCnInventory();
    virtual ~CCnInventory();

    // CBaseEntity
    void Spawn() OVERRIDE;
    void Precache() OVERRIDE;

    inline bool IsValidID( CnInventoryItemID_t nID ) const { return m_Items.HasElement( nID ) && m_Items.IsValidIndex( GetIndexFromID( nID ) ); }

    // NOTE: Do not store a reference given to you by this function!
    //       Items' underlying storage will resize at any time.
    const CnItemInfo_t &GetItem( CnInventoryItemID_t nID ) const;

    // NOTE: This is safe to store.
    const CUtlVector< CnInventoryItemID_t > &GetAllItems() const;
    
    CnInventoryItemID_t AddItem( CnItemInfo_t &&Info );

    bool RemoveItem( CnInventoryItemID_t nID );

    CNetworkHandle( CBaseEntity, m_hOwner );
    CNetworkVar( CnInventoryItemID_t, m_nNextValidID );

private:
    typedef uint32_t IndexType_t;
    static constexpr auto INDEX_MAX = UINT32_MAX;

    inline IndexType_t GetIndexFromID( CnInventoryItemID_t nID ) const
    {
        IndexType_t nIndex = m_Items.Find( nID );
        Assert( nIndex != INDEX_MAX );
        return nIndex;
    };


    CUtlMap< CnInventoryItemID_t, CnItemInfo_t, IndexType_t > m_Items;
};
