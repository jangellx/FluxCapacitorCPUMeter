//
// FCLApplicationContext
//  Maintains the system tray icon and its context menu, opening the main window
//  and quitting the program as needed.  It also runs the timer that checks the
//  memory usgae and CPU activity and updates the UI and serial port as approriate.
//

#pragma once

#include "windows.h"
#include "MainForm.h"
#include "FCLSerial.h"

namespace FluxCapacitorCPUMeterWFA {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class FCLApplicationContext : public ApplicationContext 
	{
	private:
		System::ComponentModel::IContainer^		components;			// a list of components to dispose when the context is disposed
		System::Windows::Forms::NotifyIcon^		notifyIcon;			// the icon that sits in the system tray
		System::Windows::Forms::ContextMenuStrip^	notifyIconContextMenu;		// the context menu for the notify icon
		System::Windows::Forms::ToolStripMenuItem^	exitContextMenuItem;		// exit menu command for context menu 
		System::Windows::Forms::ToolStripMenuItem^	showContextMenuItem;		// open menu command for context menu 	
		System::Windows::Forms::ImageList^		imageList;			// for our notification icons, etc
		FluxCapacitorCPUMeterWFA::MainForm^		mainForm;			// the current form we're displaying
		System::Windows::Forms::Timer^			refreshStatsTimer;		// Timer used to periodically update our stats, and thus update the flux capacitor
		System::Diagnostics::PerformanceCounter^	cpuActivityCounter;		// Performance counter for total CPU meter
		FCLSerial^					fclSerial;

	// Constructor
	public: FCLApplicationContext() {
		// Create the notify icon and it's associated context menu
		fclSerial = gcnew FCLSerial();

		InitializeContext();

		fclSerial->Connect();
	}


	// Destructor
	protected: ~FCLApplicationContext() {
		if (components)
			delete components;

		delete fclSerial;
	}

	// <summary>
	// Create the NotifyIcon UI, the ContextMenu for the NotifyIcon and an menu items. 
	// </summary>
	private: void InitializeContext() 
	{
		this->components = gcnew System::ComponentModel::Container();
		System::ComponentModel::ComponentResourceManager^  resources    = (gcnew System::ComponentModel::ComponentResourceManager(FluxCapacitorCPUMeterWFA::MainForm::typeid));
		this->imageList = (gcnew System::Windows::Forms::ImageList(this->components));
		this->refreshStatsTimer = (gcnew System::Windows::Forms::Timer(this->components));
		this->cpuActivityCounter = (gcnew System::Diagnostics::PerformanceCounter());
		this->notifyIcon = gcnew System::Windows::Forms::NotifyIcon(this->components);
		this->notifyIconContextMenu = gcnew System::Windows::Forms::ContextMenuStrip();
		this->showContextMenuItem = gcnew System::Windows::Forms::ToolStripMenuItem();		
		this->exitContextMenuItem = gcnew System::Windows::Forms::ToolStripMenuItem();

		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->cpuActivityCounter))->BeginInit();

		// 
		// imageList
		// 
		this->imageList->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^  >(resources->GetObject(L"imageList.ImageStream")));
		this->imageList->TransparentColor = System::Drawing::Color::Transparent;
		this->imageList->Images->SetKeyName(0, L"notifyIcon.Icon");

		// 
		// refreshStatsTimer
		// 
		this->refreshStatsTimer->Enabled = true;
		this->refreshStatsTimer->Interval = 500;
		this->refreshStatsTimer->Tick += gcnew System::EventHandler(this, &FCLApplicationContext::RefreshStatsTimer_Tick);

		// 
		// cpuActivityCounter
		// 
		this->cpuActivityCounter->CategoryName = L"Processor";
		this->cpuActivityCounter->CounterName = L"% Processor Time";
		this->cpuActivityCounter->InstanceName = L"_Total";

		// 
		// notifyIcon
		// 
		this->notifyIcon->BalloonTipText = L"Flux Capacitor Link";
		this->notifyIcon->Icon = Icon::FromHandle( (cli::safe_cast<System::Drawing::Bitmap^>(this->imageList->Images[ L"notifyIcon.Icon" ]))->GetHicon() );
		this->notifyIcon->Text = L"Flux Capacitor Link";
		this->notifyIcon->Visible = true;
		this->notifyIcon->ContextMenuStrip = this->notifyIconContextMenu;
		this->notifyIcon->DoubleClick += gcnew System::EventHandler(this, &FCLApplicationContext::NotifyIcon_DoubleClick);

		// 
		// notifyIconContextMenu
		// 
		this->notifyIconContextMenu->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) { showContextMenuItem, exitContextMenuItem });

		// 
		// showContextMenuItem
		// 
		this->showContextMenuItem->Text = L"&Settings";
		this->showContextMenuItem->Click += gcnew System::EventHandler(this, &FCLApplicationContext::ShowContextMenuItem_Click);

		// 
		// exitContextMenuItem
		// 
		this->exitContextMenuItem->Text = L"&Exit";
		this->exitContextMenuItem->Click += gcnew System::EventHandler(this, &FCLApplicationContext::ExitContextMenuItem_Click);

		(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->cpuActivityCounter))->EndInit();
	}

	// ShowForm():  Create the window if necessary, then bring it to the front
	private: System::Void ShowForm() {
		if (mainForm == nullptr) {
			// Create a fresh new form instance and show it.
			mainForm = gcnew FluxCapacitorCPUMeterWFA::MainForm( this->fclSerial );
			mainForm->Show();

			// Hook onto the closed event so we can NULL out the main form.
			mainForm->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler( this, &FCLApplicationContext::MainForm_Closed );		
		}

		// Make it active
		mainForm->Activate();
	}

	private: System::Void MainForm_Closed (System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
		// NULL out the main form so we know to create a new one.
		this->mainForm = nullptr;
	}

	// Notify Icon Double-Clicked:  Show the window and bring it to front
	private: System::Void NotifyIcon_DoubleClick(System::Object^  sender, System::EventArgs^  e) {
		ShowForm();
	}

	// Settings Menu Item:  Show the form
	private: System::Void ShowContextMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		ShowForm();
	}

	// Exit Menu Item:  Quit the app
	private: System::Void ExitContextMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		Application::ExitThread();
	}

	// Timer Tick:  Refresh the stats
	private: System::Void RefreshStatsTimer_Tick(System::Object^  sender, System::EventArgs^  e) {
		MEMORYSTATUSEX	mem;
		static int	calls=0;		// Used so we'll only update CPU activity once per second, thus making us consistent with Task Manager

		// Memory
		mem.dwLength = sizeof (mem);
		GlobalMemoryStatusEx (&mem);
		fclSerial->MemoryFree = 1.0 - (((double)(mem.dwMemoryLoad)) / 100.0);

		// CPU Activity
		if( calls++ % 2 )
			fclSerial->CPUActivity = cpuActivityCounter->NextValue() / 100.0;

		// Update the UI
		if( mainForm != nullptr )
			mainForm->RefreshStats();
	}

};
}
