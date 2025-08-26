#pragma once
#include "MyServer.h"
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <vcclr.h>
#include <msclr\marshal.h>
#include <msclr\marshal_cppstd.h>

namespace PopovTCPserver {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Net;
	using namespace System::Net::Sockets;
	using namespace System::Collections::Generic;
	using namespace Runtime::InteropServices;
	using namespace msclr;

	public ref class Server : public System::Windows::Forms::Form
	{
	public:
		Server(void)
		{
			SetConsoleCP(1251);
			SetConsoleOutputCP(1251);
			isBuild = true;
			const string& encryptionKey = "secretkey";
			InitializeComponent();
			myserver = new MyServer(encryptionKey);
			textBox1->Text = GetLocalIPAddress();
			textBox1->ReadOnly = true;
			textBox3->Text = gcnew System::String(encryptionKey.c_str());
			isBuild = false;
		}

	protected:
		~Server()
		{
			if (components)
			{
				delete components;
			}
			myserver->stopServer();
			delete myserver;
		}

	private: System::Windows::Forms::Button^ button3;
	private: System::Windows::Forms::RichTextBox^ richTextBox1;
	private: System::Windows::Forms::TextBox^ textBox3;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::TextBox^ textBox2;
	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::Button^ button1;

	private:
		System::ComponentModel::Container^ components;

		MyServer* myserver;
		bool isBuild;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			//
			// button3
			//
			this->button3->BackColor = System::Drawing::Color::White;
			this->button3->Location = System::Drawing::Point(457, 258);
			this->button3->Margin = System::Windows::Forms::Padding(4);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(100, 31);
			this->button3->TabIndex = 19;
			this->button3->Text = L"Очистить";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Server::buttonClear_Click);
			//
			// richTextBox1
			//
			this->richTextBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12));
			this->richTextBox1->Location = System::Drawing::Point(13, 13);
			this->richTextBox1->Margin = System::Windows::Forms::Padding(4);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->ReadOnly = true;
			this->richTextBox1->Size = System::Drawing::Size(434, 276);
			this->richTextBox1->TabIndex = 18;
			this->richTextBox1->Text = L"";
			this->richTextBox1->Enabled = false;
			this->richTextBox1->Font = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8);
			//
			// textBox3
			//
			this->textBox3->Location = System::Drawing::Point(567, 90);
			this->textBox3->Margin = System::Windows::Forms::Padding(4);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(195, 22);
			this->textBox3->TabIndex = 17;
			this->textBox3->TextChanged += gcnew System::EventHandler(this, &Server::textBox3_TextChanged);
			//
			// label3
			//
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(512, 90);
			this->label3->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(51, 17);
			this->label3->TabIndex = 16;
			this->label3->Text = L"Ключ: ";
			//
			// textBox2
			//
			this->textBox2->Location = System::Drawing::Point(567, 50);
			this->textBox2->Margin = System::Windows::Forms::Padding(4);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(195, 22);
			this->textBox2->TabIndex = 15;
			//
			// textBox1
			//
			this->textBox1->Location = System::Drawing::Point(567, 14);
			this->textBox1->Margin = System::Windows::Forms::Padding(4);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(195, 22);
			this->textBox1->TabIndex = 14;
			//
			// label2
			//
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(512, 50);
			this->label2->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(45, 17);
			this->label2->TabIndex = 13;
			this->label2->Text = L"Порт:";
			//
			// label1
			//
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(489, 13);
			this->label1->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(68, 17);
			this->label1->TabIndex = 12;
			this->label1->Text = L"IP-адрес:";
			//
			// button2
			//
			this->button2->BackColor = System::Drawing::Color::White;
			this->button2->Location = System::Drawing::Point(618, 138);
			this->button2->Margin = System::Windows::Forms::Padding(4);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(144, 29);
			this->button2->TabIndex = 11;
			this->button2->Text = L"Остановить сервер";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Server::buttonStop_Click);
			//
			// button1
			//
			this->button1->BackColor = System::Drawing::Color::White;
			this->button1->Location = System::Drawing::Point(457, 136);
			this->button1->Margin = System::Windows::Forms::Padding(4);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(153, 31);
			this->button1->TabIndex = 10;
			this->button1->Text = L"Запустить сервер";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Server::buttonStart_Click);
			//
			// Server
			//
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(777, 308);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->richTextBox1);
			this->Controls->Add(this->textBox3);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Name = L"Server";
			this->Text = L"Server";
			this->ResumeLayout(false);
			this->PerformLayout();
		}
#pragma endregion

		void buttonStart_Click(Object^ sender, EventArgs^ e) {
			const char* ip = (const char*)(Marshal::StringToHGlobalAnsi(textBox1->Text)).ToPointer();
			const char* port = (const char*)(Marshal::StringToHGlobalAnsi(textBox2->Text)).ToPointer();
			myserver->startServer(ip, port);
			textBox2->Enabled = false;
		}

		void buttonStop_Click(Object^ sender, EventArgs^ e) {
			myserver->stopServer();
			textBox2->Enabled = true;
		}

		void buttonClear_Click(Object^ sender, EventArgs^ e) {
			richTextBox1->Clear();
		}

		String^ GetLocalIPAddress()
		{
			String^ localIP = "";
			IPHostEntry^ host = Dns::GetHostEntry(Dns::GetHostName());
			for each (auto ip in host->AddressList)
			{
				if (ip->AddressFamily == System::Net::Sockets::AddressFamily::InterNetwork)
				{
					localIP = ip->ToString();
					break;
				}
			}
			return localIP;
		}

		void textBox3_TextChanged(System::Object^ sender, System::EventArgs^ e) {
			if (!isBuild) {
				TextBox^ sourceTextBox = dynamic_cast<TextBox^>(sender);
				const char* key = (const char*)(Marshal::StringToHGlobalAnsi(sourceTextBox->Text)).ToPointer();
				myserver->changeEncryptionKey(key);
			}
		}

	public:
		void AppendTextRichTextBox(String^ text) {
			if (richTextBox1->InvokeRequired) {
				// Если мы не в UI-потоке
				richTextBox1->BeginInvoke(gcnew Action<String^>(this, &Server::AppendTextRichTextBox), text);
			}
			else {
				// Если мы в UI-потоке
				this->richTextBox1->AppendText(text + "\n");
				this->richTextBox1->ScrollToCaret();
			}
		}
	};

	gcroot<Server^> wrapper;

	extern "C" __declspec(dllexport) void SetTextRichTextBox(const std::string text) {
		if (!wrapper) {
			wrapper = gcnew Server();
		}
		String^ managedText = gcnew System::String(text.c_str());
		wrapper->AppendTextRichTextBox(managedText);
	}
}
