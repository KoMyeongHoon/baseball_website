#include <windows.h>
#include <tchar.h>
#include "resource.h" //20210524--
//★★★ ● 7장 MDI 프로그래밍<기본구조> 
// : WinMain()  +  FrameWndProc()  +  ChlidWndProc() ★★★

LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
//ㄴ 20210524--1 //FrameWndProc()정의

LRESULT CALLBACK ChildWndProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
//ㄴ 20210524--2 //ChildWndProc()정의

HINSTANCE hInst; //20210524--24

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,     //응용프로그램 인스턴스 핸들  
	LPTSTR lpszCmdLine, int nCmdShow) //->wWinMain(), WinMain()
{                   //윈도우 출력스타일
	HWND hwnd;
	MSG msg;
	hInst = hInstance; //20210524--25

	WNDCLASSEX wcex; //20210524--

	//1) 프레임 윈도우 클래스 ***생성***과정●●●●●●●●●●●●●●●●
	wcex.cbSize = sizeof(WNDCLASSEX); //구조체 크기
	wcex.style = CS_HREDRAW | CS_VREDRAW; //윈도우 크기변경시 다시그리는 모드
	
	wcex.lpfnWndProc = FrameWndProc; //★★★★★
	//ㄴ 20210524--14 //ㄴ 프레임 윈도우 프로시저 함수이름

	wcex.cbClsExtra = 0; //클래스 여분 메모리
	wcex.cbWndExtra = 0; //윈도우 여분 메모리
	wcex.hInstance = hInstance; //응용프로그램 인스턴스핸들
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION); //아이콘
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW); //커서
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //배경색
	
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1); 
	//메뉴이름 //20210524--
	
	wcex.lpszClassName = _T("FrameWindow"); //★★★★★
	//ㄴ 20210524--15 //프레임윈도우 클래스이름

	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION); //작은아이콘
	//1) 프레임 윈도우 클래스 ***생성***과정●●●●●●●●●●●●●●●●


	//2) 프레임 윈도우 클래스 ***등록***과정●●●●●●●●●●●●●●●● 
	RegisterClassEx(&wcex); //20210524--



	//Child윈도우 클래스 ***생성***●●●●●●●●●●●●●●●●
	wcex.lpfnWndProc = ChildWndProc; //20210524--16
	wcex.lpszClassName = _T("ChildWindow"); 
	//ㄴ 20210524--17 //ㄴ Child윈도우 클래스이름
	
	//Child윈도우의 클래스 ***등록***●●●●●●●●●●●●●●●●
	RegisterClassEx(&wcex); //20210524--18


	//3) 프레임(Frame) 윈도우 (창) 생성 (객체생성)(=창 생성 및 화면에 띄우기)  
	//프레임&클래스이름/타이틀/스타일/x,y좌표/위치 등등이 있음.
	//ㄴ 20210524--19
	hwnd = CreateWindow(_T("FrameWindow"), _T("프레임윈도우"), 
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	// 위에 있는 39,40번째줄 : wcex.lpszClassName = _T("FrameWindow"); 
//에서 _T안에 문자열=즉,제목 완전100%같아야함!!!,,,안그러면 창안뜸!! 
	//CW_USEDEFAULT차례대로 = X좌표/Y좌표/Width/Height
	//NULL차례대로 = 부모창의핸들/메뉴/


	ShowWindow(hwnd, nCmdShow); 
	//SW_SHOW, SW_HIDE  //20210524--
	UpdateWindow(hwnd);


	//2. 메시지 루프
	while (GetMessage(&msg, NULL, 0, 0)) //WM_QUIT 들어오면 0반환 ->종료
	{
		TranslateMessage(&msg); //키보드메시지처리
		DispatchMessage(&msg); //WndProc로 메시지 전달
	}

	return (int)msg.wParam;
}

//*** 기존에있던 WndProc()함수 삭제해야함!!

//*** 기본소스 (맨처음) 실행 : SDI(창 1개만 나옴)



//20210524--3 //FrameWndProc()호출
LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndClient; 
	//20210524--8 //ㄴ 클라이언트 창의 윈도우 핸들 저장변수 

	CLIENTCREATESTRUCT clientcreate; //20210524--20 
	// ㄴ멤버변수2개 = HANDLE hWindowMenu; / UINT idFirstChild;
	//ㄴ클라이언트 윈도우 생성시 필요한 구조체
	
	static HWND hwndChild; 
	//20210531--1 //Chlid윈도우 핸들 저장변수
	MDICREATESTRUCT mdicreate; 
	//20210531--2 //Child윈도우생성시필요
	

	switch (iMsg) //20210524--5
	{
	case WM_CREATE: 
		//프레임 윈도우 창 생성시 발생 메시지 //20210524--6
		clientcreate.hWindowMenu = GetSubMenu(GetMenu(hwnd), 0);
		//ㄴ창에달려있는 메뉴전체를갖고온다!!(=그중에서 0번지=1번째)
		//20210524--21 //부모Frame윈도우의 Menu핸들 얻어오기
		clientcreate.idFirstChild = 100; 
		//20210524--22 //1번째Child윈도우ID 정해주기
		//***CLIENTCREATESTRUCT 구조체 매개변수 2개***
		
		//클라이언트 윈도우 생성 및 출력 //20210524--23
		hwndClient = CreateWindow(_T("MDICLIENT"), NULL,
			WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hwnd, NULL,
			hInst, &clientcreate);
		//ㄴ 20210524-- //★★★무조건 MDICLIENT = "대문자"로 써야한다
//hwnd=부모핸들/메뉴X/hInst/CLIENTCREATESTRUCT구조체변수주소값

		ShowWindow(hwndClient, SW_SHOW); 
		//20210524--26 //화면출력

		break;

	case WM_COMMAND: //20210524--27 & 20210531
		switch (LOWORD(wParam)) //20210524--28 & 20210531--
		{
		case ID_WINDOW_CLOSE: 
			//활성화된 창 종료 //20210531--14
			
			//1) 활성화된 창 핸들 얻어오기 //20210531--17
			hwndChild = (HWND)SendMessage(hwndClient, WM_MDIGETACTIVE, 0, 0); 
			
			//2) 해당 Child창에 종료가능한지 질의 //20210531--18
			if (SendMessage(hwndChild, WM_QUERYENDSESSION, 0, 0))
				//3) (활성화 되있는)해당 창 종료 //20210531--19
				SendMessage(hwndClient, WM_MDIDESTROY, (WPARAM)hwndChild, 0);
			break;

		case ID_TILE: //20210531--15
			SendMessage(hwndClient, WM_MDITILE, 0, 0); 
			//ㄴ 20210531--20
			break;

		case ID_CASCADE: //20210531--16
			SendMessage(hwndClient, WM_MDICASCADE, 0, 0); 
			//ㄴ 20210531--21
			break;

		case ID_FILENEW: //20210524--29 & 20210531--3
			//새문서 메뉴를 선택하면 Child윈도우 생성화면 출력
			mdicreate.szClass = _T("ChildWindow"); 
			//20210531--4 //위에있는거랑 똑같이써야함

			mdicreate.szTitle = _T("자식 윈도우 타이틀"); 
			//20210531--5

			mdicreate.x = CW_USEDEFAULT; 
			//20210531--6

			mdicreate.y = CW_USEDEFAULT; 
			//20210531--7

			mdicreate.cx = CW_USEDEFAULT; 
			//20210531--8

			mdicreate.cy = CW_USEDEFAULT; 
			//20210531--9

			mdicreate.hOwner = hInst;
			//20210531--10

			mdicreate.style = 0; 
			//20210531--11

			mdicreate.lParam = 0; 
			//20210531--12

			hwndChild = (HWND)SendMessage(hwndClient, WM_MDICREATE, 0, (LPARAM)&mdicreate);
			//20210531--13 //FrameWndProc() 윗부분에 보면 
			//HWND형 static HWND hwndClient; 으로 선언되어있으므로 HWND형으로 형변환

			break;
		}
		break;
	}

	return DefFrameProc(hwnd, hwndClient, iMsg, wParam, lParam); 
	//20210524--9

}


//20210524--4 //ChildWndProc()호출
LRESULT CALLBACK ChildWndProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg) //20210524--10
	{
	case WM_CREATE: //20210524--11
		break;
	case WM_DESTROY: //20210524--12
		break;
	}

	

	return DefMDIChildProc(hDlg, iMsg, wParam, lParam); 
	//20210524--13
}















