#include "rct_task.h"
#include "rct_time.h"
#include <iostream>

using namespace std;

int rct_task::taskNo = 0;

rct_task::rct_task(string projId, string ldId, 
		string tName, string cDate) 
	: leaderId(ldId), taskName(tName), 
	  taskMemberId(""), taskEndDate(""), taskState(TS_PUBLISHED)
{
	char buf[8];
	taskNo = taskNo % 10000 + 1; //increase task number
	sprintf(buf, "%05d", taskNo);

	taskId = projId + "-" + buf;

	if (cDate == "")
		cDate = Time::nowDay();

	taskCreateDate = cDate;
}

/* 设置任务完成 */
void rct_task::setTaskComplete(string mid, string edate)
{
	taskMemberId = mid;
	taskEndDate = edate;

	taskState = TS_COMPLETED;
}

string rct_task::getCreateDate()
{
	return taskCreateDate;
}

string rct_task::getCompletor()
{
	return taskMemberId;
}

string rct_task::getTaskId()
{
	return taskId;
}

void rct_task::showInfo()
{
	cout << "Task ID:" << taskId << endl;
	cout << "Task Name:" << taskName << endl;
	cout << "Task Create Date:" << taskCreateDate << endl;
	cout << "Task Leader:" << leaderId << endl;
	cout << "Task Completor:" << taskMemberId << endl;
	cout << "Task Complete Date:" << taskEndDate << endl;
	cout << "--------------------------------------------\n";
}
/* 计算不同KPI */
double rct_normal_task::calTaskKPI()
{
	return 0.1 * Time::diffDays(taskCreateDate, taskEndDate);
}

double rct_private_task::calTaskKPI()
{
	return 0.15 * Time::diffDays(taskCreateDate, taskEndDate);
}

double rct_assigned_task::calTaskKPI()
{
	return 0.2 * Time::diffDays(taskCreateDate, taskEndDate);
}


