#include "SceneMgr.h"

void	SceneMgr::SetDevice(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pContext)
{
    m_pd3dDevice = pd3dDevice;
    m_pImmediateContext = pContext;
}


void	SceneMgr::SceneChange(E_SCENE s_type)
{
    m_NextScene = s_type;
}

bool	SceneMgr::Init()
{
    //for (int scene = S_TITLE; scene < MAX_SCENE; scene++)
    //{
    //    m_pSceneArray[scene] = CreateScene((E_SCENE)scene);
    //    if (m_pSceneArray[scene] == nullptr)
    //        continue;
    //    m_pSceneArray[scene]->SetDevice(m_pd3dDevice, m_pImmediateContext);
    //    m_pSceneArray[scene]->Init();
    //}

    m_CurScene = S_TITLE;
    m_pSceneArray[m_CurScene] = CreateScene(m_CurScene);
    m_pSceneArray[m_CurScene]->SetDevice(m_pd3dDevice, m_pImmediateContext);
    m_pSceneArray[m_CurScene]->Init();
    m_isChangingScene = false;

    m_pCurrentScene = &m_pSceneArray[m_CurScene];

    m_pFade = new Interface();
    m_pFade->Create(m_pd3dDevice, m_pImmediateContext, L"../../data/shader/Ui.txt", L"../../data/fade.png");
    m_pFade->m_vPos = TVector3(0, 0, 0);
    m_pFade->m_vScale = TVector3(1, 1, 1);
    m_pFade->m_pWorkList.push_back(new InterfaceFadeOut());
    m_pFade->SetAttribute(TVector3(0, 0, 0), TVector3(1, 1, 1), TColor(1, 1, 1, 1));

    return true;
}

bool	SceneMgr::Frame()
{
    m_pFade->Frame();

    (*m_pCurrentScene)->Frame();

    if (!m_isChangingScene)
        m_NextScene = (*m_pCurrentScene)->NextScene();

    if (m_CurScene != m_NextScene)
    {
        m_isChangingScene = true;
        m_CurScene = m_NextScene;
        m_pFade->m_pWorkList.push_back(new InterfaceFadeIn());
    }
    if (m_isChangingScene)
    {
        if (m_pFade->m_pWorkList.size() == 0)
        {
            Scene** pScene = m_pCurrentScene;
            (*pScene)->Release();
            delete (*pScene);
            (*pScene) = nullptr;
            

            m_pSceneArray[m_CurScene] = CreateScene(m_CurScene);
            m_pSceneArray[m_CurScene]->SetDevice(m_pd3dDevice, m_pImmediateContext);
            m_pSceneArray[m_CurScene]->Init();
            m_isChangingScene = false;

            // 데이터로드를 쓰레드로 돌리면서 진행 정도마다 로딩바 UI의 퍼센트를 대입
            // 100%가 되면 페이드 아웃을 넣으면서 씬의 Init 작동
            // Init의 데이터 로드 부분을 함수로 뺄 필요성 있음
            // 

            m_pCurrentScene = &m_pSceneArray[m_CurScene];

            m_pFade->m_pWorkList.push_back(new InterfaceFadeOut());
            //for (int i = 0; i < E_SCENE::MAX_SCENE; i++)
            //{
            //    if (m_pSceneArray[i] == nullptr)
            //    {
            //        m_pSceneArray[i] = CreateScene((E_SCENE)i);
            //        m_pSceneArray[i]->SetDevice(m_pd3dDevice, m_pImmediateContext);
            //        m_pSceneArray[i]->Init();
            //    }
            //}
        }
    }
    return true;
}

bool SceneMgr::PreRender()
{
    (*m_pCurrentScene)->PreRender();
    return true;
}

bool	SceneMgr::Render()
{
    (*m_pCurrentScene)->Render();
    m_pFade->Render();
    return true;
}

bool	SceneMgr::Release()
{
    if (m_pFade)
    {
        m_pFade->Release();
        delete m_pFade;
    }
    for (Scene* scene : m_pSceneArray)
    {
        if (scene == nullptr)
            continue;
        scene->Release();
        delete scene;
    }
    return true;
}

SceneMgr::SceneMgr()
{

}

SceneMgr::~SceneMgr()
{

}