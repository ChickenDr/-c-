#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

// 메크로
#define MAX_STR_SIZE 50
#define ONE_MONTH 2592000
#define ONE_DAY 86400

//		링크드 리스트를 위한 구조체		//
struct client
{
	int studentID; // 학번
	char studentPW[MAX_STR_SIZE]; // 비밀번호
	char studentName[MAX_STR_SIZE]; // 학생 이름
	char studentAdrees[MAX_STR_SIZE]; // 학생 주소
	char studentPhonNumber[MAX_STR_SIZE]; // 학생 전화번호
	struct client* nextNode;
};

struct book
{
	int bookID; // 책 ID
	char bookName[MAX_STR_SIZE]; // 책 이름
	char bookPub[MAX_STR_SIZE]; // 책 출판사
	char bookAuth[MAX_STR_SIZE]; // 책 저자
	unsigned long long ISBN; // 책 ISBN 넘버
	char bookLocate[MAX_STR_SIZE]; // 책 위치
	char bookStat; // 책의 상태
	struct book* nextNode;
};

struct borrow
{
	int studentID; // 빌려간 학생의 학번
	int bookID; // 빌린 책 ID
	time_t bookBorrow; // 빌린시간
	time_t returnBook; // 반납 예정 시간
	struct borrow* nextNode;
};

//			동적 할당 관련 함수			//

// borrow 노드 삽입, 연결 함수
struct borrow* insertBorrowNode(struct borrow* currentNode, int studentID, int bookID, time_t bookBorrow, time_t returnBook);
// book 노드 삽입, 연결 함수
struct book* insertBookNode(struct book* currentNode, int bookID, char bookName[], char bookPub[], char bookAuth[], unsigned long long ISBN, char bookLocate[], char bookStat);
// client 노드 삽입 연결 함수
struct client* insertClientNode(struct client *currentNode, int studetID, char studentPW[], char studentName[], char studentAdrees[], char studentPhonNumber[]);

void deleteClientNode(struct client* delete, struct client* head); // client 노드 삭제 함수
void deleteBookNode(struct book* delete, struct book* head); // book 노드 삭제 함수
void deleteBorrowNode(struct borrow* delete, struct borrow* head); // book 노드 삭제 


//		 파일에서 정보를 읽고 메모리에 로드하는 함수		//
// 파일을 읽는 함수
void importData();
// 초기화 함수
void initstallze();

//		메인 메뉴 기능함수			//
void clientJoin(struct client* client); // 회원가입 함수
int mainMenu(); // 메인 메뉴 함수
int clientMenu(struct client* login); // 회원 메뉴 함수
int adminMenu(); // 관리자 메뉴 함수
void login(struct client* login); // 로그인 메뉴 함수
void bookSearch(struct book* searchBook); // 책 검색 함수

//		회원 메뉴 기능함수			//
void myBorrow(struct borrow* myBorrow, struct book* book, struct client* login); // 내 대여 목록 함수
void wClientFile(struct client* client); // 클라이언트 파일 다시쓰는 함수
void wBookFile(struct book* boko); // 북파일을 다시 쓰는 함수
void wBorrowFile(struct borrow* borrow); // 대여 파일을 다시 쓰는 함수
void editProfile(struct client* profile, struct client* login); // 회원정보 수정 함수
void deletAccount(struct client* deletAccount); // 회원 탈퇴 함수

//		관리자 메뉴 기능함수		//
void bookInsert(struct book* book); // 도서 등록 함수
void bookDelete(struct book* deletebook); // 도서 삭제 함수
void bookBorrow(struct book* book, struct borrow* borrow, struct client* client); // 도서 대여 함수
void bookReturn(struct book* book, struct borrow* borrow, struct client* client); // 도서 반납 함수
void clientList(struct client* client); // 회원 목록 함수
