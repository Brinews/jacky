/*
 * task item: 2015-04
 */

#ifndef _RCT_TASK_H
#define _RCT_TASK_H

#include <string>
#include <iostream>

using namespace std;


enum TaskState {
	TS_PUBLISHED,
	TS_COMPLETED
};

class rct_task {
protected:
	string taskId;
	string leaderId;
	string taskCreateDate; /* YYYY-MM-DD 格式, 缺省为当天 */
	string taskName;

	string taskMemberId; //完成者ID
	string taskEndDate;  //完成日期

	TaskState taskState; //任务状态 

	static int taskNo;
	
public:
	string type;		//normal, private, assign
	string todoMember;	 //指定完成者
	/* taskId = projId + 递增序号 */
	rct_task(string projId, string ldId, string tName, string cDate = "");

	virtual double calTaskKPI() = 0;
	void setTaskComplete(string mid, string edate);

	string getCreateDate();
	string getCompletor();
	string getTaskId();
	string getTaskName();
	string getLeaderId();

	bool isPublished();

	void showInfo();

	virtual bool isVisible(string uid) = 0;
	virtual bool isExecutable(string uid) = 0;

	virtual void assignCompletor(string uid) = 0;

	friend ostream& operator<<(ostream &, const rct_task&);
	friend istream& operator>>(istream &, rct_task &);
};

class rct_normal_task : public rct_task {
public:
	rct_normal_task(string projId, string ldId, string tName,
			string cDate = "") : rct_task(projId, ldId, tName, cDate)
	{
		type = "normal";
	}

	bool isVisible(string uid);
	bool isExecutable(string uid);

	void assignCompletor(string uid);

	virtual double calTaskKPI();
};

class rct_private_task : public rct_task {
public:
	rct_private_task(string projId, string ldId, string tName,
			string cDate = "") : rct_task(projId, ldId, tName, cDate)
	{
		type = "private";
	}

	bool isVisible(string uid);
	bool isExecutable(string uid);

	void assignCompletor(string uid);

	virtual double calTaskKPI();
};

class rct_assigned_task : public rct_task {
public:
	rct_assigned_task(string projId, string ldId, string tName,
			string cDate = "") : rct_task(projId, ldId, tName, cDate)
	{
		type = "assign";
	}

	bool isVisible(string uid);
	bool isExecutable(string uid);

	void assignCompletor(string uid);

	virtual double calTaskKPI();
};

#endif

