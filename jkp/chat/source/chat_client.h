#include "chat_common.h"

Client_info ct; //클라이언트 정보(fd, 대화명)

//아이디 생성하여 server로 send
void id_create(Client_info *ct);

//서버로 접속자 목록 요청
void list_request(Client_info *ct);

//메뉴를 화면에 디스플레이
void menu_display(void);

//메뉴를 선택시 그에 맞는 역할을 수행
//1.초대하기 -> 초대할 아이디 입력 -> server로 send
//2.종료하기 -> 클라이언트 종료
int menu_choice(Client_info *ct, char *msg);

//접속자 목록을 서버로부터 받아 출력.
void list_recv(Client_info *ct);

//나가기 입력시 서버로 채팅방 종료 요.
void client_exit(Client_info *ct);

//history 요청시 server에 요청을 send.
void history_request(Client_info *ct);

// 초대 요청 수신시 수락/거절을 선택하여 서버로 send
void invite_recv(Client_info *ct);
