// valberrie 2025.11.21

#pragma once

#include "igamesystem.h"

#include "platform.h"

#include "materialsystem/itexture.h"


class CImGuiSystem : public CBaseGameSystemPerFrame
{
public:
    CImGuiSystem();
    ~CImGuiSystem() OVERRIDE;

    inline const char *Name() OVERRIDE { return "CImGuiSystem"; }
    
    // IGameSystem
    bool Init() OVERRIDE;
    void PostInit() OVERRIDE;
    void Shutdown() OVERRIDE;

    void LevelInitPreEntity() OVERRIDE;
    void LevelInitPostEntity() OVERRIDE;
    void LevelShutdownPreClearSteamAPIContext() OVERRIDE;
    void LevelShutdownPreEntity() OVERRIDE;
    void LevelShutdownPostEntity() OVERRIDE;

    void OnSave() OVERRIDE;
    void OnRestore() OVERRIDE;
    void SafeRemoveIfDesired() OVERRIDE;

    inline bool IsPerFrame() OVERRIDE { return true; }

#ifdef CLIENT_DLL
    void PreRender() OVERRIDE;
    void Update( float frameTime ) OVERRIDE;
    void PostRender() OVERRIDE;
#else
    void FrameUpdatePreEntityThink() OVERRIDE;
    void FrameUpdatePostEntityThink() OVERRIDE;
    void PreClientUpdate() OVERRIDE;
#endif

private:
    void *m_pHWND;    

    ITexture *m_pRenderTexture;
    IMaterial *m_pMaterial;
};

extern CImGuiSystem g_ImGuiSystem;

inline CImGuiSystem *ImGuiSystem()
{
    return &g_ImGuiSystem;
}