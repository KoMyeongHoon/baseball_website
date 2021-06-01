#include <windows.h>
#include <tchar.h>
#include "resource.h" //20210524--
//�ڡڡ� �� 7�� MDI ���α׷���<�⺻����> 
// : WinMain()  +  FrameWndProc()  +  ChlidWndProc() �ڡڡ�

LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
//�� 20210524--1 //FrameWndProc()����

LRESULT CALLBACK ChildWndProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
//�� 20210524--2 //ChildWndProc()����

HINSTANCE hInst; //20210524--24

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,     //�������α׷� �ν��Ͻ� �ڵ�  
	LPTSTR lpszCmdLine, int nCmdShow) //->wWinMain(), WinMain()
{                   //������ ��½�Ÿ��
	HWND hwnd;
	MSG msg;
	hInst = hInstance; //20210524--25

	WNDCLASSEX wcex; //20210524--

	//1) ������ ������ Ŭ���� ***����***�����ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	wcex.cbSize = sizeof(WNDCLASSEX); //����ü ũ��
	wcex.style = CS_HREDRAW | CS_VREDRAW; //������ ũ�⺯��� �ٽñ׸��� ���
	
	wcex.lpfnWndProc = FrameWndProc; //�ڡڡڡڡ�
	//�� 20210524--14 //�� ������ ������ ���ν��� �Լ��̸�

	wcex.cbClsExtra = 0; //Ŭ���� ���� �޸�
	wcex.cbWndExtra = 0; //������ ���� �޸�
	wcex.hInstance = hInstance; //�������α׷� �ν��Ͻ��ڵ�
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION); //������
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW); //Ŀ��
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //����
	
	wcex.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1); 
	//�޴��̸� //20210524--
	
	wcex.lpszClassName = _T("FrameWindow"); //�ڡڡڡڡ�
	//�� 20210524--15 //������������ Ŭ�����̸�

	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION); //����������
	//1) ������ ������ Ŭ���� ***����***�����ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�


	//2) ������ ������ Ŭ���� ***���***�����ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ� 
	RegisterClassEx(&wcex); //20210524--



	//Child������ Ŭ���� ***����***�ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	wcex.lpfnWndProc = ChildWndProc; //20210524--16
	wcex.lpszClassName = _T("ChildWindow"); 
	//�� 20210524--17 //�� Child������ Ŭ�����̸�
	
	//Child�������� Ŭ���� ***���***�ܡܡܡܡܡܡܡܡܡܡܡܡܡܡܡ�
	RegisterClassEx(&wcex); //20210524--18


	//3) ������(Frame) ������ (â) ���� (��ü����)(=â ���� �� ȭ�鿡 ����)  
	//������&Ŭ�����̸�/Ÿ��Ʋ/��Ÿ��/x,y��ǥ/��ġ ����� ����.
	//�� 20210524--19
	hwnd = CreateWindow(_T("FrameWindow"), _T("������������"), 
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	// ���� �ִ� 39,40��°�� : wcex.lpszClassName = _T("FrameWindow"); 
//���� _T�ȿ� ���ڿ�=��,���� ����100%���ƾ���!!!,,,�ȱ׷��� â�ȶ�!! 
	//CW_USEDEFAULT���ʴ�� = X��ǥ/Y��ǥ/Width/Height
	//NULL���ʴ�� = �θ�â���ڵ�/�޴�/


	ShowWindow(hwnd, nCmdShow); 
	//SW_SHOW, SW_HIDE  //20210524--
	UpdateWindow(hwnd);


	//2. �޽��� ����
	while (GetMessage(&msg, NULL, 0, 0)) //WM_QUIT ������ 0��ȯ ->����
	{
		TranslateMessage(&msg); //Ű����޽���ó��
		DispatchMessage(&msg); //WndProc�� �޽��� ����
	}

	return (int)msg.wParam;
}

//*** �������ִ� WndProc()�Լ� �����ؾ���!!

//*** �⺻�ҽ� (��ó��) ���� : SDI(â 1���� ����)



//20210524--3 //FrameWndProc()ȣ��
LRESULT CALLBACK FrameWndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hwndClient; 
	//20210524--8 //�� Ŭ���̾�Ʈ â�� ������ �ڵ� ���庯�� 

	CLIENTCREATESTRUCT clientcreate; //20210524--20 
	// ���������2�� = HANDLE hWindowMenu; / UINT idFirstChild;
	//��Ŭ���̾�Ʈ ������ ������ �ʿ��� ����ü
	
	static HWND hwndChild; 
	//20210531--1 //Chlid������ �ڵ� ���庯��
	MDICREATESTRUCT mdicreate; 
	//20210531--2 //Child������������ʿ�
	

	switch (iMsg) //20210524--5
	{
	case WM_CREATE: 
		//������ ������ â ������ �߻� �޽��� //20210524--6
		clientcreate.hWindowMenu = GetSubMenu(GetMenu(hwnd), 0);
		//��â���޷��ִ� �޴���ü������´�!!(=���߿��� 0����=1��°)
		//20210524--21 //�θ�Frame�������� Menu�ڵ� ������
		clientcreate.idFirstChild = 100; 
		//20210524--22 //1��°Child������ID �����ֱ�
		//***CLIENTCREATESTRUCT ����ü �Ű����� 2��***
		
		//Ŭ���̾�Ʈ ������ ���� �� ��� //20210524--23
		hwndClient = CreateWindow(_T("MDICLIENT"), NULL,
			WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hwnd, NULL,
			hInst, &clientcreate);
		//�� 20210524-- //�ڡڡڹ����� MDICLIENT = "�빮��"�� ����Ѵ�
//hwnd=�θ��ڵ�/�޴�X/hInst/CLIENTCREATESTRUCT����ü�����ּҰ�

		ShowWindow(hwndClient, SW_SHOW); 
		//20210524--26 //ȭ�����

		break;

	case WM_COMMAND: //20210524--27 & 20210531
		switch (LOWORD(wParam)) //20210524--28 & 20210531--
		{
		case ID_WINDOW_CLOSE: 
			//Ȱ��ȭ�� â ���� //20210531--14
			
			//1) Ȱ��ȭ�� â �ڵ� ������ //20210531--17
			hwndChild = (HWND)SendMessage(hwndClient, WM_MDIGETACTIVE, 0, 0); 
			
			//2) �ش� Childâ�� ���ᰡ������ ���� //20210531--18
			if (SendMessage(hwndChild, WM_QUERYENDSESSION, 0, 0))
				//3) (Ȱ��ȭ ���ִ�)�ش� â ���� //20210531--19
				SendMessage(hwndClient, WM_MDIDESTROY, (WPARAM)hwndChild, 0);
			break;

		case ID_TILE: //20210531--15
			SendMessage(hwndClient, WM_MDITILE, 0, 0); 
			//�� 20210531--20
			break;

		case ID_CASCADE: //20210531--16
			SendMessage(hwndClient, WM_MDICASCADE, 0, 0); 
			//�� 20210531--21
			break;

		case ID_FILENEW: //20210524--29 & 20210531--3
			//������ �޴��� �����ϸ� Child������ ����ȭ�� ���
			mdicreate.szClass = _T("ChildWindow"); 
			//20210531--4 //�����ִ°Ŷ� �Ȱ��̽����

			mdicreate.szTitle = _T("�ڽ� ������ Ÿ��Ʋ"); 
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
			//20210531--13 //FrameWndProc() ���κп� ���� 
			//HWND�� static HWND hwndClient; ���� ����Ǿ������Ƿ� HWND������ ����ȯ

			break;
		}
		break;
	}

	return DefFrameProc(hwnd, hwndClient, iMsg, wParam, lParam); 
	//20210524--9

}


//20210524--4 //ChildWndProc()ȣ��
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















