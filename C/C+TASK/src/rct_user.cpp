/*
 * user entity, 2015.04
 */

#include "rct_user.h"
#include "rct_global.h"

#include "rct_project.h"
#include "rct_task.h"

#include <iostream>

using namespace std;

rct_user::rct_user(string i, string n, string p, int t)
{
	id = i;
	name = n;
	password = p;
	type = t;
}

/* get propertity method */
string rct_user::getId()
{
	return id;
}

string rct_user::getName()
{
	return name;
}

string rct_user::getPassword()
{
	return password;
}

int rct_user::getType()
{
	return type;
}

/* set propertity method */
void rct_user::setName(string n)
{
	name = n;
}

void rct_user::setPassword(string p)
{
	password = p;
}

/* valid method: for login */
bool rct_user::isValid(string i, string p)
{
	if (i == id && p == password) return true;

	return false;
}

void rct_user::showBaseInfo()
{
	cout << "ID.....:" << id << endl;
	cout << "Name...:" << name << endl;
}

/*****************project leader class ******************/
rct_leader::rct_leader(string id, string name, string pwd) 
	: rct_user(id, name, pwd, 0), projReleaseNum(0), taskReleaseNum(0), KPI(0.0)
{
}

/* 发布项目 */
rct_project* rct_leader::createProject(string pojName)
{
	rct_project *proj = new rct_project(getId(), pojName);

	allProjects.push_back(proj);
	projReleaseNum++;

	return proj;
}

/* 创建任务 */
rct_task* rct_leader::createTask(string projId, string taskname)
{
	rct_task * task = new rct_normal_task(projId, getId(), taskname);

	for (int i = 0; i < allProjects.size(); i++)
		if (allProjects[i]->getProjectId() == projId) {
			allProjects[i]->addTask(task);
			taskReleaseNum++;
		}

	return task;
}

double rct_leader::getKPI()
{
	return KPI;
}

void rct_leader::showInfo()
{
	//rct_user::showInfo();
	showBaseInfo();

	cout << "Release Project Num.:" << projReleaseNum << endl;
	cout << "Release Task Num....:" << taskReleaseNum << endl;
	cout << "KPI.................:" << KPI << endl;
}

void rct_leader::updateKPI(double nKpi)
{
	KPI += nKpi;
}


/*****************project member class ******************/
rct_member::rct_member(string id, string name, string pwd)
	: rct_user(id, name, pwd, 1), taskDoneNum(0), KPI(0.0)
{
}

int rct_member::getTaskDoneNum()
{
	return taskDoneNum;
}

double rct_member::getKPI()
{
	return KPI;
}

vector<rct_project *> rct_member::getProjectList(string leaderId)
{
	vector<rct_project *> projs;

	for (int i = 0; i < allProjects.size(); i++)
		if (leaderId == "" || allProjects[i]->getLeaderId() == leaderId)
			projs.push_back(allProjects[i]);

	return projs;
}

vector<rct_task *> rct_member::getTaskList(string projId)
{
	vector<rct_task *> null;

	for (int i = 0; i < allProjects.size(); i++)
		if (allProjects[i]->getProjectId() == projId) {
			/* 先排序 */
			allProjects[i]->scheduleTaskByMember();

			return allProjects[i]->getTaskList();
		}

	return null;
}


void rct_member::updateKPI(double nKpi)
{
	KPI += nKpi;
}

/* 根据项目主管ID找到相应对象 */
rct_user *rct_member::getProjectLeader(string lderId)
{
	rct_user *ret = NULL;

	for (int i = 0; i < allUsers.size(); i++)
		if (allUsers[i]->getId() == lderId) {
			ret = allUsers[i];
			break;
		}

	return ret;
}

/* 完成任务:更新KPI */
void rct_member::completeTask(string taskId, string edate)
{
	for (int i = 0; i < allProjects.size(); i++) {
		if (taskId.find(allProjects[i]->getProjectId(), 0) == 0) {
			vector<rct_task *> tList = allProjects[i]->getTaskList();

			for (int j = 0; j < tList.size(); j++) {
				if (tList[j]->getTaskId() == taskId) {
					// find the task
					taskDoneNum++;

					tList[j]->setTaskComplete(this->getId(), edate);
					double nKpi = tList[j]->calTaskKPI();
					this->updateKPI(nKpi);

					rct_user *leader = getProjectLeader(
									allProjects[i]->getLeaderId());

					if (leader != NULL)
						leader->updateKPI(nKpi*0.13);

					break;
				}
			}
		}
	}
}

void rct_member::showInfo()
{
	showBaseInfo();

	cout << "Task Done Num.:" << taskDoneNum << endl;
	cout << "KPI...........:" << KPI << endl;
}
