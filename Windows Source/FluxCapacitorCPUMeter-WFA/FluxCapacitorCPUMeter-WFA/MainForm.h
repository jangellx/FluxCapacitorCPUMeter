//
// MainForm
//  This maintains the user interface.  This interface is transient (it only exists when the dialog
//  is open), meaning we do not maintain application state here.  FCLSerial is used to maintain that
//  state.
//

#pragma once

#include <sstream>
#include "FCLSerial.h"

using namespace std;

namespace FluxCapacitorCPUMeterWFA {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MainForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		// Default constructor
		MainForm(void) {
			MainForm_CommonInit();
		}

		// Alternate constructor that takes the serial interface to use to change serial properties.
		MainForm( FCLSerial^ serial ) {
			this->fclSerial = serial;
			MainForm_CommonInit();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm() {
			if (components)
				delete components;
		}

		// Common init
		void MainForm_CommonInit() {
			// Initialize the components
			InitializeComponent();

			// Update enable values
			this->COMPortField->Text                =  fclSerial->COMPort;
			this->ConnectToggle->Text               =  fclSerial->IsOpen ?  L"Disconnect" : L"Connect";

			this->UseCPUActivityToggle->Checked     =  fclSerial->UseCPUActivity;
			this->PulseSpeedField->Value            =  fclSerial->CustomPulseSpeed;

			this->UseMemoryAvailableToggle->Checked =  fclSerial->UseMemoryFree;
			this->PulseSpeedSlider->Value           =  fclSerial->CustomPulseSpeed;
			this->LightWhiteLEDsRadio->Checked      = !fclSerial->CustomRedLEDs;
			this->LightRedLEDsRadio->Checked        =  fclSerial->CustomRedLEDs;

			// Update the UI states
			UpdateEnableStates();
			UpdateCOMPortNames();
			RefreshStats();
		}

	private:
		// Member Variables
		FCLSerial^				 fclSerial;

		// Controls
		System::Windows::Forms::GroupBox^	SerialGroupBox;
		System::Windows::Forms::ComboBox^	COMPortField;
		System::Windows::Forms::Label^		COMPortFieldLabel;
		System::Windows::Forms::Button^		ConnectToggle;

		System::Windows::Forms::GroupBox^	ConfigurationGroupBox;

		System::Windows::Forms::CheckBox^	UseCPUActivityToggle;
		System::Windows::Forms::TrackBar^	PulseSpeedSlider;
		System::Windows::Forms::Label^		PulseSpeedSliderLabel;
		System::Windows::Forms::NumericUpDown^	PulseSpeedField;

		System::Windows::Forms::CheckBox^	UseMemoryAvailableToggle;
		System::Windows::Forms::RadioButton^	LightWhiteLEDsRadio;
		System::Windows::Forms::RadioButton^	LightRedLEDsRadio;

		System::Windows::Forms::GroupBox^	StatusGroupBox;
		System::Windows::Forms::Label^		CPUStatusLabel;
		System::Windows::Forms::Label^		MemoryStatusLabel;
		System::Windows::Forms::Button^		QuitButton;

		System::Windows::Forms::ImageList^	imageList;

		System::ComponentModel::IContainer^	components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
			this->COMPortField = (gcnew System::Windows::Forms::ComboBox());
			this->COMPortFieldLabel = (gcnew System::Windows::Forms::Label());
			this->UseCPUActivityToggle = (gcnew System::Windows::Forms::CheckBox());
			this->SerialGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->ConnectToggle = (gcnew System::Windows::Forms::Button());
			this->ConfigurationGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->PulseSpeedField = (gcnew System::Windows::Forms::NumericUpDown());
			this->UseMemoryAvailableToggle = (gcnew System::Windows::Forms::CheckBox());
			this->PulseSpeedSliderLabel = (gcnew System::Windows::Forms::Label());
			this->PulseSpeedSlider = (gcnew System::Windows::Forms::TrackBar());
			this->LightWhiteLEDsRadio = (gcnew System::Windows::Forms::RadioButton());
			this->LightRedLEDsRadio = (gcnew System::Windows::Forms::RadioButton());
			this->QuitButton = (gcnew System::Windows::Forms::Button());
			this->StatusGroupBox = (gcnew System::Windows::Forms::GroupBox());
			this->MemoryStatusLabel = (gcnew System::Windows::Forms::Label());
			this->CPUStatusLabel = (gcnew System::Windows::Forms::Label());
			this->imageList = (gcnew System::Windows::Forms::ImageList(this->components));
			this->SerialGroupBox->SuspendLayout();
			this->ConfigurationGroupBox->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PulseSpeedField))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PulseSpeedSlider))->BeginInit();
			this->StatusGroupBox->SuspendLayout();
			this->SuspendLayout();
			// 
			// COMPortField
			// 
			this->COMPortField->AllowDrop = true;
			this->COMPortField->Location = System::Drawing::Point(65, 17);
			this->COMPortField->Name = L"COMPortField";
			this->COMPortField->Size = System::Drawing::Size(141, 21);
			this->COMPortField->TabIndex = 0;
			this->COMPortField->TextChanged += gcnew System::EventHandler(this, &MainForm::ComPortField_ValueChanged);
			// 
			// COMPortFieldLabel
			// 
			this->COMPortFieldLabel->AutoSize = true;
			this->COMPortFieldLabel->Location = System::Drawing::Point(6, 20);
			this->COMPortFieldLabel->Name = L"COMPortFieldLabel";
			this->COMPortFieldLabel->Size = System::Drawing::Size(53, 13);
			this->COMPortFieldLabel->TabIndex = 1;
			this->COMPortFieldLabel->Text = L"COM Port";
			// 
			// UseCPUActivityToggle
			// 
			this->UseCPUActivityToggle->AutoSize = true;
			this->UseCPUActivityToggle->Location = System::Drawing::Point(9, 19);
			this->UseCPUActivityToggle->Name = L"UseCPUActivityToggle";
			this->UseCPUActivityToggle->Size = System::Drawing::Size(185, 17);
			this->UseCPUActivityToggle->TabIndex = 1;
			this->UseCPUActivityToggle->Text = L"Use CPU Activity for Pulse Speed";
			this->UseCPUActivityToggle->UseVisualStyleBackColor = true;
			this->UseCPUActivityToggle->CheckedChanged += gcnew System::EventHandler(this, &MainForm::UseCPUActivityToggle_CheckedChanged);
			// 
			// SerialGroupBox
			// 
			this->SerialGroupBox->Controls->Add(this->ConnectToggle);
			this->SerialGroupBox->Controls->Add(this->COMPortFieldLabel);
			this->SerialGroupBox->Controls->Add(this->COMPortField);
			this->SerialGroupBox->Location = System::Drawing::Point(13, 9);
			this->SerialGroupBox->Name = L"SerialGroupBox";
			this->SerialGroupBox->Size = System::Drawing::Size(332, 47);
			this->SerialGroupBox->TabIndex = 4;
			this->SerialGroupBox->TabStop = false;
			this->SerialGroupBox->Text = L"Serial";
			// 
			// ConnectToggle
			// 
			this->ConnectToggle->Location = System::Drawing::Point(244, 15);
			this->ConnectToggle->Name = L"ConnectToggle";
			this->ConnectToggle->Size = System::Drawing::Size(82, 23);
			this->ConnectToggle->TabIndex = 2;
			this->ConnectToggle->UseVisualStyleBackColor = true;
			this->ConnectToggle->Click += gcnew System::EventHandler(this, &MainForm::ConnectToggle_Click);
			// 
			// ConfigurationGroupBox
			// 
			this->ConfigurationGroupBox->Controls->Add(this->PulseSpeedField);
			this->ConfigurationGroupBox->Controls->Add(this->UseMemoryAvailableToggle);
			this->ConfigurationGroupBox->Controls->Add(this->PulseSpeedSliderLabel);
			this->ConfigurationGroupBox->Controls->Add(this->PulseSpeedSlider);
			this->ConfigurationGroupBox->Controls->Add(this->LightWhiteLEDsRadio);
			this->ConfigurationGroupBox->Controls->Add(this->LightRedLEDsRadio);
			this->ConfigurationGroupBox->Controls->Add(this->UseCPUActivityToggle);
			this->ConfigurationGroupBox->Location = System::Drawing::Point(13, 62);
			this->ConfigurationGroupBox->Name = L"ConfigurationGroupBox";
			this->ConfigurationGroupBox->Size = System::Drawing::Size(332, 148);
			this->ConfigurationGroupBox->TabIndex = 5;
			this->ConfigurationGroupBox->TabStop = false;
			this->ConfigurationGroupBox->Text = L"Configuration";
			// 
			// PulseSpeedField
			// 
			this->PulseSpeedField->Location = System::Drawing::Point(257, 42);
			this->PulseSpeedField->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) {127, 0, 0, 0});
			this->PulseSpeedField->Name = L"PulseSpeedField";
			this->PulseSpeedField->Size = System::Drawing::Size(60, 20);
			this->PulseSpeedField->TabIndex = 9;
			this->PulseSpeedField->ValueChanged += gcnew System::EventHandler(this, &MainForm::PulseSpeedField_ValueChanged);
			// 
			// UseMemoryAvailableToggle
			// 
			this->UseMemoryAvailableToggle->AutoSize = true;
			this->UseMemoryAvailableToggle->Location = System::Drawing::Point(9, 79);
			this->UseMemoryAvailableToggle->Name = L"UseMemoryAvailableToggle";
			this->UseMemoryAvailableToggle->Size = System::Drawing::Size(197, 17);
			this->UseMemoryAvailableToggle->TabIndex = 3;
			this->UseMemoryAvailableToggle->Text = L"Use Memory Available for LED Color";
			this->UseMemoryAvailableToggle->UseVisualStyleBackColor = true;
			this->UseMemoryAvailableToggle->CheckedChanged += gcnew System::EventHandler(this, &MainForm::UseMemoryAvailableToggle_CheckedChanged);
			// 
			// PulseSpeedSliderLabel
			// 
			this->PulseSpeedSliderLabel->AutoSize = true;
			this->PulseSpeedSliderLabel->Location = System::Drawing::Point(25, 48);
			this->PulseSpeedSliderLabel->Name = L"PulseSpeedSliderLabel";
			this->PulseSpeedSliderLabel->Size = System::Drawing::Size(67, 13);
			this->PulseSpeedSliderLabel->TabIndex = 8;
			this->PulseSpeedSliderLabel->Text = L"Pulse Speed";
			// 
			// PulseSpeedSlider
			// 
			this->PulseSpeedSlider->LargeChange = 16;
			this->PulseSpeedSlider->Location = System::Drawing::Point(94, 42);
			this->PulseSpeedSlider->Maximum = 127;
			this->PulseSpeedSlider->Name = L"PulseSpeedSlider";
			this->PulseSpeedSlider->Size = System::Drawing::Size(157, 45);
			this->PulseSpeedSlider->TabIndex = 2;
			this->PulseSpeedSlider->TickFrequency = 16;
			this->PulseSpeedSlider->Scroll += gcnew System::EventHandler(this, &MainForm::PulseSpeedSlider_Scroll);
			// 
			// LightWhiteLEDsRadio
			// 
			this->LightWhiteLEDsRadio->AutoSize = true;
			this->LightWhiteLEDsRadio->Location = System::Drawing::Point(28, 125);
			this->LightWhiteLEDsRadio->Name = L"LightWhiteLEDsRadio";
			this->LightWhiteLEDsRadio->Size = System::Drawing::Size(108, 17);
			this->LightWhiteLEDsRadio->TabIndex = 5;
			this->LightWhiteLEDsRadio->TabStop = true;
			this->LightWhiteLEDsRadio->Text = L"Light White LEDs";
			this->LightWhiteLEDsRadio->UseVisualStyleBackColor = true;
			this->LightWhiteLEDsRadio->CheckedChanged += gcnew System::EventHandler(this, &MainForm::LightWhiteLEDsRadio_CheckedChanged);
			// 
			// LightRedLEDsRadio
			// 
			this->LightRedLEDsRadio->AutoSize = true;
			this->LightRedLEDsRadio->Location = System::Drawing::Point(28, 102);
			this->LightRedLEDsRadio->Name = L"LightRedLEDsRadio";
			this->LightRedLEDsRadio->Size = System::Drawing::Size(100, 17);
			this->LightRedLEDsRadio->TabIndex = 4;
			this->LightRedLEDsRadio->TabStop = true;
			this->LightRedLEDsRadio->Text = L"Light Red LEDs";
			this->LightRedLEDsRadio->UseVisualStyleBackColor = true;
			this->LightRedLEDsRadio->CheckedChanged += gcnew System::EventHandler(this, &MainForm::LightRedLEDsRadio_CheckedChanged);
			// 
			// QuitButton
			// 
			this->QuitButton->Location = System::Drawing::Point(257, 223);
			this->QuitButton->Name = L"QuitButton";
			this->QuitButton->Size = System::Drawing::Size(88, 23);
			this->QuitButton->TabIndex = 7;
			this->QuitButton->Text = L"Quit";
			this->QuitButton->UseVisualStyleBackColor = true;
			this->QuitButton->Click += gcnew System::EventHandler(this, &MainForm::QuitButton_Click);
			// 
			// StatusGroupBox
			// 
			this->StatusGroupBox->Controls->Add(this->MemoryStatusLabel);
			this->StatusGroupBox->Controls->Add(this->CPUStatusLabel);
			this->StatusGroupBox->Location = System::Drawing::Point(12, 216);
			this->StatusGroupBox->Name = L"StatusGroupBox";
			this->StatusGroupBox->Size = System::Drawing::Size(238, 35);
			this->StatusGroupBox->TabIndex = 8;
			this->StatusGroupBox->TabStop = false;
			// 
			// MemoryStatusLabel
			// 
			this->MemoryStatusLabel->AutoSize = true;
			this->MemoryStatusLabel->Location = System::Drawing::Point(120, 12);
			this->MemoryStatusLabel->Name = L"MemoryStatusLabel";
			this->MemoryStatusLabel->Size = System::Drawing::Size(0, 13);
			this->MemoryStatusLabel->TabIndex = 1;
			// 
			// CPUStatusLabel
			// 
			this->CPUStatusLabel->AutoSize = true;
			this->CPUStatusLabel->Location = System::Drawing::Point(7, 12);
			this->CPUStatusLabel->Name = L"CPUStatusLabel";
			this->CPUStatusLabel->Size = System::Drawing::Size(0, 13);
			this->CPUStatusLabel->TabIndex = 0;
			// 
			// imageList
			// 
			this->imageList->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^  >(resources->GetObject(L"imageList.ImageStream")));
			this->imageList->TransparentColor = System::Drawing::Color::Transparent;
			this->imageList->Images->SetKeyName(0, L"notifyIcon.Icon");
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(359, 269);
			this->Controls->Add(this->StatusGroupBox);
			this->Controls->Add(this->QuitButton);
			this->Controls->Add(this->SerialGroupBox);
			this->Controls->Add(this->ConfigurationGroupBox);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->MaximumSize = System::Drawing::Size(365, 297);
			this->MinimumSize = System::Drawing::Size(365, 297);
			this->Name = L"MainForm";
			this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Hide;
			this->Text = L"Flux Capacitor Link";
			this->Load += gcnew System::EventHandler(this, &MainForm::MainForm_Load);
			this->SizeChanged += gcnew System::EventHandler(this, &MainForm::MainForm_SizeChanged);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &MainForm::MainForm_FormClosed);
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MainForm::MainForm_FormClosing);
			this->SerialGroupBox->ResumeLayout(false);
			this->SerialGroupBox->PerformLayout();
			this->ConfigurationGroupBox->ResumeLayout(false);
			this->ConfigurationGroupBox->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PulseSpeedField))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->PulseSpeedSlider))->EndInit();
			this->StatusGroupBox->ResumeLayout(false);
			this->StatusGroupBox->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion

	// Update enable states
	private: void UpdateEnableStates() {
		this->PulseSpeedSliderLabel->Enabled  = !this->fclSerial->UseCPUActivity;
		this->PulseSpeedField->Enabled        = !this->fclSerial->UseCPUActivity;
		this->PulseSpeedSlider->Enabled       = !this->fclSerial->UseCPUActivity;

		this->LightRedLEDsRadio->Enabled      = !this->fclSerial->UseMemoryFree;
		this->LightWhiteLEDsRadio->Enabled    = !this->fclSerial->UseMemoryFree;
	}

	// Refresh the memory and CPU stats
	public: void RefreshStats() {
		stringstream	s1, s2;

		s1 << "Memory Free: " << (int)(fclSerial->MemoryFree * 100) << "%";
		this->MemoryStatusLabel->Text = gcnew System::String( s1.str().c_str() );

		s2 << "CPU Activity: " << (int)(fclSerial->CPUActivity * 100) << "%";
		this->CPUStatusLabel->Text = gcnew System::String( s2.str().c_str() );
	}

	// Update COM Port names
	private: void UpdateCOMPortNames() {
		int				i;
		cli::array<System::String^>^		serialPorts =  SerialPort::GetPortNames(); 

		COMPortField->Items->Clear();
		serialPorts->Sort( serialPorts );

		for( i=0; i < serialPorts->Length; i++ )
			COMPortField->Items->AddRange( serialPorts );
	}

	// Confirm Quit
	private: bool ConfirmQuit() {
		return MessageBox::Show( "Are you sure you want to quit?", "Sever Flux Capacitor Link", MessageBoxButtons::YesNo, MessageBoxIcon::Warning ) == System::Windows::Forms::DialogResult::Yes ? true : false;
	}

	// Connect/Discconect Button:  Open/Close the serial port
	private: System::Void ConnectToggle_Click(System::Object^  sender, System::EventArgs^  e) {
		// Open/close te connection
		if( this->fclSerial->IsOpen )
			this->fclSerial->Disconnect();
		else
			this->fclSerial->Connect();

		// Update the button face
		if( this->fclSerial->IsOpen )
			this->ConnectToggle->Text = L"Disconnect";
		else
			this->ConnectToggle->Text = L"Connect";

	}

	// Quit Button:  Quit the app when clicked
	private: System::Void QuitButton_Click(System::Object^  sender, System::EventArgs^  e) {
		if( this->ConfirmQuit() )
			Application::Exit();
	}

	// Use Memory Available Toggled
	private: System::Void UseMemoryAvailableToggle_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		this->fclSerial->UseMemoryFree = this->UseMemoryAvailableToggle->Checked;
		UpdateEnableStates();
	}

	// Light Red LEDs Clicked
	private: System::Void LightRedLEDsRadio_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		if( this->LightRedLEDsRadio->Checked )
			this->fclSerial->CustomRedLEDs = this->LightRedLEDsRadio->Checked;
	}

	// Light White LEDs Clicked
	private: System::Void LightWhiteLEDsRadio_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		if( !this->LightRedLEDsRadio->Checked )
			this->fclSerial->CustomRedLEDs = this->LightRedLEDsRadio->Checked;
	}

	// Use CPU Activity Toggled
	private: System::Void UseCPUActivityToggle_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		this->fclSerial->UseCPUActivity = this->UseCPUActivityToggle->Checked;
		UpdateEnableStates();
	}

	// Pulse Speed Slider Scroll Event
	private: System::Void PulseSpeedSlider_Scroll(System::Object^  sender, System::EventArgs^  e) {
		this->fclSerial->CustomPulseSpeed = this->PulseSpeedSlider->Value;
		this->PulseSpeedField->Value      = this->fclSerial->CustomPulseSpeed;
		this->PulseSpeedSlider->Value     = this->fclSerial->CustomPulseSpeed;
	}

	// Pulse Speed Field Value Changed
	private: System::Void PulseSpeedField_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
		this->fclSerial->CustomPulseSpeed = (System::Byte)this->PulseSpeedField->Value;
		this->PulseSpeedField->Value      = this->fclSerial->CustomPulseSpeed;
		this->PulseSpeedSlider->Value     = this->fclSerial->CustomPulseSpeed;
	}

	// COM Port Field
	private: System::Void ComPortField_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
		this->fclSerial->COMPort = this->COMPortField->Text;
	}

	// Form Loaded
	private: System::Void MainForm_Load(System::Object^  sender, System::EventArgs^  e) {
	}

	// Form Closed
	private: System::Void MainForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
	}

	// Form Closing:  Close the window, but do not quit
	private: System::Void MainForm_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
		;
	}

	// Form Size Changed:  If we're minimizing, close the window.
	private: System::Void MainForm_SizeChanged(System::Object^  sender, System::EventArgs^  e) {
//		if (this->WindowState == FormWindowState::Minimized)
//			this->Close();
	}
};
}

