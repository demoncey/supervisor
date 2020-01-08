#include "Arduino.h"
#include "task.h"
#include "supervisor.h"


Task::Task(String name,Callback callback):suspended(false),task_callback(callback),priority(P_LOW),taskName(name),execution(0){
}

Task::~Task(){
	this->suspend();
	this->task_callback = nullptr;
	if(supervisor){
		supervisor->deleteTask(*this);
	}
};

void Task::execute(){
	task_callback(nullptr);//nullptr is args
}


Task& Task::setPriority(uint8_t priority){
	this->priority = priority;
	return *this;
};

Task& Task::makeOnce(){
	this->execution = 1;
	return *this;
};




