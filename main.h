#pragma once
#include "imgui-1.83/imgui.h"
#include "imgui-1.83/backends/imgui_impl_dx9.h"
#include "imgui-1.83/backends/imgui_impl_win32.h"

#include <d3d9.h>
#include <dinput.h>
#include <tchar.h>
#include <chrono>
#include <thread>


// ���崰����س���
static const char* AppClass = "imgui_template_AppClass";
static const char* AppName = "imgui_template_AppName";
static HWND hwnd;
static int WindowXpos = 150;
static int WindowYpos = 100;
static int WindowWidth = 1280;
static int WindowHeight = 900;

// d3d �豸
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};
static ImFont* DefaultFont = nullptr;

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// ���ڴ�����
LRESULT WINAPI  WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;
	
	
	switch (msg) {
		// ���ڵĴ�С�����仯��
	case WM_SIZE:
		return 0;
		// ѡ�񴰿ڲ˵����ѡ����󻯻���С��ʱ
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) return 0;// Disable ALT application menu
		break;
		//������ƶ���������������ʱ��
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
	// ��������
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	//����ȱʡ�Ĵ��ڹ�����ΪӦ�ó���û�д�����κδ�����Ϣ�ṩȱʡ�Ĵ���
	return DefWindowProc(hWnd, msg, wParam, lParam);
}


// ����d3d�豸
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

// �����d3d�豸
void CleanupDeviceD3D()
{
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
	if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

// ����d3d�豸
void ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
	if (hr == D3DERR_INVALIDCALL) IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}
