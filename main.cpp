#include <windows.h>
#include<iostream>
#include <vector>

#include "main.h"
#include "helper.h"

#include "imHotKey.h"



int	main(int argc, char* argv[]) {
	std::cout << "\n Imgui 初始工程模板 win32—dx9 \n BY——御驾亲征\n" << std::endl;
	for (int i = 0; i < argc; i++)
		std::cout<<"第" <<i<<"个参数：" << argv[i] << std::endl;

	RECT desktop; //定义窗口矩形
	GetWindowRect(GetDesktopWindow(), &desktop); //获取桌面矩形大小
	std::cout<<"\n电脑开启了缩放需要右键程序>属性>兼容性>更改高DPI设置>勾选替代高DPI缩放行为>确定" <<"\n"<< std::endl;
	std::cout << "左：" << desktop.left << "右：" << desktop.right << "\n上：" << desktop.top << "下：" << desktop.bottom << std::endl;
	
	// 结构体
	WNDCLASSEX wc = {
		sizeof(WNDCLASSEX), //WNDCLASSEX 的大小
		CS_CLASSDC, // 窗口风格
		WndProc, // 窗口消息处理函数指针
		0, //指定紧跟在窗口类结构后的附加字节数
		0, //指定紧跟在窗口实例的附加字节数
		GetModuleHandle(NULL) , //本模块的实例句柄
		NULL, //图标
		NULL, //光标的句柄
		NULL, //背景画刷的句柄
		NULL, //指向菜单的指针。
		AppClass, //指向类名的指针
		NULL //和窗口类关联的小图标
	};

	// 注册窗口类
	RegisterClassEx(&wc); //注册一个窗口类。
	hwnd = ::CreateWindow(
		AppClass,
		AppName,
		WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX ,
		WindowXpos,
		WindowYpos,
		WindowWidth,
		WindowHeight,
		0, //父窗口的句柄
		0, //菜单的句柄或是子窗口的标识符
		wc.hInstance,
		0
	);


	// Initialize Direct3D
	if (CreateDeviceD3D(hwnd) < 0) {
		CleanupDeviceD3D();
		UnregisterClass(wc.lpszClassName, wc.hInstance);
		return 1;
	}


	// Show the window
	::ShowWindow(hwnd, SW_SHOWDEFAULT);
	::UpdateWindow(hwnd);


	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	DefaultFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\simhei.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(g_pd3dDevice);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	static bool open = true;
	DWORD dwFlag = ImGuiWindowFlags_NoResize  | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollWithMouse;


	// imgui
	IMGUI_CHECKVERSION();

	int num1 = 0, num2 = 0;
	bool showmenuitem = false;

	static std::vector<ImHotKey::HotKey> hotkeys = { 
		 { "Layout", "Reorder nodes in a simpler layout", 0xFFFF261D}
		,{"Save", "Save the current graph", 0xFFFF1F1D}
		,{"Load", "Load an existing graph file", 0xFFFF181D}
		,{"Play/Stop", "Play or stop the animation from the current graph", 0xFFFFFF3F}
		,{"SetKey", "Make a new animation key with the current parameters values at the current time", 0xFFFFFF1F}
		,{u8"测试test", u8"测试描述", 0xFFFFFF3F}
	};

	int keycode = 0;


	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			continue;
		}
		if (!open) ExitProcess(EXIT_SUCCESS);
		std::this_thread::sleep_for(std::chrono::milliseconds(20));

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowSize(ImVec2(WindowWidth, WindowHeight), ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Num1++", "CTRL+Z")) { num1++; }
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		ImGui::SetNextWindowSize(ImVec2(WindowWidth, WindowHeight), ImGuiCond_Once);
		ImGui::SetNextWindowPos(ImVec2(0, 25), ImGuiCond_Once);

		{

			{
				ImGui::Begin(G2U(AppName), &open, dwFlag);
				// 自己的代码
				ImGui::Text(u8"我们");
				ImGui::Text("num1= %d", num1);
				ImGui::Text("num1= %d", num2);
				if (ImGui::Button("num2", ImVec2(64, 29)))
				{
					num2++;
				}
				if (ImGui::Button(u8"热键编辑"))
				{
					ImGui::OpenPopup(u8"热键编辑");
				}
				
				ImHotKey::Edit(hotkeys.data(), hotkeys.size(), u8"热键编辑");
				// ImHotKey also provides a way to retrieve HotKey
				int hotkey = ImHotKey::GetHotKey(hotkeys.data(), hotkeys.size());
				if (hotkey != -1)
				{
					// handle the hotkey index!
					std::cout << hotkey << std::endl;
 
				}

				ImGui::InputInt("keycode", &keycode);
				
				if (ImGui::Button(u8"确认", ImVec2(64,28)))
				{
					ImHotKey::Key key = ImHotKey::GetKeyForScanCode(keycode);
					std::cout<<key.order <<std::endl;
				}

				ImGui::End();
			}
		}

		ImGui::EndFrame();

		g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
		if (g_pd3dDevice->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			g_pd3dDevice->EndScene();
		}
		HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
		if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) ResetDevice();

	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	CleanupDeviceD3D();
	DestroyWindow(hwnd);
	UnregisterClass(wc.lpszClassName, wc.hInstance);
	return 0;
}