#include "Arduino.h"
#include "supervisor.h"


Supervisor::Supervisor(String name)
{
	this->name = name;
	first = nullptr;
	last = nullptr;
}


void Supervisor::addTask(Task& task){
	if(task.getSupervisor() == this){
		SERIAL_LOGGER1("trying to add same task again, interuppted");
		return;
	}
	if( first == nullptr){
		SERIAL_LOGGER1("task first is null");
		first=&task;
		task.before = nullptr;
	}else{
		last->after = &task;
		task.before = last;
	}
	task.setSupervisor(*this);
	task.after=nullptr;
	last = &task;
	SERIAL_LOGGER(task.taskName,GET_HEX_PTR(&task)," added");
}

void Supervisor::deleteTask(Task& task){
	if(&task == first){
		first = task.after;
		task.before = nullptr;
		return;
	}
	if(&task == last){
		last = task.before;
		last->after = nullptr;
		return;
	}
	task.before->after = task.after;
	SERIAL_LOGGER(task.taskName,GET_HEX_PTR(&task)," removed");
}


void Supervisor::execute(){
	Task* current = first;
	Task* next = nullptr;
	uint8_t executed = 0;
	uint8_t killed = 0;
	while(current){
		next = current->after;
		if(current->suspended == false){
			current->execute();
			SERIAL_LOGGER(current->taskName,GET_HEX_PTR(current)," executed in chain");
			if(current->execution == MODE_ONCE){
				//inside deconstructor Task pointer is removed from List
				SERIAL_LOGGER(current->taskName,GET_HEX_PTR(current)," will be deleted");
				delete(current);
				//if stack use method below instead delete
				//deleteTask(*current);
				killed++;
			}
		executed++;
		}else{
			SERIAL_LOGGER(current->taskName,GET_HEX_PTR(current)," is suspend");
		}
		current = next;
		delay(500);
	}
	SERIAL_LOGGER1("****************************");
	SERIAL_LOGGER1("Cycle finished, reseting");
	SERIAL_LOGGER1("****************************");
}

void Supervisor::suspendAll(){
	Task *current = first;
	while(current){
		if(current->priority != P_HIGH && !(current->suspended)){
			current->suspend();
		}
		current = current->after;
	}
	SERIAL_LOGGER1("suspendAll finished");
}


void Supervisor::resumeAll(){
	Task *current = first;
	while(current){
		if(current->suspended){
			current->resume();
		}
		current = current->after;
	}
	SERIAL_LOGGER1("resumedAll finished");
	
}




