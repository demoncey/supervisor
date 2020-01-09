#ifndef supervisor_h
#define supervisor_h
#include "Arduino.h"
#include "task.h"


#define SERIAL_LOGGER(name, ptr,msg) (Serial)?Serial.println((name)+":"+String((ptr),HEX)+" "+(msg)):0
#define SERIAL_LOGGER1(msg) (Serial)?Serial.println((msg)):0
#define GET_HEX_PTR(ptr) (uint16_t)(ptr)
#define ARGS void *args


//sprintf(buffer, "Task %u flashing %u times.", taskNum, count);
//https://stackoverflow.com/questions/20059673/print-out-value-of-stack-pointer

class Supervisor
{
	public:
	    static Supervisor& getInstance(){
			static Supervisor instance("Supervisor Singleton"); // Guaranteed to be destroyed.
			return instance;
		}
		Supervisor(const Supervisor& supervisor) = delete;
		void operator =(Supervisor const& supervisor)  = delete;
		void addTask(Task& task);
		void addQueue(Task& task,Task*& last,Task*& first);
		void deleteQueue(Task& task,Task*& last,Task*& first);
		void deleteTask(Task& task);
		void execute();
		void executeAll();
		void exec(Task* current);
		void suspendAll();
		void resumeAll();
		//first argument always this hidden
		Supervisor& operator +(Task&  task){
			this->addTask(task);
			return *this;
		};
	private:
	    Supervisor(String name);
		String name;
		//task chain
		Task  *first,*last;	
		Task  *hFirst,*hLast; 
		Task  *lFirst,*lLast; 		
};




#endif
