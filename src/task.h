#ifndef task_h
#define task_h
#include "Arduino.h"


#define P_HIGH 1
#define P_LOW 3

//enum Priority{HIGH,LOW};

//enum Mode{INFINITY,ONCE};

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
		Task& makeSuspendtable();
		void kill(){};
		void setSupervisor(Supervisor &supervisor);
		const Supervisor* getSupervisor();
		bool suspended;
		String taskName;//access  directly via variable
	private: 
		Callback task_callback;
		Supervisor *supervisor;
		Task *before, *after;
		uint8_t priority;
		bool suspendable;
		bool infinity;
};




#endif
