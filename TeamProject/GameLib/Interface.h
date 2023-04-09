#pragma once
//#include "CSprite.h"
#include "BaseObject.h"
#include "Input.h"
enum	E_UIState
{
	UI_NORMAL,
	UI_HOVER,
	UI_PUSH,
	UI_SELECT,
	UI_DIS,
	UI_MAXSTATE
};
class InterfaceWork;
class Interface : public BaseObject
{
public:
	virtual bool Frame() override;
	virtual bool Render() override;
	virtual bool Release() override;
	virtual void SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj) override;
	virtual bool SetAttribute(TVector3 vPos, TRectangle rc);
	virtual bool SetAttribute(TVector3 vPos = {0, 0, 0}, TVector3 vScale = { 1, 1, 1 }, TColor color = { 1, 1, 1, 1 });
	virtual bool SetDrawList(TRectangle rcScaleXY, TRectangle rcScaleUV);
	virtual void AddChild(Interface* pUI)
	{
		m_pChildList.push_back(pUI);
	}
	int		GetMaxFrame()
	{
		return m_pTexList.size();
	}
	void	SetFrame(int index)
	{
		if (index < 0 || index >= m_pTexList.size())
			return;

		m_pTexture = m_pTexList[index];
	}
public:
	void	SetUV(float u0, float v0, float u1, float v1);
	void	ToNDC();
	void	NormalizeToCenter();
public:
	E_UIState	m_CurrentState;

public:
	std::list<InterfaceWork*> m_pWorkList;
	std::vector<Interface*> m_rcDrawList;
	std::vector<Interface*> m_pChildList;
	std::vector<Texture*>	m_pTexList;

public:
	bool	m_isUsing = true;
public:
	virtual void SetTime(float fTime);
};

class InterfaceBillboard : public Interface
{
public:
	virtual void SetMatrix(TMatrix* matWorld, TMatrix* matView, TMatrix* matProj) override;
	virtual bool Frame() override;
	virtual bool Render() override;
};

class InterfaceWork
{
public:
	virtual bool Frame(Interface* pInter) { return true; }

public:
	bool	m_isDone = false;
};

class InterfaceLifeTime : public InterfaceWork
{
public:
	bool Frame(Interface* pInter)
	{
		m_fLifeTime -= g_fSecondPerFrame;
		if (m_fLifeTime <= 0)
		{
			pInter->m_isUsing = false;
		}
		return true;
	}
public:
	InterfaceLifeTime(float fLifeTime)
	{
		m_fLifeTime = fLifeTime;
	}
public:
	float	m_fLifeTime;
};

class InterfaceAnim : public InterfaceWork
{
public:
	bool Frame(Interface* pInter)
	{
		m_fTimer += g_fSecondPerFrame;
		if (m_fTimer >= m_fStep)
		{
			m_fTimer -= m_fStep;
			m_iFrame++;
			if (m_iFrame >= pInter->GetMaxFrame())
				m_iFrame = 0;
		}
		pInter->SetFrame(m_iFrame);
		return true;
	}
public:
	InterfaceAnim(float fStep)
	{
		m_fStep = fStep;
	}
public:
	int		m_iFrame = 0;
	float	m_fTimer = 0.0f;
	float	m_fStep;
};

class InterfaceFade : public InterfaceWork
{
public:
	bool Frame(Interface* pInter)
	{
		m_fAlpha += m_iSwitch * g_fSecondPerFrame * 2.5f;
		if (m_fAlpha >= 2.5f)
		{
			m_iSwitch = -1;
			m_fAlpha = 1.2f;
		}
		if (m_fAlpha <= 0.0f)
		{
			m_isDone = true;
		}

		for (int i = 0; i < pInter->m_VertexList.size(); i++)
		{
			pInter->m_VertexList[i].c.w = m_fAlpha;
		}
		return true;
	}

public:
	bool	m_isFade = false;
	int		m_iSwitch = 1;
	float m_fAlpha = 0.0f;
};

class InterfaceFadeIn : public InterfaceWork
{
public:
	bool	Frame(Interface* pInter)
	{
		m_fAlpha += g_fSecondPerFrame * m_fFadeTime;
		if (m_fAlpha >= m_fFadeTime)
		{
			m_fAlpha = 1.0f;
			m_isDone = true;
		}

		for (int i = 0; i < pInter->m_VertexList.size(); i++)
		{
			pInter->m_VertexList[i].c.w = m_fAlpha;
		}
		return true;
	}
public:
	InterfaceFadeIn(float fFadeTime = 2.5f)
	{
		m_fFadeTime = fFadeTime;
	}
public:
	float m_fAlpha = 0.0f;
	float m_fFadeTime = 2.5f;
};

class InterfaceFadeOut : public InterfaceWork
{
public:
	bool	Frame(Interface* pInter)
	{
		m_fAlpha -= g_fSecondPerFrame * m_fFadeTime;
		if (m_fAlpha <= 0.0f)
		{
			m_isDone = true;
		}

		for (int i = 0; i < pInter->m_VertexList.size(); i++)
		{
			pInter->m_VertexList[i].c.w = m_fAlpha;
		}
		return true;
	}
public:
	InterfaceFadeOut(float fFadeTime = 2.5f)
	{
		m_fFadeTime = fFadeTime;
	}
public:
	float m_fAlpha = 1.2f;
	float m_fFadeTime = 2.5f;
};

class InterfaceLoopFade : public InterfaceWork
{
public:
	bool Frame(Interface* pInter)
	{
		m_fAlpha += m_iSwitch * g_fSecondPerFrame * m_fFadeSpeed;
		if (m_fAlpha >= 1.0f)
		{
			m_iSwitch = -1;
		}
		if (m_fAlpha <= 0.0f)
		{
			m_iSwitch = 1;
		}

		for (int i = 0; i < pInter->m_VertexList.size(); i++)
		{
			pInter->m_VertexList[i].c.w = m_fAlpha;
		}
		return true;
	}
public:
	InterfaceLoopFade(float fFadeSpeed)
	{
		m_fFadeSpeed = fFadeSpeed;
	}

public:
	int		m_iSwitch = 1;
	float m_fAlpha = 0.0f;
	float	m_fFadeSpeed = 0.0f;
};

class InterfaceClick : public InterfaceWork
{
public:
	bool Frame(Interface* pInter)
	{
		POINT ptMouse = I_Input.m_ptPos;
		pInter->m_vScale = TVector3(m_fScale);

		TVector3 pos = pInter->m_vPos;
		long width = pInter->m_pTexture->m_Desc.Width;
		long height = pInter->m_pTexture->m_Desc.Height;

		TRectangle rect = { (long)pos.x, (long)pos.y, width, height };
		if (rect.IntersectPoint(ptMouse))
		{
			pInter->m_CurrentState = UI_HOVER;
			pInter->m_vScale = TVector3(m_fScale * 1.1f);
			if (I_Input.GetKey(VK_LBUTTON) == KEY_PUSH ||
				I_Input.GetKey(VK_LBUTTON) == KEY_HOLD)
			{
				pInter->m_CurrentState = UI_PUSH;
				pInter->m_vScale = TVector3(m_fScale * 0.9f);
			}
			if (I_Input.GetKey(VK_LBUTTON) == KEY_UP)
			{
				pInter->m_CurrentState = UI_SELECT;
			}
		}
		else
		{
			pInter->m_CurrentState = UI_NORMAL;
		}
		pInter->SetFrame(pInter->m_CurrentState);
		return true;
	}

public:
	InterfaceClick(float fScale)
	{
		m_fScale = fScale;
	}
public:
	float	m_fScale;
};

//class InterfaceFadeClockwise : public InterfaceWork
//{
//public:
//	bool Frame(Interface* pInter)
//	{
//		m_fAlpha += m_iSwitch * g_fSecondPerFrame * 2.5f;
//		if (m_fAlpha >= 2.5f)
//		{
//			m_iSwitch = -1;
//			m_fAlpha = 1.2f;
//		}
//		if (m_fAlpha <= 0.0f)
//		{
//			m_isDone = true;
//		}
//
//		for (int i = 0; i < pInter->m_VertexList.size(); i++)
//		{
//			pInter->m_VertexList[i].c.w = m_fAlpha;
//		}
//		return true;
//	}
//
//public:
//	InterfaceFadeClockwise(float fScale)
//	{
//		
//	}
//public:
//	
//};

