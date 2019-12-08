#pragma once
#include <string>
#include <queue>
#include <map>
using namespace std;



//Save Process Information (프로세스 정보를 저장하는 클래스)
class ProcessClass {
private:
	int id;		// 프로세스 id
	double BurstTime;		// 작업 시간
	double RemaingTime;		// 남은 시간
	double ArrivalTime;		// 도착 시간
	double StopTime = 0;		// 최근 작업 시간
public:
	ProcessClass(int _id, double _BurstTime, double _ArrivalTime);		// 생성자
	void set_StopTime(double _time);
	void ExecutTime(double _time);
	int get_id();
	double get_BurstTime();
	double get_RemaingTime();
	double get_ArrivalTime();
	double get_StopTime();
};



//Save Scheduling Information (스케줄링 정보를 저장하는 클래스)
class RRScheduling {
private:
	double **ptrExecutionProcess;		// 실행시킨 프로세스를 가리키는 더블포인터
	double quantum;		// 타임 슬라이스 시간
	int executionIndex;		// 스케줄링 인덱스
	queue<int> readyQueue;		//준비 큐
	map<double, int> produceMap;
	int startIndex;		// 스케줄링 시작할 때 맨 처음 프로세스 아이디
public:
	//RRScheduling() {};
	RRScheduling(int _proccessMAXNumber);		// 생성자
	/*RRScheduling(int _proccessMAXNumber, double _quantum);
	~RRScheduling();*/
	void setQuantum(double);
	void setReadyQueue(ProcessClass **_pArray, int* _pArray_Index);
	void setScheduling(int _process_id, double _startTime, double _endTime, double _waitTime);
	void set_readyQueueBack(int _index);

	double * get_ExecutionProcess(int);
	int get_executionIndex();
	double get_quantum();
	int get_startIndex();
	bool is_produceMapEmpty();
	void updateReadyQueue(double _time);
	int get_readyQueueFront();
	//int get_MeanOfWaitingTime();
};



//Max proccess number
const int MAX = 100;



//Function (그 외 함수)
// 프로세스 정보 읽어오는 함수
void ReadProcessInfo(const char *FileName, ProcessClass **, int *, const int, RRScheduling *);
// 스케줄링 함수
void Scheduling(ProcessClass **proccesArrayAddress, int *, RRScheduling *Scheduling);
// 스케주링 실행결과를 cmd 창과 새로운 txt파일로 출력해주는 함수
void WirteExecutionResult(RRScheduling *, int*, const char *, const char *);
// 배열 보는 함수
void showCharArray(char *, int);
// 배열 초기화 함수
void resetCharArray(char *, int);



