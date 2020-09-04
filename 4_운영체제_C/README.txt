[hw1]
Linked List로 연결된 Hash Table을 구현하는 과제입니다.
hw1.h와 main.c 는 주어진 상태로 Hash Table의 기능 함수들을 hw1.c에서 구현했습니다.

[hw2]
hw1에서 구현한 Hash Table의 구조를 사용해 Priority-based Round Robin Scheduling 을 구현하는 과제입니다.
Testcase 및 *.h, main.c는 주어진 상태로 Init.c, Scheduler.c, Thread.c 를 구현했습니다.
Init.c 는 선언한 변수들의 초기화와 Scheduler.c 의 RunScheduler를 SIGALRM이 발생했을 때 실행시키도록 등록하는 역할을 합니다.
Scheduler.c 는 ContextSwitch 와 RunScheduler 함수가 존재합니다. RunScheduler에서 현재 Thread 상황에 맞게 교체할 준비를 하며 이후 ContextSwitch를 수행합니다. ContextSwitch는 현재 Thread와 새 Thread를 정지 및 실행합니다.
Thread.c 에서는 Thread 생성, 중지, 취소, 재개 등 Linux의 Thread의 기능들과 비슷하게 동작하도록 구현하고 있습니다.