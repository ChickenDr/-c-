#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "util.c"


char DayOfWeek[7][10] = { "일요일", "월요일", "화요일", "수요일", "목요일", "금요일", "토요일" };

struct client* Client;
struct book* Book;
struct borrow* Borrow;
struct client* Login;


int main()
{
	initstallze();
	mainMenu();
	return 0;
}

int mainMenu()
{
	int mainMenuChoice;
	printf(">> 도서관 서비스 <<\n");
	printf("1. 회원 가입 2. 로그인 3.프로그램 종료\n");
	scanf("%d", &mainMenuChoice);
	switch (mainMenuChoice)
	{
	case 1:
		clientJoin(Client);
		break;
	case 2:
		login(Client);
		break;
	case 3:
		break;
	default:
		printf("1, 2, 3 중에서 선택해 주세요.");
		mainMenu();
		break;
	}
	return 0;
}

// 초기화 함수
void initstallze()
{
	// head 노드를 만듬
	Client = (struct client*)malloc(sizeof(struct client));
	Client->nextNode = NULL; // head노드의 nextNode는 NULL을 가르킴

	Book = (struct book*)malloc(sizeof(struct book));
	Book->nextNode = NULL; // head노드의 nextNode는 NULL을 가르킴

	Borrow = (struct borrow*)malloc(sizeof(struct borrow));
	Borrow->nextNode = NULL;

	Login = (struct client*)malloc(sizeof(struct client));
	Login->nextNode = NULL;

	//파일 읽기및 메모리에 로드
	importData();
}


// 파일에서 정보를 읽고 메모리에 로드하는 함수
void importData()
{
	// 문자열 데이터를 받을 임시 배열
	char TmpData[5][MAX_STR_SIZE];

	// 클라이언트 파일 불러오기 및 메모리 할당
	FILE* clientFileOpen = fopen("client.txt", "r");// 파일을 읽어옴

	// 연결할 다음 노드를 가르킬 포인터 변수
	struct client* nextClientNode = Client;
	while (!feof(clientFileOpen))
	{
		int clientID;
		// 텍스트 파일의 지정 형식의 내용에 맞게 파일을 불러옴
		fscanf(clientFileOpen, "%d | %s | %[^|] | %[^|] | %s", &clientID, TmpData[0], TmpData[1], TmpData[2], TmpData[3]);
		// 링크드 리스트의 연결
		insertClientNode(nextClientNode, clientID, TmpData[0], TmpData[1], TmpData[2], TmpData[3]);
		// nextClientNode는 다음 연결할 노드를 가르킴
		nextClientNode = nextClientNode->nextNode;
	}
	fclose(clientFileOpen);

	// 북 파일 불러오기 및 메모리 할당
	FILE* bookFileOpen = fopen("book.txt", "r");// 파일을 읽어옴

	// 연결할 다음 노드를 가르킬 포인터 변수
	struct book* nextBookNode = Book;
	while (!feof(bookFileOpen))
	{
		int bookID;
		long long ISBN;
		char tmpBookStat;
		// 텍스트 파일의 지정 형식의 내용에 맞게 파일을 불러옴
		fscanf(bookFileOpen, "%d | %[^|] | %[^|] | %[^|] | %lld | %[^|] | %c", &bookID, TmpData[0], TmpData[1], TmpData[2], &ISBN, TmpData[3], &tmpBookStat);
		// 링크드 리스트의 연결
		insertBookNode(nextBookNode, bookID, TmpData[0], TmpData[1], TmpData[2], ISBN, TmpData[3], tmpBookStat);
		// nextBookNode는 다음 연결할 노드를 가르킴
		nextBookNode = nextBookNode->nextNode;
	}

	fclose(bookFileOpen);

	// 클라이언트 파일 불러오기 및 메모리 할당
	FILE* borrowFileOpen = fopen("borrow.txt", "r");// 파일을 읽어옴

	// 연결할 다음 노드를 가르킬 포인터 변수
	struct borrow* nextBorrowNode = Borrow;
	while (!feof(borrowFileOpen))
	{
		int clientID, bookID;
		time_t bookBorrow, returnBook;
		// 텍스트 파일의 지정 형식의 내용에 맞게 파일을 불러옴
		fscanf(borrowFileOpen, "%d | %d | %lld | %lld", &clientID, &bookID, &bookBorrow, &returnBook);
		// 링크드 리스트의 연결
		insertBorrowNode(nextBorrowNode, clientID, bookID, bookBorrow, returnBook);
		// nextClientNode는 다음 연결할 노드를 가르킴
		nextBorrowNode = nextBorrowNode->nextNode;
	}

	fclose(borrowFileOpen);
}

// borrow 노드 삽입, 연결 함수
struct borrow* insertBorrowNode(struct borrow* currentNode, int studentID, int bookID, time_t bookBorrow, time_t returnBook)
{
	// currentNode가 가르키고 있던 다음 노드가 afterNode
	struct borrow* afterNode = currentNode->nextNode;

	struct borrow* newNode = (struct borrow*)malloc(sizeof(struct borrow));

	newNode->studentID = studentID;
	newNode->bookID = bookID;
	newNode->bookBorrow = bookBorrow;
	newNode->returnBook = returnBook;
	newNode->nextNode = afterNode;

	// currentNode 는 이제 newNode를 가리킴
	currentNode->nextNode = newNode;

	return newNode;
}

// book 노드 삽입, 연결 함수
struct book* insertBookNode(struct book* currentNode, int bookID, char bookName[], char bookPub[], char bookAuth[], unsigned long long ISBN, char bookLocate[], char bookStat)
{
	// currentNode가 가르키고 있던 다음 노드가 afterNode
	struct book* afterNode = currentNode->nextNode;

	// 새로운 노드에 동적 할당
	struct book* newNode = (struct book*)malloc(sizeof(struct book));

	newNode->bookID = bookID;
	strcpy(newNode->bookName, bookName);
	strcpy(newNode->bookPub, bookPub);
	strcpy(newNode->bookAuth, bookAuth);
	newNode->ISBN = ISBN;
	strcpy(newNode->bookLocate, bookLocate);
	newNode->bookStat = bookStat;
	newNode->nextNode = afterNode;

	// currentNode 는 이제 newNode를 가리킴
	currentNode->nextNode = newNode;

	return newNode;
}

// client 노드 삽입 연결 함수
struct client* insertClientNode(struct client *currentNode, int studetID, char studentPW[], char studentName[], char studentAdrees[], char studentPhonNumber[])
{
	// currentNode가 가르키고 있던 다음 노드가 afterNode
	struct client* afterNode = currentNode->nextNode;

	// 새로운 노드에 동적 할당
	struct client* newNode = (struct client*)malloc(sizeof(struct client));

	newNode->studentID = studetID;
	strcpy(newNode->studentPW, studentPW);
	strcpy(newNode->studentName, studentName);
	strcpy(newNode->studentAdrees, studentAdrees);
	strcpy(newNode->studentPhonNumber, studentPhonNumber);
	newNode->nextNode = afterNode;

	// currentNode 는 이제 newNode를 가리킴
	currentNode->nextNode = newNode;

	return newNode;
}

// client 노드 삭제 함수
void deleteClientNode(struct client* delete, struct client* head) // 파괴하고자 하는 노드, 맨처음 노드
{
	struct client* next = head;
	// 해드 노드를 삭제 하려고 할경우
	if (delete == head)
	{
		free(delete);
		return;
	}
	// next가 NULL 일경우 종료
	while (next)
	{
		// 만약 next의 nextNode가 delete라면 next는 delete의 다음 노드를 가르킴
		if (next->nextNode == delete)
		{
			next->nextNode = delete->nextNode;
		}
		// next는 다음노드를 가르킴
		next = next->nextNode;
	}
	free(delete);
}

// book 노드 삭제 함수
void deleteBookNode(struct book* delete, struct book* head)
{
	struct book* next = head;
	// 해드 노드를 삭제 하려고 할경우
	if (delete == head)
	{
		free(delete);
		return;
	}
	// next가 NULL 일경우 종료
	while (next)
	{
		// 만약 next의 nextNode가 delete라면 next는 delete의 다음 노드를 가르킴
		if (next->nextNode == delete)
		{
			next->nextNode = delete->nextNode;
		}
		// next는 다음노드를 가르킴
		next = next->nextNode;
	}
	free(delete);

}

// borrow 노드 삭제 함수
void deleteBorrowNode(struct borrow* delete, struct borrow* head)
{
	struct borrow* next = head;
	// 해드 노드를 삭제 하려고 할경우
	if (delete == head)
	{
		free(delete);
		return;
	}
	// next가 NULL 일경우 종료
	while (next)
	{
		// 만약 next의 nextNode가 delete라면 next는 delete의 다음 노드를 가르킴
		if (next->nextNode == delete)
		{
			next->nextNode = delete->nextNode;
		}
		// next는 다음노드를 가르킴
		next = next->nextNode;
	}
	free(delete);

}


// 로그인 함수
void login(struct client* login)
{
	// 아이디와 비밀번호를 받을 배열
	char ID[MAX_STR_SIZE], PW[MAX_STR_SIZE];
	printf(">> 로그인 << \n아이디:");
	scanf("%s", &ID);
	// ID가 admin 일 경우
	if (!strcmp(ID, "admin"))
	{
		printf("비밀 번호:");
		scanf("%s", &PW);
		if (strcmp(PW, "AdminLogin"))
		{
			printf("비밀번호를 틀리셨습니다.\n");
			mainMenu();
		}
		else
		{
			strcpy(Login->studentName, "admin");
			adminMenu();
		}
	}
	else
	{
		// admin 이 아닐경우 회원 리크드 리스트에서 서치를 시작
		while (login)
		{
			if (login->studentID == atoi(ID))
			{
				printf("비밀 번호:");
				scanf("%s", &PW);
				if (strcmp(PW, login->studentPW))
				{
					printf("비밀번호를 틀리셨습니다.\n");
					mainMenu();
				}
				else
				{
					Login = login;
					clientMenu(login);
				}
			}
			login = login->nextNode;
		}
		// 모든 서치가 끝났는데 같은 ID가 없는 경우
		printf("ID가 존재하지 않습니다.\n");
		mainMenu();
	}
}

// 회원가입 함수
void clientJoin(struct client* client)
{
	int ID;
	char TmpData[5][MAX_STR_SIZE];
	
	printf(">> 회원 가입 메뉴 <<\n");
	printf("학번, 비밀번호, 이름, 주소, 전화번호를 입력하세요.\n");
	printf("학번:");
	scanf("%d", &ID);
	printf("\n비밀번호:");
	scanf("%s", &TmpData[0]);
	printf("\n이름:");
	scanf("%s", &TmpData[1]);
	printf("\n주소:");
	scanf("%s", &TmpData[2]);
	printf("\n전화번호:");
	scanf("%s", &TmpData[3]);

	// 오름 차순으로 정렬되는 순서로 넣기위해 서치를 시작
	while (client)
	{
		if (ID < client->nextNode->studentID)
		{
			insertClientNode(client, ID, TmpData[0], TmpData[1], TmpData[2], TmpData[3]);
			wClientFile(Client);
			break;
		}
		client = client->nextNode;
	}
	mainMenu();
}


// client 파일을 다시쓰는 함수
void wClientFile(struct client* client)
{
	client = client->nextNode;
	// 클라이언트 파일 불러오기 및 메모리 할당
	FILE* clientFileOpen = fopen("client.txt", "w");// 파일을 읽어옴

	while (client)
	{
		// 텍스트 파일의 지정 형식의 내용에 맞게 파일을 씀
		fprintf(clientFileOpen, "%d | %s | %s| %s| %s\n", client->studentID, client->studentPW, client->studentName, client->studentAdrees, client->studentAdrees);
		client = client->nextNode;
	}
	fclose(clientFileOpen);
}

// Book 파일을 다시쓰는 함수
void wBookFile(struct book* book)
{
	book = book->nextNode;
	// 북 파일 불러오기 및 메모리 할당
	FILE* bookFileOpen = fopen("book.txt", "w");// 파일을 읽어옴

	while (book)
	{
		// 텍스트 파일의 지정 형식의 내용에 맞게 파일을 씀
		fprintf(bookFileOpen, "%d | %s| %s| %s| %lld | %s| %c\n", book->bookID, book->bookName, book->bookPub, book->bookAuth, book->ISBN, book->bookLocate, book->bookStat);
		book = book->nextNode;
	}
	fclose(bookFileOpen);
}

// Borrow 파일을 다시쓰는 함수
void wBorrowFile(struct borrow* borrow)
{
	borrow = borrow->nextNode;
	// 대여 파일 불러오기 및 메모리 할당
	FILE* borrowFileOpen = fopen("borrow.txt", "w");// 파일을 읽어옴

	while (borrow)
	{
		// 텍스트 파일의 지정 형식의 내용에 맞게 파일을 씀
		fprintf(borrowFileOpen, "%d | %d | %lld | %lld\n", borrow->studentID, borrow->bookID, borrow->bookBorrow, borrow->returnBook);
		borrow = borrow->nextNode;
	}
	fclose(borrowFileOpen);
}

// 회원 메뉴
int clientMenu(struct client* login)
{
	int clientMenuChoice;
	printf(">> 회원 메뉴 <<\n");
	printf("1. 도서 검색           2. 내 대여 목록\n3. 개인정보 수정       4. 회원 탈퇴\n5. 로그아웃            6. 프로그램 종료");
	scanf("%d", &clientMenuChoice);
	switch (clientMenuChoice)
	{
	case 1:
		bookSearch(Book);
		break;
	case 2:
		myBorrow(Borrow, Book, login);
		break;
	case 3:
		editProfile(Client, login);
		break;
	case 4:
		deletAccount(login);
		break;
	case 5:
		Login = NULL;
		mainMenu();
		break;
	case 6:
		return 0;
	default:
		printf("1, 2, 3 중에서 선택해 주세요.");
		clientMenu(login);
		break;
	}
	return 0;
}

// 대여목록
void myBorrow(struct borrow* myBorrow, struct book* book, struct client* login)
{
	// 대여 시간을 위한 tm 구조체
	struct tm* borrowData; // 대여시간
	struct tm* returnData; // 반납 예정 시간
	printf(">> 대여 목록 <<\n");
	while (myBorrow)
	{
		if (myBorrow->studentID == login->studentID)
		{
			printf("도서 번호:%d\n", myBorrow->bookID);
			while (book)
			{
				if (book->bookID == myBorrow->bookID)
				{
					printf("도서 명:%s\n", book->bookName);
					break;
				}
				book = book->nextNode;
			}
			borrowData = localtime(&myBorrow->bookBorrow); // 대여 파일에서 대여시간을 받아옴
			printf("%d년 %d월 %d일 %s\n", borrowData->tm_year + 1900, borrowData->tm_mon + 1, borrowData->tm_mday, DayOfWeek[borrowData->tm_wday]);
			returnData = localtime(&myBorrow->returnBook); // 대여 파일에서 반납 예정 시간을 받아옴
			printf("%d년 %d월 %d일 %s\n", returnData->tm_year + 1900, returnData->tm_mon + 1, returnData->tm_mday, DayOfWeek[returnData->tm_wday]);
			clientMenu(login);
		}
		myBorrow = myBorrow->nextNode;
	}
	printf("대여 목록을 찾을수 없습니다.\n");
	clientMenu(login);
}

// 책 검색 함수
void bookSearch(struct book* searchBook)
{
	searchBook = searchBook->nextNode; // 해드 노드를 검색에 넣지 않기위해 건너뜀
	long long ISBN;
	char tmp[MAX_STR_SIZE];
	int bookSearchMenuChoice;

	printf(">> 도서 검색 <<\n");
	printf("1. 도서명 검색		2. 출판사 검색\n3. ISBN 검색		4. 저자명 검색\n5. 전체 검색	   	6. 이전 메뉴\n");
	scanf("%d", &bookSearchMenuChoice);
	switch (bookSearchMenuChoice)
	{
	case 1:
		printf("도서명을 입력하세요.\n");
		scanf("%s", &tmp);
		// 북 파일에서 서치의 시작
		while (searchBook)
		{
			if (strstr(searchBook->bookName, tmp) != NULL)
			{
				printf("도서명: %s\n", searchBook->bookName);
				printf("출판사: %s\n", searchBook->bookPub);
				printf("저자명: %s\n", searchBook->bookAuth);
				printf("ISBN: %lld\n", searchBook->ISBN);
				printf("소장처: %s\n", searchBook->bookLocate);
				printf("대여가능 여부: %c\n", searchBook->bookStat);
			}
			searchBook = searchBook->nextNode;
		}
		bookSearch(Book);
		break;
	case 2:
		printf("출판사명을 입력하세요.\n");
		scanf("%s", &tmp);
		// 북파일에서 서치의 시작
		while (searchBook)
		{
			if (strstr(searchBook->bookPub, tmp) != NULL)
			{
				printf("도서명: %s\n", searchBook->bookName);
				printf("출판사: %s\n", searchBook->bookPub);
				printf("저자명: %s\n", searchBook->bookAuth);
				printf("ISBN: %lld\n", searchBook->ISBN);
				printf("소장처: %s\n", searchBook->bookLocate);
				printf("대여가능 여부: %c\n", searchBook->bookStat);
			}
			searchBook = searchBook->nextNode;
		}
		bookSearch(Book);
		break;
	case 3:
		printf("출판사명을 입력하세요.\n");
		scanf("%lld", &ISBN);
		// 북 파일에서 서치의 시작
		while (searchBook)
		{
			if (searchBook->ISBN == ISBN)
			{
				printf("도서명: %s\n", searchBook->bookName);
				printf("출판사: %s\n", searchBook->bookPub);
				printf("저자명: %s\n", searchBook->bookAuth);
				printf("ISBN: %lld\n", searchBook->ISBN);
				printf("소장처: %s\n", searchBook->bookLocate);
				printf("대여가능 여부: %c\n", searchBook->bookStat);
			}
			searchBook = searchBook->nextNode;
		}
		bookSearch(Book);
		break;
	case 4:
		printf("저자명을 입력하세요.\n");
		scanf("%s", &tmp);
		// 북 파일에서 서치의 시작
		while (searchBook)
		{
			if (strstr(searchBook->bookAuth, tmp) != NULL)
			{
				printf("도서명: %s\n", searchBook->bookName);
				printf("출판사: %s\n", searchBook->bookPub);
				printf("저자명: %s\n", searchBook->bookAuth);
				printf("ISBN: %lld\n", searchBook->ISBN);
				printf("소장처: %s\n", searchBook->bookLocate);
				printf("대여가능 여부: %c\n", searchBook->bookStat);
			}
			searchBook = searchBook->nextNode;
		}
		bookSearch(Book);
		break;
	case 5:
		// 북 파일에서 서치의 시작
		while (searchBook)
		{
			printf("도서명: %s\n", searchBook->bookName);
			printf("출판사: %s\n", searchBook->bookPub);
			printf("저자명: %s\n", searchBook->bookAuth);
			printf("ISBN: %lld\n", searchBook->ISBN);
			printf("소장처: %s\n", searchBook->bookLocate);
			printf("대여가능 여부: %c\n", searchBook->bookStat);	
			searchBook = searchBook->nextNode;
		}
		bookSearch(Book);
		break;
	case 6:
		// 만약 어드민에서 이기능을 쓸경우 adminMenu로 넘어가는 예외 처리
		if (!strcpy(Login->studentName, "admin"))
		{
			adminMenu();
		}
		clientMenu(Login);
		break;
	default:
		printf("1, 2, 3 중에서 선택해 주세요.");
		bookSearch(searchBook);
		break;
	}
}

// 회원 정보를 수정하는 함수
void editProfile(struct client* profile, struct client* login)
{
	char tmp[MAX_STR_SIZE];
	int profileMenuChoice;
	printf(">> 개인정보 수정 <<\n");
	printf("1. 비밀 번호 변경           2. 주소 변경\n3. 번호 변경           4. 이전 메뉴\n");
	scanf("%d", &profileMenuChoice);
	switch (profileMenuChoice)
	{
	case 1:
		printf("바꾸실 비밀번호를 입력해 주세요:");
		scanf("%s", &tmp);
		strcpy(login->studentPW, tmp);
		printf("비밀번호가 변경 되었습니다.\n");
		wClientFile(Client);
		break;
	case 2:
		printf("바꾸실 주소를 입력해 주세요:");
		scanf("%s", &tmp);
		strcpy(login->studentAdrees, tmp);
		printf("주소가 변경 되었습니다.\n");
		wClientFile(Client);
		break;
	case 3:
		printf("바꾸실 번호를 입력해 주세요:");
		scanf("%s", &tmp);
		strcpy(login->studentPhonNumber, tmp);
		printf("번호가 변경 되었습니다.\n");
		wClientFile(Client);
		break;
	case 4:
		clientMenu(login);
	default:
		printf("1, 2, 3 중에서 선택해 주세요.");
		break;
	}
	clientMenu(login);
}

// 계정을 삭제하는 함수
void deletAccount(struct client* deletAccount)
{
	int deletMenuChoice;
	printf(">> 회원 탈퇴 <<\n");
	printf("정말 회원 탈퇴를 하시겠 습니까?\n1. 예           2. 아니요\n");
	scanf("%d", &deletMenuChoice);
	switch (deletMenuChoice)
	{
	case 1:
		deleteClientNode(deletAccount, Client);
		printf("회원 탈퇴가 완료 되었습니다.\n");
		wClientFile(Client);
		mainMenu();
		break;
	case 2:
		clientMenu(deletAccount);
		break;
	default:
		printf("1, 2, 3 중에서 선택해 주세요.\n");
		clientMenu(deletAccount);
		break;
	}
}

// 과리자 메뉴
int adminMenu()
{
	int adminMenuChoice;
	printf(">> 관리자 메뉴 <<\n");
	printf("1. 도서 등록           2. 도서 삭제\n3. 도서 대여           4. 도서 반납\n5. 도서 검색           6. 회원 목록\n7. 로그아웃            8. 프로그램 종료\n");
	scanf("%d", &adminMenuChoice);
	switch (adminMenuChoice)
	{
	case 1:
		bookInsert(Book);
		break;
	case 2:
		bookDelete(Book);
		break;
	case 3:
		bookBorrow(Book, Borrow, Client);
		break;
	case 4:
		bookReturn(Book, Borrow, Client);
		break;
	case 5:
		bookSearch(Book);
		mainMenu();
		break;
	case 6:
		clientList(Client);
		break;
	case 7:
		mainMenu();
		break;
	case 8:
		return 0;
		break;
	default:
		printf("1, 2, 3 중에서 선택해 주세요.");
		adminMenu();
		break;
	}
}

// 도서 등록 함수
void bookInsert(struct  book* book)
{
	int ID;
	char tmpData[5][MAX_STR_SIZE];
	unsigned long long ISBN;
	printf(">> 도서 등록 <<\n");
	printf("도서ID:");
	scanf("%d", &ID);
	printf("도서 이름:");
	scanf("%s", &tmpData[0]);
	printf("출판사:");
	scanf("%s", &tmpData[1]);
	printf("저자:");
	scanf("%s", &tmpData[2]);
	printf("ISBN:");
	scanf("%lld", &ISBN);
	printf("책 위치:");
	scanf("%s", &tmpData[3]);
	// 오름 차순으로 정렬되는 순서로 넣기위해 서치를 시작
	while (book)
	{
		if (ISBN < book->nextNode->ISBN)
		{
			insertBookNode(book, ID, tmpData[0], tmpData[1], tmpData[2], ISBN, tmpData[3], 'Y');
			wBookFile(Book);
			break;
		}
		book = book->nextNode;
	}
	printf("등록이 완료 되었습니다.\n");
	adminMenu();
}

//도서 삭제 함수
void bookDelete(struct book* deletebook)
{
	struct book* tmp = deletebook;
	int ID, bookDeleteMenuChoice;
	long long ISBN;
	char tmpData[MAX_STR_SIZE];
	printf(">> 도서 삭제 <<\n");
	printf("1. 도서명 검색       2. ISBN 검색\n");
	scanf("%d", &bookDeleteMenuChoice);
	switch (bookDeleteMenuChoice)
	{
	case 1:
		printf("도서명을 입력해 주세요:");
		scanf("%s", &tmpData);
		// 북 파일에서 서치의 시작
		while (deletebook)
		{
			if (strstr(deletebook->bookName, tmpData) != NULL)
			{
				printf("도서 ID: %d\n", deletebook->bookID);
				printf("도서명: %s\n", deletebook->bookName);
				printf("출판사: %s\n", deletebook->bookPub);
				printf("저자명: %s\n", deletebook->bookAuth);
				printf("ISBN: %lld\n", deletebook->ISBN);
				printf("소장처: %s\n", deletebook->bookLocate);
				printf("대여가능 여부: %c\n", deletebook->bookStat);
			}
			deletebook = deletebook->nextNode;
		}
		break;
	case 2:
		printf("ISBN을 입력해 주세요:");
		scanf("%lld", &ISBN);
		// 북 파일에서 서치의 시작
		while (deletebook)
		{
			if (deletebook->ISBN == ISBN)
			{
				printf("도서 ID: %d\n", deletebook->bookID);
				printf("도서명: %s\n", deletebook->bookName);
				printf("출판사: %s\n", deletebook->bookPub);
				printf("저자명: %s\n", deletebook->bookAuth);
				printf("ISBN: %lld\n", deletebook->ISBN);
				printf("소장처: %s\n", deletebook->bookLocate);
				printf("대여가능 여부: %c\n", deletebook->bookStat);
			}
			deletebook = deletebook->nextNode;
		}
		break;
	default:
		printf("1, 2, 3 중에서 선택해 주세요.");
		adminMenu();
		break;
	}

	printf("삭제할 도서의 번호를 입력해 주세요:");
	scanf("%d", &ID);
	while (tmp)
	{
		if (tmp->bookID == ID)
		{
			// 도서의 대여 가능여부가 N이면 삭제 불가능
			if (tmp->bookStat == 'N')
			{
				printf("이도서는 삭제할 수 없습니다.\n");
				break;
			}
			else
			{
				deleteBookNode(tmp, Book);
				wBookFile(Book);
				printf("삭제가 완료되었 습니다.\n");
				break;
			}
		}
		tmp = tmp->nextNode;
	}
	adminMenu();
}

// 책 대여 함수
void bookBorrow(struct book* book, struct borrow* borrow, struct client* client)
{
	struct book* tmp = book;
	int clientID, bookID, bookBorrowMenuChoice;
	long long ISBN;
	char tmpData[MAX_STR_SIZE];
	printf(">> 도서 대여 <<\n");
	printf("1. 도서명 검색       2. ISBN 검색\n");
	scanf("%d", &bookBorrowMenuChoice);
	switch (bookBorrowMenuChoice)
	{
	case 1:
		printf("도서명을 입력해 주세요:");
		scanf("%s", &tmpData);
		while (book)
		{
			if (strstr(book->bookName, tmpData) != NULL)
			{
				printf("도서 ID: %d\n", book->bookID);
				printf("도서명: %s\n", book->bookName);
				printf("출판사: %s\n", book->bookPub);
				printf("저자명: %s\n", book->bookAuth);
				printf("ISBN: %lld\n", book->ISBN);
				printf("소장처: %s\n", book->bookLocate);
				printf("대여가능 여부: %c\n", book->bookStat);
			}
			book = book->nextNode;
		}
		break;
	case 2:
		printf("ISBN을 입력해 주세요:");
		scanf("%lld", &ISBN);
		while (book)
		{
			if (book->ISBN == ISBN)
			{
				printf("도서 ID: %d\n", book->bookID);
				printf("도서명: %s\n", book->bookName);
				printf("출판사: %s\n", book->bookPub);
				printf("저자명: %s\n", book->bookAuth);
				printf("ISBN: %lld\n", book->ISBN);
				printf("소장처: %s\n", book->bookLocate);
				printf("대여가능 여부: %c\n", book->bookStat);
			}
			book = book->nextNode;
		}
		break;
	default:
		printf("1, 2, 3 중에서 선택해 주세요.");
		adminMenu();
		break;
	}
	printf("학번을 입력하세요:");
	scanf("%d", &clientID);
	while (client)
	{
		if (clientID == client->studentID)
		{
			printf("도서 번호를 입력하세요");
			scanf("%d", &bookID);
			while(tmp)
			{
				if (tmp->bookID == bookID)
				{
					time_t borrowed = time(NULL); // 시간을 받아옴
					time_t returnBook = borrowed + ONE_MONTH; // 받아온 시간에 1달만큼의 초를 더함

					insertBorrowNode(Borrow, clientID, bookID, borrowed, returnBook);
					wBorrowFile(Borrow);
					printf("도서가 대여 되었습니다.\n");
					adminMenu();
				}
				tmp = tmp->nextNode;
			}
			break;
		}
		client = client->nextNode;
	}
	printf("잘못 입력 하셨습니다.\n");
	adminMenu();
}

// 도서 반납 함수
void bookReturn(struct book* book, struct borrow* borrow, struct client* client)
{
	struct borrow* tmpBorrow = borrow;
	_Bool tmp = 1; // 대여목록이 존재 하는지 여부의 bool자료형
	int clientID, bookID;
	printf(">> 회원의 대여 목록 <<\n");
	printf("학번을 입력해 주세요:");
	scanf("%d", &clientID);

	struct tm* borrowData;
	struct tm* returnData;
	printf(">> 대여 목록 <<\n");
	while (borrow)
	{
		if (borrow->studentID == clientID)
		{
			printf("도서 번호:%d\n", borrow->bookID);
			while (book)
			{
				if (book->bookID == borrow->bookID)
				{
					printf("도서 명:%s\n", book->bookName);
					break;
				}
				book = book->nextNode;
			}
			tmp = 0;
			borrowData = localtime(&borrow->bookBorrow);
			printf("%d년 %d월 %d일 %s\n", borrowData->tm_year + 1900, borrowData->tm_mon + 1, borrowData->tm_mday, DayOfWeek[borrowData->tm_wday]);
			returnData = localtime(&borrow->returnBook);
			printf("%d년 %d월 %d일 %s\n", returnData->tm_year + 1900, returnData->tm_mon + 1, returnData->tm_mday, DayOfWeek[returnData->tm_wday]);
		}
		borrow = borrow->nextNode;
	}
	if (tmp == 0)
	{
		printf("반납할 도서번호를 입력 하세요:");
		scanf("%d", &bookID);
		while (tmpBorrow)
		{
			if (tmpBorrow->bookID == bookID)
			{
				printf("도서 반납이 완료되었 습니다.\n");
				deleteBorrowNode(tmpBorrow, Borrow);
				wBorrowFile(Borrow);
				adminMenu();
			}
			tmpBorrow = tmpBorrow->nextNode;
		}
	}
	else
	{
		printf("잘못 입력하셨습니다.\n");
		adminMenu();
	}
}

// 회원 검색 함수
void clientList(struct client* client)
{
	client = client->nextNode;
	int clientListMenuChoice, ID;
	char tmpData[MAX_STR_SIZE];
	printf(">> 회원 목록 <<\n");
	printf("1. 이름 검색		2. 학번 검색\n3. 전체 검색		4. 이전 메뉴");
	scanf("%d", &clientListMenuChoice);
	switch (clientListMenuChoice)
	{
	case 1:
		printf("이름을 입력해 주세요:");
		scanf("%s", &tmpData);
		while (client)
		{
			if (strstr(client->studentName, tmpData) != NULL)
			{
				printf("학번 ID: %d\n", client->studentID);
				printf("PW: %s\n", client->studentPW);
				printf("이름: %s\n", client->studentName);
				printf("주소: %s\n", client->studentAdrees);
				printf("번호: %s\n", client->studentPhonNumber);
			}
			client = client->nextNode;
		}
		break;
	case 2:
		printf("학번을 입력해 주세요:");
		scanf("%d", &ID);
		while (client)
		{
			if (client->studentID == ID)
			{
				printf("학번 ID: %d\n", client->studentID);
				printf("PW: %s\n", client->studentPW);
				printf("이름: %s\n", client->studentName);
				printf("주소: %s\n", client->studentAdrees);
				printf("번호: %s\n", client->studentPhonNumber);
			}
			client = client->nextNode;
		}
		break;
	case 3:
		while (client)
		{
			printf("학번 ID: %d\n", client->studentID);
			printf("PW: %s\n", client->studentPW);
			printf("이름: %s\n", client->studentName);
			printf("주소: %s\n", client->studentAdrees);
			printf("번호: %s\n", client->studentPhonNumber);
			client = client->nextNode;
		}
		break;
	case 4:
		adminMenu();
		break;
	default:
		printf("1, 2, 3 중에서 선택해 주세요.");
		adminMenu();
		break;
	}
	adminMenu();
}
