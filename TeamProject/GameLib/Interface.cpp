#include "Interface.h"
#include "DXState.h"

bool Interface::Frame()
{
	for (auto iter = m_pWorkList.begin(); iter != m_pWorkList.end();)
	{
		InterfaceWork* work = (*iter);
		if (work->m_isDone)
		{
			iter = m_pWorkList.erase(iter);
			continue;
		}
		else
		{
			work->Frame(this);
			iter++;
		}
	}
	//for (auto data : m_rcDrawList)
	//{
	//	TVector3 pos = data->m_vPos + m_vOffsetPos;
	//	data->SetPosition(pos, data->m_Box.m_vSize, m_vCameraPos);
	//	data->Frame();
	//	for (auto work : m_pWorkList)
	//	{
	//		work->Frame(data);
	//	}
	//}
	//for (auto data : m_pChildList)
	//{
	//	TVector3 pos = data->m_vPos + m_vOffsetPos;
	//	data->SetPosition(pos, data->m_Box.m_vSize, m_vCameraPos);
	//	data->Frame();
	//}
	//
	//SetPosition(m_vPos, m_Box.m_vSize, m_vCameraPos);
	//ScreenToNDC();

	ToNDC();
	UpdateVertexBuffer();
	return true;
}

bool	Interface::Render()
{
	m_pImmediateContext->PSSetSamplers(0, 1, &DXState::g_pDefaultSS);
	BaseObject::Render();

	for (int iSub = 0; iSub < m_rcDrawList.size(); iSub++)
	{
		m_rcDrawList[iSub]->Render();
	}
	for (int iChild = 0; iChild < m_pChildList.size(); iChild++)
	{
		m_pChildList[iChild]->Render();
	}
	m_pImmediateContext->PSSetSamplers(0, 1, &DXState::g_pDefaultSSMirror);
	return true;	
}

bool Interface::Release()
{
	for (auto work : m_pWorkList)
	{
		delete work;
		work = nullptr;
	}
	for (auto data : m_rcDrawList)
	{
		data->Release();
		delete data;
	}

	for (auto data : m_pChildList)
	{
		data->Release();
		delete data;
	}
	BaseObject::Release();
	return true;
}

bool Interface::SetAttribute(TVector3 vPos, TRectangle rc)
{
	Init();

	return true;
}

bool Interface::SetAttribute(TVector3 vPos, TVector3 vScale)
{
	Init();
	//m_ptImageSize.x = m_pTexture->m_Desc.Width;
	//m_ptImageSize.y = m_pTexture->m_Desc.Height;
	//_CRect rc = { 0, 0, (float)m_ptImageSize.x, (float)m_ptImageSize.y };
	//SetUVRect(rc);
	//SetPosition(vPos, m_Box.m_vSize);
	return true;
}

bool    Interface::SetDrawList(TRectangle rcScaleXY, TRectangle rcScaleUV)
{
	//m_rcDrawList.resize(9);
	//for (int i = 0; i < m_rcDrawList.size(); i++)
	//{
	//	m_rcDrawList[i] = new Interface;
	//	m_rcDrawList[i]->Create(m_pd3dDevice, m_pImmediateContext,
	//		m_szShaderPath, m_szTexPath, m_szVSname, m_szPSname);
	//}
	//float width = m_rcActiveImage.right - m_rcActiveImage.left;
	//float height = m_rcActiveImage.bottom - m_rcActiveImage.top;
	//POINTF finLT, finRB;
	//finLT.x = width * rcScaleXY.left;
	//finLT.y = height * rcScaleXY.top;
	//finRB.x = width - width * rcScaleXY.right;
	//finRB.y = height - height * rcScaleXY.bottom;
	//
	//float xArray[4] = { m_rcActiveImage.left, finLT.x, finRB.x, m_rcActiveImage.right };
	//float yArray[4] = { m_rcActiveImage.top, finLT.y, finRB.y, m_rcActiveImage.bottom };
	//
	//float uArray[4] = { 0.0f, rcScaleUV.left, 1.0f - rcScaleUV.right, 1.0f };
	//float vArray[4] = { 0.0f, rcScaleUV.top, 1.0f - rcScaleUV.bottom, 1.0f };
	//
	//for (int i = 0; i < 3; i++)
	//{
	//	for (int j = 0; j < 3; j++)
	//	{
	//		UINT index = i * 3 + j;
	//		TVector3 pos = { (xArray[j] + xArray[j + 1]) * 0.5f, (yArray[i] + yArray[i + 1]) * 0.5f, 0.0f };
	//		m_rcDrawList[index]->SetAttribute(pos + m_vPos * 0.5, { xArray[j], yArray[i], xArray[j + 1], yArray[i + 1] });
	//		m_rcDrawList[index]->m_rcDrawUV = _CRect({ uArray[j], vArray[i], uArray[j + 1], vArray[i + 1] });
	//		m_rcDrawList[index]->UpdateVertexBuffer();
	//	}
	//}

	return true;
}

void	Interface::ToNDC()
{
	// m_vPos ������ g_rcClient
	g_rcClient;
	//m_pTexture->m_Desc.Width;
	//m_pTexture->m_Desc.Height;

	m_VertexList;
	m_vScale;
	float width = m_pTexture->m_Desc.Width / (float)g_rcClient.right * m_vScale.x;
	float height = m_pTexture->m_Desc.Height / (float)g_rcClient.bottom * m_vScale.x;

	TVector3 pos = TVector3(((m_vPos.x / (float)g_rcClient.right) * 2 - 1), ((m_vPos.y / (float)g_rcClient.bottom) * 2 - 1) * -1, 0);
	m_VertexList[0].p.x = pos.x;
	m_VertexList[0].p.y = pos.y;
	
	m_VertexList[1].p = m_VertexList[0].p + TVector3(width, 0, 0);
	m_VertexList[2].p = m_VertexList[0].p + TVector3(0, -height, 0);
	m_VertexList[3].p = m_VertexList[0].p + TVector3(width, -height, 0);
	
	//m_VertexList[0].p = { -1.0f, 1.0f, 0.0f };
	//m_VertexList[1].p = { +1.0f, 1.0f,  0.0f };
	//m_VertexList[2].p = { -1.0f, -1.0f, 0.0f };
	//m_VertexList[3].p = { 1.0f, -1.0f, 0.0f };

	//m_VertexList[0].p.x = -1;
	//m_VertexList[0].p.y = 1;
	//
	//m_VertexList[1].p = m_VertexList[0].p + TVector3(2, 0, 0);
	//m_VertexList[2].p = m_VertexList[0].p + TVector3(0, -2, 0);
	//m_VertexList[3].p = m_VertexList[0].p + TVector3(2, -2, 0);

	//for (int i = 0; i < 4; i++)
	//{
	//	m_VertexList[i].p.x /= g_rcClient.right;
	//	m_VertexList[i].p.y /= g_rcClient.bottom;
	//}
}