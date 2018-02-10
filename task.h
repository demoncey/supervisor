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
		void setPriority(uint8_t priority){this->priority=priority;};
		void kill(){};
		void setSupervisor(Supervisor *supervisor){this->supervisor=supervisor;};
		void setMode(uint8_t execution){this->execution=execution;};
		Supervisor* getSupervisor(){return supervisor;};
		bool suspended;
	private:
		Callback task_callback;
		
		Supervisor *supervisor;
		Task *before, *after;
		uint8_t priority;
		uint8_t execution;
		uint16_t ptr_value;
	
};
#endif
