#include "chat_common.h"

Client_info ct; //클라이언트 정보(fd, 대화명)

//아이디 생성.
void create_name(Client_info *ct);

//user_list와 메뉴를 화면에 디스플레이
void display(Client_info *ct, char *user_list[]);

//메뉴를 선택시 그에 맞는 역할을 수행
void choice_menu(Client_info *ct, int num);

//접속자 목록을 받아 내용을 리턴.
void recv_list(Client_info *ct, char *buf);

//나가기 입력시 종료시키는 클라이언트의 fd 를 리턴하여 서버에 알림.
int exit_client(Client_info *ct);

//history 요청시 server에 요청을 send.
void history_request(Client_info *ct);

// 수락거절 회신시 데이터 받고 거절시 안내 메시지 출력
char acp_recv(Client_info *ct, char *acp);

// 초대 요청 수신시 수락/거절을 선택하여 서버로 send
void inv_recv(Client_info *ct);

//history recv는 한줄씩 보내주는대로 화면에 출력하는걸로 생각
