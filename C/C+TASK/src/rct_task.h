/*
 * task item: 2015-04
 */

#ifndef _RCT_TASK_H
#define _RCT_TASK_H

#include <string>

using namespace std;


enum TaskState {
	TS_PUBLISHED,
	TS_COMPLETED
};

class rct_task {
protected:
	string taskId;
	string leaderId;
	string taskCreateDate; /* YYYY-MM-DD ��ʽ, ȱʡΪ���� */
	string taskName;

	string taskMemberId; //�����ID
	string taskEndDate;  //�������

	TaskState taskState; //����״̬ 

	static int taskNo;
	
public:
	/* taskId = projId + ������� */
	rct_task(string projId, string ldId, string tName, string cDate = "");

	virtual double calTaskKPI() = 0;
	void setTaskComplete(string mid, string edate);

	string getCreateDate();
	string getCompletor();
	string getTaskId();

	void showInfo();
};

class rct_normal_task : public rct_task {
public:
	rct_normal_task(string projId, string ldId, string tName,
			string cDate = "") : rct_task(projId, ldId, tName, cDate)
	{
	}

	virtual double calTaskKPI();
};

class rct_private_task : public rct_task {
public:
	rct_private_task(string projId, string ldId, string tName,
			string cDate = "") : rct_task(projId, ldId, tName, cDate)
	{
	}

	virtual double calTaskKPI();
};

class rct_assigned_task : public rct_task {
public:
	rct_assigned_task(string projId, string ldId, string tName,
			string cDate = "") : rct_task(projId, ldId, tName, cDate)
	{
	}

	virtual double calTaskKPI();
};

#endif

