#include "stdafx.h"
#include"windows.h" //ListBox�ຯ������
#include "resource.h"
#include"stdio.h"
#include"windowsx.h"                       
#include"commdlg.h"
#define MAX_LOADSTRING 100
// Foward declarations of functions included in this code module://��������
BOOL				InitInstance(HINSTANCE, int);//���г����ʵ�� �Դ�����
LRESULT CALLBACK	LOADING(HWND, UINT, WPARAM, LPARAM);//��¼����������
LRESULT CALLBACK	CHATTING(HWND, UINT, WPARAM, LPARAM);//�������캯��������
HINSTANCE hInst;								// current instance���
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

struct member{//��Ա������Ϣ
	char name[25];
	char chattext[100];
	int flag;//����Ϊ600
	int flag2;//��½
	int flag3;//�˳�
}User,User_2;         //�Լ���ͨ�ñ���
char User_3[100][100];//�û�����
char tms[600];//����ǰ׺��ʾ
char Name[100];//��������
int _i,_p,n=1;//�жϷ���Ϣ�����Ƿ���������
HANDLE hMySlot;//
char MyComputerName[16];//��ȡ������
char Myaddress[100],address[100]; //����������//����������Ա����


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
	DWORD sz=sizeof(MyComputerName);
	GetComputerName(MyComputerName,&sz);	
	// Initialize global strings
	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) //��¼����Ի�����
{
   HWND hWnd;
   hInst = hInstance; // Store instance handle in our global variable
 hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	//������¼�Ի���������ȷ�����򴴽��ʼ��۲������������
    if(DialogBox(hInst,(LPCTSTR)IDD_INPUT_LOADING,hWnd,(DLGPROC)LOADING)==IDOK)
	{
		hMySlot=CreateMailslot(_T(Myaddress),0,MAILSLOT_WAIT_FOREVER,NULL);//�ʼ���
		DialogBox(hInst,(LPCTSTR)IDD_CHATROOM,hWnd,(DLGPROC)CHATTING);//�����������
	}
	else
		return FALSE;
     if (!hWnd)
   {
      return FALSE;
   }
   UpdateWindow(hWnd);
   return TRUE;
}


LRESULT CALLBACK LOADING(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)//��¼���� ��ȡ��Ϣ
{
	switch (message)
	{
		case WM_INITDIALOG:       //�ǶԻ�������յ�����Ϣ,�����Ի����������ӿؼ�����������ˡ�
				return TRUE;

		case WM_COMMAND:          //���û��Ӳ˵�ѡ��һ��������Ŀ����һ���ؼ�����֪ͨ��Ϣ��ȥ�����ڻ��߰���һ����ݼ������� WM_COMMAND ��Ϣ
			if(LOWORD(wParam) == IDOK)
			{

				GetDlgItemText(hDlg,IDC_USERNAME,User.name,32);//��ȡ�û��ǳ�
				GetDlgItemText(hDlg,IDC_LTSNAME,Name,32);//��ȡ����������
				sprintf(Myaddress,"//./mailslot/%s",Name);
				sprintf(address,"//*/mailslot/%s",Name);//�����ʼ���
				User.flag=600;//��ֵ��Ϊ600
				User.flag2=1;//���õ�¼��ֵ
				User.flag3=10;//���õǳ���ʶֵ
				strcpy(User_3[0],User.name);//�������û�����Ϣ���Ƶ������������ĵ�һ��
				EndDialog(hDlg, LOWORD(wParam));//�رնԻ���		
			}
			if (LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));//�رնԻ�����
				return TRUE;
			}
			break;
	}
    return FALSE;
}
LRESULT CALLBACK CHATTING(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)//��Ϣ����
{			
	
	HWND hWndEdit=GetDlgItem(hDlg,IDC_EDIT1);//��ȡ�༭���һ�����þ��
	HWND hWndList;
	int nItem,k;//����Ϊ��˫�����λ�ã�˫��λ������
	int del;//ɾ���ͻ��õ�����
	char getname[33];          //˫����ȡ����
	char secret[100],list[100],msgg1[100],msgg2[100];//msgg1 ������ʾ�³�Ա����
	SetTimer(hDlg,1,50,NULL);//��ʱ��1���趨����������ۣ�
	//SetTimer(hDlg,2,2*60*1000,NULL);//��ʱ��2���趨
	//SetTimer(hDlg,3,1000,NULL);//��ʱ��3���趨(�ݼ���
	switch (message)
	{
		case WM_INITDIALOG://�Ի��򴴽� ��ϲ�д���������Ա
			{
				DWORD Writted;
				HANDLE hMailslot=CreateFile(_T(address),GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);//����д���������ʼ��۵����þ��
				WriteFile(hMailslot,&User,sizeof(User),&Writted,NULL);//д��
				SendDlgItemMessage(hDlg,IDC_NAMELIST,LB_ADDSTRING,NULL,(long)User.name);//���һ����Ա���Լ���
				CloseHandle(hMailslot);//�ر��ʼ���
			}
				return TRUE;
		case WM_TIMER://�����������Ϣ
				{
					if(wParam==1) //�����ʱ��1��50ms��ʱ�䵽��//��Ϣ��Ӧ����
					{
				    SYSTEMTIME tm;//ʱ��
					HWND hWnd;
					DWORD msgSize,nextMsgSize,Unreaded,readed;
					GetLocalTime(&tm);//��ȡʱ��
					while(GetMailslotInfo(hMySlot,&msgSize,&nextMsgSize,&Unreaded,NULL) && Unreaded>0)//�������Ϣ����
					{
					ReadFile(hMySlot,&User_2,sizeof(User_2),&readed,NULL);//�ӿͻ���������
					//���û�������������
					if(User_2.chattext[0])
					{
					sprintf(tms,"%d:%d:%d ��%s��˵:%s",tm.wHour,tm.wMinute,tm.wSecond,User_2.name,User_2.chattext);//���ϳ�һ��tms
					SendDlgItemMessage(hDlg,IDC_CHAT,LB_ADDSTRING,NULL,(long)tms);//tms��������촰��
					}
					//�ж����û��������Ƿ��Ѵ���
					//���У�iniΪ�Ƿ��ظ��ı�־��pt��n��Ϊ�жϱ������������ƴ���
					for(_i=0,_p=0;_p<n;_p++)
					{
						if(strcmp(User_2.name,User_3[_p])==0)
							_i++;
					}
					if(_i==0)//���û�д����֣�����ӵ�������
					{
						strcpy(User_3[_p],User_2.name);//����
						SendDlgItemMessage(hDlg,IDC_NAMELIST,LB_ADDSTRING,NULL,(long)User_3[_p]);//���
						n++;//������Ҫ�Ƚϼ���
					}
					//������¼��Ϣ
					if(User_2.flag==600&&User_2.flag2==1) //�ս�������û������
					{
						sprintf(msgg1,"�û� %s �ѵ�½",User_2.name);//����
						MessageBox(hDlg,msgg1,"Login",MB_OK|MB_ICONEXCLAMATION);//���
						User.flag2=2;
					}
					//�����ǳ���Ϣ
					if(User_2.flag==0||User_2.flag3==1000) //����Ƿ����߻����˳�
					{
						hWndList=GetDlgItem(hDlg,IDC_NAMELIST);//��ȡ�����Ա
						del=ListBox_FindString(hWndList,-1,User_2.name);   //�ҵ�����û�
						ListBox_DeleteString(hWndList,del);                //ɾ�����û�
						sprintf(msgg2,"�û� %s �������˵�����ر������촰��",User_2.name); //��ʾ�û��˳�
						MessageBox(hDlg,msgg2,"LogOut",MB_OK|MB_ICONEXCLAMATION);
						User.flag3=100;//����flag3��ֵ����������
					}
					}
					}//
				}
				break;

		case WM_COMMAND://������
			if(LOWORD(wParam)==IDC_NAMELIST)//˽����BUG
			{
				if(HIWORD(wParam)==LBN_DBLCLK)//��Ӧ˫���¼�
				{
					hWndEdit=GetDlgItem(hDlg,IDC_EDIT1);
					hWndList=GetDlgItem(hDlg,IDC_NAMELIST);//��ȡ�б���һ�����þ��
				    nItem=SendMessage(hWndEdit,LB_GETCURSEL,0,0);//��ȡ˫�����λ��
					k=SendDlgItemMessage(hDlg,IDC_NAMELIST,LB_GETCURSEL,0,0);//��ȡ˫���������
					ListBox_GetText(hWndList,k,getname);//������windowsx.h���棬��ȡ˫�������������ַ�����Ҳ�����û���
					sprintf(secret,">[%s]",getname);
					SetWindowText(hWndEdit,secret);//��>[�û�]��������ʾ���������
				}
				
			}
			if(LOWORD(wParam) == IDOK)//����˷��ͣ����»س���������Ϣ
			{
				DWORD Writted;
				DWORD msgSize,nextMsgSize,Unreaded,readed;
				SYSTEMTIME tm;
				GetLocalTime(&tm);
				GetDlgItemText(hDlg,IDC_EDIT1,User.chattext,32);//��ȡ���
				HANDLE hMailslot=CreateFile(_T(address),GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);//����д���������ʼ��۵����þ��
				WriteFile(hMailslot,&User,sizeof(User),&Writted,NULL);//д��
				SetWindowText(hWndEdit,"");//��������
				CloseHandle(hMailslot);//�ر��ʼ���
			}
			if (LOWORD(wParam) == IDCANCEL)
			{
				DWORD Writted;
				User.flag3=1000;       //�˳�ʱ�޸Ĳ���flag3
				HANDLE hMailslot=CreateFile(_T(address),GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);//����д���������ʼ��۵����þ��
				WriteFile(hMailslot,&User,sizeof(User),&Writted,NULL);//д��
				CloseHandle(hMailslot);//�ر��ʼ���
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
