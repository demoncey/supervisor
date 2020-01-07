#include <task.h>
#include <supervisor.h>
void send_msg(void) ;
void send_msg2(void) ;

Supervisor supervisor("example1 supervisor");
Task task1(send_msg);
Task task2(send_msg2);

void setup() {
  Serial.begin(9600);
    while (!Serial) {
    ; 
  }
  supervisor.addTask(*task1.setPriority(P_HIGH));
  supervisor = supervisor+ task2;
}

void loop() {
  // put your main code here, to run repeatedly:
    supervisor.execute();
}


void send_msg(void*) {
  int id = random(1,65536);
  Serial.println("Heartbeat msg YYYYYYYYYYYYYYYYY: "+String(id,HEX));
}


void send_msg2(void*) {
  int id = random(1,65536);
  Serial.println("Heartbeat msg YYYYYYYYYYYYYYYYY: "+String(id,HEX));
}
