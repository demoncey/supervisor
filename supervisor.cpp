#include "Arduino.h"
#include "supervisor.h"


Supervisor::Supervisor(String name)
{
	this->name=name;
	first=NULL;
	last=NULL;
}


void Supervisor::addTask(Task& task){
	is_com("addTask");
	if(task.getSupervisor()==this){
		is_com("Supervisor: trying to add same task again, interuppted");
		return;
	}
	if( first==NULL){
		first=&task;
		task.before=NULL;
	}else{
		last->after=&task;
		task.before=last;
	}
	task.setSupervisor(this);
	task.after=NULL;
	last=&task;
}


void Supervisor::deleteTask(Task& task){
	is_com("Supervisor:task "+String(task.ptr_value,HEX)+" removed");
	if(&task==first){
		task.before=NULL;
		first=task.after;
		return;
	}
	if(&task==last){
		task.before->after=NULL;
		return;
	}
	task.before->after=task.after;
}


void Supervisor::execute(){
	Task* current=first;
	while(current){
		if(current->suspended == false){
			current->execute();
			is_com("Supervisor:task "+String(current->ptr_value,HEX)+" executed in chain");
			if(current->execution == MODE_ONCE){
				delete(current);
				is_com("Supervisor:task "+String(current->ptr_value,HEX)+" deleted");
			}	
		}else{
			is_com("Supervisor:task "+String(current->ptr_value,HEX)+" is suspend omitting execute");
		}

		current=current->after;
		delay(300);
	}
	is_com("Supervisor:Cycle finished, reseting");
}



void Supervisor::suspendAll(){
	Task *current=first;
	while(current){
		if(current->priority!=P_HIGH && !(current->suspended)){
			current->suspend();
		}
		current=current->after;
	}
	is_com("Supervisor:suspendAll finished");
}


void Supervisor::resumeAll(){
	Task *current=first;
	while(current){
		if(current->suspended){
			current->resume();
		}
		current=current->after;
	}
	is_com("Supervisor:resumedAll finished");
	
}


void Supervisor::is_com(String msg){
		if(!Serial){return;}
		Serial.println(msg);
	}




