#pragma once
#include <string>
#include <queue>
#include <vector>
using namespace std;



/* 프로세스 정보를 저장하는 클래스*/
class ProcessClass {
private:
	int id;		// 프로세스 id
	double BurstTime;		// 작업 시간
	double RemaingTime;		// 남은 시간
	double ArrivalTime;		// 도착 시간
	double StopTime = 0;		// 최근 작업 중단 시간
public:
	// 클래스 내장 함수
	ProcessClass(int _id, double _BurstTime, double _ArrivalTime);		// 생성자
	void set_StopTime(double _time);				// 프로세스 최근 작업 중단 시간 set하는 함수 (파라미터 : 현재시간)
	void ExecutTime(double _time);		// 프로세스의 작업 잔여 시간 갱신하는 함수 (파라미터 : 작업한 시간)
	int get_id();		// 프로세스 아이디 get하는 함수 (return : id)
	double get_BurstTime();		// 프로세스 작업시간 get하는 함수 (return : BurstTime)
	double get_RemaingTime();		// 프로세스 잔여시간 get하는 함수 (return : RemaingTime)
	double get_ArrivalTime();		// 프로세스 도착시간 get하는 함수 (return : ArrivalTime)
	double get_StopTime();		// 프로세스 최근 작업 중단 시간 get하는 함수 (return : StopTime)
};



/* Round-Robin 스케줄링 정보를 저장하는 클래스*/
class RRScheduling {
private:
	double **ptrExecutionProcess;		// 실행시킨 프로세스를 가리키는 더블포인터 (포인터가 가리키는 배열에 스케줄링 정보 저장)
	double quantum;		// 타임 슬라이스 시간
	int executionIndex;		// 스케줄링 인덱스
	queue<int> readyQueue;		//준비 큐
	// 생성된 프로세스들을 준비 큐에 도착하기 전 도착시간 순으로 정렬하여 담는 벡터
	vector<pair<double, int>> produceVec;		// <도착시간, 프로세스 아이디>
	int startIndex;		// 스케줄링 시작할 때 맨 처음 프로세스 아이디
public:
	// 클래스 내장 함수
	RRScheduling(int _proccessMAXNumber);		// 생성자
	void set_Quantum(double __quantum);		// 타임 슬라이스를 set하는 함수 (파라미터 : quantum)
	// 프로세스 실행시켜 스케줄링 정보를 클래스 객체에 저장하는 함수
	// 파라미터 : (프로세스 아이디, 시작시간, 종료시간, 해당 프로세스의 대기시간)
	void set_Scheduling(int _process_id, double _startTime, double _endTime, double _waitTime);

	// 스케줄링 시작 전, 준비 큐 대기시키는(set) 함수
	// 파라미터 : (프로세스정보 객체, 프로세스 개수)
	void set_ReadyQueue(ProcessClass **_pArray, int* _pArray_Index);

	void set_readyQueueBack(int _index);		// 프로세스가 작업을 완료하지 못하였을 경우, 다시 준비 큐의 back으로 push하는 함수 (파라미터 : 프로세스 인덱스)

	// 특정 스케줄링 인덱스(_index)의 스케줄링 정보 get하는 함수 (return : ptrExecutionProcess[_index])
	/* 사용 예시)
		get_ExecutionProcess(i)[0]; : i번째 스케줄링 인덱스의 프로세스 아이디 get
		get_ExecutionProcess(i)[1]; : i번째 스케줄링 인덱스의 스케줄링 시작시간 get
		get_ExecutionProcess(i)[2]; : i번째 스케줄링 인덱스의 스케줄링 종료시간 get
		get_ExecutionProcess(i)[3]; : i번째 스케줄링 인덱스의 해당 프로세스 대기시간 get
	*/
	double * get_ExecutionProcess(int _index);

	double get_quantum();		// 타임 슬라이스를 get하는 함수 (return : quantom)
	int get_executionIndex();		// 스케줄링 횟수를 get하는 함수 (return : executionIndex)
	int get_startIndex();		// 스케줄링 시작할 때 맨 처음 실행시키는 프로세스 아이디 get하는 함수 (return : startIndex)
	int get_readyQueueFront();		// 준비 큐의 맨 앞에서 기다리는 프로세스(front) get하고 pop해주는 함수 (return : readyQueue.front())
	bool is_produceVecEmpty();		// 생성 벡터가 비어있는지 boolean으로 리턴해주는 함수 (return : produceMap.empty())
	void update_ReadyQueue(double _time);		// 프로세스들의 도착시간에 따라 도착했다면, 준비 큐에 push해주는 함수 (파라미터 : 현재시간)
};






/* 그 외 함수*/
// txt파일을 불러와 프로세스 정보 읽어오는 함수
// 파라미터 : (READ용 txt파일 이름, 프로세스정보 객체, 총 프로세스 개수, 생성 가능한 프로세스 최대 개수, 스케줄링 정보 객체)
void ReadProcessInfo(const char *_FileName, ProcessClass **_processArray, int *_proceeArray_Index, const int MAX_PROCESS_NUMBER, RRScheduling *_RRSch);

// 스케줄링 함수
// 파라미터 : (프로세스정보 객체, 프로세스 개수, 스케줄링정보 객체)
void Scheduling(ProcessClass **_processArray, int*_processArray_Index, RRScheduling *_RRSch);

// 스케줄링 실행결과를 cmd 창과 새로운 txt파일로 출력해주는 함수
// 파라미터 : (스케줄링정보 객체, 총 프로세스 개수, READ용 txt파일 이름, WRITE용 txt파일 이름)
void WriteExecutionResult(RRScheduling *_RRSch, int* _processArray_Index, const char *_READ_FILE_NAME, const char *_WRITE_FILE_NAME);

// 배열 출력 함수 (개발 시 확인용)
void showCharArray(char *, int);
// 배열 초기화 함수
void resetCharArray(char *, int);



