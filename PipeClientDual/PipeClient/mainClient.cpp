#include <conio.h>
#include <iostream>
#include <Windows.h>

using namespace std;

int main(int argc, char *argv[])
{
	HANDLE hWritePipe, hReadPipe;
	HANDLE hEnableRead;					// ��� ������������� ������ �������
	char lpszEnableRead[] = "EnableRead";

	// ��������� �������, ����������� ������
	hEnableRead = OpenEvent(EVENT_ALL_ACCESS, FALSE, lpszEnableRead);

	// ����������� ���������� ������������� ������������ � �����
	hWritePipe = (HANDLE)atoi(argv[1]);
	hReadPipe = (HANDLE)atoi(argv[2]);

	// ���� ������� � ������ ������ � ��������� �����
	cout << "Enter any key to start communication.\n";
	_getch();

	// ����� � ��������� �����
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

	// ���� ���������� �� ������
	WaitForSingleObject(hEnableRead, INFINITE);

	// ������ � ����� �� ���������� ������
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


	// ��������� ����������� ������
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(hEnableRead);

	_getch();
	return 0;
}