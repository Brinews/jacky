#ifndef _RCT_USER_H
#define _RCT_USER_H

#include <vector>
#include <string>

using namespace std;

// class declaration
class rct_project;
class rct_task;
class rct_user;

extern vector<rct_user*> allUsers;

class rct_user {
private:
	string id;
	string name;
	string password;
	int type; /* 0: leader, 1:member */

public:

	//带参数的构造函数
	rct_user(string i, string n, string p, int type);

	string getId();
	string getName();
	string getPassword();
	int getType();

	void setName(string n);
	void setPassword(string p);

	/* 验证用户ID和密码 是否匹配 */
	bool isValid(string i, string p);
	void showBaseInfo();
	vector<rct_task *> getTaskByName(string name);
	vector<rct_task *> getTaskByCreateDate(string date);

	virtual vector<rct_project *> getProjectList(string leaderId = "") = 0;
	virtual vector<rct_task *> getTaskList(string projId = "") = 0;

	virtual void completeTask(string taskId, string edate) = 0;

	virtual rct_project* createProject(string pojName) = 0;
	virtual rct_task* createTask(string projId, string taskname,
			string type, string uid) = 0;

	virtual void updateKPI(double nKpi) = 0;
	virtual void showInfo() = 0;
};

class rct_member : public rct_user {
private:
	int taskDoneNum;
	double KPI;
public:
	rct_member(string id, string name, string pwd);

	int getTaskDoneNum();
	double getKPI();
	rct_user* getProjectLeader(string lderId);

	/* 根据项目负责人条件返回项目列表 */
	vector<rct_project *> getProjectList(string leaderId = "");
	/* 根据项目条件返回任务列表 */
	vector<rct_task *> getTaskList(string projId = "");

	void completeTask(string taskId, string edate);

	rct_project* createProject(string pojName){ return NULL; }
	rct_task* createTask(string projId, string taskname,
			string type, string uid) { return NULL; }

	void updateKPI(double nKpi);
	void showInfo();
};

class rct_leader : public rct_user {
private:
	int projReleaseNum;
	int taskReleaseNum;
	double KPI;

public:
	rct_leader(string id, string name, string pwd);

	double getKPI();

	/* 根据项目负责人条件返回项目列表 */
	vector<rct_project *> getProjectList(string leaderId = ""){ 
		vector<rct_project *> null;
		return null;
	}
	/* 根据项目条件返回任务列表 */
	vector<rct_task *> getTaskList(string projId = "");

	void completeTask(string taskId, string edate){}

	/* 创建项目,ID缺省为系统生成 */
	rct_project* createProject(string pojName);
	/* 创建某项目下的任务, 任务编号缺省为系统生成 */
	rct_task* createTask(string projId, string taskname,
			string type, string uid);

	void updateKPI(double nKpi);
	void showInfo();
};


#endif
