#include "Arduino.h"
#include "task.h"
#include "supervisor.h"


Task::Task(String name,Callback callback):suspended(false),task_callback(callback),priority(P_LOW),taskName(name),suspendable(false),infinity(true){
}

Task::~Task(){
	suspend();
	task_callback = nullptr;
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
	infinity = false;
	return *this;
};

Task& Task::makeSuspendtable(){
	suspendable = true;
	return *this;
};
void Task::setSupervisor(Supervisor &supervisor){
	this->supervisor = &supervisor;
};
const Supervisor* Task::getSupervisor(){
	return supervisor;
};




