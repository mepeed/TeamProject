#pragma once

#define _USE_MATH_DEFINES 

#include <Unknwnbase.h>
#include <math.h>
#include <cstdlib>
#include <string>
#include "DXDevice.h"

namespace SSB
{
	typedef char Byte;

	class Common
	{
	public:
		bool Release(IUnknown* ptr) { if (ptr) ptr->Release(); return true; }

		ID3D11Device* m_pd3dDevice = nullptr;
		ID3D11DeviceContext* m_pImmediateContext = nullptr;

		void	SetDevice(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		{
			m_pd3dDevice = pDevice;
			m_pImmediateContext = pContext;
		}

	public:
		/// <summary>
		/// Request Memory and Fill it
		/// </summary>
		/// <returns></returns>
		virtual bool Init() = 0;
		/// <summary>
		/// Calculate everything before Rendering.
		/// </summary>
		/// <returns></returns>
		virtual bool Frame() = 0;
		/// <summary>
		/// when this function called.
		/// It should be drawn.
		/// </summary>
		/// <returns></returns>
		virtual bool Render() = 0;
		/// <summary>
		/// Release memory but don't destroy it when using Memory pool
		/// </summary>
		/// <returns></returns>
		virtual bool Release() = 0;
	};

	inline float DegreeToRadian(float degree)
	{
		return (degree / 180.0f) * M_PI;
	}

	inline bool IsZero(float val) { return abs(val) < 0.001f; }

	static std::wstring mtw(std::string str)
	{
		//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		//return conv.from_bytes(str);
		std::wstring ret;
		ret.assign(str.begin(), str.end());
		return ret;
	}

	static std::string wtm(std::wstring str)
	{
		//std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		//return conv.to_bytes(str);
		std::string ret;
		ret.assign(str.begin(), str.end());
		return ret;
	}

	static const float FDelta = 0.01f;

	static bool ZeroFloat(float val)
	{
		return abs(val) < FDelta;
	}
};
