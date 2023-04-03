#include "MyMain.h"
#include "Input.h"
#include "SceneMgr.h"
#include "LightManager.h"

int		MyMain::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    //if (m_pMainCamera)
    //{
    //    m_pMainCamera->WndProc(hWnd, msg, wParam, lParam);
    //}
    return -1;
}

bool    MyMain::Init()
{
    m_MRT.Initialize_SetData(m_pd3dDevice, m_pImmediateContext);
    m_MRT.Init();

    m_screen.Initialize_SetData(m_pd3dDevice, m_pImmediateContext);
    m_screen.Init();

    I_Scene.SetDevice(m_pd3dDevice, m_pImmediateContext);
    I_Scene.Init();

    return true;
}

bool    MyMain::Frame()
{
    if (I_Input.GetKey(VK_ESCAPE) == KEY_PUSH)
        m_bGameRun = false;

    m_MRT.Frame();
    m_screen.Frame();
    
    I_Scene.Frame();
    return true;
}

bool    MyMain::Render()
{
    //// Calculate Shadow Depth Map
    //{
    //    auto lights = SSB::I_Light.GetLightList();
    //    for (auto light : lights)
    //    {
    //        light->PreRender();
    //        I_Scene.PreRender();
    //    }
    //}

    // Calculate MRT data
    m_MRT.Render();
    I_Scene.Render();

    // Change m_RT.m_pTexture desc if test
	//m_pImmediateContext->CopyResource(m_RT.m_pTexture.Get(), m_MRT.m_pPositionTexture);

    // Draw
    {
		m_screen.SetMultiRenderTargetResult(m_MRT.GetResult());
        auto lights = SSB::I_Light.GetLightList();
        for (auto light : lights)
        {
            light->Render();
            m_screen.Render();
            m_pImmediateContext->CopyResource(m_RT.m_pTexture.Get(), m_screen.GetRenderTargetTexture());
        }
    }

    ClearD3D11DeviceContext();
	return true;
}

bool    MyMain::Release()
{
    I_Scene.Release();
    m_MRT.Release();
    m_screen.Release();
    return true;
}

GAME_RUN(MyMain, PROJECTNAME, 1600, 900)