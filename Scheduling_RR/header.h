#pragma once
#include <string>
#include <queue>
#include <map>
using namespace std;



//Save Process Information (���μ��� ������ �����ϴ� Ŭ����)
class ProcessClass {
private:
	int id;		// ���μ��� id
	double BurstTime;		// �۾� �ð�
	double RemaingTime;		// ���� �ð�
	double ArrivalTime;		// ���� �ð�
	double StopTime = 0;		// �ֱ� �۾� �ð�
public:
	ProcessClass(int _id, double _BurstTime, double _ArrivalTime);		// ������
	void set_StopTime(double _time);
	void ExecutTime(double _time);
	int get_id();
	double get_BurstTime();
	double get_RemaingTime();
	double get_ArrivalTime();
	double get_StopTime();
};



//Save Scheduling Information (�����ٸ� ������ �����ϴ� Ŭ����)
class RRScheduling {
private:
	double **ptrExecutionProcess;		// �����Ų ���μ����� ����Ű�� ����������
	double quantum;		// Ÿ�� �����̽� �ð�
	int executionIndex;		// �����ٸ� �ε���
	queue<int> readyQueue;		//�غ� ť
	map<double, int> produceMap;
	int startIndex;		// �����ٸ� ������ �� �� ó�� ���μ��� ���̵�
public:
	//RRScheduling() {};
	RRScheduling(int _proccessMAXNumber);		// ������
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



//Function (�� �� �Լ�)
// ���μ��� ���� �о���� �Լ�
void ReadProcessInfo(const char *FileName, ProcessClass **, int *, const int, RRScheduling *);
// �����ٸ� �Լ�
void Scheduling(ProcessClass **proccesArrayAddress, int *, RRScheduling *Scheduling);
// �����ָ� �������� cmd â�� ���ο� txt���Ϸ� ������ִ� �Լ�
void WirteExecutionResult(RRScheduling *, int*, const char *, const char *);
// �迭 ���� �Լ�
void showCharArray(char *, int);
// �迭 �ʱ�ȭ �Լ�
void resetCharArray(char *, int);



