#ifndef _RCT_PROJECT_H
#define _RCT_PROJECT_H

#include <vector>
#include <string>
#include <iostream>

using namespace std;

class rct_task;

class rct_project {
private:
	string projId;
	string leaderId;
	string projCreateDate; /* 2011-11-02 格式, 默认为当天 */
	string projName;

	vector<rct_task *> projTaskSet;
	static int projNo;

public:

	/* 项目ID=leaderId + 当天日期 +  递增序列号(每天最多10000个项目) */
	rct_project(string lderId, string pName, string cDate = "");

	void setLeaderId(string id);
	void setCreateDate(string cdate);

	void addTask(rct_task *task);

	/* 按照任务创建日期排序 */
	vector<rct_task *> scheduleTaskByDate();
	/* 按照任务完成人排序 */
	vector<rct_task *> scheduleTaskByMember();

	/* 挑选某组员完成任务 */
	vector<rct_task *> findTaskByMember(string memberId);
	vector<rct_task *> findTaskByName(string name);
	vector<rct_task *> findTaskByCreateDate(string date);

	string getLeaderId();
	string getProjectId();
	vector<rct_task *> getTaskList();

	void showInfo();

	friend ostream& operator<<(ostream &,const rct_project&);
	friend istream& operator>>(istream &, rct_project&);
};

#endif
