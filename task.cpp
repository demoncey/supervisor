#include "Arduino.h"
#include "task.h"
#include "supervisor.h"


Task::Task(Callback callback):suspended(false),task_callback(callback),priority(P_LOW){
	ptr_value=(uint16_t)this;
	execution=0; 
}

Task::Task(Callback callback,bool suspend):suspended(suspend),task_callback(callback),priority(P_LOW){
	ptr_value=(uint16_t)this;
	execution=0;  
}

Task::Task():suspended(false),priority(P_LOW){
	ptr_value=(uint16_t)this;
	execution=0; 
}

Task::~Task(){
	this->suspend();
	this->task_callback=NULL;
	if(supervisor){
		supervisor->deleteTask(*this);
	}
};

void Task::execute(){
	task_callback();
}




