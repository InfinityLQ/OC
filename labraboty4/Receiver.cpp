#include <windows.h>
#include <iostream>
#include <fstream>

using namespace std;

HANDLE SenderSemaphore;//处理发送者信号量 

int main()
{
	char binary_file_name[256];

	cout << "Process Receiver 开始工作。Process Receiver started work." << endl << endl;

	cout << "输入二进制文件名（例如，\"file\ Enter binary file name (for example, \"file\") : ";
	cin.getline(binary_file_name, 256);

	int records_num = 0;
	cout << "输入该文件中的记录数：Enter the number of records in that file: ";
	cin >> records_num;

	char binary_file_path[1000];
	sprintf_s(binary_file_path, "%s", binary_file_name);
	ofstream f(binary_file_path, ios::binary);
	f.close();

	int senders_amount = 0;
	cout << "输入进程数发件人：Enter the number of processes Sender: ";
	cin >> senders_amount;

	cin.ignore();

	SenderSemaphore = CreateSemaphore(
		//Semaphore是旗语的意思，在Windows中，Semaphore对象用来控制对资源的并发访问数。
		//Semaphore对象具有一个计数值，当值大于0时，Semaphore被置信号，当计数值等于0时，Semaphore被清除信号。
		//每次针对Semaphore的wait functions返回时，计数值被减1，调用ReleaseSemaphore可以将计数值增加 l
		//ReleaseCount 参数值指定的值。
		NULL,
		0,
		records_num,
		TEXT("Sender"));

	HANDLE* EventsFromSender;

	EventsFromSender = new HANDLE[senders_amount];
	for (int i = 0; i < senders_amount; i++)
	{
		char EventNum[256];
		sprintf_s(EventNum, "过程事件Event of the process %d", (i + 1));
		EventsFromSender[i] = CreateEvent(
			NULL,
			TRUE,
			FALSE,
			TEXT(EventNum)

		);
	}

	STARTUPINFO* Sender = new STARTUPINFO[senders_amount];//结构体
	PROCESS_INFORMATION* Sender_Info = new PROCESS_INFORMATION[senders_amount];

	for (int i = 0; i < senders_amount; i++)
	{
		ZeroMemory(&(Sender[i]), sizeof(Sender[i]));//SDK中的一个宏。其作用是用0来填充一块内存区域。
		(Sender[i]).cb = sizeof(Sender[i]);
		ZeroMemory(&(Sender_Info[i]), sizeof(Sender_Info[i]));

		char buffer[1000];

		sprintf_s(buffer, "%s %s %d", "Sender.exe", binary_file_name, (i + 1));

		CreateProcess(NULL,
			buffer,
			NULL,
			NULL,
			FALSE,
			CREATE_NEW_CONSOLE,
			NULL,
			NULL,
			&(Sender[i]),
			&(Sender_Info[i])
		);
	}

	cout << endl << "等待所有 Sender 进程为下一部分工作做好准备...Waiting all Sender processes to be ready for next part of work..." << endl;

	WaitForMultipleObjects(senders_amount, EventsFromSender, TRUE, 1000);
	//这两个函数的优点是它们在等待的过程中会进入一个非常高效沉睡状态，只占用极少的CPU时间片。
	//（这两个函数都是在内核状态下等待内核对象，不切换到用户模式下，因而效率很高）

	cout << "所有发送方进程都准备好恢复工作......All the sender processes are ready to resume work..." << endl << endl;

	int current_records_num = 0;

	while (true)
	{
		if (current_records_num >= records_num)
		{
			cout << endl << "达到记录限制，进程退出读取模式...Record limit reached, process exits read mode..." << endl;
			break;
		}
		ifstream fin(binary_file_path, ios::binary);
		bool file_empty = false;
		if (fin.peek() == EOF)
			file_empty = true;
		fin.close();
		if (file_empty)
		{
			cout << "二进制文件现在是空的。 等待来自 Sender 进程的消息..Binary file is empty now. Waiting for messages from Sender processes..." << endl << endl;
			while (true)
			{
				ifstream fin(binary_file_path, ios::binary);
				if (fin.peek() != EOF)
					break;
				fin.close();
			}
		}
		cout << "有来自发送方进程之一的一些新消息...There is some new message from one of the Sender processes..." << endl;
		cout << "Process Receiver：写入“exit”以完成工作或其他任何操作，以便从文件中读取更多消息：Process Receiver: Write \"exit\" to complete work or anything else in order to read some more message from file:" << endl;
		char answer[256];
		cin.getline(answer, 256);
		if (!strncmp(answer, "exit", 4))
			break;
		else
		{
			char message[20];
			ifstream fin(binary_file_path, ios::binary);
			while (fin.peek() != EOF)
			{
				message[0] = '\0';
				fin.getline(message, 10);
				if (message[0] == '\0')
					cout << "没有来自 Sender 进程的任何新消息...There are no any new messages from Sender processes..." << endl;
				else
				{
					current_records_num++;
					cout << "阅读留言Reading message " << current_records_num << "/" << records_num << " 来自二进制文件：from binary file:" << endl;
					cout << message << endl;
				}
			}
			fin.close();
		}
		fin.close();
		ofstream f(binary_file_path, ios::binary);
		f.close();
		cout << endl;
	}

	cout << "Process Receiver 完成的工作。Process Receiver completed work." << endl;

	for (int i = 0; i < senders_amount; i++)
	{
		CloseHandle(Sender_Info[i].hProcess);
		CloseHandle(Sender_Info[i].hThread);
	}

	CloseHandle(SenderSemaphore);

	for (int i = 0; i < senders_amount; i++)
		CloseHandle(EventsFromSender[i]);

	delete[] Sender;
	delete[] Sender_Info;
	delete[] EventsFromSender;

	return 0;
}