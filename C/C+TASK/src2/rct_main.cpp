/*
 * 2015-05, main control
 */
#include "rct_global.h"

#include "rct_user.h"
#include "rct_project.h"
#include "rct_task.h"

#include <iostream>
#include <fstream>

#include <iterator>

using namespace std;

vector<rct_project *> allProjects;
vector<rct_user *> allUsers;

void rct_process_cmd(rct_user *user);
void process_leader(rct_user *user);
void process_member(rct_user *user);

void initUsers();
void loadProjects(const char *fileName);
void saveProjects(const char *fileName);

void displayTaskList(vector<rct_task *> list);

void initUsers()
{
	rct_user *u;

	string id = "RCT";
	string name = "ACTOR";
	string pwd = "PWD";

	char buf[8];

	/* 六个账户:三个负责人,三个组员 */
	for (int i = 0; i < 6; i++) {
		sprintf(buf, "%02d", i);
		if (i%2) {
			u = new rct_leader(id+buf, name+buf, pwd+buf);
		}
		else {
			u = new rct_member(id+buf, name+buf, pwd+buf);
		}

		allUsers.push_back(u);
	}
}

void loadProjects(const char *fileName)
{
	ifstream fin(fileName);

	if (!fin) {
		cout << "Open file failed." << endl;
		return;
	}
	/*
	 * poj文件格式:
	 * ------------
	 * poj number
	 * id name leader create date
	 * ------------
	 * task number
	 * type id name create-date leader completor complete-date todomember
	 */
	int number, i;
	string type;

	fin >> number;
	for (int i = 0; i < number; i++) {
		rct_project *proj = new rct_project("", "");

		fin >> *proj;

		allProjects.push_back(proj);
	}

	fin >> number;
	for (int i = 0; i < number; i++) {
		rct_task *task;

		fin >> type;

		if (type == "normal") task = new rct_normal_task("","","","");
		if (type == "private") task = new rct_private_task("","","","");
		if (type == "assign") task = new rct_assigned_task("","","","");

		fin >> *task;

		for (int j = 0; j < allProjects.size(); j++) {
			if (task->getTaskId().
					find(allProjects[j]->getProjectId(), 0) == 0)
				allProjects[j]->addTask(task);
		}
	}

	fin.close();
}

void saveProjects(const char *fileName)
{
	ofstream fout(fileName);

	int taskNum = 0;

	fout << allProjects.size() << endl;
	for (int i = 0; i < allProjects.size(); i++) {
		fout << *allProjects[i] << endl;
		taskNum += allProjects[i]->getTaskList().size();
	}

	fout << taskNum << endl;

	for (int i = 0; i < allProjects.size(); i++) {
		vector<rct_task *> list = allProjects[i]->getTaskList();

		for (int j = 0; j < list.size(); j++)
			fout << *list[j] << endl;
	}

	fout.close();
}

int main(int argc, char **argv)
{
	allProjects.clear();
	allUsers.clear();

	/* users 初始化 */
	initUsers();

	/* 加载 项目-任务 文件*/
	loadProjects("poj.txt");

	int opt;
	string id, password;

	while (1) {
		cout << " 1. Login;\n 2. Quit" << endl;
		cin >> opt;

		if (opt == 2) break;
		if (opt == 1) {
			rct_user *user = NULL;

			cout << "Input userID:";
			cin >> id;
			cout << "Input password:";
			cin >> password;

			for (int i = 0; i < allUsers.size(); i++) {
				if (allUsers[i]->isValid(id, password))
					user = allUsers[i];
			}

			if (NULL != user) {
				rct_process_cmd(user);
			}
		}
	}

	/* 保存项目信息 */
	saveProjects("poj.txt");

	return 0;
}

void rct_process_cmd(rct_user *user)
{
	if (user->getType() == 0) {
		process_leader(user);
	}
	else {
		process_member(user);
	}
}

void displayTaskList(vector<rct_task *> list)
{
	for (int i = 0; i < list.size(); i++)
		list[i]->showInfo();
}

void process_leader(rct_user *user)
{
	char opt;
	string pojId, tskName, pojName;
	rct_project *poj;
	rct_task *task;
	string cdate;

	string type, todoMember;

	vector<rct_task *> tskList;

	while (1) {
		cout << "1.create project.\n2.create task.\n3.infomation show\n4.Search task by name.\n5.Search task by create date.\n6.View all tasks\n0.exit\n";
		cin >> opt;

		switch (opt) {
			case '1':
				cout << "Input Project Name:";
				cin >> pojName;
				poj = user->createProject(pojName);
				if (poj != NULL) {
					cout << "ProjectID:" << poj->getProjectId() << endl;
					cout << "-ok, project create success.\n";
				}
				break;
			case '2':
				cout << "Input Project ID:";
				cin >> pojId;
				cout << "Input Task Name:";
				cin >> tskName;
				cout << "Input task type(normal, private, assign):";
				cin >> type; 
				if (type == "private" || type == "assign") {
					cout << "Input the User ID to complete it:";
					cin >> todoMember;
				}
				task = user->createTask(pojId, tskName, 
						type, todoMember);
				if (NULL != task) {
					cout << "TaskID:" << task->getTaskId() << endl;
				}
				break;
			case '3':
				user->showInfo();
				break;
			case '4':
				cout << "Input Task Name:" ;
				cin >> tskName;
				tskList = user->getTaskByName(tskName);
				/*
				copy(tskList.begin(), tskList.end(),
						ostream_iterator<rct_task *>(cout, "\n"));
				*/
				displayTaskList(tskList);
				break;
			case '5':
				cout << "Input Task Create Date:" ;
				cin >> cdate;
				tskList = user->getTaskByCreateDate(cdate);
				/*
				copy(tskList.begin(), tskList.end(),
						ostream_iterator<rct_task *>(cout, "\n"));
				*/
				displayTaskList(tskList);

				break;
			case '6':
				tskList = user->getTaskList();
				displayTaskList(tskList);
				break;

			case '0':
			default:
				return;
		}
	}
}

void process_member(rct_user *user)
{
	char opt;

	string pojId, taskId, edate;
	string tskName, tskDate;

	vector<rct_task *> tskList;
	vector<rct_project *> pojList;

	while (1) {
		cout << "1.Projects list.\n2.Tasks list.\n3.Task Complete\n4.Show Information\n5.Search Task by name.\n6.Search task by create date.\n0.Exit\n";
		cin >> opt;

		switch (opt) {
			case '1':
				pojList = user->getProjectList("");
				for (int i = 0; i < pojList.size(); i++)
					pojList[i]->showInfo();
				break;

			case '2':
				cout << "Input Project Id:";
				cin >> pojId;
				tskList = user->getTaskList(pojId);
				for (int i = 0; i < tskList.size(); i++)
					tskList[i]->showInfo();

				break;

			case '3':
				cout << "Input Task Id:";
				cin >> taskId;
				cout << "Input Complete Date:(YYYY-MM-DD)";
				cin >> edate;

				user->completeTask(taskId, edate);
				break;
			case '4':
				user->showInfo();
				break;
			case '5':
				cout << "Input Task Name:";
				cin >> tskName;
				tskList = user->getTaskByName(tskName);
				displayTaskList(tskList);
				break;
			case '6':
				cout << "Input task create date:";
				cin >> tskDate;
				tskList = user->getTaskByCreateDate(tskDate);
				displayTaskList(tskList);
				break;
			case '0':
			deafult:
				return;

		}
	}
}
