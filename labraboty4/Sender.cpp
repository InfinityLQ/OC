#include <windows.h>
#include <iostream>
#include <fstream>
#include "unistd.h"
using namespace std;

int main(int argc, char* argv[])
{
	int process_num = atoi(argv[2]);

	cout << "��������Process Sender " << process_num << " ��ʼ�����ˡ�started work." << endl << endl;

	char EventNum[256];
	sprintf_s(EventNum, "�����¼�Event of the process %d", process_num);

	char binary_file_path[1000];
	sprintf_s(binary_file_path, "%s", argv[1]);

	HANDLE EventFromSender = OpenEvent(
		NULL,
		TRUE, 
		TEXT(EventNum)
	);

	SetEvent(EventFromSender);

	while (true)
	{
		cout << "��������Process Sender " << process_num << ": �ȴ�����...Waiting for the queue..." << endl;
		HANDLE Semaphore = OpenSemaphore(
			NULL,
			TRUE,
			TEXT("Sender")
		);

		ReleaseSemaphore(
			Semaphore,
			1,
			NULL
		);
		cout << "��������Process Sender " << process_num << ": ����Ϣд�� Receiver ��д \"exit\" ��ɹ�����: Write the message to the Receiver or write \"exit\" to complete work:" << endl;
		char message[20];
		cin.getline(message, 20);
		if (!strncmp(message, "exit", 4))
			break;
		else
		{
			Queue MessagesQueue; //��һ�ֽ��̼�ͨ�Ż�ͬһ���̵Ĳ�ͬ�̼߳��ͨ�ŷ�ʽ�������������������һϵ�е����룬ͨ���������û��� 
			ifstream fin(binary_file_path);
			while (fin.peek() != EOF)
			{
				char* temp = new char[20];
				fin.getline(temp, 20);
				AddToQueue(&MessagesQueue, temp);
				delete[] temp;
			}
			fin.close();
			ofstream fout(binary_file_path);
			while (!QueueIsEmpty(&MessagesQueue))
			{
				fout << MessagesQueue.first->msg << endl;
				DeleteFromQueue(&MessagesQueue);
			}
			fout << message << endl;
			cout << "���Խ��̷����ߵ���ϢMessage from Process Sender " << process_num << " successfully sent" << endl;
			fout.close();
		}
		cout << endl;
	}

	cout << "��������Process Sender " << process_num << " ��ɵĹ�����completed work." << endl;

	CloseHandle(EventFromSender);

	return 0;
}