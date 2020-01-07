#include "Arduino.h"
#include "task.h"
#include "supervisor.h"


Task::Task(Callback callback):suspended(false),task_callback(callback),priority(P_LOW),name("unknown"),execution(0){
}

Task::Task(Callback callback,bool suspend):suspended(suspend),task_callback(callback),priority(P_LOW),name("unknown"),execution(0){
}

Task::Task():suspended(false),priority(P_LOW),name("unknown"),execution(0){
}

Task::~Task(){
	this->suspend();
	this->task_callback = nullptr;
	if(supervisor){
		supervisor->deleteTask(*this);
	}
};

void Task::execute(){
	task_callback(nullptr);
}


Task * Task::setPriority(uint8_t priority){
	this->priority = priority;
	return this;
};




