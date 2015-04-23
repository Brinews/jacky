#include "rct_task.h"
#include "rct_time.h"
#include <iostream>
#include <exception>

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

	todoMember = "";
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

string rct_task::getLeaderId()
{
	return leaderId;
}

string rct_task::getTaskName()
{
	return taskName;
}

void rct_task::showInfo()
{
	cout << "Task ID:" << taskId << endl;
	cout << "Task Name:" << taskName << endl;
	cout << "Task Create Date:" << taskCreateDate << endl;
	cout << "Task Leader:" << leaderId << endl;
	cout << "Task Type:" << type << endl;
	cout << "Task Completor:" << taskMemberId << endl;
	cout << "Task Complete Date:" << taskEndDate << endl;
	cout << "--------------------------------------------\n";
}

ostream & operator<<(ostream & out, const rct_task &p)
{
	out << p.type << endl;
	out << p.taskId << endl;
	out << p.taskName << endl;
	out << p.taskCreateDate << endl;
	out << p.leaderId << endl;

	if (p.taskMemberId == "")
		out << "null" << endl;
	else
		out << p.taskMemberId << endl;

	if (p.taskEndDate == "")
		out << "null" << endl;
	else
		out << p.taskEndDate << endl;

	if (p.todoMember == "")
		out << "null" << endl;
	else
		out << p.todoMember << endl;

	return out;
}

istream& operator>>(istream &in, rct_task &p)
{
	try {
		in >> p.type;
		in >> p.taskId;
		in >> p.taskName;
		in >> p.taskCreateDate;
		in >> p.leaderId;
		in >> p.taskMemberId;
		in >> p.taskEndDate;
		in >> p.todoMember;
	}
	catch (exception &e) {
		cout << e.what() << endl;
	}

	if (p.taskMemberId == "null") p.taskMemberId = "";
	if (p.taskEndDate == "null") p.taskEndDate = "";
	if (p.todoMember == "null") p.todoMember = "";

	return in;
}

bool rct_task::isPublished()
{
	return taskState == TS_PUBLISHED;
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


bool rct_normal_task::isVisible(string uid)
{
	return true;
}

bool rct_normal_task::isExecutable(string uid)
{
	return true;
}

void rct_normal_task::assignCompletor(string uid)
{
	//do nothing
}

bool rct_private_task::isVisible(string uid)
{
	if (uid == todoMember)
		return true;

	return false;
}

bool rct_private_task::isExecutable(string uid)
{
	if (uid == todoMember)
		return true;
	return false;
}

void rct_private_task::assignCompletor(string uid)
{
	todoMember = uid;
}

bool rct_assigned_task::isVisible(string uid)
{
	return true;
}

bool rct_assigned_task::isExecutable(string uid)
{
	if (uid == todoMember)
		return true;
	return false;
}

void rct_assigned_task::assignCompletor(string uid)
{
	todoMember = uid;
}
