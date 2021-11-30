#pragma once

#include "EngineWin.h"
#include "EngineException.h"
#include <d3d11.h>
#include "EngineWRL.h"
#include <vector>
#include "DxgiInfoManager.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <memory>
#include <random>
#include "ConditionalNoexcept.h"

#include "Colors.h"
#include "Surface2D.h"
#include "RectI.h"
#include "HitBox.h"
#include "Line.h"

class Graphics
{
public:
	friend class GUISystem;
public:
	class Exception : public EngineException
	{
		using EngineException::EngineException;
	};
	class HrException : public Exception
	{
	public:
		HrException( int line,const char* file,HRESULT hr,std::vector<std::string> infoMsgs = {} ) noexcept;
		
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		
		HRESULT GetErrorCode() const noexcept;
		
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;
	private:
		HRESULT hr;
		std::string info;
	};
	class InfoException : public Exception
	{
	public:
		InfoException( int line,const char* file,std::vector<std::string> infoMsgs ) noexcept;
		
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		
		std::string GetErrorInfo() const noexcept;
	private:
		std::string info;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;
	public:
		const char* GetType() const noexcept override;
	private:
		std::string reason;
	};
public:
	Graphics(HWND hWnd, int width, int height, std::string dataPath);
	Graphics(const Graphics&) = delete;
	~Graphics();
	Graphics& operator=(const Graphics&) = delete;
public:
	void  BeginFrame()			 noexcept;
	void  EndFrame();
	void  EnableImgui()			 noexcept;
	void  DisableImgui()		 noexcept;
	bool  IsImguiEnabled() const noexcept;
	UINT  GetWidth()	   const noexcept;
	UINT  GetHeight()	   const noexcept;
	RectI GetScreenRect();
public:
	Color GetPixel(int x, int y) const;
	void  PutPixel(int x, int y, int r, int g, int b);
	void  PutPixel(int x, int y, Color c);
public:
	void DrawBackground();
	void DrawGrid();
	void SetViewPort(D3D11_VIEWPORT& vp);
public:
	void DrawLine(Physics::Line line, Color c = Colors::White);
	void DrawLine(DirectX::XMFLOAT2 p0, DirectX::XMFLOAT2 p1, Color c = Colors::White);
	void DrawHitBox(HitBox hb, Color c = Colors::DodgerBlue);
public:
	void DrawSpriteNonChroma(int x, int y, const Surface2D& s);
	void DrawSpriteNonChroma(int x, int y, const RectI& srcRect, const Surface2D& s);
	void DrawSpriteNonChroma(int x, int y, RectI srcRect, const RectI& clip, const Surface2D& s);
	void DrawSprite(int x, int y, const Surface2D& s, Color chroma = Colors::Magenta);
	void DrawSprite(int x, int y, const RectI& srcRect, const Surface2D& s, Color chroma = Colors::Magenta);
	void DrawSprite(int x, int y, RectI srcRect, const RectI& clip, const Surface2D& s, Color chroma = Colors::Magenta);
	void DrawSpriteSubstitute(int x, int y, Color substitute, const Surface2D& s, Color chroma = Colors::Magenta);
	void DrawSpriteSubstitute(int x, int y, Color substitute, const RectI& srcRect, const Surface2D& s, Color chroma = Colors::Magenta);
	void DrawSpriteSubstitute(int x, int y, Color substitute, RectI srcRect, const RectI& clip, const Surface2D& s, Color chroma = Colors::Magenta);
	void DrawSpriteGhost(int x, int y, const Surface2D& s, float deep = 2.0f, Color chroma = Colors::Magenta);
	void DrawSpriteGhost(int x, int y, const RectI& srcRect, const Surface2D& s, float deep = 2.0f, Color chroma = Colors::Magenta);
	void DrawSpriteGhost(int x, int y, RectI srcRect, const RectI& clip, const Surface2D& s, float deep = 2.0f, Color chroma = Colors::Magenta);
	void DrawSpriteGhostNonChroma(int x, int y, const Surface2D& s, float deep = 2.0f);
	void DrawSpriteGhostNonChroma(int x, int y, const RectI& srcRect, const Surface2D& s, float deep = 2.0f);
	void DrawSpriteGhostNonChroma(int x, int y, RectI srcRect, const RectI& clip, const Surface2D& s, float deep = 2.0f);
	void DrawFullscreenImage(const Surface2D& s, float deep);
public:
	bool LoadTextureFromFile(std::string filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
	bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
private:
	struct FSQVertex
	{
		float x, y, z; // Кооординаты
		float u, v;	   // Координаты текстуры
	};
	UINT   width;				// Ширина экрана
	UINT   height;			    // Высота экрана
	bool   imguiEnabled = true; // Статус доступности GUI
private:
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
	Microsoft::WRL::ComPtr<IDXGISwapChain>			 pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>			 pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		 pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	 pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			 pSysBufferTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pSysBufferTextureView;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		 pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>		 pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			 pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>		 pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		 pSamplerState;
	D3D11_MAPPED_SUBRESOURCE						 mappedSysBufferTexture;
	Color* pSysBuffer = nullptr;
private:
	std::string dataPath;			// Путь к данным о графическом движке
	bool        IsBackgroundDrawn;	// Статус отрисовки заднего фона
	float       backgroundColor[3];	// Цвет заднего фона
	bool        IsGridDrawn;		// Статус отрисовки сетки
	float       gridColor[3];		// Цвет сетки
	float       gridScale;			// Масштаб
	float		POS_X_LIMIT;		// Граница мира по оси X
	float		POS_Y_LIMIT;		// Граница мира по оси Y
};