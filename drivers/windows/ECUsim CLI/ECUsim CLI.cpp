// ECUsim CLI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ECUsim DLL\ECUsim.h"

std::unique_ptr<ECUsim> sim;

BOOL CtrlHandler(DWORD fdwCtrlType)
{
	if (fdwCtrlType != CTRL_C_EVENT) return FALSE;

	sim->stop();
	sim->join();

	return(TRUE);
}

typedef long	(__stdcall *PTOPEN)(void *pName, unsigned long *pDeviceID);

int main(int argc,      // Number of strings in array argv
	     char *argv[],  // Array of command-line argument strings
	     char *envp[])  // Array of environment variable strings
{

	HINSTANCE hDLL = LoadLibraryA("C:\\nanami\\panda\\drivers\\windows\\Release_x86\\pandaJ2534_0404_32.dll");
	PTOPEN LocalOpen = (PTOPEN)(GetProcAddress(hDLL, "PassThruOpen"));
	unsigned long devId = 0;
	LocalOpen("aa", &devId);
	int count;

	// Display each command-line argument.
	std::cout << "\nCommand-line arguments:\n";
	for (count = 0; count < argc; count++)
		std::cout << "  argv[" << count << "]   " << argv[count] << "\n";

	//SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);

	//sim.reset(new ECUsim("", 500000));
	//sim->join();

    return 0;
}

