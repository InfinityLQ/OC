#include <windows.h>
#include <iostream>
#include <fstream>
#include "unistd.h"
using namespace std;

int main(int argc, char* argv[])
{
	int process_num = atoi(argv[2]);

	cout << "处理发件人Process Sender " << process_num << " 开始工作了。started work." << endl << endl;

	char EventNum[256];
	sprintf_s(EventNum, "过程事件Event of the process %d", process_num);

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
		cout << "处理发件人Process Sender " << process_num << ": 等待队列...Waiting for the queue..." << endl;
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
		cout << "处理发件人Process Sender " << process_num << ": 将消息写给 Receiver 或写 \"exit\" 完成工作：: Write the message to the Receiver or write \"exit\" to complete work:" << endl;
		char message[20];
		cin.getline(message, 20);
		if (!strncmp(message, "exit", 4))
			break;
		else
		{
			Queue MessagesQueue; //是一种进程间通信或同一进程的不同线程间的通信方式，软件的贮列用来处理一系列的输入，通常是来自用户。 
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
			cout << "来自进程发送者的消息Message from Process Sender " << process_num << " successfully sent" << endl;
			fout.close();
		}
		cout << endl;
	}

	cout << "处理发件人Process Sender " << process_num << " 完成的工作。completed work." << endl;

	CloseHandle(EventFromSender);

	return 0;
}