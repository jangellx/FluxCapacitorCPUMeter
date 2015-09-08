//
// FCLSerial
//  Maintains the serial port and application state.  This means that it communicates with
//  the flux capacitor, as well as maintaining, loading and saving the settings.
//

#pragma once

namespace FluxCapacitorCPUMeterWFA {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO::Ports;
	using namespace System::Configuration;

	public ref class FCLSerial : System::IO::Ports::SerialPort 
	{
	private:
		// Member Variables
		System::String^				comPort;		// COM port the Arduino is on

		System::Boolean				useCPUActivity;		// True to use CPU activity for pulse speed; false to override
		System::Boolean				useMemoryFree;		// True to use available memory for red LEDs; false to override

		System::Byte				customPulseSpeed;	// Custom pulse speed, when useCPUActivity is true
		System::Boolean				customRedLEDs;		// Custom red LED state, when useMemoryFree is true

		System::Byte				systemPulseSpeed;	// System-derived pulse speed based on CPU activity
		System::Boolean				systemRedLEDs;		// System-derived red LED state based on available memory

		//System::String^			logBuffer;		// Log from the serial port.

		System::Double				memoryFree;		// 0-1 value representing the amount of free memory (0% means all memory free)
		System::Double				cpuActivity;		// 0-1 value representing the amount of CPU activity (0% means idle CPU)

	public:
		// Constructor
		FCLSerial()  {
			System::String^			path;

			// Default values
			comPort	         = L"COM3";

			useCPUActivity   = true;
			useMemoryFree    = true;

			customPulseSpeed = 64;
			customRedLEDs    = false;

			systemPulseSpeed = 64;
			systemRedLEDs    = false;

			memoryFree       = 1.0;
			cpuActivity      = 0.0;

			// Load settings from our config, if applicable
			path = System::Environment::GetFolderPath( System::Environment::SpecialFolder::ApplicationData );
			path = System::IO::Path::Combine( path, "FluxCapacitorLink" );
			path = System::IO::Path::Combine( path, "fcl.cfg" );
			if( System::IO::File::Exists( path ) ) {
				System::IO::StreamReader^	cfg = gcnew System::IO::StreamReader( path );
				if( cfg != nullptr ) {
					while( cfg->Peek() != -1 ) {
						System::String^	line = cfg->ReadLine();
						if( line->StartsWith( "COMPort" ) )
							comPort = line->Substring( 8 );
						else if( line->StartsWith( "UseCPUActivity" ) )
							useCPUActivity = System::Boolean::Parse( line->Substring( 15 ) );
						else if( line->StartsWith( "UseMemoryFree" ) )
							useMemoryFree = System::Boolean::Parse( line->Substring( 14 ) );
						else if( line->StartsWith( "CustomPulseSpeed" ) )
							customPulseSpeed = (System::Byte)System::Int32::Parse( line->Substring( 17 ) );
						else if( line->StartsWith( "CustomRedLEDs" ) )
							customRedLEDs = System::Boolean::Parse( line->Substring( 14 ) );
					}

					cfg->Close();
				}
			}

			/* Serial setup */
			DtrEnable        = true;				// Need this for the Arduino to work
			PortName         = comPort;

			// Listen for data received events
			DataReceived += gcnew SerialDataReceivedEventHandler( this, &FCLSerial::DataReceivedHandler );
		}

	protected:
		// Finalizer
		!FCLSerial() {
			// Save the settings to our config
			System::String^			path;

			path = System::Environment::GetFolderPath( System::Environment::SpecialFolder::ApplicationData );
			path = System::IO::Path::Combine( path, "FluxCapacitorLink" );
			if( !System::IO::Directory::Exists( path ) )
				System::IO::Directory::CreateDirectory( path );

			path = System::IO::Path::Combine( path, "fcl.cfg" );
			System::IO::StreamWriter^	cfg = gcnew System::IO::StreamWriter( path );

			cfg->WriteLine( "COMPort "          + comPort          );
			cfg->WriteLine( "UseCPUActivity "   + useCPUActivity   );
			cfg->WriteLine( "UseMemoryFree "    + useMemoryFree    );
			cfg->WriteLine( "CustomPulseSpeed " + (System::Int32)customPulseSpeed );
			cfg->WriteLine( "CustomRedLEDs "    + customRedLEDs    );

			cfg->Close();
		}

		// Destructor
		~FCLSerial() {
			;
		}

	private:
		// Send new state to the Arduino
		void SendUpdate (void) {
			if( !IsOpen )
				return;

			array<System::Byte>^		c = {0};

			if( UseCPUActivity )
				c[0] |= systemPulseSpeed & ~0x80;
			else
				c[0] |= customPulseSpeed & ~0x80;

			if( UseMemoryFree )
				c[0] |= systemRedLEDs ? 0x80 : 0;
			else
				c[0] |= customRedLEDs ? 0x80 : 0;

			this->Write( c, 0, 1 );
		}

		System::Void DataReceivedHandler( System::Object^ sender, System::IO::Ports::SerialDataReceivedEventArgs^ e ) {
			System::String^		data = ReadExisting();

			//logBuffer += data;

			System::Diagnostics::Debug::Write( data );
		}

	public:
		// Connect
		public: void Connect() {
			if( this->IsOpen )
				return;

			try {
				PortName = COMPort;
				this->Open();
			}
			catch( UnauthorizedAccessException^ e) { MessageBox::Show( "Error Opening Connection", "Uauthorized Access: "       + e->Message, MessageBoxButtons::OK, MessageBoxIcon::Error ); }
			catch( ArgumentOutOfRangeException^ e) { MessageBox::Show( "Error Opening Connection", "Argument out of range: "    + e->Message, MessageBoxButtons::OK, MessageBoxIcon::Error ); }
			catch( ArgumentException^           e) { MessageBox::Show( "Error Opening Connection", "Invalid COM port name: "    + e->Message, MessageBoxButtons::OK, MessageBoxIcon::Error ); }
			catch( InvalidOperationException^   e) { MessageBox::Show( "Error Opening Connection", "Port already open: "        + e->Message, MessageBoxButtons::OK, MessageBoxIcon::Error ); }
//			catch( IOException^		    e) { MessageBox::Show( "Error Opening Connection", "IO Exception: "             + e->Message, MessageBoxButtons::OK, MessageBoxIcon::Error ); }
//			catch (const std::exception        &e) { MessageBox::Show( "Error Opening Connection", "Unknown C++ Exception :"    + std::string(e.what()),   MessageBoxButtons::OK, MessageBoxIcon::Error ); }
			catch( ...                           ) { MessageBox::Show( "Error Opening Connection", "Error opening connection: ",              MessageBoxButtons::OK, MessageBoxIcon::Error ); }

/*			try {
				String^				output = gcnew String("");
				array<System::Byte>^		buffer = {0x8F};

				System::Diagnostics::Debug::WriteLine( "Sending line" );
				this->fclSerial->Write( buffer, 0, 1 );
			}
			catch(...) { ; }
*/
		}

		// Disconnect
		public: void Disconnect() {
			if( !this->IsOpen )
				return;

			try {
				this->Close();
			}
			catch( ...                          ) { MessageBox::Show( "Error Closing Connection", "Error closing connection: ",               MessageBoxButtons::OK, MessageBoxIcon::Error ); }
		}

		// Accessors
		property System::String^ COMPort {
			void set( System::String^ port ) {
				comPort = port;
				SendUpdate();
			}

			System::String^ get(void) {
				return comPort;
			}
		}

		property System::Boolean UseCPUActivity {
			void set( System::Boolean state ) {
				useCPUActivity = state;
				SendUpdate();
			}

			System::Boolean get(void) {
				return useCPUActivity;
			}
		}

		property System::Boolean UseMemoryFree {
			void set( System::Boolean state ) {
				useMemoryFree = state;
				SendUpdate();
			}

			System::Boolean get(void) {
				return useMemoryFree;
			}
		}

		property System::Byte CustomPulseSpeed {
			void set( System::Byte speed ) {
				customPulseSpeed = speed;

				if( customPulseSpeed <   0 )	customPulseSpeed =   0;
				if( customPulseSpeed > 127 )	customPulseSpeed = 127;

				SendUpdate();
			}

			System::Byte get(void) {
				return customPulseSpeed;
			}
		}

		property System::Byte SystemPulseSpeed {
			void set( System::Byte speed ) {
				systemPulseSpeed = speed;
				SendUpdate();
			}

			System::Byte get(void) {
				return systemPulseSpeed;
			}
		}

		property System::Boolean CustomRedLEDs {
			void set( System::Boolean doRed ) {
				customRedLEDs = doRed;
				SendUpdate();
			}

			System::Boolean get(void) {
				return customRedLEDs;
			}
		}

		property System::Boolean SystemRedLEDs {
			void set( System::Boolean doRed ) {
				systemRedLEDs = doRed;
				SendUpdate();
			}

			System::Boolean get(void) {
				return systemRedLEDs;
			}
		}

		// System State Accessors
		property System::Double MemoryFree {
			void set( System::Double memFree ) {
				memoryFree    = memFree;
				systemRedLEDs = (memoryFree < 0.2) ? true : false;
				SendUpdate();
			}

			System::Double get(void) {
				return memoryFree;
			}
		}

		property System::Double CPUActivity {
			void set( System::Double activity ) {
				cpuActivity      = activity;
				systemPulseSpeed = (System::Byte) (127.0 * (1.0 - cpuActivity));
				SendUpdate();
			}

			System::Double get(void) {
				return cpuActivity;
			}
		}
	};
}
