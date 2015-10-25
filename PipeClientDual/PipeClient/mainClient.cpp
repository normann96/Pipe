#include <conio.h>
#include <iostream>
#include <Windows.h>

using namespace std;

int main(int argc, char *argv[])
{
	HANDLE hWritePipe, hReadPipe;
	HANDLE hEnableRead;					// дл€ синхронизации обмена данными
	char lpszEnableRead[] = "EnableRead";

	// открываем событие, разрешающее чтение
	hEnableRead = OpenEvent(EVENT_ALL_ACCESS, FALSE, lpszEnableRead);

	// преобразуем символьное представление дескрипторов в число
	hWritePipe = (HANDLE)atoi(argv[1]);
	hReadPipe = (HANDLE)atoi(argv[2]);

	// ждем команды о начале записи в анонимный канал
	cout << "Enter any key to start communication.\n";
	_getch();

	// ѕишем в анонимный канал
	for (int i = 0; i < 10; i++)
	{
		DWORD dwBytesWritten;
		if (!WriteFile(hWritePipe, &i, sizeof(i), &dwBytesWritten, NULL))
		{
			cout << "Write to file failed.\n";
			cout << "Press any key to finish.\n";
			_getch();
			return GetLastError();
		}
		//cout << "The number " << i << " is written to the pipe.\n";
		_cprintf("The number %d is written to the pipe.\n", i);
	}
	cout << "The process finished writing to the pipe.\n";

	// ждем разрешение на чтение
	WaitForSingleObject(hEnableRead, INFINITE);

	// читаем в ответ из анонимного канала
	for (int j = 0; j < 10; j++)
	{
		int nData;
		DWORD dwBytesRead;
		if (!ReadFile(hReadPipe, &nData, sizeof(nData), &dwBytesRead, NULL))
		{
			cout << "Read from the pipe failed.\n";
			cout << "Press any key to finish.\n";
			_getch();
			return GetLastError();
		}
//		cout << "The number " << nData << " is read from pipe.\n";
		_cprintf("The number %d is read from pipe.\n", nData);
	}
	cout << "The process finished reading to the pipe.\n";
	cout << "Press any key to exit.\n"; 
	_getch();


	// «акрываем дескрипторы канала
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(hEnableRead);

	_getch();
	return 0;
}