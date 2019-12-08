#include "header.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <map>
using namespace std;



// 프로세스클래스 생성자 id = 프로세스 아이디, BurstTime = 작업 시간, RemaingTime = 남은 시간
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
	// 생성 단계의 프로세스들의 도착시간 기준으로 오름차순 정렬하여 map에 저장
	// (key값이 도착시간인 map에 value로 프로세스 아이디 삽입)
	for (int i = 0; i < *_pArray_Index; i++) {
		produceMap.insert(pair<double, int>(_pArray[i]->get_ArrivalTime(), _pArray[i]->get_id()));
	}

	// 준비 큐에 맨 처음의 생성 단계의 프로세스 아이디 push 후 map에서 지우기
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
	// map 안 생성 단계의 프로세스들 중, 도착시간이 현재 시간보다 빠를 시(프로세스가 도착했다면), 준비 큐로 이동
	// 이동했으므로 생성 map에서는 원소 삭제, value인 프로세스 아이디를 준비 큐에 push
	for (auto iter = produceMap.begin(); iter != produceMap.end();) {
		if (iter->first < _time) {
			readyQueue.push(iter->second);
			produceMap.erase(iter++);
		}
		else {		// 런타임에러 방지
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

// 프로세스의 정보를 읽어 저장하는 함수
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

		// 프로세스 정보별 버퍼
		const int BUFFER_SIZE = 20;
		char buffer[BUFFER_SIZE] = { NULL };
		char process_id[BUFFER_SIZE] = { NULL };
		char burst_time[BUFFER_SIZE] = { NULL };
		char arrival_time[BUFFER_SIZE] = { NULL };
		char quantum[BUFFER_SIZE] = { NULL };

		while (*_proceeArray_Index < MAX_PROCESS_NUMBER) {		// 총 프로세스의 개수가 최대 프로세스 개수보다 적을 때 실행 가능
			fin.getline(buffer, BUFFER_SIZE, ' ');		// 한 토큰씩 버퍼에 txt 읽어오기
			if (fin.eof()) {		// 파일 끝일 때

				// 타임 슬라이스 시간 설정
				for (int i = 0; i < BUFFER_SIZE; i++) {
					quantum[i] = buffer[i];
				}//end for

				_RRSch->setQuantum(atof(quantum));
				cout << "Quantum = " << _RRSch->get_quantum() << endl;

				break;
			}
			else {
				// 버퍼에 담긴 프로세스 아이디, 작업 시간, 도착시간 쪼개서 담기
				for (int i = 0; buffer[i] != NULL; i++) {
					process_id[i] = buffer[i];
				}//end for
				fin.getline(buffer, BUFFER_SIZE, ' ');
				for (int i = 0; buffer[i] != NULL; i++) {
					burst_time[i] = buffer[i];
				}//end for
				fin.getline(arrival_time, BUFFER_SIZE, '\n');
			}//end if

			// 각각의 버퍼에 담은 프로세스 정보들을 이용하여 새로운 프로세스클래스 객체 생성
			_processArray[*_proceeArray_Index] = new ProcessClass(atoi(process_id), atof(burst_time), atof(arrival_time));

			// 프로세스 정보가 잘 담겼는지 확인
			cout << "Process Id = " << _processArray[*_proceeArray_Index]->get_id();
			cout << ", Burst Time = " << _processArray[*_proceeArray_Index]->get_BurstTime();
			cout << ", Arrival Time = " << _processArray[*_proceeArray_Index]->get_ArrivalTime();
			cout << endl;

			// 배열 초기화
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
	// 프로세스 도착시간에 맞춰서 준비 큐에 프로세스 인덱스 삽입
	_RRSch->setReadyQueue(_processArray, _processArray_Index);
	int NotFinshProcessNumber = *_processArray_Index;	//처리되지 않은 process 수
	int executingIndex = 0;		// 실행할 프로세스 아이디
	double currentTime = _processArray[_RRSch->get_startIndex()]->get_ArrivalTime();		// 제일 처음으로 도착한 프로세스의 시간으로 갱신

	while (NotFinshProcessNumber > 0) {		// 모든 프로세스가 처리될 때까지 실행
		// 준비 큐의 맨 앞에서 기다리는(front) 프로세스 실행시키기
		executingIndex = _RRSch->get_readyQueueFront();
		if (_processArray[executingIndex]->get_RemaingTime() > 0) {
			if (_processArray[executingIndex]->get_RemaingTime() > _RRSch->get_quantum()) {
				// process 잔여 시간이 Quantum보다 큰 경우, Quantum Time만큼 실행 가능
				_RRSch->setScheduling(_processArray[executingIndex]->get_id(), currentTime, currentTime + _RRSch->get_quantum(),
					currentTime - _processArray[executingIndex]->get_StopTime());
				_processArray[executingIndex]->ExecutTime(_RRSch->get_quantum());		// process 잔여 시간 갱신
				currentTime += _RRSch->get_quantum();		// 현재 시간 갱신
				_processArray[executingIndex]->set_StopTime(currentTime);		// 해당 프로세스 중단 시간 갱신
				if (!_RRSch->is_produceMapEmpty()) { _RRSch->updateReadyQueue(currentTime); }		// 도착한 프로세스 준비 큐로 이동
				_RRSch->set_readyQueueBack(executingIndex);		// 프로세스를 끝내지 못했으므로 준비 큐의 맨 뒤로 push
			}
			else {
				// process 남은 시간이 quantom보다 작을 때
				_RRSch->setScheduling(_processArray[executingIndex]->get_id(), currentTime, currentTime + _processArray[executingIndex]->get_RemaingTime(),
					currentTime - _processArray[executingIndex]->get_StopTime());
				currentTime += _processArray[executingIndex]->get_RemaingTime();		// 현재 시간 갱신
				_processArray[executingIndex]->ExecutTime(_processArray[executingIndex]->get_RemaingTime());		// process 잔여 시간 0
				if (!_RRSch->is_produceMapEmpty()) { _RRSch->updateReadyQueue(currentTime); }		// 도착한 프로세스 준비 큐로 이동
				// 프로세스 하나를 완료하였으니 미완료된 process 수--
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

	// 출력하는 txt 파일에 입력한 txt 파일 내용 그대로 출력
	while (true) {
		fin.getline(buffer, BUFFER_SIZE, '\n');
		fout << buffer << endl;
		resetCharArray(buffer, BUFFER_SIZE);
		if (fin.eof()) { break; }
	}//end while
	fout << endl;

	double meanOfWaitingTime = 0.0;
	// 라운드로빈 타임라인 출력
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

		// 평균대기시간 구하고 출력
		if (i == _RRSch->get_executionIndex() - 1) {
			meanOfWaitingTime /= (double)*_processArray_Index;
			cout << "mean of waitingtime = " << meanOfWaitingTime << endl;
			fout << "mean of waitingtime = " << meanOfWaitingTime << endl;
		}//end if

	}//end for

	fin.close();
	fout.close();

}//end WirteExecutionResult


// Array를 출력하는 함수 (개발 때 값 확인용 함수)
void showCharArray(char *_showchar, int _charsize) {
	for (int i = 0; i < _charsize; i++) {
		if (_showchar[i] == NULL) { break; }
		cout << _showchar[i];
	}//end for

}//end showCharArray



// Array를 NULL로 초기화하는 함수
void resetCharArray(char *_resetchar, int _charsize) {
	for (int i = 0; i < _charsize; i++) {
		_resetchar[i] = NULL;
	}//end for
}//end resetCharArray

