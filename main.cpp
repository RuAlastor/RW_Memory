#include <iostream>
#include <Windows.h>
#include <string>
#include <sstream>

enum Actions { do_exit, do_RInt, do_RCh, do_RStr, do_WInt, do_WCh, do_WStr };

UINT readInteger(HANDLE);
UINT readChar(HANDLE);
UINT readString(HANDLE);
UINT writeInteger(HANDLE);
UINT writeChar(HANDLE);
UINT writeString(HANDLE);

int main(int argc, char* argv[]) {
	DWORD PID = 0;
	if (argc < 2) {
		std::cout << "Please, enter Process ID: ";
		std::cin >> std::dec >> PID;
	}
	else {
		PID = atol(argv[1]);
	}
	HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, false, PID); 
	if (pHandle == NULL) {
		std::cout << "Failed to open process. Error: " << GetLastError() << '\n';
		system("pause");
		return EXIT_FAILURE;
	}	

	int action = 0;
	while (true) {
		std::cout << "\nWhat do you want to do?\n"
			<< "0 - exit\n"
			<< "1 - read pointer to an integer\n"
			<< "2 - read char\n"
			<< "3 - read string\n"
			<< "4 - write to an integer\n"
			<< "5 - write to a char array\n"
			<< "6 - write to a string\n"
			<< "...\n";
		std::cin >> action;
		switch (action) {
		case do_exit:
			return 0;
			break;

		case  do_RInt:
			if (readInteger(pHandle)) {
				std::cout << "Reading integer failed!\n";
				system("puase");
				return EXIT_FAILURE;
			}
			break;

		case do_RCh:
			if (readChar(pHandle)) {
				std::cout << "Reading char failed!\n";
				system("puase");
				return EXIT_FAILURE;
			}
			break;

		case do_RStr:
			if (readString(pHandle)) {
				std::cout << "Reading string failed!\n";
				system("puase");
				return EXIT_FAILURE;
			}
			break;

		case do_WInt:
			if (writeInteger(pHandle)) {
				std::cout << "Writing integer failed!\n";
				system("pause");
				return EXIT_FAILURE;
			}
			break;

		case do_WCh:
			if (writeChar(pHandle)) {
				std::cout << "Failed to write to char array!\n";
				system("pause");
				return EXIT_FAILURE;
			}
			break;

		case do_WStr:
			if (writeString(pHandle)) {
				std::cout << "Failed to write to string!\n";
				system("pause");
				return EXIT_FAILURE;

			}
			break;

		default:
			return 0;
			break;
		}
	}

	CloseHandle(pHandle);

	system("pause");
	return 0;
}

UINT readInteger(HANDLE pHandle) {

	uintptr_t memAddr2Read = 0x0;
	std::cout << "\nPlease, enter Integer Memory Address: ";
	std::cin >> std::hex >> memAddr2Read;

	uintptr_t readPtr = 0;
	SIZE_T written2Buffer = 0;
	if (!ReadProcessMemory(pHandle,
		reinterpret_cast<LPCVOID>(memAddr2Read),
		reinterpret_cast<LPVOID>(&readPtr),
		sizeof(uintptr_t),
		&written2Buffer)) {
		std::cout << "Failed to read memory\n";
		system("pause");
		return EXIT_FAILURE;
	}

	memAddr2Read = readPtr;
	int buffer;
	if (!ReadProcessMemory(pHandle,
		reinterpret_cast<LPCVOID>(memAddr2Read),
		reinterpret_cast<LPVOID>(&buffer),
		sizeof(int),
		&written2Buffer)) {
		std::cout << "Failed to read memory\n";
		system("pause");
		return EXIT_FAILURE;
	}

	std::cout << "Read integer: " << buffer << '\n'
		<< "Bytes read: " << written2Buffer << '\n';

	return 0;

}

UINT readChar(HANDLE pHandle) {

	uintptr_t memAddr2Read = 0x0;
	std::cout << "\nPlease, enter array address to read: ";
	std::cin >> std::hex >> memAddr2Read;
	std::cout << "Please, enter how many symbols to read: ";
	unsigned int symbols_to_read = 0;
	std::cin >> std::dec >> symbols_to_read;

	while (symbols_to_read > 50) {
		std::cout << "Please, reenter your number. It should be less than 50!\n";
		std::cin >> symbols_to_read;
	}

	char* ch_buffer = new char[50];
	memset(ch_buffer, 0, sizeof(char) * 50);

	SIZE_T written2Buffer = 0;
	SIZE_T totalBytesRead = 0;
	
	if (!ReadProcessMemory(pHandle,
							reinterpret_cast<LPCVOID>(memAddr2Read),
							reinterpret_cast<LPVOID>(ch_buffer),
							sizeof(char) * symbols_to_read,
							&written2Buffer)) {
		return EXIT_FAILURE;
	}
	totalBytesRead += written2Buffer;

	std::cout << "String: " << ch_buffer << '\n'
		<< "Read bytes: " << totalBytesRead << '\n';

	delete[] ch_buffer;
	return 0;

}

UINT readString(HANDLE pHandle) {

	uintptr_t memAddr2Read = 0x0;
	std::cout << "\nPlease, enter string address to read: ";
	std::cin >> std::hex >> memAddr2Read;
	std::cout << "Please, enter how many symbols to read: ";
	unsigned int symbols_to_read = 0;
	std::cin >> std::dec >> symbols_to_read;

	while (symbols_to_read > 50) {
		std::cout << "Please, reenter your number. It should be less than 50!\n";
		std::cin >> symbols_to_read;
	}

	char* ch_buffer = new char[50];
	memset(ch_buffer, 0, sizeof(char) * 50);

	SIZE_T written2Buffer = 0;
	SIZE_T totalBytesRead = 0;

	if (!ReadProcessMemory(pHandle,
							reinterpret_cast<LPCVOID>(memAddr2Read + 4),
							reinterpret_cast<LPVOID>(ch_buffer),
							sizeof(char) * symbols_to_read,
							&written2Buffer)) {
		return EXIT_FAILURE;
	}
	totalBytesRead += written2Buffer;

	std::cout << "String: " << ch_buffer << '\n'
		<< "Read bytes: " << totalBytesRead << '\n';

	delete[] ch_buffer;	
	return 0;

}

UINT writeInteger(HANDLE pHandle) {

	uintptr_t memAddr2Write = 0x0;
	std::cout << "\nPlease, enter Integer Memory Address: ";
	std::cin >> std::hex >> memAddr2Write;

	uintptr_t writeInteger = 0;
	std::cout << "Enter any integer: ";
	std::cin >> std::dec >> writeInteger;

	SIZE_T writtenFromBuffer = 0;

	if (!WriteProcessMemory(pHandle,
							reinterpret_cast<LPVOID>(memAddr2Write),
							reinterpret_cast<LPCVOID>(&writeInteger),
							sizeof(int),	
							&writtenFromBuffer)) {
		std::cout << "Error: " << GetLastError() << '\n';
		return EXIT_FAILURE;
	}
	std::cout << "Overwritten successfully!\n";

	return 0;

}

UINT writeChar(HANDLE pHandle) {

	uintptr_t memAddr2Write = 0x0;
	std::cout << "\nPlease, enter array address to write: ";
	std::cin >> std::hex >> memAddr2Write;
	getchar();
	
	std::string str2Write("");
	std::cout << "Enter your string: ";
	getline(std::cin, str2Write);

	SIZE_T writtenFromBuffer = 0;
	SIZE_T totalWrittenFromBuffer = 0;

	std::stringstream deriv(str2Write);
	str2Write.clear();
	while (deriv >> str2Write) {
		if (!WriteProcessMemory(pHandle,
								reinterpret_cast<LPVOID>(memAddr2Write + totalWrittenFromBuffer),
								reinterpret_cast<LPCVOID>(str2Write.c_str()),
								str2Write.size(),
								&writtenFromBuffer)) {
			std::cout << "Error: " << GetLastError() << '\n';
			return EXIT_FAILURE;
		}
		totalWrittenFromBuffer += writtenFromBuffer;
		str2Write = ' ';
		if (!WriteProcessMemory(pHandle,
			reinterpret_cast<LPVOID>(memAddr2Write + totalWrittenFromBuffer),
			reinterpret_cast<LPCVOID>(str2Write.c_str()),
			str2Write.size(),
			&writtenFromBuffer)) {
			std::cout << "Error: " << GetLastError() << '\n';
			return EXIT_FAILURE;
		}
		totalWrittenFromBuffer += writtenFromBuffer;
	}

	std::cout << "Char array was overwritten successfully!" << '\n'
		<< "Read bytes: " << totalWrittenFromBuffer << '\n';

	return 0;

}

UINT writeString(HANDLE pHandle) {

	uintptr_t memAddr2Write = 0x0;
	std::cout << "\nPlease, enter string address to write: ";
	std::cin >> std::hex >> memAddr2Write;
	getchar();

	std::string str2Write("");
	std::cout << "Enter your string: ";
	getline(std::cin, str2Write);

	SIZE_T writtenFromBuffer = 0;
	SIZE_T totalWrittenFromBuffer = 0;

	std::stringstream deriv(str2Write);
	str2Write.clear();
	while (deriv >> str2Write) {
		if (!WriteProcessMemory(pHandle,
			reinterpret_cast<LPVOID>(memAddr2Write + 4 + totalWrittenFromBuffer),
			reinterpret_cast<LPCVOID>(str2Write.c_str()),
			str2Write.size(),
			&writtenFromBuffer)) {
			std::cout << "Error: " << GetLastError() << '\n';
			return EXIT_FAILURE;
		}
		totalWrittenFromBuffer += writtenFromBuffer;
		str2Write = ' ';
		if (!WriteProcessMemory(pHandle,
			reinterpret_cast<LPVOID>(memAddr2Write + 4 + totalWrittenFromBuffer),
			reinterpret_cast<LPCVOID>(str2Write.c_str()),
			str2Write.size(),
			&writtenFromBuffer)) {
			std::cout << "Error: " << GetLastError() << '\n';
			return EXIT_FAILURE;
		}
		totalWrittenFromBuffer += writtenFromBuffer;
	}

	std::cout << "String was overwritten successfully!" << '\n'
		<< "Read bytes: " << totalWrittenFromBuffer << '\n';
	return 0;

}