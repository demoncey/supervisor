#include "Arduino.h"
#include "supervisor.h"

Supervisor::Supervisor(String name)
{
	this->name = name;
	first = nullptr;
	last = nullptr;
	SERIAL_LOGGER1("CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC");
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
	
	if(task.priority == P_HIGH){
		addQueue(task,hFirst,hLast);
	}
	if(task.priority == P_LOW){
			addQueue(task,lFirst,lLast);
	}	
	SERIAL_LOGGER(task.taskName,GET_HEX_PTR(&task)," added");
}

void Supervisor::addQueue(Task& task,Task*& last,Task*& first){
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
	
	
	if(task.priority == P_HIGH){
		deleteQueue(task,hFirst,hLast);
	}
	if(task.priority == P_LOW){
		deleteQueue(task,lFirst,lLast);
	}	
	SERIAL_LOGGER(task.taskName,GET_HEX_PTR(&task)," removed");
}


void Supervisor::deleteQueue(Task& task,Task*& last,Task*& first){
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
}

void Supervisor::executeAll(){
	uint8_t index = 1;
	Task *hCurrent = hFirst;
	Task *lCurrent = lFirst;
	SERIAL_LOGGER(hFirst->taskName,GET_HEX_PTR(hFirst)," SSSSSSSSSSSSSSSSSSSSSSSS");
	SERIAL_LOGGER(lFirst->taskName,GET_HEX_PTR(lFirst)," SSSSSSSSSSSSSSSSSSSSSSSSS");
	while(hCurrent != nullptr || lCurrent != nullptr ){
		if(index%3 != 0 ){
			SERIAL_LOGGER1("HIGH PRIORITY SLOT");
			exec(hCurrent);
		}else{
			SERIAL_LOGGER1("LOW PRIORITY SLOT");
			exec(lCurrent);	
		}
		index++;
	}
	//SERIAL_LOGGER1(hFirst->taskName);
	//SERIAL_LOGGER1(lFirst->taskName);
	SERIAL_LOGGER1("Cycle finished, reseting EXPERIMENTAL!!!!!!!!!!!!!!!");
	SERIAL_LOGGER1("****************************");
}


void Supervisor::exec(Task* current){
	if(current != nullptr){
		if(current->suspended == false){
			current->execute();
			if(!current->infinity){
				//inside deconstructor Task pointer is removed from List
				SERIAL_LOGGER(current->taskName,GET_HEX_PTR(current)," will be deleted");
				delete(current);//if on stack will crash
			}	
		}else{
			SERIAL_LOGGER(current->taskName,GET_HEX_PTR(current)," is suspend");
		}	
		current = current->after;
	}else{
		SERIAL_LOGGER1("missed execution slot");
	}	
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
			if(!current->infinity){
				//inside deconstructor Task pointer is removed from List
				SERIAL_LOGGER(current->taskName,GET_HEX_PTR(current)," will be deleted");
				delete(current);//if on stack will crash
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
		if(current->suspendable && !(current->suspended)){
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




