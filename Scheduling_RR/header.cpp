#include "header.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <map>
using namespace std;



// ���μ���Ŭ���� ������ id = ���μ��� ���̵�, BurstTime = �۾� �ð�, RemaingTime = ���� �ð�
ProcessClass::ProcessClass(int _id, double _BurstTime, double _ArrivalTime) {
	id = _id;
	BurstTime = _BurstTime;
	ArrivalTime = _ArrivalTime;
	RemaingTime = _BurstTime;
	StopTime = _ArrivalTime;
}//end ProcessClass Constructor

int ProcessClass::get_id() { return id; };
double ProcessClass::get_BurstTime() { return BurstTime; };
double ProcessClass::get_RemaingTime() { return RemaingTime; };
double ProcessClass::get_ArrivalTime() { return ArrivalTime; };
double ProcessClass::get_StopTime() { return StopTime; };

void ProcessClass::ExecutTime(double _time) {
	RemaingTime = RemaingTime - _time;
}//end ProcessClass ExecuTime

void ProcessClass::set_StopTime(double _time) {
	StopTime = _time;
}



RRScheduling::RRScheduling(int _processMAXNumber) {

	//2-D Address to pointer
	ptrExecutionProcess = new double*[_processMAXNumber];

	for (int i = 0; i < _processMAXNumber; i++) {
		ptrExecutionProcess[i] = new double[4];
	}//end for
	executionIndex = 0;
}//end RRScheduling Constructor
void RRScheduling::setQuantum(double _quantum) { quantum = _quantum; }
void RRScheduling::setScheduling(int _process_id, double _startTime, double _endTime, double _waitTime) {
	ptrExecutionProcess[executionIndex][0] = _process_id;
	ptrExecutionProcess[executionIndex][1] = _startTime;
	ptrExecutionProcess[executionIndex][2] = _endTime;
	ptrExecutionProcess[executionIndex][3] = _waitTime;
	executionIndex++;
}
void RRScheduling::setReadyQueue(ProcessClass **_pArray, int* _pArray_Index) {
	// ���� �ܰ��� ���μ������� �����ð� �������� �������� �����Ͽ� map�� ����
	// (key���� �����ð��� map�� value�� ���μ��� ���̵� ����)
	for (int i = 0; i < *_pArray_Index; i++) {
		produceMap.insert(pair<double, int>(_pArray[i]->get_ArrivalTime(), _pArray[i]->get_id()));
	}

	// �غ� ť�� �� ó���� ���� �ܰ��� ���μ��� ���̵� push �� map���� �����
	auto start = produceMap.begin();
	readyQueue.push(start->second);
	startIndex = start->second;
	produceMap.erase(start);

}
void RRScheduling::set_readyQueueBack(int _index) { readyQueue.push(_index); };

double * RRScheduling::get_ExecutionProcess(int _index) { return ptrExecutionProcess[_index]; };
int RRScheduling::get_executionIndex() { return executionIndex; };
double RRScheduling::get_quantum() { return quantum; };
int RRScheduling::get_startIndex() { return startIndex; };
void RRScheduling::updateReadyQueue(double _time) {
	// map �� ���� �ܰ��� ���μ����� ��, �����ð��� ���� �ð����� ���� ��(���μ����� �����ߴٸ�), �غ� ť�� �̵�
	// �̵������Ƿ� ���� map������ ���� ����, value�� ���μ��� ���̵� �غ� ť�� push
	for (auto iter = produceMap.begin(); iter != produceMap.end();) {
		if (iter->first < _time) {
			readyQueue.push(iter->second);
			produceMap.erase(iter++);
		}
		else {		// ��Ÿ�ӿ��� ����
			++iter;
		}
	}
}
bool RRScheduling::is_produceMapEmpty() { return produceMap.empty(); };
int RRScheduling::get_readyQueueFront() {
	if (!readyQueue.empty()) {
		int exec = readyQueue.front();
		readyQueue.pop();
		return exec;
	}
};
//int RRScheduling::get_MeanOfWaitingTime() {return };

// ���μ����� ������ �о� �����ϴ� �Լ�
void ReadProcessInfo(const char *_FileName, ProcessClass **_processArray, int *_proceeArray_Index, const int MAX_PROCESS_NUMBER, RRScheduling *_RRSch) {
	//Open File
	try {
		fstream fin;
		fin.open(_FileName, ios::in);
		if (fin.fail()) {
			fin.close();
			cout << "\n#01-Read file opening failed.\n";
			system("pause");
			exit(1);
		}//end if

		cout << "\n#01-Read File Success" << endl;

		// ���μ��� ������ ����
		const int BUFFER_SIZE = 20;
		char buffer[BUFFER_SIZE] = { NULL };
		char process_id[BUFFER_SIZE] = { NULL };
		char burst_time[BUFFER_SIZE] = { NULL };
		char arrival_time[BUFFER_SIZE] = { NULL };
		char quantum[BUFFER_SIZE] = { NULL };

		while (*_proceeArray_Index < MAX_PROCESS_NUMBER) {		// �� ���μ����� ������ �ִ� ���μ��� �������� ���� �� ���� ����
			fin.getline(buffer, BUFFER_SIZE, ' ');		// �� ��ū�� ���ۿ� txt �о����
			if (fin.eof()) {		// ���� ���� ��

				// Ÿ�� �����̽� �ð� ����
				for (int i = 0; i < BUFFER_SIZE; i++) {
					quantum[i] = buffer[i];
				}//end for

				_RRSch->setQuantum(atof(quantum));
				cout << "Quantum = " << _RRSch->get_quantum() << endl;

				break;
			}
			else {
				// ���ۿ� ��� ���μ��� ���̵�, �۾� �ð�, �����ð� �ɰ��� ���
				for (int i = 0; buffer[i] != NULL; i++) {
					process_id[i] = buffer[i];
				}//end for
				fin.getline(buffer, BUFFER_SIZE, ' ');
				for (int i = 0; buffer[i] != NULL; i++) {
					burst_time[i] = buffer[i];
				}//end for
				fin.getline(arrival_time, BUFFER_SIZE, '\n');
			}//end if

			// ������ ���ۿ� ���� ���μ��� �������� �̿��Ͽ� ���ο� ���μ���Ŭ���� ��ü ����
			_processArray[*_proceeArray_Index] = new ProcessClass(atoi(process_id), atof(burst_time), atof(arrival_time));

			// ���μ��� ������ �� ������ Ȯ��
			cout << "Process Id = " << _processArray[*_proceeArray_Index]->get_id();
			cout << ", Burst Time = " << _processArray[*_proceeArray_Index]->get_BurstTime();
			cout << ", Arrival Time = " << _processArray[*_proceeArray_Index]->get_ArrivalTime();
			cout << endl;

			// �迭 �ʱ�ȭ
			resetCharArray(process_id, BUFFER_SIZE);
			resetCharArray(burst_time, BUFFER_SIZE);
			resetCharArray(quantum, BUFFER_SIZE);

			*_proceeArray_Index += 1;

		}//end while

		fin.close();
	}
	catch (exception e) {
		cout << "\n#01-ReadProcessInfo : " << e.what() << endl;
	}//end try

}//end ReadProcessInfo


void Scheduling(ProcessClass **_processArray, int*_processArray_Index, RRScheduling *_RRSch) {
	// ���μ��� �����ð��� ���缭 �غ� ť�� ���μ��� �ε��� ����
	_RRSch->setReadyQueue(_processArray, _processArray_Index);
	int NotFinshProcessNumber = *_processArray_Index;	//ó������ ���� process ��
	int executingIndex = 0;		// ������ ���μ��� ���̵�
	double currentTime = _processArray[_RRSch->get_startIndex()]->get_ArrivalTime();		// ���� ó������ ������ ���μ����� �ð����� ����

	while (NotFinshProcessNumber > 0) {		// ��� ���μ����� ó���� ������ ����
		// �غ� ť�� �� �տ��� ��ٸ���(front) ���μ��� �����Ű��
		executingIndex = _RRSch->get_readyQueueFront();
		if (_processArray[executingIndex]->get_RemaingTime() > 0) {
			if (_processArray[executingIndex]->get_RemaingTime() > _RRSch->get_quantum()) {
				// process �ܿ� �ð��� Quantum���� ū ���, Quantum Time��ŭ ���� ����
				_RRSch->setScheduling(_processArray[executingIndex]->get_id(), currentTime, currentTime + _RRSch->get_quantum(),
					currentTime - _processArray[executingIndex]->get_StopTime());
				_processArray[executingIndex]->ExecutTime(_RRSch->get_quantum());		// process �ܿ� �ð� ����
				currentTime += _RRSch->get_quantum();		// ���� �ð� ����
				_processArray[executingIndex]->set_StopTime(currentTime);		// �ش� ���μ��� �ߴ� �ð� ����
				if (!_RRSch->is_produceMapEmpty()) { _RRSch->updateReadyQueue(currentTime); }		// ������ ���μ��� �غ� ť�� �̵�
				_RRSch->set_readyQueueBack(executingIndex);		// ���μ����� ������ �������Ƿ� �غ� ť�� �� �ڷ� push
			}
			else {
				// process ���� �ð��� quantom���� ���� ��
				_RRSch->setScheduling(_processArray[executingIndex]->get_id(), currentTime, currentTime + _processArray[executingIndex]->get_RemaingTime(),
					currentTime - _processArray[executingIndex]->get_StopTime());
				currentTime += _processArray[executingIndex]->get_RemaingTime();		// ���� �ð� ����
				_processArray[executingIndex]->ExecutTime(_processArray[executingIndex]->get_RemaingTime());		// process �ܿ� �ð� 0
				if (!_RRSch->is_produceMapEmpty()) { _RRSch->updateReadyQueue(currentTime); }		// ������ ���μ��� �غ� ť�� �̵�
				// ���μ��� �ϳ��� �Ϸ��Ͽ����� �̿Ϸ�� process ��--
				NotFinshProcessNumber -= 1;
			}//end if
		}//end if
	}//end while
}//end Scheduling



void WirteExecutionResult(RRScheduling *_RRSch, int* _processArray_Index, const char *_READ_FILE_NAME, const char *_WRITE_FILE_NAME) {

	fstream fin(_READ_FILE_NAME, ios::in);
	fstream fout(_WRITE_FILE_NAME, ios::out);

	const int BUFFER_SIZE = 20;
	char buffer[BUFFER_SIZE];

	// ����ϴ� txt ���Ͽ� �Է��� txt ���� ���� �״�� ���
	while (true) {
		fin.getline(buffer, BUFFER_SIZE, '\n');
		fout << buffer << endl;
		resetCharArray(buffer, BUFFER_SIZE);
		if (fin.eof()) { break; }
	}//end while
	fout << endl;

	double meanOfWaitingTime = 0.0;
	// ����κ� Ÿ�Ӷ��� ���
	for (int i = 0; i < _RRSch->get_executionIndex(); i++) {
		cout << "pId = " << _RRSch->get_ExecutionProcess(i)[0];
		cout << ", start = " << _RRSch->get_ExecutionProcess(i)[1];
		cout << ", end = " << _RRSch->get_ExecutionProcess(i)[2];
		cout << ", wait = " << _RRSch->get_ExecutionProcess(i)[3];
		cout << endl;

		fout << _RRSch->get_ExecutionProcess(i)[0];
		fout << " " << _RRSch->get_ExecutionProcess(i)[1];
		fout << " ~ " << _RRSch->get_ExecutionProcess(i)[2];
		fout << " wait : " << _RRSch->get_ExecutionProcess(i)[3];
		fout << endl;

		meanOfWaitingTime += _RRSch->get_ExecutionProcess(i)[3];

		// ��մ��ð� ���ϰ� ���
		if (i == _RRSch->get_executionIndex() - 1) {
			meanOfWaitingTime /= (double)*_processArray_Index;
			cout << "mean of waitingtime = " << meanOfWaitingTime << endl;
			fout << "mean of waitingtime = " << meanOfWaitingTime << endl;
		}//end if

	}//end for

	fin.close();
	fout.close();

}//end WirteExecutionResult


// Array�� ����ϴ� �Լ� (���� �� �� Ȯ�ο� �Լ�)
void showCharArray(char *_showchar, int _charsize) {
	for (int i = 0; i < _charsize; i++) {
		if (_showchar[i] == NULL) { break; }
		cout << _showchar[i];
	}//end for

}//end showCharArray



// Array�� NULL�� �ʱ�ȭ�ϴ� �Լ�
void resetCharArray(char *_resetchar, int _charsize) {
	for (int i = 0; i < _charsize; i++) {
		_resetchar[i] = NULL;
	}//end for
}//end resetCharArray

