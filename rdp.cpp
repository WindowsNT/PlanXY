#include "pch.h"
#include "fetfile.hpp"
#include <wmcodecdsp.h>
#include "rdp.hpp"

extern HICON hIcon1;

bool Rs = 0;
void ShareScreen(int f)
{
	CoInitializeEx(0, COINIT_APARTMENTTHREADED);
	if (Rs)
		return;
	Rs = 1;
	RDP::RDP rdp;
	std::wstring key;
	rdp.CreateANewServer(0, 0, 0, 0, &key);
	Rs = 0;
}


void JoinScreen()
{
	std::wstring the_key;
	bool AskText(HWND hh, const TCHAR * ti, const TCHAR * as, TCHAR * re, std::wstring * re2 = 0, int mxt = 1000);
	std::wstring res;
	if (!AskText((HWND)0, s(1), s(157), 0, &res, 0))
		return;

	RDP::RDP rdp;
	rdp.CreateNewViewer(0, res, 0, 0);

}
