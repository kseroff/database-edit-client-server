#include "Chat.h"
#include <Windows.h>

using namespace PopovTCPclient;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew Chat);
	return 0;
}