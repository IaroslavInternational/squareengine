#include "Graphics.h"
#include "dxerr.h"
#include <sstream>
#include <d3dcompiler.h>
#include <cmath>
#include <DirectXMath.h>
#include <array>
#include "GraphicsThrowMacros.h"

#include "EngineFunctions.hpp"

#if IS_ENGINE_MODE
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#endif // IS_ENGINE_MODE

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace FramebufferShaders
{
	#include "FramebufferPS.shh"
	#include "FramebufferVS.shh"
}

Graphics::Graphics( HWND hWnd,int width,int height, std::string dataPath)
	:
	width( width ),
	height( height ),
	dataPath(dataPath)
{
	std::ifstream dataFile(dataPath);
	if (!dataFile.is_open())
	{
		throw ("Не удаётся открыть файл с данными о графике");
	}

	json j;
	dataFile >> j;

	dataFile.close();

	for (json::iterator m = j.begin(); m != j.end(); ++m)
	{
		auto d = m.key();

		for (const auto& obj : j.at(d))
		{
			/* Получение настроек заднего фона */

			IsBackgroundDrawn  = obj.at("b-s");
			backgroundColor[0] = obj.at("b-r");
			backgroundColor[1] = obj.at("b-g");
			backgroundColor[2] = obj.at("b-b");

			/*******************************/
		}
	}

	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// for checking results of d3d functions
	HRESULT hr;
	using Microsoft::WRL::ComPtr;
	// create device and front/back buffers, and swap chain and rendering context
	GFX_THROW_INFO( D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pImmediateContext
	) );

	// get handle to backbuffer
	ComPtr<ID3D11Resource> pBackBuffer;
	if (FAILED(hr = pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBuffer)))
	{
		//throw ENGINE_GFX_EXCEPTION(hr, L"Getting back buffer");
	}

	// create a view on backbuffer that we can render to
	if (FAILED(hr = pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pRenderTargetView)))
	{
		//throw ENGINE_GFX_EXCEPTION(hr, L"Creating render target view on backbuffer");
	}


	// set backbuffer as the render target using created view
	pImmediateContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), nullptr);

	// viewport always fullscreen (for now)
	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pImmediateContext->RSSetViewports( 1u,&vp );
	
	///////////////////////////////////////
	// create texture for cpu render target
	D3D11_TEXTURE2D_DESC sysTexDesc;
	sysTexDesc.Width = width;
	sysTexDesc.Height = height;
	sysTexDesc.MipLevels = 1;
	sysTexDesc.ArraySize = 1;
	sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sysTexDesc.SampleDesc.Count = 1;
	sysTexDesc.SampleDesc.Quality = 0;
	sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;
	sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sysTexDesc.MiscFlags = 0;

	// create the texture
	if (FAILED(hr = pDevice->CreateTexture2D(&sysTexDesc, nullptr, &pSysBufferTexture)))
	{
		GFX_THROW_INFO(hr, L"Creating sysbuffer texture");
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = sysTexDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	// create the resource view on the texture
	if (FAILED(hr = pDevice->CreateShaderResourceView(pSysBufferTexture.Get(),
		&srvDesc, &pSysBufferTextureView)))
	{
		GFX_THROW_INFO(hr, L"Creating view on sysBuffer texture");
	}

	////////////////////////////////////////////////
	// create pixel shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	if (FAILED(hr = pDevice->CreatePixelShader(
		FramebufferShaders::g_FramebufferPS,
		sizeof(FramebufferShaders::g_FramebufferPS),
		nullptr,
		&pPixelShader)))
	{
		GFX_THROW_INFO(hr, L"Creating pixel shader");
	}

	/////////////////////////////////////////////////
	// create vertex shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	if (FAILED(hr = pDevice->CreateVertexShader(
		FramebufferShaders::g_FramebufferVS,
		sizeof(FramebufferShaders::g_FramebufferVS),
		nullptr,
		&pVertexShader)))
	{
		GFX_THROW_INFO(hr, L"Creating vertex shader");
	}

	//////////////////////////////////////////////////////////////
	// create and fill vertex buffer with quad for rendering frame
	const FSQVertex vertices[] =
	{
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,1.0f,0.5f,1.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,-1.0f,0.5f,0.0f,1.0f },
	};
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(FSQVertex) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;
	if (FAILED(hr = pDevice->CreateBuffer(&bd, &initData, &pVertexBuffer)))
	{
		GFX_THROW_INFO(hr, L"Creating vertex buffer");
	}

	//////////////////////////////////////////
	// create input layout for fullscreen quad
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};

	// Ignore the intellisense error "namespace has no member"
	if (FAILED(hr = pDevice->CreateInputLayout(ied, 2,
		FramebufferShaders::g_FramebufferVS,
		sizeof(FramebufferShaders::g_FramebufferVS),
		&pInputLayout)))
	{
		GFX_THROW_INFO(hr, L"Creating input layout");
	}

	////////////////////////////////////////////////////
	// Create sampler state for fullscreen textured quad
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	if (FAILED(hr = pDevice->CreateSamplerState(&sampDesc, &pSamplerState)))
	{
		GFX_THROW_INFO(hr, L"Creating sampler state");
	}

	// allocate memory for sysbuffer (16-byte aligned for faster access)
	pSysBuffer = reinterpret_cast<Color*>(
		_aligned_malloc(sizeof(Color) * width * height, 16u));

#if IS_ENGINE_MODE
	// init imgui d3d impl
	ImGui_ImplDX11_Init( pDevice.Get(),pImmediateContext.Get() );
#endif // IS_ENGINE_MODE
}

Graphics::~Graphics()
{
#if IS_ENGINE_MODE
	ImGui_ImplDX11_Shutdown();
#endif // IS_ENGINE_MODE

	// free sysbuffer memory (aligned free)
	if (pSysBuffer)
	{
		_aligned_free(pSysBuffer);
		pSysBuffer = nullptr;
	}
	// clear the state of the device context before destruction
	if (pImmediateContext) pImmediateContext->ClearState();
}

void Graphics::EndFrame()
{
#if IS_ENGINE_MODE
	// imgui frame end
	if( imguiEnabled )
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData() );
	}
#endif // IS_ENGINE_MODE

	HRESULT hr;
	
	// lock and map the adapter memory for copying over the sysbuffer
	if (FAILED(hr = pImmediateContext->Map(pSysBufferTexture.Get(), 0u,
		D3D11_MAP_WRITE_DISCARD, 0u, &mappedSysBufferTexture)))
	{
		GFX_THROW_INFO(hr, L"Mapping sysbuffer");
	}
	// setup parameters for copy operation
	Color* pDst = reinterpret_cast<Color*>(mappedSysBufferTexture.pData);
	const size_t dstPitch = mappedSysBufferTexture.RowPitch / sizeof(Color);
	const size_t srcPitch = width;
	const size_t rowBytes = srcPitch * sizeof(Color);
	// perform the copy line-by-line
	for (size_t y = 0u; y < height; y++)
	{
		memcpy(&pDst[y * dstPitch], &pSysBuffer[y * srcPitch], rowBytes);
	}
	// release the adapter memory
	pImmediateContext->Unmap(pSysBufferTexture.Get(), 0u);

	// render offscreen scene texture to back buffer
	pImmediateContext->IASetInputLayout(pInputLayout.Get());
	pImmediateContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	pImmediateContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	const UINT stride = sizeof(FSQVertex);
	const UINT offset = 0u;
	pImmediateContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	pImmediateContext->PSSetShaderResources(0u, 1u, pSysBufferTextureView.GetAddressOf());
	pImmediateContext->PSSetSamplers(0u, 1u, pSamplerState.GetAddressOf());

	
	// flip back/front buffers
	if (FAILED(hr = pSwapChain->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			GFX_THROW_INFO(pDevice->GetDeviceRemovedReason(), L"Presenting back buffer [device removed]");
		}
		else
		{
			GFX_THROW_INFO(hr, L"Presenting back buffer");
		}
	}

	pImmediateContext->Draw(6u, 0u);
}

void Graphics::BeginFrame() noexcept
{
#if IS_ENGINE_MODE
	// imgui begin frame
	if( imguiEnabled )
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
#endif // IS_ENGINE_MODE

	// clear the sysbuffer
	memset(pSysBuffer, 0u, sizeof(Color) * height * width);
}

#if IS_ENGINE_MODE
void Graphics::EnableImgui() noexcept
{
	imguiEnabled = true;
}

void Graphics::DisableImgui() noexcept
{
	imguiEnabled = false;
}

bool Graphics::IsImguiEnabled() const noexcept
{
	return imguiEnabled;
}
#endif // IS_ENGINE_MODE

UINT Graphics::GetWidth() const noexcept
{
	return width;
}

UINT Graphics::GetHeight() const noexcept
{
	return height;
}

// Graphics exception stuff
Graphics::HrException::HrException( int line,const char* file,HRESULT hr,std::vector<std::string> infoMsgs ) noexcept
	:
	Exception( line,file ),
	hr( hr )
{
	// join all info messages with newlines into single string
	for( const auto& m : infoMsgs )
	{
		info += m;
		info.push_back( '\n' );
	}
	// remove final newline if exists
	if( !info.empty() )
	{
		info.pop_back();
	}
}

const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Код ошибки] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Строка ошибки] " << GetErrorString() << std::endl
		<< "[Описание] " << GetErrorDescription() << std::endl;
	if( !info.empty() )
	{
		oss << "\n[Информация]\n" << GetErrorInfo() << std::endl << std::endl;
	}
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::HrException::GetType() const noexcept
{
	return "Engine Graphics Exception";
}

HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string Graphics::HrException::GetErrorString() const noexcept
{
	return DXGetErrorString( hr );
}

std::string Graphics::HrException::GetErrorDescription() const noexcept
{
	char buf[512];
	DXGetErrorDescription( hr,buf,sizeof( buf ) );
	return buf;
}

std::string Graphics::HrException::GetErrorInfo() const noexcept
{
	return info;
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "Engine Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}

Graphics::InfoException::InfoException( int line,const char * file,std::vector<std::string> infoMsgs ) noexcept
	:
	Exception( line,file )
{
	// join all info messages with newlines into single string
	for( const auto& m : infoMsgs )
	{
		info += m;
		info.push_back( '\n' );
	}
	// remove final newline if exists
	if( !info.empty() )
	{
		info.pop_back();
	}
}

const char* Graphics::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Информация]\n" << GetErrorInfo() << std::endl << std::endl;
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::InfoException::GetType() const noexcept
{
	return "Engine Graphics Info Exception";
}

std::string Graphics::InfoException::GetErrorInfo() const noexcept
{
	return info;
}

RectI Graphics::GetScreenRect()
{
	return{ 0, static_cast<int>(width),0,static_cast<int>(height) };
}

void Graphics::PutPixel(int x, int y, Color c)
{
	assert(x >= 0);
	assert(x < int(Graphics::width));
	assert(y >= 0);
	assert(y < int(Graphics::height));
	pSysBuffer[Graphics::width * y + x] = c;
}

void Graphics::DrawLine(DirectX::XMFLOAT2 p0, DirectX::XMFLOAT2 p1, Color c)
{
	float m = 0.0f;

	if (p1.x != p0.x)
	{
		m = (p1.y - p0.y) / (p1.x - p0.x);
	}

	if (p1.x != p0.x && std::abs(m) <= 1.0f)
	{
		if (p0.x > p1.x)
		{
			std::swap(p0, p1);
		}

		const float b = p0.y - m * p0.x;

		for (int x = (int)p0.x; x < (int)p1.x; x++)
		{
			const float y = m * (float)x + b;

			if (x >= 0 && x < (int)width &&
				y >= 0 && y < (int)height)
			{
				PutPixel(x, (int)y, c);
			}
		}
	}
	else
	{
		if (p0.y > p1.y)
		{
			std::swap(p0, p1);
		}

		const float w = (p1.x - p0.x) / (p1.y - p0.y);
		const float p = p0.x - w * p0.y;

		for (int y = (int)p0.y; y < (int)p1.y; y++)
		{
			const float x = w * (float)y + p;

			if (x >= 0 && x < (int)width &&
				y >= 0 && y < (int)height)
			{
				PutPixel((int)x, y, c);
			}
		}
	}
}

void Graphics::DrawVerticalLine(int start_x, int start_y, int end_y, Color c)
{
	for (int i = start_y; i < end_y; i++)
	{
		if (i < static_cast<int>(height) && start_x < static_cast<int>(width) &&
			i >= 0 && start_x >= 0)
		{
			PutPixel(start_x, i, c);
		}
	}
}

void Graphics::DrawHorizontalLine(int start_x, int end_x, int start_y, Color c)
{
	for (int i = start_x; i < end_x; i++)
	{
		if (i < static_cast<int>(width) && start_y < static_cast<int>(height) &&
			i >= 0 && start_y >= 0)
		{
			PutPixel(i, start_y, c);
		}
	}
}

void Graphics::DrawHitBox(HitBox hb, Color c)
{
	auto& coord = hb.GetCoordinates();
	
	DrawLine({coord.x, coord.y}, {coord.z, coord.y}, c);
	DrawLine({coord.x, coord.w}, {coord.z, coord.w}, c);
	DrawLine({coord.x, coord.y}, {coord.x, coord.w}, c);
	DrawLine({coord.z, coord.y}, {coord.z, coord.w}, c);
}

Color Graphics::GetPixel(int x, int y) const
{
	assert(x >= 0);
	assert(x < int(Graphics::width));
	assert(y >= 0);
	assert(y < int(Graphics::height));
	return pSysBuffer[Graphics::width * y + x];
}

void Graphics::DrawSpriteNonChroma(int x, int y, const Surface2D& s)
{
	DrawSpriteNonChroma(x, y, s.GetRect(), s);
}

void Graphics::DrawSpriteNonChroma(int x, int y, const RectI& srcRect, const Surface2D& s)
{
	DrawSpriteNonChroma(x, y, srcRect, GetScreenRect(), s);
}

void Graphics::DrawSpriteNonChroma(int x, int y, RectI srcRect, const RectI& clip, const Surface2D& s)
{
	assert(srcRect.left >= 0);
	assert(srcRect.right <= s.GetWidth());
	assert(srcRect.top >= 0);
	assert(srcRect.bottom <= s.GetHeight());
	if (x < clip.left)
	{
		srcRect.left += clip.left - x;
		x = clip.left;
	}
	if (y < clip.top)
	{
		srcRect.top += clip.top - y;
		y = clip.top;
	}
	if (x + srcRect.GetWidth() > clip.right)
	{
		srcRect.right -= x + srcRect.GetWidth() - clip.right;
	}
	if (y + srcRect.GetHeight() > clip.bottom)
	{
		srcRect.bottom -= y + srcRect.GetHeight() - clip.bottom;
	}
	for (int sy = srcRect.top; sy < srcRect.bottom; sy++)
	{
		for (int sx = srcRect.left; sx < srcRect.right; sx++)
		{
			PutPixel(x + sx - srcRect.left, y + sy - srcRect.top, s.GetPixel(sx, sy));
		}
	}
}

void Graphics::DrawSprite(int x, int y, const Surface2D& s, Color chroma)
{
	DrawSprite(x, y, s.GetRect(), s, chroma);
}

void Graphics::DrawSprite(int x, int y, const RectI& srcRect, const Surface2D& s, Color chroma)
{
	DrawSprite(x, y, srcRect, GetScreenRect(), s, chroma);
}

void Graphics::DrawSprite(int x, int y, RectI srcRect, const RectI& clip, const Surface2D& s, Color chroma)
{
	assert(srcRect.left >= 0);
	assert(srcRect.right <= s.GetWidth());
	assert(srcRect.top >= 0);
	assert(srcRect.bottom <= s.GetHeight());
	if (x < clip.left)
	{
		srcRect.left += clip.left - x;
		x = clip.left;
	}
	if (y < clip.top)
	{
		srcRect.top += clip.top - y;
		y = clip.top;
	}
	if (x + srcRect.GetWidth() > clip.right)
	{
		srcRect.right -= x + srcRect.GetWidth() - clip.right;
	}
	if (y + srcRect.GetHeight() > clip.bottom)
	{
		srcRect.bottom -= y + srcRect.GetHeight() - clip.bottom;
	}
	for (int sy = srcRect.top; sy < srcRect.bottom; sy++)
	{
		for (int sx = srcRect.left; sx < srcRect.right; sx++)
		{
			const Color srcPixel = s.GetPixel(sx, sy);
			if (srcPixel != chroma)
			{
				PutPixel(x + sx - srcRect.left, y + sy - srcRect.top, srcPixel);
			}
		}
	}
}

void Graphics::DrawSpriteSubstitute(int x, int y, Color substitute, const Surface2D& s, Color chroma)
{
	DrawSpriteSubstitute(x, y, substitute, s.GetRect(), s, chroma);
}

void Graphics::DrawSpriteSubstitute(int x, int y, Color substitute, const RectI& srcRect, const Surface2D& s, Color chroma)
{
	DrawSpriteSubstitute(x, y, substitute, srcRect, GetScreenRect(), s, chroma);
}

void Graphics::DrawSpriteSubstitute(int x, int y, Color substitute, RectI srcRect, const RectI& clip, const Surface2D& s, Color chroma)
{
	assert(srcRect.left >= 0);
	assert(srcRect.right <= s.GetWidth());
	assert(srcRect.top >= 0);
	assert(srcRect.bottom <= s.GetHeight());
	if (x < clip.left)
	{
		srcRect.left += clip.left - x;
		x = clip.left;
	}
	if (y < clip.top)
	{
		srcRect.top += clip.top - y;
		y = clip.top;
	}
	if (x + srcRect.GetWidth() > clip.right)
	{
		srcRect.right -= x + srcRect.GetWidth() - clip.right;
	}
	if (y + srcRect.GetHeight() > clip.bottom)
	{
		srcRect.bottom -= y + srcRect.GetHeight() - clip.bottom;
	}
	for (int sy = srcRect.top; sy < srcRect.bottom; sy++)
	{
		for (int sx = srcRect.left; sx < srcRect.right; sx++)
		{
			const Color srcPixel = s.GetPixel(sx, sy);
			if (srcPixel != chroma)
			{
				// use substitute color instead of color from the Surface2D (if not chroma)
				PutPixel(x + sx - srcRect.left, y + sy - srcRect.top, substitute);
			}
		}
	}
}

void Graphics::DrawSpriteGhost(int x, int y, const Surface2D& s, float deep, Color chroma)
{
	DrawSpriteGhost(x, y, s.GetRect(), s, deep, chroma);
}

void Graphics::DrawSpriteGhost(int x, int y, const RectI& srcRect, const Surface2D& s, float deep, Color chroma)
{
	DrawSpriteGhost(x, y, srcRect, GetScreenRect(), s, deep, chroma);
}

void Graphics::DrawSpriteGhost(int x, int y, RectI srcRect, const RectI& clip, const Surface2D& s, float deep, Color chroma)
{
	assert(srcRect.left >= 0);
	assert(srcRect.right <= s.GetWidth());
	assert(srcRect.top >= 0);
	assert(srcRect.bottom <= s.GetHeight());
	if (x < clip.left)
	{
		srcRect.left += clip.left - x;
		x = clip.left;
	}
	if (y < clip.top)
	{
		srcRect.top += clip.top - y;
		y = clip.top;
	}
	if (x + srcRect.GetWidth() > clip.right)
	{
		srcRect.right -= x + srcRect.GetWidth() - clip.right;
	}
	if (y + srcRect.GetHeight() > clip.bottom)
	{
		srcRect.bottom -= y + srcRect.GetHeight() - clip.bottom;
	}
	for (int sy = srcRect.top; sy < srcRect.bottom; sy++)
	{
		for (int sx = srcRect.left; sx < srcRect.right; sx++)
		{
			const Color srcPixel = s.GetPixel(sx, sy);
			if (srcPixel != chroma)
			{
				const int xDest = x + sx - srcRect.left;
				const int yDest = y + sy - srcRect.top;
				const Color dstPixel = GetPixel(xDest, yDest);

				const Color blendedPixel = {
					unsigned char((dstPixel.GetR() + srcPixel.GetR()) / deep),
					unsigned char((dstPixel.GetG() + srcPixel.GetG()) / deep),
					unsigned char((dstPixel.GetB() + srcPixel.GetB()) / deep)
				};
				PutPixel(x + sx - srcRect.left, y + sy - srcRect.top, blendedPixel);
			}
		}
	}
}

bool Graphics::LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	pDevice->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	pDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();

	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free(image_data);

	return true;
}

void Graphics::DrawBackground()
{
	if (IsBackgroundDrawn)
	{
		for (size_t i = 0; i < width; i++)
		{
			for (size_t j = 0; j < height; j++)
			{
				PutPixel(i, j, Color(backgroundColor[0] * 255.0f, backgroundColor[1] * 255.0f, backgroundColor[2] * 255.0f));
			}
		}
	}
}

void Graphics::SetViewPort(D3D11_VIEWPORT& vp)
{
	pImmediateContext->RSSetViewports(1u, &vp);
}
