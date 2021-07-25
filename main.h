#pragma once
#include "imgui-1.83/imgui.h"
#include "imgui-1.83/backends/imgui_impl_dx9.h"
#include "imgui-1.83/backends/imgui_impl_win32.h"

#include <d3d9.h>
#include <dinput.h>
#include <tchar.h>
#include <chrono>
#include <thread>


// 定义窗口相关常量
static const char* AppClass = "imgui_template_AppClass";
static const char* AppName = "imgui_template_AppName";
static HWND hwnd;
static int WindowXpos = 150;
static int WindowYpos = 100;
static int WindowWidth = 1280;
static int WindowHeight = 900;

// d3d 设备
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};
static ImFont* DefaultFont = nullptr;

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 窗口处理函数
LRESULT WINAPI  WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;
	
	
	switch (msg) {
		// 窗口的大小发生变化后
	case WM_SIZE:
		return 0;
		// 选择窗口菜单项或选择最大化或最小化时
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) return 0;// Disable ALT application menu
		break;
		//当鼠标移动或者有鼠标键按下时候
	//case WM_NCHITTEST:
	//{
	//	ImVec2 Shit = ImGui::GetMousePos();
	//	if (Shit.y < 25 && Shit.x < WindowWidth - 25)
	//	{
	//		LRESULT hit = DefWindowProc(hWnd, msg, wParam, lParam);
	//		if (hit == HTCLIENT) hit = HTCAPTION;
	//		return hit;
	//	}
	//	else break;
	//}
	// 窗口销毁
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	//调用缺省的窗口过程来为应用程序没有处理的任何窗口消息提供缺省的处理
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


// 创建d3d设备
HRESULT CreateDeviceD3D(HWND hWnd)
{
	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) return E_FAIL;

	ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
	g_d3dpp.Windowed = TRUE;
	g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	g_d3dpp.EnableAutoDepthStencil = TRUE;
	g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	//g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;           // Present with vsync
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;   // Present without vsync, maximum unthrottled framerate
	if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0) return E_FAIL;

	return S_OK;
}

// 清清除d3d设备
void CleanupDeviceD3D()
{
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
	if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

// 重置d3d设备
void ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
	if (hr == D3DERR_INVALIDCALL) IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}
