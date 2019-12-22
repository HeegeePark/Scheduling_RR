#pragma once
#include <string>
#include <queue>
#include <vector>
using namespace std;



/* ���μ��� ������ �����ϴ� Ŭ����*/
class ProcessClass {
private:
	int id;		// ���μ��� id
	double BurstTime;		// �۾� �ð�
	double RemaingTime;		// ���� �ð�
	double ArrivalTime;		// ���� �ð�
	double StopTime = 0;		// �ֱ� �۾� �ߴ� �ð�
public:
	// Ŭ���� ���� �Լ�
	ProcessClass(int _id, double _BurstTime, double _ArrivalTime);		// ������
	void set_StopTime(double _time);				// ���μ��� �ֱ� �۾� �ߴ� �ð� set�ϴ� �Լ� (�Ķ���� : ����ð�)
	void ExecutTime(double _time);		// ���μ����� �۾� �ܿ� �ð� �����ϴ� �Լ� (�Ķ���� : �۾��� �ð�)
	int get_id();		// ���μ��� ���̵� get�ϴ� �Լ� (return : id)
	double get_BurstTime();		// ���μ��� �۾��ð� get�ϴ� �Լ� (return : BurstTime)
	double get_RemaingTime();		// ���μ��� �ܿ��ð� get�ϴ� �Լ� (return : RemaingTime)
	double get_ArrivalTime();		// ���μ��� �����ð� get�ϴ� �Լ� (return : ArrivalTime)
	double get_StopTime();		// ���μ��� �ֱ� �۾� �ߴ� �ð� get�ϴ� �Լ� (return : StopTime)
};



/* Round-Robin �����ٸ� ������ �����ϴ� Ŭ����*/
class RRScheduling {
private:
	double **ptrExecutionProcess;		// �����Ų ���μ����� ����Ű�� ���������� (�����Ͱ� ����Ű�� �迭�� �����ٸ� ���� ����)
	double quantum;		// Ÿ�� �����̽� �ð�
	int executionIndex;		// �����ٸ� �ε���
	queue<int> readyQueue;		//�غ� ť
	// ������ ���μ������� �غ� ť�� �����ϱ� �� �����ð� ������ �����Ͽ� ��� ����
	vector<pair<double, int>> produceVec;		// <�����ð�, ���μ��� ���̵�>
	int startIndex;		// �����ٸ� ������ �� �� ó�� ���μ��� ���̵�
public:
	// Ŭ���� ���� �Լ�
	RRScheduling(int _proccessMAXNumber);		// ������
	void set_Quantum(double __quantum);		// Ÿ�� �����̽��� set�ϴ� �Լ� (�Ķ���� : quantum)
	// ���μ��� ������� �����ٸ� ������ Ŭ���� ��ü�� �����ϴ� �Լ�
	// �Ķ���� : (���μ��� ���̵�, ���۽ð�, ����ð�, �ش� ���μ����� ���ð�)
	void set_Scheduling(int _process_id, double _startTime, double _endTime, double _waitTime);

	// �����ٸ� ���� ��, �غ� ť ����Ű��(set) �Լ�
	// �Ķ���� : (���μ������� ��ü, ���μ��� ����)
	void set_ReadyQueue(ProcessClass **_pArray, int* _pArray_Index);

	void set_readyQueueBack(int _index);		// ���μ����� �۾��� �Ϸ����� ���Ͽ��� ���, �ٽ� �غ� ť�� back���� push�ϴ� �Լ� (�Ķ���� : ���μ��� �ε���)

	// Ư�� �����ٸ� �ε���(_index)�� �����ٸ� ���� get�ϴ� �Լ� (return : ptrExecutionProcess[_index])
	/* ��� ����)
		get_ExecutionProcess(i)[0]; : i��° �����ٸ� �ε����� ���μ��� ���̵� get
		get_ExecutionProcess(i)[1]; : i��° �����ٸ� �ε����� �����ٸ� ���۽ð� get
		get_ExecutionProcess(i)[2]; : i��° �����ٸ� �ε����� �����ٸ� ����ð� get
		get_ExecutionProcess(i)[3]; : i��° �����ٸ� �ε����� �ش� ���μ��� ���ð� get
	*/
	double * get_ExecutionProcess(int _index);

	double get_quantum();		// Ÿ�� �����̽��� get�ϴ� �Լ� (return : quantom)
	int get_executionIndex();		// �����ٸ� Ƚ���� get�ϴ� �Լ� (return : executionIndex)
	int get_startIndex();		// �����ٸ� ������ �� �� ó�� �����Ű�� ���μ��� ���̵� get�ϴ� �Լ� (return : startIndex)
	int get_readyQueueFront();		// �غ� ť�� �� �տ��� ��ٸ��� ���μ���(front) get�ϰ� pop���ִ� �Լ� (return : readyQueue.front())
	bool is_produceVecEmpty();		// ���� ���Ͱ� ����ִ��� boolean���� �������ִ� �Լ� (return : produceMap.empty())
	void update_ReadyQueue(double _time);		// ���μ������� �����ð��� ���� �����ߴٸ�, �غ� ť�� push���ִ� �Լ� (�Ķ���� : ����ð�)
};






/* �� �� �Լ�*/
// txt������ �ҷ��� ���μ��� ���� �о���� �Լ�
// �Ķ���� : (READ�� txt���� �̸�, ���μ������� ��ü, �� ���μ��� ����, ���� ������ ���μ��� �ִ� ����, �����ٸ� ���� ��ü)
void ReadProcessInfo(const char *_FileName, ProcessClass **_processArray, int *_proceeArray_Index, const int MAX_PROCESS_NUMBER, RRScheduling *_RRSch);

// �����ٸ� �Լ�
// �Ķ���� : (���μ������� ��ü, ���μ��� ����, �����ٸ����� ��ü)
void Scheduling(ProcessClass **_processArray, int*_processArray_Index, RRScheduling *_RRSch);

// �����ٸ� �������� cmd â�� ���ο� txt���Ϸ� ������ִ� �Լ�
// �Ķ���� : (�����ٸ����� ��ü, �� ���μ��� ����, READ�� txt���� �̸�, WRITE�� txt���� �̸�)
void WriteExecutionResult(RRScheduling *_RRSch, int* _processArray_Index, const char *_READ_FILE_NAME, const char *_WRITE_FILE_NAME);

// �迭 ��� �Լ� (���� �� Ȯ�ο�)
void showCharArray(char *, int);
// �迭 �ʱ�ȭ �Լ�
void resetCharArray(char *, int);



