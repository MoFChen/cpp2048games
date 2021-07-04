#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#define JG	10

UINT score=0;
UINT color_mat[13]={0xFFCDC1B4,0xFFEDE4DA,0xFFEDE0C8,0xFFF2B179,0xFFF59563,0xFFF67C5F,0xFFF65E3B,0xFFEDCF72,0xFFEDCC61,0xFFEDC850,0xFFEDC53F,0xFFEDC22E,0xFF776F65};

typedef struct myMAT
{
    PINT p;
    INT  n;
}s_MAT,*MAT;

BOOL gameover=false;
MAT g_MAT,gc_MAT;

VOID getColor(UINT number,UINT *cmat,UINT *ctext)
{
	if(cmat==NULL||ctext==NULL)return;
	UINT i=0,temp=number;
	while(number)i++,number>>=1;
	if(i==0)*cmat=color_mat[0],*ctext=0x00000000;
	else
	{
		*cmat  = (i<=11)?color_mat[i-1]:color_mat[12];
		*ctext = (i<= 3)?0xFF776E65:0xFFF9F6F2;
	}
}

MAT matInit(INT n)
{
    if(n<2)return NULL;
    MAT mat=(MAT)malloc(sizeof(s_MAT));
    if(mat==NULL)return NULL;
    INT l=n*n;
    mat->p=(PINT)malloc(l*sizeof(INT));
    if(mat->p==NULL)
    {
        free(mat);
        return NULL;
    }
    INT i;
    for(i=0;i<l;i++)mat->p[i]=0;
    mat->n=n;
    return mat;
}
VOID matFree(MAT mat)
{
    if(mat!=NULL)
    {
        if(mat->p!=NULL)free(mat->p);
        free(mat);
    }
}

INT matRand(MAT mat)
{
    if(mat==NULL)return 0;
    INT i,l=mat->n*mat->n,s1,s2,r;
    for(i=0,s1=0;i<l;i++)if(mat->p[i]==0)s1++;
    if(s1==0)return s1;
    srand((unsigned int)clock());
    r=rand()%s1;
    for(i=0,s2=0;i<l;i++)
    {
        if(mat->p[i]==0)
        {
            if(s2==r)
            {
                r=rand()&0x00000001,r++,r<<=1;
                mat->p[i]=r;
                break;
            }
            else s2++;
        }
    }
    return s1-1;
}
INT matIsOver(MAT mat)
{
    if(mat==NULL)return 0;
    INT i,j,n=mat->n,m=n-1;
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            if(mat->p[i*n+j]==mat->p[i*n+j+1])return 0;
        }
        if(i!=m)
        {
            for(j=0;j<n;j++)
            {
                if(mat->p[i*n+j]==mat->p[(i+1)*n+j])return 0;
            }
        }
    }
    return 1;
}
INT matEqual(MAT mat1,MAT mat2)
{
    if(mat1==NULL||mat2==NULL)return 0;
    if(mat1->n!=mat2->n)return 0;
    INT i,l=mat1->n*mat1->n;
    for(i=0;i<l;i++)if(mat1->p[i]!=mat2->p[i])return 0;
    return 1;
}
INT matCopy(MAT mat1,MAT mat2)
{
    if(mat1==NULL||mat2==NULL)return 0;
    if(mat1->n!=mat2->n)return 0;
    INT i,l=mat1->n*mat1->n;
    for(i=0;i<l;i++)mat1->p[i]=mat2->p[i];
    return 1;
}

INT matPlus(PINT s,INT n)
{
    if(n<2)return 0;
    INT i,j,m=n-1;
    for(i=0;i<m;i++)
    {
        if(s[i]==0)
        {
            for(j=i+1;j<n;j++)
            {
                if(s[j]!=0)
                {
                    s[i]=s[j],s[j]=0;
                    break;
                }
            }
        }
        if(s[i]!=0)
        {
            for(j=i+1;j<n;j++)
            {
                if(s[j]==0)continue;
                else
                {
                    if(s[i]==s[j])
                    {
                        s[i]<<=1,s[j]=0,score+=s[i];
                    }
                    else break;
                }
            }
        }
    }
    return 1;
}
INT matUp(MAT mat)
{
    if(mat==NULL)return 0;
    INT i,j,n=mat->n;
    INT s[n]={0};
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)s[j]=mat->p[i+j*n];
        matPlus(s,n);
        for(j=0;j<n;j++)mat->p[i+j*n]=s[j];
    }
    return 1;
}
INT matDown(MAT mat)
{
    if(mat==NULL)return 0;
    INT i,j,n=mat->n,r;
    INT s[n]={0};
    for(i=0;i<n;i++)
    {
        for(j=n-1,r=0;j>=0;j--)s[r++]=mat->p[i+j*n];
        matPlus(s,n);
        for(j=n-1,r=0;j>=0;j--)mat->p[i+j*n]=s[r++];
    }
    return 1;
}
INT matLeft(MAT mat)
{
    if(mat==NULL)return 0;
    INT i,j,n=mat->n;
    INT s[n]={0};
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)s[j]=mat->p[i*n+j];
        matPlus(s,n);
        for(j=0;j<n;j++)mat->p[i*n+j]=s[j];
    }
    return 1;
}
INT matRight(MAT mat)
{
    if(mat==NULL)return 0;
    INT i,j,n=mat->n,r;
    INT s[n]={0};
    for(i=0;i<n;i++)
    {
        for(j=n-1,r=0;j>=0;j--)s[r++]=mat->p[i*n+j];
        matPlus(s,n);
        for(j=n-1,r=0;j>=0;j--)mat->p[i*n+j]=s[r++];
    }
    return 1;
}

VOID OnPaint(HWND hwnd,HDC hdc)
{
	RECT        rect;
	Gdiplus::Graphics		g(hdc);
    Gdiplus::SolidBrush 	solidBrush(Color(255, 187, 173, 160));
	Gdiplus::FontFamily 	fontFamily(L"微软雅黑");
	Gdiplus::Font 			font (&fontFamily, 24, FontStyleBold, UnitPixel);
	Gdiplus::Font 			font2(&fontFamily, 18, FontStyleBold, UnitPixel);
	Gdiplus::StringFormat 	stringformat;
	stringformat.SetAlignment(StringAlignmentCenter);
	stringformat.SetLineAlignment(StringAlignmentCenter);
    GetClientRect(hwnd,&rect);
	INT	x0=rect.right/2-200,y0=rect.bottom/2-200+21;
    g.FillRectangle(&solidBrush,x0,y0,400,400);
	solidBrush.SetColor(Color(255,119,110,101));
	g.DrawString(L"Score:",-1,&font,RectF(x0,y0-32,90,32),&stringformat,&solidBrush);
	CHAR 	str1[12]={0};
	WCHAR 	str2[12]={0};
	INT 	len;
	len	= sprintf_s(str1,11,"%u",score);
	len = mbstowcs_s(NULL,str2,11,str1,len);
	solidBrush.SetColor(Color(255,245,149,99));
	g.DrawString(str2,wcslen(str2),&font,RectF(x0+90,y0-32,180,32),&stringformat,&solidBrush);
	if(g_MAT==NULL)return;
	INT	N=g_MAT->n,a=(400-(N+1)*JG)/N,i,j;
	UINT cmat,ctext;
	for(i=0;i<N;i++)
	{
		for(j=0;j<N;j++)
		{
			rect.top=(i+1)*JG+i*a+y0;
			rect.left=(j+1)*JG+j*a+x0;
			getColor(g_MAT->p[i*N+j],&cmat,&ctext);
			solidBrush.SetColor(cmat);
			g.FillRectangle(&solidBrush,rect.left,rect.top,a,a);
			solidBrush.SetColor(ctext);
			len	= sprintf_s(str1,11,"%u",g_MAT->p[i*N+j]);
			len = mbstowcs_s(NULL,str2,11,str1,len);
			g.DrawString(str2,wcslen(str2),&font2,RectF(rect.left,rect.top,a,a),&stringformat,&solidBrush);
		}
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	HDC			hdc;
	PAINTSTRUCT	ps;
	MINMAXINFO *mminfo;
	RECT        rect;
	INT 		s;
	switch(Message)
	{
		case WM_KEYDOWN:
			if(gameover==true)MessageBoxA(hwnd,"Game Over!","Sorry~",MB_OK);
			if(wParam==VK_UP)matUp(gc_MAT);
			else if(wParam==VK_DOWN)matDown(gc_MAT);
			else if(wParam==VK_LEFT)matLeft(gc_MAT);
			else if(wParam==VK_RIGHT)matRight(gc_MAT);
			else break;
			if(matEqual(gc_MAT,g_MAT))
			{
				matCopy(gc_MAT,g_MAT);
				break;
			}
			s=matRand(gc_MAT);
			if(s==0)if(matIsOver(gc_MAT))gameover=true;
			matCopy(g_MAT,gc_MAT);
			InvalidateRect(hwnd,NULL,TRUE);
			break;
		case WM_PAINT:
        	hdc = BeginPaint(hwnd,&ps);
        	OnPaint(hwnd,hdc);
        	EndPaint(hwnd,&ps);
			break;
		case WM_GETMINMAXINFO:				//限制最小窗口尺寸
			mminfo=(PMINMAXINFO)lParam;
			mminfo->ptMinTrackSize.x=416;
			mminfo->ptMinTrackSize.y=480;
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	HWND                hWnd;
    MSG                 msg;
    WNDCLASS            wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	g_MAT=matInit(4);
	if(g_MAT==NULL)goto Error;
	gc_MAT=matInit(4);
	if(gc_MAT==NULL)
	{
		matFree(g_MAT);
		goto Error;
	}
	matRand(g_MAT);
	matCopy(gc_MAT,g_MAT);

    wndClass.style          = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc    = WndProc;
    wndClass.cbClsExtra     = 0;
    wndClass.cbWndExtra     = 0;
    wndClass.hInstance      = hInstance;
    wndClass.hIcon          = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName   = NULL;
    wndClass.lpszClassName  = TEXT("2048Games");

    RegisterClass(&wndClass);

    hWnd = CreateWindow(TEXT("2048Games"),TEXT("2048 Games"),WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,360,480,NULL,NULL,hInstance,NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
	matFree(g_MAT);
	matFree(gc_MAT);
	Error:
    GdiplusShutdown(gdiplusToken);
    return msg.wParam;
}
