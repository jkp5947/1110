#include "chat_common.h"
#include "linked_list.h"


#pragma pack(1)
typedef struct data
{
   Client_info ci;			//client 정보(fd, 대화명)
}Client_data;

typedef struct _history
{
	char msg[1024];
}History;

typedef struct _room
{
	int att_fd[2];			//참여중인 client fd
	List h_list;
}Room;

typedef struct global
{
   List c_list;
   List r_list;
}jkp_global;

jkp_global global_data;


void msg_send(jkp_global *g_data, int fd, char *buf);

//recv한 클라이언트 대화명을 리스트에 추가하고
//Server화면에 입장 메시지를 출력.
int id_recv(jkp_global *g_data, char *id, int fd);

//클라이언트에서 접속자 목록 요청시 클라이언트 데이터를 가지고있는
//linked list에서 대화명들을 요청한 클라이언트 fd를 통하여 전송.
void list_send(jkp_global *g_data, int request_fd);

//클라이언트로 부터 초대 요청시 일치하는 대화명이 있는지 확인후
//일치하는 접속자가 있으면 그 클라이언트의 fd를 리턴한다.
//int inv_request(jkp_global *g_data, char *id);

//찾은 접속자에게 초대를 보냄
void invite_send(jkp_global *g_data, int inv_fd, int request_fd);

//요청한 fd를 통해 대화내용을 불러들이고 대화내용을 send
void history_call(jkp_global *g_data, int request_fd);

//종료 메시지 수신시 해당 fd를 받아와 클라이언트 정보를 삭제
//클라이언트의 퇴장을 출력한다.
void exit_client(jkp_global *g_data, int exit_client_fd);
/////////////////////////////////////////
// server에서 recv 하는 상황의 flag    //
// 0 - 채팅 메시지                     //
// 1 - 대화명 recv                     //
// 2 - 접속자 목록 요청                //
// 3 - 초대 요청                       //
// 4 - 수락 or 거절 회신 recv          //
// 5 - history 요청                    //
// 6 - client 종료 알림                //
/////////////////////////////////////////

//초대 수락시 방생성(h_list init)
void room_create(jkp_global *g_data, int index);

//메세지 보내기
void send_msg(jkp_global *g_data, int fd, char *buf);

//
void invite_recv(jkp_global *g_data, int fd, int reply_flag);

//
void chat_flag_change(jkp_global *g_data, int fd, int change_value);

//
int chat_flag_search(jkp_global *g_data, int fd);

//
void chat_quit(jkp_global *g_data, int fd);
