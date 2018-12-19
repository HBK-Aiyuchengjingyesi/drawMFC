// termprjView.cpp : implementation of the CTermprjView class
//

#include "stdafx.h"
#include "termprj.h"

#include "termprjDoc.h"
#include "termprjView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTermprjView

IMPLEMENT_DYNCREATE(CTermprjView, CView)

BEGIN_MESSAGE_MAP(CTermprjView, CView)
	//{{AFX_MSG_MAP(CTermprjView)
	ON_COMMAND(ID_Drawline, OnDrawline)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTermprjView construction/destruction

CTermprjView::CTermprjView()
{
	// TODO: add construction code here
	//��ʼ��
	m_drawline=false;  
	m_drawarc=false;
	m_capture=false;
	m_inrect=false;
	num = 0;
}

CTermprjView::~CTermprjView()
{
}

BOOL CTermprjView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTermprjView drawing

void CTermprjView::OnDraw(CDC* pDC)
{
	CTermprjDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CTermprjView printing

BOOL CTermprjView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTermprjView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTermprjView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTermprjView diagnostics

#ifdef _DEBUG
void CTermprjView::AssertValid() const
{
	CView::AssertValid();
}

void CTermprjView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTermprjDoc* CTermprjView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTermprjDoc)));
	return (CTermprjDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTermprjView message handlers


//��ֱ��
void CTermprjView::OnDrawline() 
{
	// TODO: Add your command handler code here
	m_drawline = true;  //������ֱ��ģʽ
	m_capture = false;  //mousemove�еĶ�̬���߲���Ч�������������Ч
}


void CTermprjView::OnMouseMove(UINT nFlags, CPoint point) 
{
	//ÿ���ƶ����ж��Ƿ����Զ�����������
	for(int i=0; i<rects.size(); i++)
	{
		BOOL flag = rects[i].PtInRect(point);  //point�Ƿ���rect������
		if (flag){//��point�ڹ���ľ���������;
			//AfxMessageBox(_T("�������ڣ�"));
			m_inrect = true;
			temp = rects[i];
			HCURSOR cursor = LoadCursor(NULL,IDC_CROSS);
			::SetCursor(cursor);//��������ó�ʮ��
			SetClassLong(this->GetSafeHwnd(),GCL_HCURSOR,(LONG)LoadCursor(NULL, IDC_CROSS));//ʹ����ڸ���������˸
			break;
		}
		 else
		 {
			 m_inrect = false;
			  //����뿪������ָ�Ĭ�ϼ�ͷ��״
			 SetClassLong(this->GetSafeHwnd(),GCL_HCURSOR,(LONG)LoadCursor(NULL, IDC_ARROW));
		 }

	}

	if (m_capture){  //�Ѿ����˵�һ����
		
		BOOL bCtrl = GetKeyState(VK_CONTROL) & 0x8000; //�ж�ctrl�Ƿ񱻰���
		CClientDC  ClientDC(this); //this = ������
		CPen pen;
		pen.CreatePen(PS_SOLID,3,0x00ffffff-RGB(0,0,255));    //����������ɫ����������ɫ���ķ�ɫ����
		ClientDC.SelectObject(&pen);	
		ClientDC.SetROP2(R2_XORPEN);   //����ģʽΪ���ʵķ�ɫ
		
		//���ɵ���
		ClientDC.MoveTo(beginPt.x,beginPt.y);  
		ClientDC.LineTo(endPt.x,endPt.y); 
		if(bCtrl)   //ctrl������
		{
			if(abs(point.x-beginPt.x)>abs(point.y-beginPt.y))  //���λ����������ӣ�С��45����ˮƽ��
			{
				endPt.x = point.x; 
				endPt.y = beginPt.y;
			}
			else   //���λ����������ӣ�����45������ֱ��
			{
				endPt.y = point.y; 
				endPt.x = beginPt.x;
			}

		}
		else {
			endPt = point;
		}
	//���µ���
	ClientDC.MoveTo(beginPt.x,beginPt.y);  
	ClientDC.LineTo(endPt.x,endPt.y);  
	}

	CView::OnMouseMove(nFlags, point);
}

void CTermprjView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_drawline) {//��ǰ���ǻ���ֱ��ģʽ
		if(m_inrect)  //�����������
		{
			//temp
			beginPt = temp.CenterPoint();
			endPt = temp.CenterPoint();
			//AfxMessageBox(_T("��׽�ɹ�"));
		}
		else{
			beginPt = point;
			endPt = point;
		}
		
		m_capture = true;   //mousemove��ʼ��Ч
		CRect rc(beginPt.x-20,beginPt.y-20,beginPt.x+20,beginPt.y+20); //����һ�㴴��һ���Զ���������40*40
		//AfxMessageBox(_T("111111��"));
		rects.push_back(rc);  //���뵽�����������������
	}
	CView::OnLButtonDown(nFlags, point);
}

void CTermprjView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//	m_drawline = false;  //�رջ�ֱ��ģʽ
	m_capture = false;  //�رն�̬����
	if(m_inrect)  //���������Զ�����������
	{
		CClientDC  ClientDC(this); //this = ������
		CPen pen;
		pen.CreatePen(PS_SOLID,3,0x00ffffff-RGB(0,0,255));    //�����߽�ɫ�ķ�ɫ����
		ClientDC.SelectObject(&pen);	
		ClientDC.SetROP2(R2_XORPEN);   //����ģʽΪ���ʵķ�ɫ
		//���ɵ���
		ClientDC.MoveTo(beginPt.x,beginPt.y);  
		ClientDC.LineTo(endPt.x,endPt.y); 

		//�յ㶨λ���������������
		endPt = temp.CenterPoint();
		endPt = temp.CenterPoint();
		//AfxMessageBox(_T("��׽�ɹ�"));
		ClientDC.MoveTo(beginPt.x,beginPt.y);  
		ClientDC.LineTo(endPt.x,endPt.y);  
	}

	CRect rc(endPt.x-20,endPt.y-20,endPt.x+20,endPt.y+20);   //�����µĲ���������ӵ�������
	rects.push_back(rc);
	CView::OnLButtonUp(nFlags, point);
}
