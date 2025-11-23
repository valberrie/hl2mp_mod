// valberrie 2025.11.21

#include "cbase.h"
#include "imgui_system.h"
#include "cdll_client_int.h"

#include <imgui.h>

#ifdef WIN32
    #include <windows.h>
    #include <backends/imgui_impl_win32.h>
#else
    #include "SDL.h"
#endif

#include "materialsystem/imaterialvar.h"

#include "memdbgon.h"

CImGuiSystem g_ImGuiSystem;

CImGuiSystem::CImGuiSystem()
{
#ifdef WIN32
    m_pHWND = GetActiveWindow();
#else
    Assert(false);
#endif
}

CImGuiSystem::~CImGuiSystem()
{
}

bool CImGuiSystem::Init()
{
    /*switch ( materials->GetRenderBackend() )
    {
    case RENDER_BACKEND_D3D9:
        AssertMsg( false, "d3d9 is unsupported!" );
        return false;
    case RENDER_BACKEND_VULKAN:
        AssertMsg( false, "vulkan is unsupported!" );
        return false;
    case RENDER_BACKEND_TOGL:
        break;
    default:
        AssertMsg( false, "render backend is unsupported!" );
        return false;
    }*/

    //m_pRenderTexture = materials->CreateNamedRenderTargetTexture( "ImGuiRenderTarget", 800, 600, // dummy values
    //                                           RT_SIZE_FULL_FRAME_BUFFER, IMAGE_FORMAT_RGBA8888, MATERIAL_RT_DEPTH_NONE, true, false );

    //Assert( m_pRenderTexture );

    //m_pMaterial = materials->FindMaterial( "imgui/render", TEXTURE_GROUP_OTHER, true );
    //if ( m_pMaterial == nullptr || m_pMaterial->IsErrorMaterial() )
    //{
    //    Error( "failed to retrieve imgui/render" );
    //    Assert( false );
    //}
    //m_pMaterial->IncrementReferenceCount();

    //IMGUI_CHECKVERSION();
    //ImGui::CreateContext();
    //ImGuiIO &io = ImGui::GetIO();
    //( void )io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    //io.BackendRendererName = "imgui_impl_source_materialsystem";
    //io.Fonts->AddFontDefault();
    //io.Fonts->Build();

    //// Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    //// ImGui::StyleColorsLight();

    //// Setup scaling
    //ImGuiStyle &style = ImGui::GetStyle();
    ////style.ScaleAllSizes( main_scale ); // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    ////style.FontScaleDpi = main_scale;   // Set initial font scale. (using io.ConfigDpiScaleFonts=true makes this unnecessary. We leave both here for documentation purpose)

    //auto *pRender = materials->GetRenderContext();
    //
    //if ( !ImGui_ImplWin32_Init( m_pHWND ) )
    //{   
    //    Warning( "Failed to initialize ImGui." );
    //    return false;
    //}

    return true;
}

void CImGuiSystem::PostInit()
{
}

void CImGuiSystem::Shutdown()
{
    /*ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    m_pMaterial->DecrementReferenceCount();
    m_pRenderTexture->DeleteIfUnreferenced();*/
}

void CImGuiSystem::LevelInitPreEntity()
{
}

void CImGuiSystem::LevelInitPostEntity()
{
}

void CImGuiSystem::LevelShutdownPreClearSteamAPIContext()
{
}

void CImGuiSystem::LevelShutdownPreEntity()
{
}

void CImGuiSystem::LevelShutdownPostEntity()
{
}

void CImGuiSystem::OnSave()
{
}

void CImGuiSystem::OnRestore()
{
}

void CImGuiSystem::SafeRemoveIfDesired()
{
}

#if defined( CLIENT_DLL )

void CImGuiSystem::PreRender()
{
    
}

void CImGuiSystem::Update( float frameTime )
{
    
}

void CImGuiSystem::PostRender()
{
    //ImGui_ImplWin32_NewFrame();
    //ImGui::NewFrame();

    //ImGui::ShowDemoWindow();

    //ImGui::Render();

    //CMatRenderContextPtr pRender( materials );
    //pRender->MatrixMode( MATERIAL_VIEW );
    //pRender->PushMatrix();
    //pRender->LoadIdentity();

    //pRender->MatrixMode( MATERIAL_PROJECTION );
    //pRender->PushMatrix();
    //pRender->LoadIdentity();

    //pRender->MatrixMode( MATERIAL_MODEL );
    //pRender->PushMatrix();
    //pRender->LoadIdentity();

    //IMaterial *pPrevMaterial = pRender->GetCurrentMaterial();
    //pRender->Bind( m_pMaterial );

    ////pRender->OverrideAlphaWriteEnable( true, true );
    //
    //IMesh *pMesh = pRender->GetDynamicMesh();
    //{
    //    auto *pDrawData = ImGui::GetDrawData();

    //    ImVec2 vecClipOff = pDrawData->DisplayPos;
    //    ImVec2 vecClipScale = pDrawData->FramebufferScale;
    //    
    //    int iFbWidth = ( int )( pDrawData->DisplaySize.x * pDrawData->FramebufferScale.x );
    //    int iFbHeight = ( int )( pDrawData->DisplaySize.y * pDrawData->FramebufferScale.y );
    //    //if ( fb_width == 0 || fb_height == 0 )
    //        //return;
    //
    //    // TODO(val): handle imgui textures

    //    // setup render state
    //    // pRender->CullMode( MATERIAL_CULLMODE_CW );
    //    // blend
    //    //pRender->OverrideDepthEnable( true, false );
    //    //pRender->ClearColor4ub( 0, 0, 0, 255 );
    //    //pRender->ClearBuffers( true, true, true );

    //    //Assert( m_pRenderTexture->IsRenderTarget() && !m_pRenderTexture->IsError() );
    //    
    //    CMeshBuilder Builder{};
    //    Builder.Begin( pMesh, MATERIAL_TRIANGLES, 1 );

    //    Builder.Color4ub( 255, 255, 255, 255 );
    //    Builder.Position3f( 0.0f, 0.5f, 0.0f );
    //    Builder.AdvanceVertex();

    //    Builder.Color4ub( 255, 255, 255, 255 );
    //    Builder.Position3f( -0.5f, 0.0f, 0.0f );
    //    Builder.AdvanceVertex();

    //    Builder.Color4ub( 255, 255, 255, 255 );
    //    Builder.Position3f( 0.5f, 0.0f, 0.0f );
    //    Builder.AdvanceVertex();

    //    Builder.End( true, true );

    //    //pRender->DrawScreenSpaceQuad( m_pMaterial );
    //    
    //    
    //    for (const ImDrawList *pDrawList : pDrawData->CmdLists)
    //    {
    //        // define mesh vertices
    //        //CMeshBuilder Builder{};
    //        // Builder.Begin( pMesh, MATERIAL_TRIANGLES, pDrawList->VtxBuffer.Size, pDrawList->IdxBuffer.Size );  
    //        
    //        //for ( ImDrawVert Vert : pDrawList->VtxBuffer )
    //        //{
    //        //    // imgui stores rgba8888 colour as a uint32_t while we need 4x uint8_t
    //        //    uint8_t *Col = reinterpret_cast<uint8_t *>(&Vert.col);
    //        //    Builder.Color4ub( Col[0], Col[1], Col[2], Col[3] );
    //        //    Builder.TexCoord2f(0, Vert.uv.x, Vert.uv.y);
    //        //    Builder.Position3f(Vert.pos.x, Vert.pos.y, 0.0); // inefficient, storing useless Z
    //        //    Builder.AdvanceVertex();

    //        //    Msg( "Adding Vertex{ %d %d %d %d, %.3f %.3f, %.3f, %.3f }\n", Col[0], Col[1], Col[2], Col[3], Vert.uv.x, Vert.uv.y, Vert.pos.x, Vert.pos.y );
    //        //}


    //        //for ( ImDrawIdx Idx : pDrawList->IdxBuffer )
    //        //{
    //        //    Builder.Index(Idx);
    //        //    Builder.AdvanceIndex();
    //        //}

    //        //pMesh->Draw();

    //        //for ( ImDrawCmd Cmd : pDrawList->CmdBuffer ) 
    //        //{   
    //        //    if ( Cmd.UserCallback )
    //        //    {
    //        //        // User callback, registered via ImDrawList::AddCallback()
    //        //        // (ImDrawCallback_ResetRenderState is a special callback value used by the user to request the renderer to reset render state.)
    //        //        if ( Cmd.UserCallback == ImDrawCallback_ResetRenderState )
    //        //            AssertMsg( false, "Reset render state" );
    //        //        else
    //        //            Cmd.UserCallback( pDrawList, &Cmd );

    //        //        continue;
    //        //    }
    //
    //        //    // Project scissor/clipping rectangles into framebuffer space
    //        //    ImVec2 vecClipMin( ( Cmd.ClipRect.x - vecClipOff.x ) * vecClipScale.x, ( Cmd.ClipRect.y - vecClipOff.y ) * vecClipScale.y );
    //        //    ImVec2 vecClipMax( ( Cmd.ClipRect.z - vecClipOff.x ) * vecClipScale.x, ( Cmd.ClipRect.w - vecClipOff.y ) * vecClipScale.y );
    //        //    if ( vecClipMax.x <= vecClipMin.x || vecClipMax.y <= vecClipMin.y )
    //        //        continue;

    //        //    // Apply scissor/clipping rectangle (Y is inverted in OpenGL)
    //        //    //pRender->SetScissorRect( ( int )vecClipMin.x, ( int )( ( float )iFbHeight - vecClipMax.y ), ( int )( vecClipMax.x - vecClipMin.x ), ( int )( vecClipMax.y - vecClipMin.y ), true );
 
    //        //    // Bind texture, Draw
    //        //    //glBindTexture( GL_TEXTURE_2D, ( GLuint )( intptr_t )Cmd.GetTexID() );
    //        //    //glDrawElements( GL_TRIANGLES, ( GLsizei )Cmd.ElemCount, sizeof( ImDrawIdx ) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer + Cmd.IdxOffset );
    //        //    pMesh->Draw(Cmd.IdxOffset, Cmd.ElemCount);
    //        //}
    //    }
    //        

    //    // finally, render
    //    // presumably this just uses an ortho matrix
    //    //pRender->DrawScreenSpaceQuad( m_pMaterial );
    //}
    //
    //// restore state
    ////pRender->OverrideDepthEnable( false, true );
    ////pRender->PopRenderTargetAndViewport();
    ////pRender->Bind( pPrevMaterial );
    //pRender->Flush();

    //pRender->MatrixMode( MATERIAL_VIEW );
    //pRender->PopMatrix();

    //pRender->MatrixMode( MATERIAL_PROJECTION );
    //pRender->PopMatrix();
    //
    //pRender->MatrixMode( MATERIAL_MODEL );
    //pRender->PopMatrix();
}

#else

void CImGuiSystem::FrameUpdatePreEntityThink()
{
}

void CImGuiSystem::FrameUpdatePostEntityThink()
{
}

void CImGuiSystem::PreClientUpdate()
{
}

#endif // CLIENT_DLL