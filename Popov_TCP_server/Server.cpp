#include "Server.h"
#include <Windows.h>

using namespace PopovTCPserver;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	PopovTCPserver::Server^ mainForm = gcnew PopovTCPserver::Server();
	wrapper = mainForm;
	Application::Run(mainForm);

	return 0;
}
