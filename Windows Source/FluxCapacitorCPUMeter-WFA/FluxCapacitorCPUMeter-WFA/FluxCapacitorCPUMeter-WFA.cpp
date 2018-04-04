// FluxCapacitorCPUMeter-WFA.cpp : main project file.

#include "stdafx.h"
#include "FCLApplication.h"

using namespace FluxCapacitorCPUMeterWFA;

[STAThreadAttribute]
int main(cli::array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault( false ); 

	// Create our own application context so that we can avoid showing a window on startup
	ApplicationContext ^applicationContext = gcnew FCLApplicationContext();
	Application::Run( applicationContext );

	return 0;
}
