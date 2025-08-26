#include <windows.h>
#include <string>
#include <vcclr.h>
#include <windows.h>

#pragma once

namespace UDP {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Net;
	using namespace System::Net::Sockets;
	using namespace System::Text;
	using namespace System::Threading;


	public ref class MainForm : public System::Windows::Forms::Form
	{
	private:
		UdpClient^ udpClient;
		IPEndPoint^ remoteEndPoint;
		String^ username;
		int listenPort;


	public:
		MainForm(void)
		{
			InitializeComponent();
			textBox1->Text = GetLocalIPAddress(); // Устанавливаем локальный IP
			textBox1->ReadOnly = true; // Запрет редактирования IP
			udpClient = gcnew UdpClient();
		}

	protected:
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
			if (udpClient) {
				udpClient->Close();
			}
		}

		virtual void OnFormClosing(FormClosingEventArgs^ e) override
		{
			if (components)
			{
				delete components;
			}
			exit(0);
		}

		String^ GetLocalIPAddress()
		{
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

		void ReceiveMessages()
		{
			IPEndPoint^ endPoint = gcnew IPEndPoint(IPAddress::Any, listenPort);
			udpClient->Client->Bind(endPoint);

			while (true)
			{
				array<Byte>^ receivedBytes = udpClient->Receive(endPoint);
				String^ message = Encoding::UTF8->GetString(receivedBytes);

				// Отображение полученного сообщения в richTextBox1
				this->Invoke(gcnew Action<String^>(this, &MainForm::AppendReceivedMessage), message);
			}
		}

		void AppendReceivedMessage(String^ message)
		{
			AppendMessage(message, Color::Red);
		}

		// Новый метод для добавления сообщения
		void AppendMessage(String^ message, Color color)
		{
			this->richTextBox1->SelectionStart = this->richTextBox1->TextLength;
			this->richTextBox1->SelectionLength = 0;

			this->richTextBox1->SelectionColor = color;
			this->richTextBox1->AppendText(message + "\n");
			this->richTextBox1->SelectionStart = richTextBox1->Text->Length; // Прокрутка вниз
			this->richTextBox1->ScrollToCaret();
			this->richTextBox1->SelectionColor = this->richTextBox1->ForeColor; // Сброс цвета на стандартный
		}

		// Метод для отправки сообщений
		void SendMessage(String^ message)
		{
			if (remoteEndPoint)
			{
				array<Byte>^ data = Encoding::UTF8->GetBytes(message);
				udpClient->Send(data, data->Length, remoteEndPoint);
			}
		}


	private: System::Windows::Forms::RichTextBox^ richTextBox1;
	protected:
	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::TextBox^ textBox2;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::RichTextBox^ richTextBox2;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::TextBox^ textBox3;
	private: System::Windows::Forms::TextBox^ textBox4;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::TextBox^ textBox5;
	private: System::Windows::Forms::Label^ label6;

	private:
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code

		void InitializeComponent(void)
		{
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->richTextBox2 = (gcnew System::Windows::Forms::RichTextBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox4 = (gcnew System::Windows::Forms::TextBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->textBox5 = (gcnew System::Windows::Forms::TextBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// richTextBox1
			// 
			this->richTextBox1->Location = System::Drawing::Point(12, 66);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(734, 342);
			this->richTextBox1->TabIndex = 0;
			this->richTextBox1->Text = L"";
			this->richTextBox1->ReadOnly = true;
			this->richTextBox1->Font = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(173, 33);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(157, 22);
			this->textBox1->TabIndex = 1;
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(336, 33);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(118, 22);
			this->textBox2->TabIndex = 2;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(460, 28);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(118, 32);
			this->button1->TabIndex = 3;
			this->button1->Text = L"подключиться";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MainForm::button1_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(627, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(0, 17);
			this->label1->TabIndex = 4;
			// 
			// richTextBox2
			// 
			this->richTextBox2->Location = System::Drawing::Point(13, 414);
			this->richTextBox2->Name = L"richTextBox2";
			this->richTextBox2->Size = System::Drawing::Size(375, 70);
			this->richTextBox2->TabIndex = 5;
			this->richTextBox2->Text = L"";
			this->richTextBox2->Font = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(394, 431);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(116, 38);
			this->button2->TabIndex = 6;
			this->button2->Text = L"Отправить";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MainForm::button2_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(170, 13);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(20, 17);
			this->label2->TabIndex = 7;
			this->label2->Text = L"IP";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(333, 13);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(34, 17);
			this->label3->TabIndex = 8;
			this->label3->Text = L"Port";
			// 
			// textBox3
			// 
			this->textBox3->Location = System::Drawing::Point(569, 414);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(177, 22);
			this->textBox3->TabIndex = 9;
			// 
			// textBox4
			// 
			this->textBox4->Location = System::Drawing::Point(569, 447);
			this->textBox4->Name = L"textBox4";
			this->textBox4->Size = System::Drawing::Size(177, 22);
			this->textBox4->TabIndex = 10;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(543, 414);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(20, 17);
			this->label4->TabIndex = 11;
			this->label4->Text = L"IP";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(529, 447);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(34, 17);
			this->label5->TabIndex = 12;
			this->label5->Text = L"Port";
			// 
			// textBox5
			// 
			this->textBox5->Location = System::Drawing::Point(13, 33);
			this->textBox5->Name = L"textBox5";
			this->textBox5->Size = System::Drawing::Size(154, 22);
			this->textBox5->TabIndex = 13;
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(12, 13);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(45, 17);
			this->label6->TabIndex = 14;
			this->label6->Text = L"Name";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(759, 502);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->textBox5);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->textBox4);
			this->Controls->Add(this->textBox3);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->richTextBox2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->richTextBox1);
			this->Name = L"MainForm";
			this->Text = L"MainForm";
			this->ResumeLayout(false);
			this->PerformLayout();

		}

#pragma endregion

		void button1_Click(System::Object^ sender, System::EventArgs^ e)
		{
			// Получение порта и имени пользователя
			listenPort = Int32::Parse(textBox2->Text);
			username = textBox5->Text;

			// Делаем поля недоступными для редактирования
			textBox2->Enabled = false;
			textBox5->Enabled = false;

			// Запуск потока для получения сообщений
			Thread^ receiveThread = gcnew Thread(gcnew ThreadStart(this, &MainForm::ReceiveMessages));
			receiveThread->IsBackground = true;
			receiveThread->Start();
		}

		void button2_Click(System::Object^ sender, System::EventArgs^ e)
		{
			// Проверка на пустой ввод
			if (String::IsNullOrWhiteSpace(textBox3->Text) || String::IsNullOrWhiteSpace(textBox4->Text))
			{
				MessageBox::Show("Пожалуйста, введите корректный IP и порт получателя.", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			}

			// Проверка IP-адреса
			IPAddress^ remoteIPAddress;
			if (!IPAddress::TryParse(textBox3->Text, remoteIPAddress))
			{
				MessageBox::Show("Некорректный IP-адрес.", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			}

			// Проверка порта
			int remotePort;
			if (!Int32::TryParse(textBox4->Text, remotePort) || remotePort < 0 || remotePort > 65535)
			{
				MessageBox::Show("Некорректный порт. Убедитесь, что порт находится в диапазоне от 0 до 65535.", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
				return;
			}

			// Создание конечной точки для отправки
			remoteEndPoint = gcnew IPEndPoint(remoteIPAddress, remotePort);


			// Формируем сообщение
			String^ messageToSend = String::Format("{0}: {1}", username, richTextBox2->Text);
			SendMessage(messageToSend);

			String^ message = richTextBox2->Text;
			String^ formattedMessage = username + ": " + message;
			AppendMessage(formattedMessage, Color::Green);

			// Очищаем поле для ввода сообщения
			richTextBox2->Clear();
		}

};
}
