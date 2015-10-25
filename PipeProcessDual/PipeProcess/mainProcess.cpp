// Пример процесса сервера анонимного канала.  
// Сервер сначала читает из анонимного канала, а затем пишет в него. 
// Дескриптор анонимного канала передается клиенту через командную строку.  

#include <Windows.h>
#include <conio.h>
#include <iostream>

using namespace std;

int main()
{
	setlocale(0, "");

	char lpszComLine[80];				// для командной строки

	HANDLE hEnableRead;					// для синхронизации обмена данными
	char lpszEnableRead[] = "EnableRead";

	STARTUPINFO si = {sizeof(STARTUPINFO)};
	PROCESS_INFORMATION pi;

	HANDLE hWritePipe, hReadPipe;
	SECURITY_ATTRIBUTES sa;

	// создаем событие для синхронизации обмена данными
	hEnableRead = CreateEvent(NULL, FALSE, FALSE, lpszEnableRead);

	// устанавливаем атрибуты защиты канала
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;				// защита по умолчанию
	sa.bInheritHandle = TRUE;					// дескрипторы наследуемые

	// создаем анонимный канал
	if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0))
	{
		cout << "Create pipe failed.\n";
		cout << "Create any key to finish.\n";
		_getch();
		return GetLastError();
	}

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	// форматируем командную строку
	wsprintf(lpszComLine, "C:/PipeClient.exe %d %d", (int)hWritePipe, (int)hReadPipe);

	// запускаем новый консольный процесс
	if (!CreateProcess(NULL, lpszComLine, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		cout << "Create process failed.\n";
		cout << "Press any key to finish.\n";
		_getch();
		return GetLastError();
	}

	// закрываем дескрипторы нового процесса
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	// читаем из анонимного канала
	for (int i = 0; i < 10; i++)
	{
		int nData;
		DWORD dwBytesRead;
		if (!ReadFile(hReadPipe, &nData, sizeof(nData), &dwBytesRead,NULL))
		{
			cout << "Read from the pipe failed.\n";
			cout << "Press any key to finish.\n";
			_getch();
			return GetLastError();
		}
		_cprintf("The number %d is read from pipe.\n", nData);
//		cout << "The number " << nData << " is read from pipe.\n";
	}
	cout << "The process finished reading from the pipe.\n";

	// даем сигнал на разрешение чтения клиентом
	SetEvent(hEnableRead);

	// пишем в ответ в анонимный канал
	for (int j = 10; j < 20; j++)
	{
		DWORD dwBytesWritten;
		if (!WriteFile(hWritePipe, &j, sizeof(j), &dwBytesWritten, NULL))
		{
			cout << "Write to file failed.\n";
			cout << "Press any key to finish.\n";
			_getch();
			return GetLastError();
		}
//		cout << "The number " << j << " is written to the pipe.\n";
		_cprintf("The number %d is written to the pipe.\n", j);
	}

	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe);
	CloseHandle(hEnableRead);

	cout << "The process finished writing to the pipe.\n";
	cout << "Press any key to finish.\n";
	_getch();

	return 0;
}