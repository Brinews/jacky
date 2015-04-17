#ifndef _RCT_PROJECT_H
#define _RCT_PROJECT_H

#include <vector>
#include <string>

using namespace std;

class rct_task;

class rct_project {
private:
	string projId;
	string leaderId;
	string projCreateDate; /* 2011-11-02 ��ʽ, Ĭ��Ϊ���� */
	string projName;

	vector<rct_task *> projTaskSet;
	static int projNo;

public:

	/* ��ĿID=leaderId + �������� +  �������к�(ÿ�����10000����Ŀ) */
	rct_project(string lderId, string pName, string cDate = "");

	void addTask(rct_task *task);

	/* �������񴴽��������� */
	vector<rct_task *> scheduleTaskByDate();
	/* ����������������� */
	vector<rct_task *> scheduleTaskByMember();

	/* ��ѡĳ��Ա������� */
	vector<rct_task *> findTaskByMember(string memberId);

	string getLeaderId();
	string getProjectId();
	vector<rct_task *> getTaskList();

	void showInfo();
};

#endif
