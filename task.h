#ifndef task_h
#define task_h
#include "Arduino.h"


#define P_HIGH 1
#define P_MEDIUM 2
#define P_LOW 3


#define MODE_INFINITY 0
#define MODE_ONCE 1
#define MODE_DEFINED 1

class Supervisor;

typedef  void (*Callback)();

class Task
{
	friend class Supervisor;//access to private Task  (before after) in Supervisor
	public:
		Task();
		Task(Callback callback);
		Task(Callback callback, bool suspend);
		~Task();
		void execute();
		bool isRunning(){return !suspended;};
		void suspend(){this->suspended=true;};
		void resume(){this->suspended=false;};
		Task* setPriority(uint8_t priority);
		void kill(){};
		void setSupervisor(Supervisor *supervisor){this->supervisor=supervisor;};
		void setMode(uint8_t execution){this->execution=execution;};
		Supervisor* getSupervisor(){return supervisor;};
		bool suspended;
		String name;//access  directly via variable
	private:
		Callback task_callback;
		
		Supervisor *supervisor;
		Task *before, *after;
		uint8_t priority;
		uint8_t execution;
		uint16_t ptr_value;
	
};


class TaskBuilder{
	
	public:
		TaskBuilder(){
			priority=P_LOW;
			execution=MODE_ONCE;
			name="unknown";
		};	
		TaskBuilder& setCallback(Callback callback){
			this->callback=callback;
			return *this;
		}
		TaskBuilder& setPriority(uint8_t priority){
			this->priority=priority;
			return *this;
		}
	
		TaskBuilder& setMode(uint8_t execution){
			this->execution=execution;
			return *this;
		}
		
		TaskBuilder& setName(String name){
			this->name=name;
			return *this;
		}
		
		
		
		Task* build(){
			//task is created out of scope so it is persistent
			this->task=new Task(this->callback);
			this->task->setMode(this->execution);
			this->task->setPriority(this->priority);
			this->task->name=this->name;
			return task;
		}
		void reset(){
			task=NULL;
		}
	private:
		Task *task;
		uint8_t execution;
		uint8_t priority;
		String name;
		Callback callback;
};



#endif
