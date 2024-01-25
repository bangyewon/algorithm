#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "CircularQueue.h"
#include <limits.h>

#define CUS_COME_TERM 22 // 손님 입장 간격시간
#define BURGER 0 // 햄버거 
#define SANDWICH 1 // 샌드위치
#define RICE 2 // 김밥
#define BUR_TERM 18 // 햄버거 시간
#define SAND_TERM 22 // 샌드위치 시간
#define RICE_TERM 29 // 김밥 시간

int main(void) {
    int makeProc = 0; // 현재 주문 처리하는데 남은 시간
    int burOrder = 0, sandOrder = 0, riceOrder = 0;
    int sec;
    Queue que;

    QueueInit(&que); // 큐 초기화
    srand(time(NULL)); // 난수 발생기 초기화

    int minTime = INT_MAX; // 최소 대기 시간 (초기 값을 INT_MAX로 설정)
    int maxTime = 0; // 초기 최대 대기 시간을 0으로 초기화
    int totalTime = 0; // 대기시간의 합 - 평균 대기시간을 위해서 
    int numOrdersProcessed = 0; // 처리된 주문의 수
    int customersEntered = 0; // 고객이 대기실에 들어온 횟수 (누적)
    int customerCount = 0; // 주문 고객의 수

    // 첫 번째 주문이 들어온 시간 초기화
    int firstOrderTime = -1;

    // 한시간 동안 반복하도록 설정함
    for (sec = 0; sec < 3600; sec++) {
        // 매 초마다 CUS_COME_TERM을 22초로 나눠 나머지 확인하기
        // 만약 나머지가 0이라면 22초 뒤 즉 손님이 도착했을때 실행됨
        if (sec % CUS_COME_TERM == 0) { 
            // 새로운 고객 도착했을 때
            switch (rand() % 3) {
                case BURGER: // 햄버거 주문했을 때
                    if (firstOrderTime == -1) {
                        minTime = BUR_TERM;
                    }
                    Enqueue(&que, BUR_TERM); // 큐에 주문 추가하고 해당 음식의 주문 카운트 증가시키기
                    burOrder++;
                    customersEntered++; // 고객이 들어온 횟수 누적
                    break;
                case SANDWICH: // 샌드위치 주문했을 때
                    if (firstOrderTime == -1) {
                        minTime = SAND_TERM;
                    }
                    Enqueue(&que, SAND_TERM);
                    sandOrder++;
                    customersEntered++;
                    break;
                case RICE: // 김밥 주문했을 때
                    if (firstOrderTime == -1) {
                        minTime = RICE_TERM;
                    }
                    Enqueue(&que, RICE_TERM);
                    riceOrder++;
                    customersEntered++;
                    break;
            }
        }

        // makeProc가 0이면 주문처리가 가능, 큐가 비어있으면 처리할 주문이 대기중이므로 실행됨
        if (makeProc == 0 && !QIsEmpty(&que)) {
            makeProc = Dequeue(&que); // 큐에서 음식 제작 시간을 꺼내서 makeProc에 저장함
            customerCount++;

            // endTime을 현재 시간으로 설정
            int endTime = sec;
            
            // 대기 시간을 계산
            int waitTime = endTime - firstOrderTime;

            // 최소 대기 시간 갱신
            if (waitTime < minTime) {
                minTime = waitTime;
            }

            // 최대 대기 시간 갱신
            if (waitTime > maxTime) {
                maxTime = waitTime;
            }

            // 대기 시간의 총합 갱신
            totalTime += waitTime;
        }

        makeProc--; // 음식 제작 시간 감소
    }

    printf("< 시물레이션 결과 >\n");
    printf(" - 햄버거: %d\n", burOrder);
    printf(" - 샌드위치: %d\n", sandOrder);
    printf(" - 김밥: %d\n", riceOrder);
    printf("대기실에 입장한 고객의 수 : %d\n", customersEntered);
    printf("대기실 최대 크기: %d\n", QUE_LEN);

    // 대기 시간 통계 정보 출력하기
    double avgTime = (double)totalTime / customerCount;
    printf("최소 대기시간: %d 초\n", minTime);
    printf("최대 대기시간: %d 초\n", maxTime);
    printf("평균 대기시간: %.2lf 초\n", avgTime);
    return 0;
}
