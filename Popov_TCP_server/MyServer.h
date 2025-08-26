#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdexcept>
#include <bitset>
#include <list>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

extern "C" __declspec(dllexport) void SetTextRichTextBox(const string text);

class Client {
public:
	string username;
	SOCKET socket;

	Client(string name, SOCKET sock) : username(name), socket(sock) {}
};

class EncryptionKey {
private:
	string key;

public:
	EncryptionKey(const string& initialKey) {
		if (initialKey.empty()) {
			throw invalid_argument("Ключ шифрования не может быть пустым");
		}
		key = initialKey;
	}

	string getKey() const {
		return key;
	}

	void setKey(const string& newKey) {
		if (newKey.empty()) {
			throw invalid_argument("Новый ключ шифрования не может быть пустым");
		}
		key = newKey;
	}

	string encryptDecrypt(const string& input) {
		string output = input;
		for (size_t i = 0; i < input.size(); ++i) {
			output[i] = input[i] ^ key[i % key.size()];
		}
		return output;
	}

	string encrypt(const string& input) {
		string encrypted = encryptDecrypt(input);
		ostringstream oss;
		for (unsigned char c : encrypted) {
			oss << hex << setw(2) << setfill('0') << (int)c;
		}
		return oss.str();
	}

	string decrypt(const string& hexInput) {
		string decrypted;
		for (size_t i = 0; i < hexInput.length(); i += 2) {
			string byteString = hexInput.substr(i, 2);
			char byte = (char)(int)strtol(byteString.c_str(), nullptr, 16);
			decrypted += byte;
		}
		return encryptDecrypt(decrypted);
	}

};

class MyServer {
	SOCKET serverSocket;
	WSADATA wsaData;
	list<Client> clients;
	HANDLE clientsLock;
	EncryptionKey encryptionKey;
	bool isRunning;
	list<HANDLE> clientThreads;

public:
	// Обновленный конструктор
	MyServer(const string& initialEncryptionKey)
		: encryptionKey(initialEncryptionKey), isRunning(false) {
		SetConsoleCP(1251);
		SetConsoleOutputCP(1251);
		clientsLock = CreateMutex(nullptr, FALSE, nullptr);
		if (clientsLock == NULL) {
			throw runtime_error("Не удалось создать мьютекс");
		}
	}

	~MyServer() {
		stopServer();
		if (clientsLock != nullptr) {
			CloseHandle(clientsLock);
		}
	}

	void startServer(const string& ip, const string& port) {
		if (isRunning) {
			SetTextRichTextBox("Сервер уже запущен!");
			return;
		}
		initializeServer(ip, port);
	}

	void stopServer() {
		if (!isRunning) {
			SetTextRichTextBox("Сервер уже остановлен!");
			return;
		}
		shutdownServer();
	}

	void changeEncryptionKey(const string& newKey) {
		try {
			encryptionKey.setKey(newKey);
			SetTextRichTextBox("Ключ шифрования успешно изменен.");
		}
		catch (const invalid_argument& e) {
			SetTextRichTextBox(string("Ошибка: ") + e.what());
		}
	}

private:
	void initializeServer(const string& ip, const string& port) {
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
			SetTextRichTextBox("Не удалось инициализировать Winsock");
			return;
		}

		serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (serverSocket == INVALID_SOCKET) {
			SetTextRichTextBox("Не удалось создать сокет");
			WSACleanup();
			return;
		}

		sockaddr_in serverAddr;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(stoi(port));

		if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0) {
			SetTextRichTextBox("Неверный IP-адрес");
			closesocket(serverSocket);
			WSACleanup();
			return;
		}

		if (::bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR) {
			SetTextRichTextBox("Не удалось привязать сокет");
			closesocket(serverSocket);
			WSACleanup();
			return;
		}

		if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
			SetTextRichTextBox("Не удалось начать прослушивание");
			closesocket(serverSocket);
			WSACleanup();
			return;
		}

		isRunning = true;
		SetTextRichTextBox("Сервер запущен и ожидает подключений...");

		CreateThread(nullptr, 0, acceptClientsHelper, this, 0, nullptr);
	}

	static DWORD WINAPI acceptClientsHelper(LPVOID param) {
		MyServer* server = static_cast<MyServer*>(param);
		server->acceptClients();
		return 0;
	}

	void acceptClients() {
		while (isRunning) {
			SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
			if (clientSocket == INVALID_SOCKET) {
				if (isRunning) {
					SetTextRichTextBox("Не удалось принять подключение клиента");
				}
				continue;
			}

			char username[50];
			int recvResult = recv(clientSocket, username, sizeof(username), 0);
			if (recvResult <= 0) {
				closesocket(clientSocket);
				continue;
			}
			username[recvResult] = '\0';

			WaitForSingleObject(clientsLock, INFINITE);
			if (isUsernameTaken(username)) {
				string errorMsg = "Ошибка: Имя пользователя занято.";
				send(clientSocket, encryptionKey.encrypt(errorMsg).c_str(), errorMsg.size(), 0);
				closesocket(clientSocket);
				ReleaseMutex(clientsLock);
				continue;
			}

			clients.emplace_back(Client(username, clientSocket));
			SetTextRichTextBox("Клиент подключен: " + string(username));
			string successMsg = "TRUE";
			send(clientSocket, successMsg.c_str(), successMsg.size(), 0);
			ReleaseMutex(clientsLock);

			// Создаем поток для обработки клиента
			CreateThread(nullptr, 0, handleClientHelper, new pair<MyServer*, SOCKET>{ this, clientSocket }, 0, nullptr);
		}
	}

	static DWORD WINAPI handleClientHelper(LPVOID param) {
		auto* data = static_cast<pair<MyServer*, SOCKET>*>(param);
		MyServer* server = data->first;
		SOCKET clientSocket = data->second;
		delete data;

		server->handleClient(clientSocket);
		return 0;
	}

	void handleClient(SOCKET clientSocket) {
		char buffer[1024];

		WaitForSingleObject(clientsLock, INFINITE);
		Client* client = findClientBySocket(clientSocket);
		ReleaseMutex(clientsLock);

		if (client) {
			string welcomeMessage = encryptionKey.encrypt("Welcome to chat, " + client->username + "!");
			int messageSize = welcomeMessage.size();
			send(clientSocket, reinterpret_cast<char*>(&messageSize), sizeof(messageSize), 0);
			send(clientSocket, welcomeMessage.c_str(), messageSize, 0);
		}

		while (true) {
			int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
			if (bytesReceived <= 0) {
				SetTextRichTextBox(client->username + " отключился");
				disconnectClient(clientSocket);
				break;
			}

			buffer[bytesReceived] = '\0';
			string encryptedMessage(buffer, bytesReceived);

			if (encryptedMessage == "/exit") {
				SetTextRichTextBox(client->username + " отключился");
				disconnectClient(clientSocket);
				break;
			}
			// Попытка расшифровки
			string decryptedMessage = encryptionKey.decrypt(encryptedMessage);

			// Вывод расшифрованного сообщения
			SetTextRichTextBox("Сообщение от " + client->username + ": " + decryptedMessage);

			// Обработка расшифрованного сообщения
			if (decryptedMessage.rfind("/sendall ", 0) == 0) {
				sendToAll(clientSocket, decryptedMessage.substr(9));
			}
			else if (decryptedMessage == "/list") {
				listClients(clientSocket);
			}
			else if (decryptedMessage.rfind("/send ", 0) == 0) {
				size_t pos = decryptedMessage.find(" ", 6);
				if (pos != string::npos) {
					string targetName = decryptedMessage.substr(6, pos - 6);
					string privateMessage = decryptedMessage.substr(pos + 1);
					sendToUser(clientSocket, targetName, privateMessage);
				}
				else {
					string errorMsg = encryptionKey.encrypt("Error: Invalid command format");
					int messageSize = errorMsg.size();
					send(clientSocket, reinterpret_cast<char*>(&messageSize), sizeof(messageSize), 0);
					send(clientSocket, errorMsg.c_str(), messageSize, 0);
				}
			}
			else {
				string errorMsg = encryptionKey.encrypt("Error: Unknown command");
				int messageSize = errorMsg.size();
				send(clientSocket, reinterpret_cast<char*>(&messageSize), sizeof(messageSize), 0);
				send(clientSocket, errorMsg.c_str(), messageSize, 0);
			}
		}
	}


	void listClients(SOCKET clientSocket) {
		WaitForSingleObject(clientsLock, INFINITE);
		string clientList = "Users:\n";
		for (const auto& client : clients) {
			clientList += client.username + "\n";
		}
		string listUserMessage = encryptionKey.encrypt(clientList);
		int messageSize = listUserMessage.size();
		send(clientSocket, reinterpret_cast<char*>(&messageSize), sizeof(messageSize), 0);
		send(clientSocket, listUserMessage.c_str(), messageSize, 0);
		ReleaseMutex(clientsLock);
	}

	Client* findClientBySocket(SOCKET clientSocket) {
		for (auto& client : clients) {
			if (client.socket == clientSocket) {
				return &client;
			}
		}
		return nullptr;
	}

	bool isUsernameTaken(const string& username) {
		for (const auto& client : clients) {
			if (client.username == username) {
				return true;
			}
		}
		return false;
	}

	void sendToAll(SOCKET senderSocket, const string& message) {
		string senderName = findClientBySocket(senderSocket)->username;
		string encryptedMessage = encryptionKey.encrypt("(General) " + senderName + ": " + message);

		WaitForSingleObject(clientsLock, INFINITE);
		for (const auto& client : clients) {
			if (client.socket != senderSocket) {
				int messageSize = encryptedMessage.size();
				send(client.socket, reinterpret_cast<char*>(&messageSize), sizeof(messageSize), 0);
				send(client.socket, encryptedMessage.c_str(), messageSize, 0);
			}
		}
		ReleaseMutex(clientsLock);

		// Возвращаем подтверждение отправки
		/*string confirmation = encryptionKey.crypt("true");
		int messageSize = confirmation.size();
		send(senderSocket, reinterpret_cast<char*>(&messageSize), sizeof(messageSize), 0);
		send(senderSocket, confirmation.c_str(), messageSize, 0);*/
	}

	void sendToUser(SOCKET senderSocket, const string& targetName, const string& message) {
		string senderName = findClientBySocket(senderSocket)->username;

		WaitForSingleObject(clientsLock, INFINITE);
		for (const auto& client : clients) {
			if (client.username == targetName) {
				string outMessage = encryptionKey.encrypt("(Personal) " + senderName + ": " + message);
				int messageSize = outMessage.size();
				send(client.socket, reinterpret_cast<char*>(&messageSize), sizeof(messageSize), 0);
				send(client.socket, outMessage.c_str(), messageSize, 0);

				// Подтверждение отправки
				/*string confirmation = encryptionKey.crypt("true");
				messageSize = confirmation.size();
				send(senderSocket, reinterpret_cast<char*>(&messageSize), sizeof(messageSize), 0);
				send(senderSocket, confirmation.c_str(), messageSize, 0);*/

				ReleaseMutex(clientsLock);
				return;
			}
		}
		ReleaseMutex(clientsLock);

		// Если пользователь не найден, возвращаем ошибку
		string errorMsg = encryptionKey.encrypt("Error: The user was not found");
		int messageSize = errorMsg.size();
		send(senderSocket, reinterpret_cast<char*>(&messageSize), sizeof(messageSize), 0);
		send(senderSocket, errorMsg.c_str(), messageSize, 0);
	}

	void disconnectClient(SOCKET clientSocket) {
		WaitForSingleObject(clientsLock, INFINITE);
		auto it = find_if(clients.begin(), clients.end(), [clientSocket](const Client& client) {
			return client.socket == clientSocket;
			});
		if (it != clients.end()) {
			closesocket(clientSocket);
			clients.erase(it);
		}
		ReleaseMutex(clientsLock);
	}

	void shutdownServer() {
		isRunning = false;

		string shutdownMessage = "SERVER_SHUTDOWN";
		int messageSize = shutdownMessage.size();
		for (auto& client : clients) {
			try {
				send(client.socket, reinterpret_cast<char*>(&messageSize), sizeof(messageSize), 0);
				send(client.socket, shutdownMessage.c_str(), messageSize, 0);
				closesocket(client.socket); // Закрываем соединение с клиентом
			}
			catch (...) {
			}
		}

		for (auto& thread : clientThreads) {
			WaitForSingleObject(thread, INFINITE);
			CloseHandle(thread); // Закрываем дескриптор потока
		}

		clientThreads.clear();
		clients.clear();
		ReleaseMutex(clientsLock);

		closesocket(serverSocket);
		WSACleanup();

		SetTextRichTextBox("Сервер остановлен.");
	}
};
