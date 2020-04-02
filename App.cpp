#include <iostream>
#include <string>
#include <sstream>
#include "ThreadPool.h"

using namespace std;

mutex consoleMutex;


void PrintNumbers(const string& taskNumber)
{
	std::lock_guard<mutex> g(consoleMutex);
	cout << taskNumber << endl;
	for (size_t i = 0; i < 5; i++)
	{
		cout << i << "\t";
	}

	cout << endl;
}

int main()
{
	int taskNumber = 1;
	ThreadPool tp(PrintNumbers, 5);
	while (true)
	{
		ostringstream os;
		os << "Task" << taskNumber++;
		tp.QueueWork(os.str());
	}

	system("PAUSE");
}


