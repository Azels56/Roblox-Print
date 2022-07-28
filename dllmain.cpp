#include <Windows.h>
#include <iostream>
#include <string>
#include <thread>
#define x(x) (x - 0x400000)

void console(const char* title) {
	DWORD old;
	VirtualProtect(reinterpret_cast<PVOID>(&FreeConsole), 1, PAGE_EXECUTE_READWRITE, &old);
	*reinterpret_cast<std::uint8_t*>(&FreeConsole) = 0xC3;
	VirtualProtect(&FreeConsole, 1, old, &old);
	AllocConsole();
	SetConsoleTitleA(title);
	FILE* file_stream;
	freopen_s(&file_stream, "CONOUT$", "w", stdout);
	freopen_s(&file_stream, "CONOUT$", "w", stderr);
	freopen_s(&file_stream, "CONIN$", "r", stdin);
}

void main()
{
	console("test");
	// Singeton Print
	const auto print = reinterpret_cast<int(__cdecl*)(int, const char*, ...)>(reinterpret_cast<std::uintptr_t>(GetModuleHandle(nullptr)) + x(0x805230));
	std::string str;
	int type;
	std::cout << "0 - Regular, 1 - Info, 2 - Warn, 3 - Error" << std::endl;
	while (true) {
		std::cout << "Type: ";
		std::cin >> type;
		std::cout << "Print: ";
		std::cin >> str;
		const char* c = str.c_str();
		print(type, c);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		std::thread(main).detach();
	return TRUE;
}
