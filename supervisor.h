#ifndef supervisor_h
#define supervisor_h
#include "Arduino.h"
#include "task.h"

class Supervisor
{
	public:
		Supervisor(String name);
		void addTask(Task& task);
		void addTask(Task* task);
		void deleteTask(Task& task);
		void execute();
		void suspendAll();
		void resumeAll();
		//first arg always this hidden
		Supervisor& operator +(Task&  task){
			this->addTask(task);
			return *this;
		};
	private:
		String name;
		void is_com(String msg);
		//task chain
		Task  *first,*last;	
};




#endif
