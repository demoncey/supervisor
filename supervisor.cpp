#include "Arduino.h"
#include "supervisor.h"


Supervisor::Supervisor(String name)
{
	this->name=name;
	first=NULL;
	last=NULL;
}


void Supervisor::addTask(Task& task){
	if(task.getSupervisor()==this){
		is_com(this->name+": trying to add same task again, interuppted");
		return;
	}
	if( first==NULL){
		is_com(this->name+": task first is null");
		first=&task;
		task.before=NULL;
	}else{
		last->after=&task;
		task.before=last;
	}
	task.setSupervisor(this);
	task.after=NULL;
	last=&task;
	is_com(this->name+": task "+String(task.ptr_value,HEX)+" added");
}

void Supervisor::addTask(Task* task){
	is_com(this->name+": task "+String(task->ptr_value,HEX)+" will be added by *");
	this->addTask(*task);
}


void Supervisor::deleteTask(Task& task){
	if(&task==first){
		first=task.after;
		task.before=NULL;
		return;
	}
	if(&task==last){
		last=task.before;
		last->after=NULL;
		return;
	}
	task.before->after=task.after;
	is_com(this->name+": task "+String(task.ptr_value,HEX)+" removed");
}


void Supervisor::execute(){
	Task* current=first;
	uint8_t executed=0;
	uint8_t killed=0;
	while(current){
		if(current->suspended == false){
			current->execute();
			is_com(this->name+": task "+String(current->ptr_value,HEX)+" executed in chain");
			if(current->execution == MODE_ONCE){
				delete(current);
				is_com(this->name+": task "+String(current->ptr_value,HEX)+" deleted");
				killed++;
			}
		executed++;
		}else{
			is_com(this->name+": task "+String(current->ptr_value,HEX)+" is suspend omitting execute");
		}

		current=current->after;
		delay(50);
	}
	is_com(this->name+": Cycle finished, reseting");
	is_com("Summary:");
	is_com("	executed: "+String(executed));
	is_com("	killed: "+String(killed));
}



void Supervisor::suspendAll(){
	Task *current=first;
	while(current){
		if(current->priority!=P_HIGH && !(current->suspended)){
			current->suspend();
		}
		current=current->after;
	}
	is_com(this->name+": suspendAll finished");
}


void Supervisor::resumeAll(){
	Task *current=first;
	while(current){
		if(current->suspended){
			current->resume();
		}
		current=current->after;
	}
	is_com(this->name+": resumedAll finished");
	
}


void Supervisor::is_com(String msg){
		if(!Serial){return;}
		Serial.println(msg);
	}




