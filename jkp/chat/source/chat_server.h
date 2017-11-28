#include "chat_common.h"
#include "linked_list.h"


#pragma pack(1)
typedef struct data
{
   Client_info ci;		//client 정보(fd, 대화명)
}Client_data;

typedef struct _history
{
	char msg[1024];      //Message
}History;

typedef struct _room
{
	int att_fd[2];			//참여중인 client fd
	List h_list;         //History list
}Room;

typedef struct global
{
   List c_list;         //Client list
   List r_list;         //Room list
}jkp_global;

jkp_global global_data;

// 메시지를 수신하였을 때 상대 Client에게 send하고 history에 저장.
void msg_send(jkp_global *g_data, int fd, char *buf);

//recv한 클라이언트 대화명을 리스트에 추가하고
//Server화면에 입장 메시지를 출력.
int id_recv(jkp_global *g_data, char *id, int fd);

//클라이언트에서 접속자 목록 요청시 클라이언트 데이터를 가지고있는
//linked list에서 대화명들을 요청한 클라이언트 fd를 통하여 전송.
void list_send(jkp_global *g_data, int request_fd);

//찾은 접속자에게 초대를 보냄
void invite_send(jkp_global *g_data, int inv_fd, int request_fd);

//요청한 fd를 통해 대화내용을 불러들이고 대화내용을 send
void history_call(jkp_global *g_data, int request_fd);

//초대 수락시 방생성(h_list init)
void room_create(jkp_global *g_data, int index);

//수락/거절 회신시 flag를 통하여 수락상황과 거절상황 처리
// 수락시 -> chat_flag를 변경하고 채팅시작을 알림
// 거절시 -> 초대요청한 Client에게 거절알림.
void invite_recv(jkp_global *g_data, int fd, int reply_flag);

//chat_flag의 값을 Change_value로 변경하는 함수.
void chat_flag_change(jkp_global *g_data, int fd, int change_value);

//chat_flag의 값이 무엇인지 찾는 함수.
int chat_flag_search(jkp_global *g_data, int fd);

//채팅방 종료시 모든 history data를 destroy, room data를 삭제 
//채팅 종료를 알림.
void chat_quit(jkp_global *g_data, int fd);
