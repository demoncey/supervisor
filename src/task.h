#ifndef task_h
#define task_h
#include "Arduino.h"


#define P_HIGH 1
#define P_LOW 3

#define MODE_INFINITY 0
#define MODE_ONCE 1





class Supervisor;

using Callback = void (*)(void*);

class Task
{
	friend class Supervisor;//access to private Task  (before after) in Supervisor
	public:
		Task() = delete;
		Task(String name,Callback callback);
		~Task();
		void execute();
		bool isRunning(){
			return !suspended;
		};
		void suspend(){
			this->suspended = true;
		};
		void resume(){
			this->suspended = false;
		};
		Task& setPriority(uint8_t priority);
		Task& makeOnce();
		void kill(){};
		void setSupervisor(Supervisor &supervisor){
			this->supervisor = &supervisor;
		};
		const Supervisor* getSupervisor(){
			return supervisor;
		};
		void setMode(uint8_t execution){
			this->execution = execution;
		};
		bool suspended;
		String taskName;//access  directly via variable
	private:
		Callback task_callback;
		Supervisor *supervisor;
		Task *before, *after;
		uint8_t priority;
		uint8_t execution;	
};




#endif
