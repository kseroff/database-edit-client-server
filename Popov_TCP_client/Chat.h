#pragma once
#include <windows.h>
#include <string>
#include <vcclr.h>
#include <windows.h>
#include <bitset>
#include <sstream>
#include <iomanip>
#include <msclr\marshal.h>
#include <msclr\marshal_cppstd.h>

namespace PopovTCPclient {
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Net;
	using namespace System::Text;
	using namespace System::Net::Sockets;
	using namespace System::Collections::Generic;
	using namespace Runtime::InteropServices;
	using namespace System::Threading::Tasks;
	using namespace msclr;

	public ref class Chat : public System::Windows::Forms::Form
	{

	public:
		Chat(void)
		{
			SetConsoleCP(1251);
			SetConsoleOutputCP(1251);
			InitializeComponent();
			textBox3->Text = GetLocalIPAddress();
			encryptionKey = gcnew EncryptionKey(textBox1->Text);
		}

	protected:
		~Chat()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::RichTextBox^ richTextBox1;
	private: System::Windows::Forms::RichTextBox^ richTextBox2;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::Button^ button4;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::CheckBox^ checkBox1;
	private: System::Windows::Forms::TextBox^ textBox3;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::TextBox^ textBox2;


	private: System::Windows::Forms::Button^ button3;
	private: System::Windows::Forms::TextBox^ textBox5;
	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::TextBox^ textBox6;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::TextBox^ textBox1;

	private:

		ref class EncryptionKey {
		private:
			String^ key;

		public:
			EncryptionKey(String^ initialKey)
			{
				if (String::IsNullOrEmpty(initialKey))
				{
					throw gcnew ArgumentException("Ключ шифрования не может быть пустым");
				}
				key = initialKey;
			}

			property String^ Key
			{
				String^ get()
				{
					return key;
				}
				void set(String^ newKey)
				{
					if (String::IsNullOrEmpty(newKey))
					{
						throw gcnew ArgumentException("Новый ключ шифрования не может быть пустым");
					}
					key = newKey;
				}
			}

			std::string encryptDecrypt(const std::string& input) {
				std::string output = input;
				for (size_t i = 0; i < input.size(); ++i) {
					output[i] = input[i] ^ key[i % key->Length];
				}
				return output;
			}

			String^ encrypt(String^ source) {
				std::string& input = msclr::interop::marshal_as< std::string >(source);
				std::string encrypted = encryptDecrypt(input);
				std::ostringstream oss;
				for (unsigned char c : encrypted) {
					oss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
				}
				return gcnew System::String((oss.str()).c_str());
			}

			String^ decrypt(String^ source) {
				std::string& hexInput = msclr::interop::marshal_as< std::string >(source);
				std::string decrypted;
				for (size_t i = 0; i < hexInput.length(); i += 2) {
					std::string byteString = hexInput.substr(i, 2);
					char byte = (char)(int)strtol(byteString.c_str(), nullptr, 16);
					decrypted += byte;
				}
				return gcnew System::String(encryptDecrypt(decrypted).c_str());
			}

		};

		System::ComponentModel::Container^ components;
		EncryptionKey^ encryptionKey;
		TcpClient^ tcpClient;
		NetworkStream^ stream;
		String^ username;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->richTextBox2 = (gcnew System::Windows::Forms::RichTextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->textBox5 = (gcnew System::Windows::Forms::TextBox());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->textBox6 = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// richTextBox1
			// 
			this->richTextBox1->Location = System::Drawing::Point(12, 48);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(493, 361);
			this->richTextBox1->TabIndex = 0;
			this->richTextBox1->Text = L"";
			// 
			// richTextBox2
			// 
			this->richTextBox2->Location = System::Drawing::Point(511, 358);
			this->richTextBox2->Name = L"richTextBox2";
			this->richTextBox2->Size = System::Drawing::Size(367, 51);
			this->richTextBox2->TabIndex = 1;
			this->richTextBox2->Text = L"";
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(884, 358);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(124, 51);
			this->button1->TabIndex = 2;
			this->button1->Text = L"Отправить";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Chat::buttonSend_Click);
			// 
			// button2
			// 
			this->button2->Enabled = false;
			this->button2->Location = System::Drawing::Point(739, 167);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(169, 34);
			this->button2->TabIndex = 3;
			this->button2->Text = L"Отключиться";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Chat::buttonDisconnect_Click);
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(396, 12);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(109, 30);
			this->button4->TabIndex = 5;
			this->button4->Text = L"Очистить";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &Chat::buttonClear_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label1->Location = System::Drawing::Point(13, 12);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(84, 25);
			this->label1->TabIndex = 6;
			this->label1->Text = L"Статус:";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label2->ForeColor = System::Drawing::Color::Red;
			this->label2->Location = System::Drawing::Point(91, 12);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(33, 25);
			this->label2->TabIndex = 7;
			this->label2->Text = L"off";
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->checkBox1->Location = System::Drawing::Point(540, 328);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(173, 24);
			this->checkBox1->TabIndex = 9;
			this->checkBox1->Text = L"Сообщение всем";
			this->checkBox1->UseVisualStyleBackColor = true;
			// 
			// textBox3
			// 
			this->textBox3->Location = System::Drawing::Point(726, 87);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(182, 22);
			this->textBox3->TabIndex = 14;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label5->Location = System::Drawing::Point(547, 89);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(160, 20);
			this->label5->TabIndex = 15;
			this->label5->Text = L"IP-адрес сервера:";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label4->Location = System::Drawing::Point(576, 126);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(131, 20);
			this->label4->TabIndex = 16;
			this->label4->Text = L"Порт сервера:";
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(727, 126);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(182, 22);
			this->textBox2->TabIndex = 17;
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(551, 167);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(169, 34);
			this->button3->TabIndex = 20;
			this->button3->Text = L"Подключиться";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Chat::buttonСonnect_Click);
			// 
			// textBox5
			// 
			this->textBox5->Location = System::Drawing::Point(727, 250);
			this->textBox5->Name = L"textBox5";
			this->textBox5->Size = System::Drawing::Size(182, 22);
			this->textBox5->TabIndex = 21;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label7->Location = System::Drawing::Point(557, 250);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(150, 20);
			this->label7->TabIndex = 22;
			this->label7->Text = L"Имя получателя:";
			// 
			// textBox6
			// 
			this->textBox6->Location = System::Drawing::Point(727, 47);
			this->textBox6->Name = L"textBox6";
			this->textBox6->Size = System::Drawing::Size(182, 22);
			this->textBox6->TabIndex = 23;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label3->Location = System::Drawing::Point(536, 47);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(171, 20);
			this->label3->TabIndex = 24;
			this->label3->Text = L"Имя пользователя:";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label6->Location = System::Drawing::Point(532, 284);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(173, 20);
			this->label6->TabIndex = 25;
			this->label6->Text = L"Ключ шифрования:";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(726, 284);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(182, 22);
			this->textBox1->TabIndex = 26;
			this->textBox1->Text = L"secretkey";
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &Chat::textBox1_TextChanged);
			// 
			// Chat
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1020, 427);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->textBox6);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->textBox5);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->textBox3);
			this->Controls->Add(this->checkBox1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->richTextBox2);
			this->Controls->Add(this->richTextBox1);
			this->Name = L"Chat";
			this->Text = L"Chat";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: String^ GetLocalIPAddress() {
		String^ localIP = "";
		IPHostEntry^ host = Dns::GetHostEntry(Dns::GetHostName());
		for each (auto ip in host->AddressList)
		{
			if (ip->AddressFamily == System::Net::Sockets::AddressFamily::InterNetwork) // IPv4
			{
				localIP = ip->ToString();
				break;
			}
		}
		return localIP;
	}

	private: System::Void buttonSend_Click(System::Object^ sender, System::EventArgs^ e) {
		if (stream == nullptr || !tcpClient->Connected) {
			AppendMessage("Ошибка: Вы не подключены к серверу!", Color::Red);
			return;
		}

		String^ textMessage = richTextBox2->Text->Trim();
		if (String::IsNullOrEmpty(textMessage)) {
			return; // Не отправляем пустые сообщения
		}

		try {
			// Шифруем сообщение
			String^ messageToSend;

			if (checkBox1->Checked) {
				// Сообщение всем
				messageToSend = "/sendall " + textMessage;
			}
			else {
				// Личное сообщение
				String^ recipient = textBox5->Text->Trim();
				if (String::IsNullOrEmpty(recipient)) {
					AppendMessage("Ошибка: Укажите имя получателя!", Color::Red);
					return;
				}
				messageToSend = "/send " + recipient + " " + textMessage;
			}

			String^ encryptedMessage = encryptionKey->encrypt(messageToSend);

			// Отправляем сообщение серверу
			array<Byte>^ messageData = Encoding::UTF8->GetBytes(encryptedMessage);
			stream->Write(messageData, 0, messageData->Length);
			stream->Flush();

			AppendMessage("Я: " + textMessage, Color::Green);
			richTextBox2->Text = "";
		}
		catch (Exception^ ex) {
			AppendMessage("Ошибка при отправке сообщения: " + ex->Message, Color::Red);
		}
	}

	private: System::Void buttonClear_Click(System::Object^ sender, System::EventArgs^ e) {
		richTextBox1->Clear();
	}

	private: System::Void buttonСonnect_Click(System::Object^ sender, System::EventArgs^ e) {
		try {
			String^ ip = textBox3->Text;
			String^ port = textBox2->Text;
			username = textBox6->Text;

			tcpClient = gcnew TcpClient(ip, std::stoi((const char*)(Marshal::StringToHGlobalAnsi(port)).ToPointer()));
			stream = tcpClient->GetStream();

			// Проверяем состояние подключения
			if (tcpClient->Connected) {
				array<Byte>^ data = Encoding::UTF8->GetBytes(username);
				stream->Write(data, 0, data->Length);
				stream->Flush();

				array<Byte>^ responseBuffer = gcnew array<Byte>(1024);
				int bytesRead = stream->Read(responseBuffer, 0, responseBuffer->Length);

				if (bytesRead > 0) {
					String^ response = Encoding::UTF8->GetString(responseBuffer, 0, bytesRead);
					if (response->Contains("TRUE")) {
						AppendMessage("Вы успешно подключились к серверу!", Color::Black);
						Task::Run(gcnew Action(this, &Chat::ListenForServer));
					}
					else {
						AppendMessage("Ошибка при входе", Color::Red);
					}
				}

				textBox6->Enabled = false;
				textBox2->Enabled = false;
				label2->Text = L"online";
				label2->ForeColor = System::Drawing::Color::Green;
				this->button2->Enabled = true;
				this->button3->Enabled = false;
			}
			else {
				MessageBox::Show(L"Сбой при подключении к сервер\n", L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			}
		}
		catch (Exception^ ex) {
			MessageBox::Show(L"Ошибка: " + ex->Message, L"Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
	}

	private: System::Void buttonDisconnect_Click(System::Object^ sender, System::EventArgs^ e) {
		Disconnect();
		AppendMessage("Вы отключены от сервера", Color::Red);
	}

		   void ListenForServer() {
			   try {
				   array<Byte>^ sizeBuffer = gcnew array<Byte>(sizeof(int));

				   while (tcpClient != nullptr && tcpClient->Connected) {
					   if (stream == nullptr) break;
					   stream->Read(sizeBuffer, 0, sizeBuffer->Length);
					   int messageSize = BitConverter::ToInt32(sizeBuffer, 0);
					   if (messageSize > 0) {
						   array<Byte>^ buffer = gcnew array<Byte>(messageSize);
						   int bufferRead = stream->Read(buffer, 0, buffer->Length);
						   if (bufferRead) {
							   String^ encryptedMessage = Encoding::UTF8->GetString(buffer);

							   if (encryptedMessage == "SERVER_SHUTDOWN") {
								   AppendMessage("Сервер завершил свою работу. Соединение разорвано.", Color::Red);
								   Disconnect();
								   return;
							   }

							   // Расшифровка сообщения
							   String^ decryptedMessage;
							   try {
								   decryptedMessage = encryptionKey->decrypt(encryptedMessage);
							   }
							   catch (Exception^ ex) {
								   decryptedMessage = "Ошибка расшифровки: " + encryptedMessage;
							   }

							   AppendReceivedMessage(decryptedMessage);
						   }
					   }
				   }
			   }
			   catch (Exception^ ex) {
				   Disconnect();
			   }
		   }

		   void Disconnect() {
			   if (stream != nullptr) {
					   array<Byte>^ disconnectMessage = Encoding::UTF8->GetBytes("/exit");
					   stream->Write(disconnectMessage, 0, disconnectMessage->Length);
					   stream->Flush();
					   stream->Close();
			   }
			   if (tcpClient != nullptr) {
				   tcpClient->Close();
			   }

			   tcpClient = nullptr;
			   stream = nullptr;

			   // Обновляем UI
			   textBox6->Enabled = true;
			   textBox2->Enabled = true;
			   label2->Text = L"off";
			   label2->ForeColor = System::Drawing::Color::Red;
			   button3->Enabled = true;
			   button2->Enabled = false;
		   }

		   void AppendReceivedMessage(String^ message) {
			   AppendMessage(message, Color::Blue);
		   }

		   void AppendMessage(String^ message, Color color) {
			   if (richTextBox1->InvokeRequired) {
				   // Если мы не в UI-потоке
				   richTextBox1->BeginInvoke(gcnew Action<String^, Color>(this, &Chat::AppendMessage), message, color);
			   }
			   else {
				   this->richTextBox1->SelectionStart = this->richTextBox1->TextLength;
				   this->richTextBox1->SelectionLength = 0;

				   this->richTextBox1->SelectionColor = color;
				   this->richTextBox1->AppendText(message + "\n");
				   this->richTextBox1->SelectionStart = richTextBox1->Text->Length; // Прокрутка вниз
				   this->richTextBox1->ScrollToCaret();
				   this->richTextBox1->SelectionColor = this->richTextBox1->ForeColor; // Сброс цвета на стандартный
			   }
		   }

	private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
		String^ newKey = textBox1->Text->Trim();

		if (String::IsNullOrEmpty(newKey)) {
			AppendMessage("Ключ шифрования не может быть пустым!", Color::Red);
			return;
		}

		try {
			// Обновляем ключ в EncryptionKey
			if (encryptionKey == nullptr) {
				encryptionKey = gcnew EncryptionKey(newKey);
			}
			else {
				encryptionKey->Key = newKey;
			}

			AppendMessage("Ключ шифрования успешно обновлен.", Color::Green);
		}
		catch (Exception^ ex) {
			AppendMessage("Ошибка при обновлении ключа: " + ex->Message, Color::Red);
		}
	}
	};
}
