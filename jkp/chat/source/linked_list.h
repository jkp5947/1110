#define OFFSET(structure, member)  ((long)&(((structure *) 0) -> member))

#pragma pack(1)
typedef struct _node
{
   void *pData;
   struct _node *next;
}Node;

typedef struct _list
{
   struct _node *head;
   struct _node *tail;
}List;

//linked list 초기화 
void linkedlist_init(List *list);

//linked list 추가
void linkedlist_add(List *list, void *struct_data, int choice_flag);

//linked list 에서 원하는 데이터 찾기
//ofs - offset , len - 비교하고자하는 길이
int linkedlist_search(List *list, void *data, long ofs, int len, int return_flag);

//linked list에서 삭제하고자 하는 노드를 자료를 통해 찾아 노드삭제.
int linkedlist_delete(List *list, void *find, long ofs, int len);

//list출력
void linkedlist_enumerate(List *list,int struct_flag);

//linked list 삭제 
void linkedlist_destroy(List *list);
