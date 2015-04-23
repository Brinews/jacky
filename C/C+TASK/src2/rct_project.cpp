/*
 * project manager, 2015-04
 */
#include "rct_project.h"
#include "rct_task.h"
#include "rct_time.h"
#include <exception>

#include <iostream>

using namespace std;

int rct_project::projNo = 0;

rct_project::rct_project(string lderId, string pName, string cDate)
	: leaderId(lderId), projName(pName)
{
	if (cDate == "")
		cDate = Time::nowDay();

	cout << cDate << endl;

	projCreateDate = cDate;

	projNo = (projNo++)%10000;
	char buf[8];
	sprintf(buf, "%04d", projNo);

	projId = lderId + "-" + projCreateDate + "-" + buf;

	projTaskSet.clear(); //初始化
}

string rct_project::getLeaderId()
{
	return leaderId;
}

void rct_project::setCreateDate(string cdate)
{
	projCreateDate = cdate;
}

void rct_project::setLeaderId(string id)
{
	leaderId = id;
}

string rct_project::getProjectId()
{
	return projId;
}

vector<rct_task *> rct_project::getTaskList()
{
	return projTaskSet;
}

void rct_project::addTask(rct_task *task)
{
	projTaskSet.push_back(task);
}

vector<rct_task *> rct_project::scheduleTaskByDate()
{
	/* bubble sort */
	for (int i = 0; i < projTaskSet.size(); i++) {
		for (int j = 0; j < projTaskSet.size()-i-1; j++) {
			if (projTaskSet[j]->getCreateDate() > 
					projTaskSet[j+1]->getCreateDate()) {
				/* swap */
				rct_task *tmp = projTaskSet[j];
				projTaskSet[j] = projTaskSet[j+1];
				projTaskSet[j+1] = tmp;
			}
		}
	}

	return projTaskSet;
}

vector<rct_task *> rct_project::scheduleTaskByMember()
{
	/* partition: 将未完成与已完成任务分为前后两段 */
	int i = 0, j = projTaskSet.size()-1;

	rct_task *privot;

	while (i < j) {

		while (i < projTaskSet.size() && 
				projTaskSet[i]->getCompletor() != "")
			i++;

		while (j >= 0 &&
				projTaskSet[j]->getCompletor() == "")
			j--;

		if (i < j) {
			privot = projTaskSet[i];
			projTaskSet[i] = projTaskSet[j];
			projTaskSet[j] = privot;
		}
	}

	/* bubble sort: 对已完成任务进行排序 */
	for (int m = 0; m < j; m++) {
		for (int n = 0; n < j - m - 1; n++) {

			if (projTaskSet[j]->getCompletor() > 
					projTaskSet[j+1]->getCompletor()) {
				/* swap */
				rct_task *tmp = projTaskSet[j];
				projTaskSet[j] = projTaskSet[j+1];
				projTaskSet[j+1] = tmp;
			}
		}
	}

	return projTaskSet;
}

/*
 * member Id is a key
 */
vector<rct_task *> rct_project::findTaskByMember(string memberId)
{
	vector<rct_task *> memberTask;

	for (int i = 0; i < projTaskSet.size(); i++)
		if (projTaskSet[i]->getCompletor() == memberId)
			memberTask.push_back(projTaskSet[i]);

	return memberTask;
}

/*
 * search by name, do a pattern match
 */
vector<rct_task *> rct_project::findTaskByName(string name)
{
	vector<rct_task *> memberTask;

	for (int i = 0; i < projTaskSet.size(); i++)
		if (projTaskSet[i]->getTaskName().find(name, 0) != string::npos)
			memberTask.push_back(projTaskSet[i]);

	return memberTask;
}

vector<rct_task *> rct_project::findTaskByCreateDate(string date)
{
	vector<rct_task *> memberTask;

	for (int i = 0; i < projTaskSet.size(); i++)
		if (projTaskSet[i]->getCreateDate() == date)
			memberTask.push_back(projTaskSet[i]);

	return memberTask;
}

void rct_project::showInfo()
{
	cout << "Project ID:" << projId << endl;
	cout << "Project Name:" << projName << endl;
	cout << "Project Leader:" << leaderId << endl;
	cout << "Project Create Date:" << projCreateDate << endl;
	cout << "-----------------------------------------" << endl;
}

ostream & operator<<(ostream & out, const rct_project &p)
{
	out << p.projId << endl;
	out << p.projName << endl;
	out << p.leaderId << endl;
	out << p.projCreateDate << endl;

	return out;
}

istream& operator>>(istream &in, rct_project &p)
{
	try {
		in >> p.projId;
		in >> p.projName;
		in >> p.leaderId;
		in >> p.projCreateDate;
	} catch (exception &e) {
		cout << e.what() << endl;
	}

	return in;
}
