#include "rct_global.h"

#include "rct_user.h"
#include "rct_project.h"
#include "rct_task.h"

#include <iostream>

vector<rct_project *> allProjects;
vector<rct_user *> allUsers;

void rct_process_cmd(rct_user *user);
void process_leader(rct_user *user);
void process_member(rct_user *user);

void initUsers();


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

int main(int argc, char **argv)
{
	allProjects.clear();
	allUsers.clear();

	/* users 初始化 */
	initUsers();

	int opt;
	string id, password;

	while (1) {
		cout << "1. Login;\n 2. Quit" << endl;
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

void process_leader(rct_user *user)
{
	char opt;
	string pojId, tskName, pojName;
	rct_project *poj;
	rct_task *task;

	while (1) {
		cout << "1.create project.\n2.create task.\n3.infomation show\n0.exit\n";
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
				task = user->createTask(pojId, tskName);
				if (NULL != task) {
					cout << "TaskID:" << task->getTaskId() << endl;
				}
				break;
			case '3':
				user->showInfo();
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

	vector<rct_task *> tskList;
	vector<rct_project *> pojList;

	while (1) {
		cout << "1.Projects list.\n2.Tasks list.\n3.Task Complete\n4.Show Information\n0.Exit\n";
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
			case '0':
			deafult:
				return;

		}
	}
}
