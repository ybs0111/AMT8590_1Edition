	// Animate.cpp : implementation file
//
#include "stdafx.h"
#include "handler.h"
#include "Animate.h"
#include "Public_Function.h"
// #include "FastechPublic_IO.h"
#include "math.h"

#include "Variable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnimate

#define getrandom(min,max) ((rand()%(int)(((max)+1)-(min)))+(min));

//*********************************************
//					CAxisMod
//*********************************************
CAxisMod::CAxisMod() : m_pBitAxis(NULL),m_HomePoint(100,100),m_dEqWidth(1),m_nImgCurPos_X(1),m_nImgCurPos_Y(1)
					  ,m_dRatio(1),m_dEqCurPos_X(1),m_dEqCurPos_Y(1)
{

}

CAxisMod::~CAxisMod()
{
	m_BitAxis.DeleteObject();
	m_BitAxisMask.DeleteObject();
}

void CAxisMod::ComverEqPosToImgPos()
{
	m_nImgCurPos_X =    m_HomePoint.x + int(m_dEqCurPos_X * m_dRatio)  - int(m_ImgSize.cx/2.0+ 0.5) ;
	m_nImgCurPos_Y =    m_HomePoint.y + int(m_dEqCurPos_Y * m_dRatio)  - int(m_ImgSize.cy/2.0+ 0.5) ;
}
void CAxisMod::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk)
{
	m_DcAxis.CreateCompatibleDC(pDC);
	m_BitAxis.LoadBitmap(ImgID);
	m_pBitAxis = (CBitmap *)m_DcAxis.SelectObject(&m_BitAxis);

	m_DcAxisMask.CreateCompatibleDC(pDC);
	m_BitAxisMask.LoadBitmap(ImgIDMsk);
	m_pBitAxisMask = (CBitmap *)m_DcAxisMask.SelectObject(&m_BitAxisMask);	
	
	BITMAP bs;
	m_BitAxis.GetBitmap(&bs);
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;	
}

void CAxisMod::DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();

	dc.BitBlt(m_nImgCurPos_X,m_nImgCurPos_Y, m_ImgSize.cx, m_ImgSize.cy, &m_DcAxisMask, 0, 0, SRCAND);
	dc.BitBlt(m_nImgCurPos_X,m_nImgCurPos_Y, m_ImgSize.cx, m_ImgSize.cy, &m_DcAxis, 0, 0, SRCPAINT);
}


//*********************************************
//					CGlass
//*********************************************
CEqGlass::CEqGlass():m_sizGlassSize(80,140) ,m_nGlassPos(100,100),m_nDegree(0),m_nDir(2)
{}	 
CEqGlass::~CEqGlass()
{} 

void CEqGlass::CellRectCf(CRect &CellRect)
{

	return;
	CellRect.right	-= 10;
	CellRect.top	+= 10;
	

	const int deg =  abs(m_nDegree%180) /45;

	
	switch(m_nDir)
	{
	case 0:
		switch(deg)
		{
		case 0:
			CellRect.right	+= 10;
			CellRect.top	+= 10;
			break;

		case 1:
			CellRect.left	+= 10;
			CellRect.top	+= 10;
			break;

		case 3:
			CellRect.right	+= 10;
			CellRect.top	+= 10;
			break;
		}
		break;

	case 1:
		CellRect.left	+= 10;
		CellRect.bottom -= 10;
		break;

	case 2:
		CellRect.right	-= 10;
		CellRect.top	+= 10;
//
//		switch(deg)
//		{
//		case 0:
//			CellRect.left 	-= 10;
//			CellRect.top	+= 10;
//			break;
//
//		case 1:
//			CellRect.right	-= 10;
//			CellRect.top	+= 10;
//			break;
//
//		case 3:
//			CellRect.right	-= 10;
//			CellRect.top	+= 10;
//			break;
//		}

		break;

	case 3:
		CellRect.right	-= 10;
		CellRect.bottom	-= 10;
		break;
	}	
}

void CEqGlass::RotateRectAngle(CRect rect,double nDegree, CRect rectCen)
{
	CPoint pointCenter;
	double cosine = cos(nDegree * 3.14159265359 / 180.);
	double sine	  = sin(nDegree * 3.14159265359 / 180.);
	// 중심축을 기준으로 회전
	pointCenter.x = rect.CenterPoint().x;
	pointCenter.y = rect.CenterPoint().y;

	int x0 = pointCenter.x;
	int y0 = pointCenter.y;

	double x1 = cosine	*	(rect.TopLeft().x - x0)		- sine		*	(rect.TopLeft().y - y0)		+ x0;
	double y1 = sine	*   (rect.TopLeft().x - x0)		+ cosine	*	(rect.TopLeft().y - y0)		+ y0;

	double x2 = cosine	*	(rect.BottomRight().x - x0) - sine		*   (rect.TopLeft().y - y0)		+ x0;
	double y2 = sine	*   (rect.BottomRight().x - x0) + cosine	*	(rect.TopLeft().y - y0)		+ y0;

	double x3 = cosine	*	(rect.BottomRight().x - x0) - sine		*   (rect.BottomRight().y - y0) + x0;
	double y3 = sine	*   (rect.BottomRight().x - x0) + cosine	*	(rect.BottomRight().y - y0) + y0;

	double x4 = cosine	*	(rect.TopLeft().x - x0)		- sine		*   (rect.BottomRight().y - y0) + x0;
    double y4 = sine	*	(rect.TopLeft().x - x0)		+ cosine	*   (rect.BottomRight().y - y0) + y0;	

	m_arrGlassPoint[0].x = int(x1); 
	m_arrGlassPoint[0].y = int(y1);
	m_arrGlassPoint[1].x = int(x2); 
	m_arrGlassPoint[1].y = int(y2); 
	m_arrGlassPoint[2].x = int(x3); 
	m_arrGlassPoint[2].y = int(y3); 
	m_arrGlassPoint[3].x = int(x4); 
	m_arrGlassPoint[3].y = int(y4);		
}

void CEqGlass::DrawImg(CDC &dc)
{
//	m_sizGlassSize.cx = GdbSetCellRecipe.fCellInfoShort/7.5;
//	m_sizGlassSize.cy = GdbSetCellRecipe.fCellInfoLong/7.5;

	CRect CellRect;
	CBrush tmpBrush,tmpBrush1,*pBrush;
	CellRect.left	 = m_nGlassPos.x - (m_sizGlassSizeMod.cx / 2);
	CellRect.top	 = m_nGlassPos.y - (m_sizGlassSizeMod.cy / 2);
	CellRect.right	 = m_nGlassPos.x + (m_sizGlassSizeMod.cx / 2);
	CellRect.bottom	 = m_nGlassPos.y + (m_sizGlassSizeMod.cy / 2);
	RotateRectAngle(CellRect, m_nDegree, CellRect);
//	tmpBrush.CreateSolidBrush(GdbSetColor.colorLightGray);
	pBrush = dc.SelectObject(&tmpBrush);
	dc.Polygon(m_arrGlassPoint, 4);
	CellRectCf(CellRect);
	RotateRectAngle(CellRect, m_nDegree, CellRect);
//	tmpBrush1.CreateSolidBrush(GdbSetColor.colorUser3);
	dc.SelectObject(&tmpBrush1);
	dc.Polygon(m_arrGlassPoint, 4);
	dc.SelectObject(pBrush);
	tmpBrush1.DeleteObject();
	tmpBrush.DeleteObject();

}

//*********************************************
//					CGlass2
//*********************************************
CEqGlass2::CEqGlass2():m_sizGlassSize2(80,140) ,m_nGlassPos2(100,100),m_nDegree2(0),m_nDir2(2)
{}	 
CEqGlass2::~CEqGlass2()
{} 

void CEqGlass2::CellRectCf2(CRect &CellRect2)
{

	return;
	CellRect2.right	-= 10;
	CellRect2.top	+= 10;
	

	const int deg2 =  abs(m_nDegree2%180) /45;

	
	switch(m_nDir2)
	{
	case 0:
		switch(deg2)
		{
		case 0:
			CellRect2.right	+= 10;
			CellRect2.top	+= 10;
			break;

		case 1:
			CellRect2.left	+= 10;
			CellRect2.top	+= 10;
			break;

		case 3:
			CellRect2.right	+= 10;
			CellRect2.top	+= 10;
			break;
		}
		break;

	case 1:
		CellRect2.left	+= 10;
		CellRect2.bottom -= 10;
		break;

	case 2:
		CellRect2.right	-= 10;
		CellRect2.top	+= 10;
//
//		switch(deg)
//		{
//		case 0:
//			CellRect.left 	-= 10;
//			CellRect.top	+= 10;
//			break;
//
//		case 1:
//			CellRect.right	-= 10;
//			CellRect.top	+= 10;
//			break;
//
//		case 3:
//			CellRect.right	-= 10;
//			CellRect.top	+= 10;
//			break;
//		}

		break;

	case 3:
		CellRect2.right	-= 10;
		CellRect2.bottom	-= 10;
		break;
	}	
}

void CEqGlass2::RotateRectAngle2(CRect rect2,double nDegree2, CRect rectCen2)
{
	CPoint pointCenter2;
	double cosine2 = cos(nDegree2 * 3.14159265359 / 180.);
	double sine2	  = sin(nDegree2 * 3.14159265359 / 180.);
	// 중심축을 기준으로 회전
	pointCenter2.x = rect2.CenterPoint().x;
	pointCenter2.y = rect2.CenterPoint().y;

	int x0 = pointCenter2.x;
	int y0 = pointCenter2.y;

	double x1 = cosine2	*	(rect2.TopLeft().x - x0)		- sine2		*	(rect2.TopLeft().y - y0)		+ x0;
	double y1 = sine2	*   (rect2.TopLeft().x - x0)		+ cosine2	*	(rect2.TopLeft().y - y0)		+ y0;

	double x2 = cosine2	*	(rect2.BottomRight().x - x0) - sine2		*   (rect2.TopLeft().y - y0)		+ x0;
	double y2 = sine2	*   (rect2.BottomRight().x - x0) + cosine2	*	(rect2.TopLeft().y - y0)		+ y0;

	double x3 = cosine2	*	(rect2.BottomRight().x - x0) - sine2		*   (rect2.BottomRight().y - y0) + x0;
	double y3 = sine2	*   (rect2.BottomRight().x - x0) + cosine2	*	(rect2.BottomRight().y - y0) + y0;

	double x4 = cosine2	*	(rect2.TopLeft().x - x0)		- sine2		*   (rect2.BottomRight().y - y0) + x0;
    double y4 = sine2	*	(rect2.TopLeft().x - x0)		+ cosine2	*   (rect2.BottomRight().y - y0) + y0;	

	m_arrGlassPoint2[0].x = int(x1); 
	m_arrGlassPoint2[0].y = int(y1);
	m_arrGlassPoint2[1].x = int(x2); 
	m_arrGlassPoint2[1].y = int(y2); 
	m_arrGlassPoint2[2].x = int(x3); 
	m_arrGlassPoint2[2].y = int(y3); 
	m_arrGlassPoint2[3].x = int(x4); 
	m_arrGlassPoint2[3].y = int(y4);		
}

void CEqGlass2::DrawImg2(CDC &dc)
{
//	m_sizGlassSize.cx = GdbSetCellRecipe.fCellInfoShort/7.5;
//	m_sizGlassSize.cy = GdbSetCellRecipe.fCellInfoLong/7.5;

	CRect CellRect2;
	CBrush tmpBrush,tmpBrush2,*pBrush2;
	CellRect2.left	 = m_nGlassPos2.x - (m_sizGlassSizeMod2.cx / 2);
	CellRect2.top	 = m_nGlassPos2.y - (m_sizGlassSizeMod2.cy / 2);
	CellRect2.right	 = m_nGlassPos2.x + (m_sizGlassSizeMod2.cx / 2);
	CellRect2.bottom	 = m_nGlassPos2.y + (m_sizGlassSizeMod2.cy / 2);
	RotateRectAngle2(CellRect2, m_nDegree2, CellRect2);
//	tmpBrush.CreateSolidBrush(GdbSetColor.colorLightGray);
	pBrush2 = dc.SelectObject(&tmpBrush2);
	dc.Polygon(m_arrGlassPoint2, 4);
	CellRectCf2(CellRect2);
	RotateRectAngle2(CellRect2, m_nDegree2, CellRect2);
//	tmpBrush1.CreateSolidBrush(GdbSetColor.colorUser3);
	dc.SelectObject(&tmpBrush2);
	dc.Polygon(m_arrGlassPoint2, 4);
	dc.SelectObject(pBrush2);
	tmpBrush2.DeleteObject();
	tmpBrush.DeleteObject();
}

//*********************************************
//					CStageMod
//*********************************************

CStageMod::CStageMod():m_StageSize(100,50),m_nDegree(0),m_GlassColor(RGB(0,255,0)),m_TableColor(RGB(0,0,0)),m_bGlass(0)
{

}

CStageMod::~CStageMod()
{
	m_BitStage.DeleteObject();
	m_BitStage_mask.DeleteObject();

}

void CStageMod::CellRectCf(CRect &CellRect, int nDir)
{
	switch(nDir)
	{
	case 0:
		CellRect.left	+= 10;
		CellRect.top	+= 10;
		break;

	case 1:
		CellRect.left	+= 10;
		CellRect.bottom -= 10;
		break;

	case 2:
		CellRect.right	-= 10;
		CellRect.top	+= 10;
		break;

	case 3:
		CellRect.right	-= 10;
		CellRect.bottom	-= 10;
		break;
	}

}

void CStageMod::RotateRectAngle(CRect rect, double nDegree)
{
	CPoint pointCenter;

	
	double cosine = cos(nDegree * 3.14159265359 / 180.);
	double sine	  = sin(nDegree * 3.14159265359 / 180.);
	// 중심축을 기준으로 회전
	pointCenter.x = rect.CenterPoint().x;
	pointCenter.y = rect.CenterPoint().y;

	int x0 = pointCenter.x;
	int y0 = pointCenter.y;

	double x1 = cosine	*	(rect.TopLeft().x - x0)		- sine		*	(rect.TopLeft().y - y0)		+ x0;
	double y1 = sine	*   (rect.TopLeft().x - x0)		+ cosine	*	(rect.TopLeft().y - y0)		+ y0;

	double x2 = cosine	*	(rect.BottomRight().x - x0) - sine		*   (rect.TopLeft().y - y0)		+ x0;
	double y2 = sine	*   (rect.BottomRight().x - x0) + cosine	*	(rect.TopLeft().y - y0)		+ y0;

	double x3 = cosine	*	(rect.BottomRight().x - x0) - sine		*   (rect.BottomRight().y - y0) + x0;
	double y3 = sine	*   (rect.BottomRight().x - x0) + cosine	*	(rect.BottomRight().y - y0) + y0;

	double x4 = cosine	*	(rect.TopLeft().x - x0)		- sine		*   (rect.BottomRight().y - y0) + x0;
    double y4 = sine	*	(rect.TopLeft().x - x0)		+ cosine	*   (rect.BottomRight().y - y0) + y0;	

	m_StagePont[0].x = int(x1); 
	m_StagePont[0].y = int(y1);
	m_StagePont[1].x = int(x2); 
	m_StagePont[1].y = int(y2); 
	m_StagePont[2].x = int(x3); 
	m_StagePont[2].y = int(y3); 
	m_StagePont[3].x = int(x4); 
	m_StagePont[3].y = int(y4);


}



void CStageMod:: DrawImg(CDC &dc)
{
//	CBrush tmpBrush,tmpCfBrush,*pOldtmpBrush;
    
   CAxisMod::ComverEqPosToImgPos();
    
	m_nDegree = (int)m_dEqCurPos_T;
	m_nHight = (int)m_dEqCurPos_Z;



//-----------------------
//|p1                   |p2
//|                     |
//|p3                   |
//-----------------------
/*
    Graphics graphics(dc.m_hDC);
*/	

   m_StageSizeMod.cx = m_StageSize.cx + m_nHight/10;
   m_StageSizeMod.cy = m_StageSize.cy + m_nHight/10;

	CRect rect;
	rect.top    = m_nImgCurPos_Y + m_ImgSize.cy/2  - m_StageSizeMod.cy/2;
	rect.left   = m_nImgCurPos_X + m_ImgSize.cx/2  - m_StageSizeMod.cx/2;
	rect.right  = m_nImgCurPos_X + m_ImgSize.cx/2  - m_StageSizeMod.cx/2 + m_StageSizeMod.cx;
	rect.bottom = m_nImgCurPos_Y + m_ImgSize.cy/2  - m_StageSizeMod.cy/2 + m_StageSizeMod.cy;
    

	RotateRectAngle(rect,m_nDegree);
/*	
	Point points[] = {Point(m_StagePont[0].x ,m_StagePont[0].y),
			Point(m_StagePont[1].x , m_StagePont[1].y),
			Point(m_StagePont[3].x, m_StagePont[3].y)};
		
		graphics.DrawImage(&image, points, 3);
*/
	if(m_bGlass)
	{
		m_Glass.m_nDegree				= m_nDegree; 
		m_Glass.m_sizGlassSizeMod.cx	= m_Glass.m_sizGlassSize.cx + m_nHight/10;
    	m_Glass.m_sizGlassSizeMod.cy	= m_Glass.m_sizGlassSize.cy + m_nHight/10;
		m_Glass.m_nGlassPos.x			= rect.left + rect.Width()/2;
		m_Glass.m_nGlassPos.y			= rect.top  + rect.Height()/2;
		m_Glass.DrawImg(dc);
	}
}



//*********************************************
//					CVariantMod
//*********************************************
CVariantMod::CVariantMod()
{
}

CVariantMod::~CVariantMod()
{
}

void CVariantMod::DrawImg(CDC &dc)
{
    CAxisMod::ComverEqPosToImgPos();
 
	dc.BitBlt(m_nImgCurPos_X,m_nImgCurPos_Y, m_ImgSize.cx, m_ImgSize.cy, &m_DcAxisMask, 0, 0, SRCAND);
	dc.BitBlt(m_nImgCurPos_X,m_nImgCurPos_Y, m_ImgSize.cx, m_ImgSize.cy, &m_DcAxis, 0, 0, SRCPAINT);
}

//*********************************************
//					CTrnaferMod
//*********************************************

CTrnaferMod::CTrnaferMod():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CTrnaferMod::~CTrnaferMod()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CTrnaferMod::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}


void CTrnaferMod::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{

	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);

	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);

	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CTrnaferMod:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	

	m_dRadioZ = 1;

// 	if(m_bGlass < 0)
// 		m_bGlass = 0;
// 
// 	if(!m_bGlass)
// 	{
// 		m_Glass.m_nDegree = 0; 
// 		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 - m_dEqCurPos_Z );
//     	m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 - m_dEqCurPos_Z );
// 
// 		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
// 		m_Glass.DrawImg(dc);
// 	}

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

}

//*********************************************
//					CCrrPkTransferMod
//*********************************************
CCrrPkTransferMod::CCrrPkTransferMod():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CCrrPkTransferMod::~CCrrPkTransferMod()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CCrrPkTransferMod::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CCrrPkTransferMod::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CCrrPkTransferMod:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	if(m_bGlass < 0)
		m_bGlass = 0;
	
	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}

//*********************************************
//					CCrrPkTransferMod
//*********************************************

CLoadCvTransferMod::CLoadCvTransferMod():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CLoadCvTransferMod::~CLoadCvTransferMod()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CLoadCvTransferMod::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CLoadCvTransferMod::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{

	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);

	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);

	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CLoadCvTransferMod:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	

	m_dRadioZ = 1;

	if(m_bGlass < 0)
		m_bGlass = 0;

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);	

	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.88 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.70 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx-55,m_nImgCurPos_Y+m_ImgSize.cy-100);
		m_Glass.DrawImg(dc);
	}
}

//*********************************************
//					CAttPkRbtLTransferMod
//*********************************************

CAttPkRbtLTransferMod::CAttPkRbtLTransferMod():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CAttPkRbtLTransferMod::~CAttPkRbtLTransferMod()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CAttPkRbtLTransferMod::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CAttPkRbtLTransferMod::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{

	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);

	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);

	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CAttPkRbtLTransferMod:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	

	m_dRadioZ = 1;

	if(m_bGlass < 0)
		m_bGlass = 0;

	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.7 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.15 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);	

}

//*********************************************
//					CBufPlateLTransferModCAttPkRbtLTransferMod
//*********************************************

CBufPlateLTransferMod::CBufPlateLTransferMod():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CBufPlateLTransferMod::~CBufPlateLTransferMod()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CBufPlateLTransferMod::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CBufPlateLTransferMod::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{

	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);

	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);

	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CBufPlateLTransferMod:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	

	m_dRadioZ = 1;

	if(m_bGlass < 0)
		m_bGlass = 0;

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);	
	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.35 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.05 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}
}


//*********************************************
//					CAttPkRbtRTransferMod
//*********************************************

CAttPkRbtRTransferMod::CAttPkRbtRTransferMod():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CAttPkRbtRTransferMod::~CAttPkRbtRTransferMod()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CAttPkRbtRTransferMod::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CAttPkRbtRTransferMod::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{

	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);

	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);

	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CAttPkRbtRTransferMod:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	

	m_dRadioZ = 1;

	if(m_bGlass < 0)
		m_bGlass = 0;

	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.7 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.15 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);	
}

//*********************************************
//					CBufPlateRTransferMod
//*********************************************

CBufPlateRTransferMod::CBufPlateRTransferMod():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CBufPlateRTransferMod::~CBufPlateRTransferMod()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CBufPlateRTransferMod::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CBufPlateRTransferMod::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{

	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);

	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);

	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CBufPlateRTransferMod:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	

	m_dRadioZ = 1;

	if(m_bGlass < 0)
		m_bGlass = 0;

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);	

	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.35 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.05 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}
}

//*********************************************
//					CAttCvTransferMod
//*********************************************

CAttCvTransferMod::CAttCvTransferMod():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CAttCvTransferMod::~CAttCvTransferMod()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CAttCvTransferMod::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CAttCvTransferMod::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{

	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);

	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);

	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CAttCvTransferMod:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	

	m_dRadioZ = 1;

	if(m_bGlass < 0)
		m_bGlass = 0;

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);	

	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.88 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx-55,m_nImgCurPos_Y+m_ImgSize.cy-60);
		m_Glass.DrawImg(dc);
	}

}

//*********************************************
//					CPCBPkTransferLMod
//*********************************************
CPCBPkTransferLMod::CPCBPkTransferLMod():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CPCBPkTransferLMod::~CPCBPkTransferLMod()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CPCBPkTransferLMod::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}


void CPCBPkTransferLMod::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{

	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);

	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);

	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CPCBPkTransferLMod:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	

	m_dRadioZ = 1;

	if(m_bGlass < 0)
		m_bGlass = 0;

	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.4 + m_dEqCurPos_Z );
    	m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.7 + m_dEqCurPos_Z );

		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

}

//*********************************************
//					CPCBPkTransferRMod
//*********************************************
CPCBPkTransferRMod::CPCBPkTransferRMod():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CPCBPkTransferRMod::~CPCBPkTransferRMod()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CPCBPkTransferRMod::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}


void CPCBPkTransferRMod::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{

	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);

	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);

	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CPCBPkTransferRMod:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	

	m_dRadioZ = 1;

	if(m_bGlass < 0)
		m_bGlass = 0;

	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.4 + m_dEqCurPos_Z );
    	m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.7 + m_dEqCurPos_Z );

		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

}

//*********************************************
//					CAttPkTransferMod
//*********************************************

CAttPkTransferMod::CAttPkTransferMod():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CAttPkTransferMod::~CAttPkTransferMod()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CAttPkTransferMod::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}


void CAttPkTransferMod::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{

	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);

	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);

	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CAttPkTransferMod:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	

	m_dRadioZ = 1;

	if(m_bGlass < 0)
		m_bGlass = 0;

	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 - m_dEqCurPos_Z );
    	m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 - m_dEqCurPos_Z );

		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
				  int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
				  &m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

}

//*********************************************
//					CConveyorMod
//*********************************************
CConveyorMod::CConveyorMod():m_nRotateSeq(1)
{
}

CConveyorMod::~CConveyorMod()
{

}

void CConveyorMod::ComverEqPosToImgPos()
{

	m_nImgCurPos_X =    m_HomePoint.x + int(m_dEqCurPos_X * m_dRatio)  - int(m_ImgSize.cx/2) ;
	m_nImgCurPos_Y =    m_HomePoint.y + int(m_dEqCurPos_Y * m_dRatio)  - int(m_ImgSize.cy/2) ;
}

void CConveyorMod::DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();
   
	dc.BitBlt(m_nImgCurPos_X,m_nImgCurPos_Y ,m_ImgSize.cx,m_ImgSize.cy, &m_DcAxisMask, 0, 0, SRCAND);
	dc.BitBlt(m_nImgCurPos_X,m_nImgCurPos_Y ,m_ImgSize.cx,m_ImgSize.cy, &m_DcAxis, 0, 0, SRCPAINT);
}

CDoor::CDoor(){}
CDoor::~CDoor(){}

void CDoor::DrawImg(CDC &dc)
{
	CBrush DoorBrush,*pBrush = NULL;

	
	if(m_bDoorOpen)
	{
		if(bTogle) 
		{
//			pBrush = (CBrush*)dc.SelectObject(&GdbSetColor.brushRed);
		}
		else
		{
//			pBrush = (CBrush*)dc.SelectObject(&GdbSetColor.brushWhite);
		}
		
		bTogle = !bTogle;
		dc.Rectangle(m_DooRect);
		dc.SelectObject(pBrush);
			
	}

}

CEMO::CEMO():m_nRotateSeq(1)
{}
CEMO::~CEMO(){}

void CEMO::DrawImg(CDC &dc)
{ 
	
	ComverEqPosToImgPos();
	if(m_bSwOn)
		(m_nRotateSeq > 1) ? m_nRotateSeq =0 : m_nRotateSeq++;
	else
		m_nRotateSeq = 2;

	dc.BitBlt(m_nImgCurPos_X,m_nImgCurPos_Y, m_ImgSize.cx , m_ImgSize.cy, &m_DcAxisMask,m_ImgSize.cx * m_nRotateSeq, 0, SRCAND);
	dc.BitBlt(m_nImgCurPos_X,m_nImgCurPos_Y, m_ImgSize.cx/3 , m_ImgSize.cy, &m_DcAxis,m_ImgSize.cx * m_nRotateSeq/3, 0, SRCPAINT);
	m_nRotateSeq++; 

	
}

//*********************************************
//					CAnimate
//*********************************************

CAnimate::CAnimate():m_CursorPoint(0,0)
{
	int nBtnWidth, nBtnGab;
	nBtnWidth = 120;
	nBtnGab = 30;
	for(int i=0; i<3; i++)
	{
		m_BtnRect[i] = CRect((nBtnGab+(nBtnWidth)*(i)),65,((nBtnWidth)*(i+1)),95);
		m_bClicked[i] = FALSE;
	}
}

CAnimate::~CAnimate()
{
	m_AniFont.DeleteObject();
	m_AniPen.DeleteObject();
	m_AniBrush.DeleteObject();
	m_AniPen1.DeleteObject();
	m_AniBrush1.DeleteObject();
	m_AniBicFont.DeleteObject();
	m_FrameBase.DeleteDC();
}

BEGIN_MESSAGE_MAP(CAnimate, CWnd)
	//{{AFX_MSG_MAP(CAnimate)
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAnimate message handlers
BOOL CAnimate::Create ( DWORD exstyle, DWORD style, const CRect& r, CWnd *p, UINT id, CCreateContext *pc ) 
{					
	
	CBrush mybrush ( RGB (255, 255, 0 ) ) ;

	CString mywindowclass ;
	mywindowclass = AfxRegisterWndClass ( CS_HREDRAW | CS_VREDRAW, 
										AfxGetApp( ) -> LoadCursor ( IDI_CURSOR ), 
										mybrush, 0 ) ;

	return CreateEx ( exstyle, mywindowclass, "", style, 
						r.left, r.top, r.Width(),r.Height(), 
						p -> GetSafeHwnd( ), NULL, NULL ) ;
	mybrush.DeleteObject();

	return TRUE;
	
}

void CAnimate::OnDestroy() 
{
	CWnd::OnDestroy();

	m_AniFont.DeleteObject();
	m_AniPen.DeleteObject();
	m_AniBicFont.DeleteObject();
	m_AniPen1.DeleteObject();
	m_AniBrush1.DeleteObject();

}


void CAnimate::InitAniWnd()//2014,1125 ysj
{
	CDC* pAniDC = GetDC(); 

	m_AniFont.CreateFont(12, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
						OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
						DEFAULT_PITCH|FF_SWISS, "굴림체");

	m_AniBicFont.CreateFont(17, 10, 0, 0, 0, FALSE, FALSE, FALSE, ANSI_CHARSET,
						OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
						DEFAULT_PITCH|FF_SWISS, "굴림체");

	m_AniSmallFont.CreateFont(9, 10, 0, 0, 0, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
						DEFAULT_PITCH|FF_SWISS, "굴림체");

	m_AniPen.CreatePen(PS_DASH, 3, RGB(0,255,50));
	m_AniBrush.CreateSolidBrush(RGB(0,255,50));

	m_FrameBase.CreateCompatibleDC(pAniDC);

	// ===== BackFrame ======
 	m_FrameBaseBmp.LoadBitmap(IDB_MAIN_ANIVIEW);
 	m_pFrameBaseBmp = (CBitmap *)m_FrameBase.SelectObject(&m_FrameBaseBmp);

 	// ===== Unit Position ======
	m_ID_Buffer1.SetRatioEqToImg(0.5);
	m_ID_Buffer1.SetImgHomePos(CPoint(130,420));
	m_ID_Buffer2.SetRatioEqToImg(0.5);
	m_ID_Buffer2.SetImgHomePos(CPoint(300,420));
	m_ID_Buffer3.SetRatioEqToImg(0.5);
	m_ID_Buffer3.SetImgHomePos(CPoint(455,420));
	m_ID_Buffer4.SetRatioEqToImg(0.5);
	m_ID_Buffer4.SetImgHomePos(CPoint(620,420));
	m_ID_Buffer5.SetRatioEqToImg(0.5);
	m_ID_Buffer5.SetImgHomePos(CPoint(770,420));

	////2014,1125 ysj
	m_T_Box1.SetRatioEqToImg(0.5);
	m_T_Box1.SetImgHomePos(CPoint(97,420));
	m_T_Box2.SetRatioEqToImg(0.5);
	m_T_Box2.SetImgHomePos(CPoint(167,420));
	m_T_Box3.SetRatioEqToImg(0.5);
	m_T_Box3.SetImgHomePos(CPoint(267,420));
	m_T_Box4.SetRatioEqToImg(0.5);
	m_T_Box4.SetImgHomePos(CPoint(337,420));
	m_T_Box5.SetRatioEqToImg(0.5);
	m_T_Box5.SetImgHomePos(CPoint(422,420));
	m_T_Box6.SetRatioEqToImg(0.5);
	m_T_Box6.SetImgHomePos(CPoint(492,420));
	m_T_Box7.SetRatioEqToImg(0.5);
	m_T_Box7.SetImgHomePos(CPoint(587,420));
	m_T_Box8.SetRatioEqToImg(0.5);
	m_T_Box8.SetImgHomePos(CPoint(657,420));
	m_T_Box9.SetRatioEqToImg(0.5);
	m_T_Box9.SetImgHomePos(CPoint(737,420));
	m_T_Box10.SetRatioEqToImg(0.5);
	m_T_Box10.SetImgHomePos(CPoint(807,420));
	
	m_T_Box_Skip1.SetRatioEqToImg(0.5);
	m_T_Box_Skip1.SetImgHomePos(CPoint(97,420));
	m_T_Box_Skip2.SetRatioEqToImg(0.5);
	m_T_Box_Skip2.SetImgHomePos(CPoint(167,420));
	m_T_Box_Skip3.SetRatioEqToImg(0.5);
	m_T_Box_Skip3.SetImgHomePos(CPoint(267,420));
	m_T_Box_Skip4.SetRatioEqToImg(0.5);
	m_T_Box_Skip4.SetImgHomePos(CPoint(337,420));
	m_T_Box_Skip5.SetRatioEqToImg(0.5);
	m_T_Box_Skip5.SetImgHomePos(CPoint(422,420));
	m_T_Box_Skip6.SetRatioEqToImg(0.5);
	m_T_Box_Skip6.SetImgHomePos(CPoint(492,420));
	m_T_Box_Skip7.SetRatioEqToImg(0.5);
	m_T_Box_Skip7.SetImgHomePos(CPoint(587,420));
	m_T_Box_Skip8.SetRatioEqToImg(0.5);
	m_T_Box_Skip8.SetImgHomePos(CPoint(657,420));
	m_T_Box_Skip9.SetRatioEqToImg(0.5);
	m_T_Box_Skip9.SetImgHomePos(CPoint(737,420));
	m_T_Box_Skip10.SetRatioEqToImg(0.5);
	m_T_Box_Skip10.SetImgHomePos(CPoint(807,420));
	////
 	// ===== Move Unit ======
////amt8590
	m_ID_Buffer1.CreateImg(pAniDC,IDB_IDBUFFER1,IDB_IDBUFFER1_BK,IDB_IDBUFFER1,IDB_IDBUFFER1_BK);
	m_ID_Buffer2.CreateImg(pAniDC,IDB_IDBUFFER2,IDB_IDBUFFER2_BK,IDB_IDBUFFER2,IDB_IDBUFFER2_BK);
	m_ID_Buffer3.CreateImg(pAniDC,IDB_IDBUFFER3,IDB_IDBUFFER3_BK,IDB_IDBUFFER3,IDB_IDBUFFER3_BK);
	m_ID_Buffer4.CreateImg(pAniDC,IDB_IDBUFFER4,IDB_IDBUFFER4_BK,IDB_IDBUFFER4,IDB_IDBUFFER4_BK);
	m_ID_Buffer5.CreateImg(pAniDC,IDB_IDBUFFER5,IDB_IDBUFFER5_BK,IDB_IDBUFFER5,IDB_IDBUFFER5_BK);

	m_T_Box1.CreateImg(pAniDC,IDB_T_BOX1,IDB_T_BOX1_BK,IDB_T_BOX1,IDB_T_BOX1_BK);
	m_T_Box2.CreateImg(pAniDC,IDB_T_BOX2,IDB_T_BOX2_BK,IDB_T_BOX2,IDB_T_BOX2_BK);
	m_T_Box3.CreateImg(pAniDC,IDB_T_BOX3,IDB_T_BOX3_BK,IDB_T_BOX3,IDB_T_BOX3_BK);
	m_T_Box4.CreateImg(pAniDC,IDB_T_BOX4,IDB_T_BOX4_BK,IDB_T_BOX4,IDB_T_BOX4_BK);
	m_T_Box5.CreateImg(pAniDC,IDB_T_BOX5,IDB_T_BOX5_BK,IDB_T_BOX5,IDB_T_BOX5_BK);
	m_T_Box6.CreateImg(pAniDC,IDB_T_BOX6,IDB_T_BOX6_BK,IDB_T_BOX6,IDB_T_BOX6_BK);
	m_T_Box7.CreateImg(pAniDC,IDB_T_BOX7,IDB_T_BOX7_BK,IDB_T_BOX7,IDB_T_BOX7_BK);
	m_T_Box8.CreateImg(pAniDC,IDB_T_BOX8,IDB_T_BOX8_BK,IDB_T_BOX8,IDB_T_BOX8_BK);
	m_T_Box9.CreateImg(pAniDC,IDB_T_BOX9,IDB_T_BOX9_BK,IDB_T_BOX9,IDB_T_BOX9_BK);
	m_T_Box10.CreateImg(pAniDC,IDB_T_BOX10,IDB_T_BOX10_BK,IDB_T_BOX10,IDB_T_BOX10_BK);
	
	m_T_Box_Skip1.CreateImg(pAniDC,IDB_T_BOX_SKIP1,IDB_T_BOX_SKIP1_BK,IDB_T_BOX_SKIP1,IDB_T_BOX_SKIP1_BK);
	m_T_Box_Skip2.CreateImg(pAniDC,IDB_T_BOX_SKIP2,IDB_T_BOX_SKIP2_BK,IDB_T_BOX_SKIP2,IDB_T_BOX_SKIP2_BK);
	m_T_Box_Skip3.CreateImg(pAniDC,IDB_T_BOX_SKIP3,IDB_T_BOX_SKIP3_BK,IDB_T_BOX_SKIP3,IDB_T_BOX_SKIP3_BK);
	m_T_Box_Skip4.CreateImg(pAniDC,IDB_T_BOX_SKIP4,IDB_T_BOX_SKIP4_BK,IDB_T_BOX_SKIP4,IDB_T_BOX_SKIP4_BK);
	m_T_Box_Skip5.CreateImg(pAniDC,IDB_T_BOX_SKIP5,IDB_T_BOX_SKIP5_BK,IDB_T_BOX_SKIP5,IDB_T_BOX_SKIP5_BK);
	m_T_Box_Skip6.CreateImg(pAniDC,IDB_T_BOX_SKIP6,IDB_T_BOX_SKIP6_BK,IDB_T_BOX_SKIP6,IDB_T_BOX_SKIP6_BK);
	m_T_Box_Skip7.CreateImg(pAniDC,IDB_T_BOX_SKIP7,IDB_T_BOX_SKIP7_BK,IDB_T_BOX_SKIP7,IDB_T_BOX_SKIP7_BK);
	m_T_Box_Skip8.CreateImg(pAniDC,IDB_T_BOX_SKIP8,IDB_T_BOX_SKIP8_BK,IDB_T_BOX_SKIP8,IDB_T_BOX_SKIP8_BK);
	m_T_Box_Skip9.CreateImg(pAniDC,IDB_T_BOX_SKIP9,IDB_T_BOX_SKIP9_BK,IDB_T_BOX_SKIP9,IDB_T_BOX_SKIP9_BK);
	m_T_Box_Skip10.CreateImg(pAniDC,IDB_T_BOX_SKIP10,IDB_T_BOX_SKIP10_BK,IDB_T_BOX_SKIP10,IDB_T_BOX_SKIP10_BK);

	ReleaseDC(pAniDC);	
}


void CAnimate::CylImageChange(CDC *pDC /*= NULL*/)
{ 
	////////////////////////////////////////////////////////////
	// 선택 되어 있는 image object를 dc에 넣고 삭제한다. 
	////////////////////////////////////////////////////////////
// 	m_ConveyorZ.m_DcAxis.SelectObject(&m_ConveyorZ.m_BitAxis);
// 	DeleteObject(m_ConveyorZ.m_BitAxis);
// 
// 	m_PickerVac1.m_DcAxis.SelectObject(&m_PickerVac1.m_BitAxis);
// 	DeleteObject(m_PickerVac1.m_BitAxis);
// 	
// 	m_PickerVac2.m_DcAxis.SelectObject(&m_PickerVac2.m_BitAxis);
// 	DeleteObject(m_PickerVac2.m_BitAxis);
// 	
// 	m_PickerVac3.m_DcAxis.SelectObject(&m_PickerVac3.m_BitAxis);
// 	DeleteObject(m_PickerVac3.m_BitAxis);
// 	
// 	m_PickerVac4.m_DcAxis.SelectObject(&m_PickerVac4.m_BitAxis);
// 	DeleteObject(m_PickerVac4.m_BitAxis);
// 	
// 	m_PickerCyl1.m_DcAxis.SelectObject(&m_PickerCyl1.m_BitAxis);
// 	DeleteObject(m_PickerCyl1.m_BitAxis);
// 	
// 	m_PickerCyl2.m_DcAxis.SelectObject(&m_PickerCyl2.m_BitAxis);
// 	DeleteObject(m_PickerCyl2.m_BitAxis);
// 	
// 	m_PickerCyl3.m_DcAxis.SelectObject(&m_PickerCyl3.m_BitAxis);
// 	DeleteObject(m_PickerCyl3.m_BitAxis);
// 	
// 	m_PickerCyl4.m_DcAxis.SelectObject(&m_PickerCyl4.m_BitAxis);
// 	DeleteObject(m_PickerCyl4.m_BitAxis);
// 	
// 	m_ConveyorPX.m_DcAxis.SelectObject(&m_ConveyorPX.m_BitAxis);
// 	DeleteObject(m_ConveyorPX.m_BitAxis);
// 	
// 	m_ConveyorPY.m_DcAxis.SelectObject(&m_ConveyorPY.m_BitAxis);
// 	DeleteObject(m_ConveyorPY.m_BitAxis);
// 
//  	if(ani_cyl_pos.m_nConveyorPX_Up_Dn == FALSE )
// 		m_ConveyorPX.m_BitAxis.LoadBitmap(IDB_ANI_CONVEYOR_PX);
//  	else
// 		m_ConveyorPX.m_BitAxis.LoadBitmap(IDB_ANI_CONVEYOR_PX_FWD);
//  	if(ani_cyl_pos.m_nConveyorRail_Fwd_Bwd == FALSE)
// 		m_ConveyorPY.m_BitAxis.LoadBitmap(IDB_ANI_CONVEYOR);
//  	else
// 		m_ConveyorPY.m_BitAxis.LoadBitmap(IDB_ANI_CONVEYOR_FWD);
//  	if(ani_cyl_pos.m_nConveyorZ_Up_Dn == FALSE)
// 		m_ConveyorZ.m_BitAxis.LoadBitmap(IDB_ANI_CONVEYOR_Z_DN);
//  	else
// 		m_ConveyorZ.m_BitAxis.LoadBitmap(IDB_ANI_CONVEYOR_Z_UP);
// 	if(ani_cyl_pos.m_nPickerVac_On_Off[0] == FALSE)
// 		m_PickerVac1.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_VAC_OFF);
// 	else
// 		m_PickerVac1.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_VAC_ON);
// 	if(ani_cyl_pos.m_nPickerVac_On_Off[1] == FALSE)
// 		m_PickerVac2.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_VAC_OFF);
// 	else
// 		m_PickerVac2.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_VAC_ON);
// 	if(ani_cyl_pos.m_nPickerVac_On_Off[2] == FALSE)
// 		m_PickerVac3.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_VAC_OFF);
// 	else
// 		m_PickerVac3.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_VAC_ON);
// 	if(ani_cyl_pos.m_nPickerVac_On_Off[3] == FALSE)
// 		m_PickerVac4.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_VAC_OFF);
// 	else
// 		m_PickerVac4.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_VAC_ON);
// 	if(ani_cyl_pos.m_nPickerCyl_On_Off[0] == TRUE)
// 		m_PickerCyl1.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_CYL_DN);
// 	else
// 		m_PickerCyl1.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_CYL_UP);
// 	if(ani_cyl_pos.m_nPickerCyl_On_Off[1] == TRUE)
// 		m_PickerCyl2.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_CYL_DN);
// 	else
// 		m_PickerCyl2.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_CYL_UP);
// 	if(ani_cyl_pos.m_nPickerCyl_On_Off[2] == TRUE)
// 		m_PickerCyl3.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_CYL_DN);
// 	else
// 		m_PickerCyl3.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_CYL_UP);
// 	if(ani_cyl_pos.m_nPickerCyl_On_Off[3] == TRUE)
// 		m_PickerCyl4.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_CYL_DN);
// 	else
// 		m_PickerCyl4.m_BitAxis.LoadBitmap(IDB_ANI_PICKER_CYL_UP);
// 
// 	////////////////////////////////////////////////////////////
// 	// 선택 되어 있는 image object를 삭제한다. 
// 	////////////////////////////////////////////////////////////
// 	m_ConveyorZ.m_DcAxis.SelectObject(&m_ConveyorZ.m_BitAxis);
// 	DeleteObject(m_ConveyorZ.m_BitAxis);
// 	
// 	m_PickerVac1.m_DcAxis.SelectObject(&m_PickerVac1.m_BitAxis);
// 	DeleteObject(m_PickerVac1.m_BitAxis);
// 	
// 	m_PickerVac2.m_DcAxis.SelectObject(&m_PickerVac2.m_BitAxis);
// 	DeleteObject(m_PickerVac2.m_BitAxis);
// 	
// 	m_PickerVac3.m_DcAxis.SelectObject(&m_PickerVac3.m_BitAxis);
// 	DeleteObject(m_PickerVac3.m_BitAxis);
// 	
// 	m_PickerVac4.m_DcAxis.SelectObject(&m_PickerVac4.m_BitAxis);
// 	DeleteObject(m_PickerVac4.m_BitAxis);
// 	
// 	m_PickerCyl1.m_DcAxis.SelectObject(&m_PickerCyl1.m_BitAxis);
// 	DeleteObject(m_PickerCyl1.m_BitAxis);
// 	
// 	m_PickerCyl2.m_DcAxis.SelectObject(&m_PickerCyl2.m_BitAxis);
// 	DeleteObject(m_PickerCyl2.m_BitAxis);
// 	
// 	m_PickerCyl3.m_DcAxis.SelectObject(&m_PickerCyl3.m_BitAxis);
// 	DeleteObject(m_PickerCyl3.m_BitAxis);
// 	
// 	m_PickerCyl4.m_DcAxis.SelectObject(&m_PickerCyl4.m_BitAxis);
// 	DeleteObject(m_PickerCyl4.m_BitAxis);
// 	
// 	m_ConveyorPX.m_DcAxis.SelectObject(&m_ConveyorPX.m_BitAxis);
// 	DeleteObject(m_ConveyorPX.m_BitAxis);
// 	
// 	m_ConveyorPY.m_DcAxis.SelectObject(&m_ConveyorPY.m_BitAxis);
// 	DeleteObject(m_ConveyorPY.m_BitAxis);
// 
// 	ani_cyl_pos.m_nImageChange = FALSE;
}

void CAnimate::uMesJudgResultMonitoring(CDC *pDC, int *nMEStoEQ_TotalSotInfo, int *nPCBVisionGoodInfo, int *nPickerPickUpDnInfo, 
										int *nLeftGoodBufExistInfo, int *nRightGoodBufExistInfo)
{
 	CString sTmp,sMesDtSum,sVisionDtSum,sPickerDtSum,sLBufTmpDtSum,sRBufTmpDtSum;
 	
// 	sMesDtSum	 = "MES RESULT : ";
// 	sVisionDtSum = "VISION_RESULT : ";
// 	sPickerDtSum = "PICKER_RESULT : ";
// 
// 	sLBufTmpDtSum = "LEFT_BUF_TMP : ";
// 	sRBufTmpDtSum  = "RIGHT_BUF_TMP : ";
// 
//  	for(int i=0; i<st_basic.n_Tray_YCnt; i++)
// 	{
// 		if(i < _PICKER_4)
// 		{
// 			sTmp.Format("%d", nPickerPickUpDnInfo[i]);
// 			sPickerDtSum += sTmp;
// 		}
//  		sTmp.Format("%d", nMEStoEQ_TotalSotInfo[i]);
// 		sMesDtSum += sTmp;
// 		sTmp.Format("%d", nPCBVisionGoodInfo[i]);
// 		sVisionDtSum += sTmp;
// 
// 		sTmp.Format("%d", nLeftGoodBufExistInfo[i]);
// 		sLBufTmpDtSum += sTmp;
// 
// 		sTmp.Format("%d", nRightGoodBufExistInfo[i]);
// 		sRBufTmpDtSum += sTmp;
//  	}
// 
// 	pDC->TextOut(352+(i*10), 10, sPickerDtSum);
// 	pDC->TextOut(352+(i*10), 25, sVisionDtSum);
// 	pDC->TextOut(373+(i*10), 40, sMesDtSum);
// 	pDC->TextOut(359+(i*10), 55, sLBufTmpDtSum);
// 	pDC->TextOut(352+(i*10), 70, sRBufTmpDtSum);
// 
// 	sTmp.Format("SAV:%d/ACC:%d", st_basic.n_Discharge_Lifter_Limit, st_work.m_nUnloadTrayAccCnt);
// 	pDC->TextOut(40, 123, sTmp);
// 	sTmp.Format("SAV:%d/ACC:%d", st_basic.n_tray_Lifter, st_work.m_nCoverTrayAccCnt);
// 	pDC->TextOut(170, 123, sTmp);
// 
// 	if(nColChange == FALSE)
// 	{
// 		nColChange = TRUE;
// 		nColChangeWait[0] = GetCurrentTime();
// 	}
// 	else
// 	{
// 		nColChangeWait[1] = GetCurrentTime();
// 		nColChangeWait[2] = (nColChangeWait[1] - nColChangeWait[0]);
// 		if(nColChangeWait[2] > 400)
// 		{
// 			nColChange = FALSE;
// 			nRandCol = getrandom(0,255);
// 			pDC->SetTextColor(RGB(nRandCol,nRandCol,0));
// 			
// 		}
// 
// 	}
// 	pDC->SetTextColor(RGB(nRandCol,0,0));
// 	sTmp.Format("%s", st_work.strCurActionMsg);
// 	pDC->TextOut(310, 127, sTmp);
}

void CAnimate::uGoodBufLPcbMonitoring(CDC *pDC, int *nLeftGoodBufExistInfo)
{
// 	int nRtn,nTmp;
// 	int nRejectLineStPos = 575;	

// 	nTmp = st_basic.n_Tray_YCnt / 2;
// 
// 	for(int i=0; i<nTmp; i++)
// 	{
// 		nRtn = nLeftGoodBufExistInfo[i];
// 		if(nRtn == TRUE)
// 		{	
// 			pRejectPt.x = nRejectLineStPos + (8 * i);
// 			pRejectPt.y = 270;
// 			
// 			pDC->MoveTo(pRejectPt);
// 			
// 			pRejectPt.x = nRejectLineStPos + (8 * i);
// 			pRejectPt.y = 170;
// 			
// 			pDC->LineTo(pRejectPt);
// 		}
// 		else
// 		{
// 			pRejectPt.x = nRejectLineStPos + (8 * i);
// 			pRejectPt.y = 0;
// 			
// 			pDC->MoveTo(pRejectPt);
// 			
// 			pRejectPt.x = nRejectLineStPos + (8 * i);
// 			pRejectPt.y = 0;
// 			
// 			pDC->LineTo(pRejectPt);
// 		}
// 	}
}

void CAnimate::uGoodBufRPcbMonitoring(CDC *pDC, int *nRightGoodBufExistInfo)
{
// 	int nRtn, nTmp;
// 	int nRejectLineStPos = 575 /*622*/;

// 	nTmp = st_basic.n_Tray_YCnt/* / 2*/;
// 
// 	for(int i=0; i<nTmp; i++)
// 	{
// 		nRtn = nRightGoodBufExistInfo[/*nTmp + */i];
// 		if(nRtn == TRUE)
// 		{	
// 			pRejectPt.x = nRejectLineStPos + (8 * i);
// 			pRejectPt.y = 270;
// 			
// 			pDC->MoveTo(pRejectPt);
// 			
// 			pRejectPt.x = nRejectLineStPos + (8 * i);
// 			pRejectPt.y = 170;
// 			
// 			pDC->LineTo(pRejectPt);
// 		}
// 		else
// 		{
// 			pRejectPt.x = nRejectLineStPos + (8 * i);
// 			pRejectPt.y = 0;
// 			
// 			pDC->MoveTo(pRejectPt);
// 			
// 			pRejectPt.x = nRejectLineStPos + (8 * i);
// 			pRejectPt.y = 0;
// 			
// 			pDC->LineTo(pRejectPt);
// 		}
// 	}
}

void CAnimate::uRejectBufPcbMonitoring(CDC *pDC, int *nRejectBufPcbExistInfo)
{
//	int nRtn,nRejectLineStPos = 446;
// 	for(int i=0; i<st_basic.n_Tray_YCnt; i++)
// 	{
// 		nRtn = nRejectBufPcbExistInfo[i];
// 		if(nRtn == TRUE)
// 		{	
// 			pRejectPt.x = nRejectLineStPos + (8 * i);
// 			pRejectPt.y = 270;
// 
// 			pDC->MoveTo(pRejectPt);
// 
// 			pRejectPt.x = nRejectLineStPos + (8 * i);
// 			pRejectPt.y = 170;
// 			
// 			pDC->LineTo(pRejectPt);
// 		}
// 		else
// 		{
// 			pRejectPt.x = nRejectLineStPos + (8 * i);
// 			pRejectPt.y = 0;
// 
// 			pDC->MoveTo(pRejectPt);
// 
// 			pRejectPt.x = nRejectLineStPos + (8 * i);
// 			pRejectPt.y = 0;
// 			
// 			pDC->LineTo(pRejectPt);
// 		}
// 	}
}

void CAnimate::ActionDisplay(CDC *pDC /*= NULL*/)
{
	//if(!IsWindowVisible()) return;
	//===default Code =====
	CDC		memDC;
	CFont	*pOldFont = NULL;
	CPen    *pOldPen = NULL;
	CBrush	*pOldBrush = NULL;
	CString strTmp;		 
  	if(pDC== NULL)
		pDC  = GetDC();

	CRect rect;

	CRect rBtn[3] = {(m_BtnRect[0]),(m_BtnRect[1]),(m_BtnRect[2])};
 	rBtn[0] += CRect(6,6,6,6);
	rBtn[1] += CRect(6,6,6,6);

	CRect Rectangle = m_BtnRect[0];

 	GetClientRect(&rect);
	CBitmap memBitmap, *OldmemBitmap;
	memDC.CreateCompatibleDC(pDC);

	memBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	OldmemBitmap = (CBitmap *)memDC.SelectObject(&memBitmap);

	pOldFont  = memDC.SelectObject(&m_AniFont);
	pOldPen  = memDC.SelectObject(&m_AniPen);
	pOldBrush  = memDC.SelectObject(&m_AniBrush);

	memDC.SetBkMode(TRANSPARENT);
	memDC.SetTextColor(RGB(0,0,255));

	memDC.BitBlt(0,0, rect.Width(), rect.Height(), &m_FrameBase, 0, 0, SRCCOPY);				

	//////////////////////////////////////////////////////
	// 이송 축
	//////////////////////////////////////////////////////2014,1125 ysj
	////amt8590
	m_ID_Buffer1.DrawImg(memDC);
	m_ID_Buffer2.DrawImg(memDC);
	m_ID_Buffer3.DrawImg(memDC);
	m_ID_Buffer4.DrawImg(memDC);
	m_ID_Buffer5.DrawImg(memDC);
	
	if(m_ID_Buffer1.m_bGlass[0] == TRUE) m_T_Box1.DrawImg(memDC);
	if(m_ID_Buffer1.m_bGlass[1] == TRUE) m_T_Box2.DrawImg(memDC);
	if(m_ID_Buffer2.m_bGlass[0] == TRUE) m_T_Box3.DrawImg(memDC);
	if(m_ID_Buffer2.m_bGlass[1] == TRUE) m_T_Box4.DrawImg(memDC);
	if(m_ID_Buffer3.m_bGlass[0] == TRUE) m_T_Box5.DrawImg(memDC);
	if(m_ID_Buffer3.m_bGlass[1] == TRUE) m_T_Box6.DrawImg(memDC);
	if(m_ID_Buffer4.m_bGlass[0] == TRUE) m_T_Box7.DrawImg(memDC);
	if(m_ID_Buffer4.m_bGlass[1] == TRUE) m_T_Box8.DrawImg(memDC);
	if(m_ID_Buffer5.m_bGlass[0] == TRUE) m_T_Box9.DrawImg(memDC);
	if(m_ID_Buffer5.m_bGlass[1] == TRUE) m_T_Box10.DrawImg(memDC);
	
	if(m_ID_Buffer1.m_bSkip[0] == TRUE) m_T_Box_Skip1.DrawImg(memDC);
	if(m_ID_Buffer1.m_bSkip[1] == TRUE) m_T_Box_Skip2.DrawImg(memDC);
	if(m_ID_Buffer2.m_bSkip[0] == TRUE) m_T_Box_Skip3.DrawImg(memDC);
	if(m_ID_Buffer2.m_bSkip[1] == TRUE) m_T_Box_Skip4.DrawImg(memDC);
	if(m_ID_Buffer3.m_bSkip[0] == TRUE) m_T_Box_Skip5.DrawImg(memDC);
	if(m_ID_Buffer3.m_bSkip[1] == TRUE) m_T_Box_Skip6.DrawImg(memDC);
	if(m_ID_Buffer4.m_bSkip[0] == TRUE) m_T_Box_Skip7.DrawImg(memDC);
	if(m_ID_Buffer4.m_bSkip[1] == TRUE) m_T_Box_Skip8.DrawImg(memDC);
	if(m_ID_Buffer5.m_bSkip[0] == TRUE) m_T_Box_Skip9.DrawImg(memDC);
	if(m_ID_Buffer5.m_bSkip[1] == TRUE) m_T_Box_Skip10.DrawImg(memDC);

// 



	// 	//===default Code =====
 	pDC->BitBlt(0,0,  rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	
	memDC.SelectObject(OldmemBitmap);
	memDC.SelectObject(pOldFont);
	memDC.SelectObject(pOldPen);
	memDC.SelectObject(pOldBrush);
	DeleteObject(OldmemBitmap);
	ReleaseDC(pDC);
	memDC.DeleteDC();
	
	//=====================
}

void CAnimate::AniPioStatus()
{
	//
}


void CAnimate::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_BtnRect[0].PtInRect(point))
	{
		m_bClicked[0] = TRUE/*!m_bClicked*/;
		RedrawWindow(&m_BtnRect[0]);
	}

	if(m_BtnRect[1].PtInRect(point))
	{
		m_bClicked[1] = TRUE/*!m_bClicked*/;
		RedrawWindow(&m_BtnRect[1]);
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CAnimate::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_BtnRect[0].PtInRect(point))
	{
		m_bClicked[0] = FALSE/*!m_bClicked*/;
		RedrawWindow(&m_BtnRect[0]);
	}

	if(m_BtnRect[1].PtInRect(point))
	{
		m_bClicked[1] = FALSE/*!m_bClicked*/;
		RedrawWindow(&m_BtnRect[1]);
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CAnimate::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_CursorPoint = point;

	if(m_bClicked[0] == TRUE)
	{
		if(!m_BtnRect[0].PtInRect(point))
		{
			m_bClicked[0] = FALSE;
			RedrawWindow(&m_BtnRect[0]);
		}
	}

	if(m_bClicked[1] == TRUE)
	{
		if(!m_BtnRect[1].PtInRect(point))
		{
			m_bClicked[1] = FALSE;
			RedrawWindow(&m_BtnRect[1]);
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}


////2014.0821
//*********************************************
//					CID_Buffer1
//*********************************************

CID_Buffer1::CID_Buffer1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0), m_ptLeftTopAlign(0), m_ptLeftBtmAlign(0), m_ptRightTopAlign(0), m_ptRightBtmAlign(0)
{
	sprintf(m_cLeftTopText,"", sizeof(m_cLeftTopText));
	sprintf(m_cLeftBtmText,"", sizeof(m_cLeftBtmText));	
	sprintf(m_cRightTopText,"", sizeof(m_cRightTopText));
	sprintf(m_cRighBtmtText,"", sizeof(m_cRighBtmtText));	
}

CID_Buffer1::~CID_Buffer1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CID_Buffer1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CID_Buffer1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CID_Buffer1:: DrawImg(CDC &dc)
{
	int i;

	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;

	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));

	for(i=0; i<2; i++)
	{
		if(m_bGlass[i] < 0 && m_bSkip[i] <0)
		{
			m_bGlass[i] = 0;
			m_bSkip[i]  = 0;
		}
	}

	
////amt8580s
	if(m_bGlass[0] == TRUE || m_bSkip[0] == TRUE)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.55 + m_dEqCurPos_Z );//좌우 폭
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.38 + m_dEqCurPos_Z );//상하 폭
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/4.2,m_nImgCurPos_Y+m_ImgSize.cy/2.1);//상하좌우 높이
		m_Glass.DrawImg(dc);	
	}
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

	if(m_bGlass[1] == TRUE || m_bSkip[1] == TRUE)
	{
		m_Glass2.m_nDegree2 = 0; 
		m_Glass2.m_sizGlassSizeMod2.cx = (long) ( m_Glass2.m_sizGlassSize2.cx  * 0.55 + m_dEqCurPos_Z );
		m_Glass2.m_sizGlassSizeMod2.cy = (long) ( m_Glass2.m_sizGlassSize2.cy  * 0.38 + m_dEqCurPos_Z );
		
		m_Glass2.m_nGlassPos2=CPoint(m_nImgCurPos_X+ m_ImgSize2.cx/1.33,m_nImgCurPos_Y+m_ImgSize2.cy/2.1);
		m_Glass2.DrawImg2(dc);
	}
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

	if(m_bGlass[0] == TRUE)
	{
		dc.SetTextColor(m_cLeftTopColor);
		dc.TextOut(rect.left + m_ptLeftTopAlign.x,  rect.top + m_ptLeftTopAlign.y - 30, m_cLeftTopText);
		
		dc.SetTextColor(m_cLeftBtmColor);
		dc.TextOut(rect.left + m_ptLeftBtmAlign.x, rect.top + m_ptLeftBtmAlign.y + 170, m_cLeftBtmText);
	}
	if(m_bGlass[1] == TRUE)
	{		
		dc.SetTextColor(m_cRightTopColor);
		dc.TextOut(rect.left + m_ptRightTopAlign.x,  rect.top + m_ptRightTopAlign.y - 15, m_cRightTopText);
		
		dc.SetTextColor(m_cRightBtmColor);
		dc.TextOut(rect.left + m_ptRightBtmAlign.x, rect.top + m_ptRightBtmAlign.y + 185, m_cRighBtmtText);
	}

////	
}


//*********************************************
//					CID_Buffer2
//*********************************************

CID_Buffer2::CID_Buffer2():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0), m_ptLeftTopAlign(0), m_ptLeftBtmAlign(0), m_ptRightTopAlign(0), m_ptRightBtmAlign(0)
{
	sprintf(m_cLeftTopText,"", sizeof(m_cLeftTopText));
	sprintf(m_cLeftBtmText,"", sizeof(m_cLeftBtmText));	
	sprintf(m_cRightTopText,"", sizeof(m_cRightTopText));
	sprintf(m_cRighBtmtText,"", sizeof(m_cRighBtmtText));		
}

CID_Buffer2::~CID_Buffer2()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CID_Buffer2::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CID_Buffer2::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CID_Buffer2:: DrawImg(CDC &dc)
{
	int i;

	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));

	for(i=0; i<2; i++)
	{
		if(m_bGlass[i] < 0 && m_bSkip[i] <0)
		{
			m_bGlass[i] = 0;
			m_bSkip[i]  = 0;
		}
	}

////amt8580s
	if(m_bGlass[0] == TRUE || m_bSkip[0] == TRUE)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.55 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.38 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/4.1,m_nImgCurPos_Y+m_ImgSize.cy/2.1);
		m_Glass.DrawImg(dc);	
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
	
	if(m_bGlass[1] == TRUE || m_bSkip[1] == TRUE)
	{
		m_Glass2.m_nDegree2 = 0; 
		m_Glass2.m_sizGlassSizeMod2.cx = (long) ( m_Glass2.m_sizGlassSize2.cx  * 0.55 + m_dEqCurPos_Z );
		m_Glass2.m_sizGlassSizeMod2.cy = (long) ( m_Glass2.m_sizGlassSize2.cy  * 0.38 + m_dEqCurPos_Z );
		
		m_Glass2.m_nGlassPos2=CPoint(m_nImgCurPos_X+ m_ImgSize2.cx/1.33,m_nImgCurPos_Y+m_ImgSize2.cy/2.1);
		m_Glass2.DrawImg2(dc);
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

	
	
	if(m_bGlass[0] == TRUE)
	{
		dc.SetTextColor(m_cLeftTopColor);
		dc.TextOut(rect.left + m_ptLeftTopAlign.x,  rect.top + m_ptLeftTopAlign.y - 30, m_cLeftTopText);
		
		dc.SetTextColor(m_cLeftBtmColor);
		dc.TextOut(rect.left + m_ptLeftBtmAlign.x, rect.top + m_ptLeftBtmAlign.y + 170, m_cLeftBtmText);
	}
	if(m_bGlass[1] == TRUE)
	{		
		dc.SetTextColor(m_cRightTopColor);
		dc.TextOut(rect.left + m_ptRightTopAlign.x,  rect.top + m_ptRightTopAlign.y - 15, m_cRightTopText);
		
		dc.SetTextColor(m_cRightBtmColor);
		dc.TextOut(rect.left + m_ptRightBtmAlign.x, rect.top + m_ptRightBtmAlign.y + 185, m_cRighBtmtText);
	}
////	
}


//*********************************************
//					CID_Buffer3
//*********************************************

CID_Buffer3::CID_Buffer3():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0), m_ptLeftTopAlign(0), m_ptLeftBtmAlign(0), m_ptRightTopAlign(0), m_ptRightBtmAlign(0)
{
	sprintf(m_cLeftTopText,"", sizeof(m_cLeftTopText));
	sprintf(m_cLeftBtmText,"", sizeof(m_cLeftBtmText));	
	sprintf(m_cRightTopText,"", sizeof(m_cRightTopText));
	sprintf(m_cRighBtmtText,"", sizeof(m_cRighBtmtText));	
	
}

CID_Buffer3::~CID_Buffer3()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CID_Buffer3::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;
}

void CID_Buffer3::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CID_Buffer3:: DrawImg(CDC &dc)
{
	int i;

	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	
	for(i=0; i<2; i++)
	{
		if(m_bGlass[i] < 0 && m_bSkip[i] <0)
		{
			m_bGlass[i] = 0;
			m_bSkip[i]  = 0;
		}
	}
	
////amt8580s
	if(m_bGlass[0] == TRUE || m_bSkip[0] == TRUE)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.55 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.38 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/4.1,m_nImgCurPos_Y+m_ImgSize.cy/2.1);
		m_Glass.DrawImg(dc);	
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
	
	if(m_bGlass[1] == TRUE || m_bSkip[1] == TRUE)
	{
		m_Glass2.m_nDegree2 = 0; 
		m_Glass2.m_sizGlassSizeMod2.cx = (long) ( m_Glass2.m_sizGlassSize2.cx  * 0.55 + m_dEqCurPos_Z );
		m_Glass2.m_sizGlassSizeMod2.cy = (long) ( m_Glass2.m_sizGlassSize2.cy  * 0.38 + m_dEqCurPos_Z );
		
		m_Glass2.m_nGlassPos2=CPoint(m_nImgCurPos_X+ m_ImgSize2.cx/1.33,m_nImgCurPos_Y+m_ImgSize2.cy/2.1);
		m_Glass2.DrawImg2(dc);
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

	
	if(m_bGlass[0] == TRUE)
	{
		dc.SetTextColor(m_cLeftTopColor);
		dc.TextOut(rect.left + m_ptLeftTopAlign.x,  rect.top + m_ptLeftTopAlign.y - 30, m_cLeftTopText);
		
		dc.SetTextColor(m_cLeftBtmColor);
		dc.TextOut(rect.left + m_ptLeftBtmAlign.x, rect.top + m_ptLeftBtmAlign.y + 170, m_cLeftBtmText);
	}
	if(m_bGlass[1] == TRUE)
	{		
		dc.SetTextColor(m_cRightTopColor);
		dc.TextOut(rect.left + m_ptRightTopAlign.x,  rect.top + m_ptRightTopAlign.y - 15, m_cRightTopText);
		
		dc.SetTextColor(m_cRightBtmColor);
		dc.TextOut(rect.left + m_ptRightBtmAlign.x, rect.top + m_ptRightBtmAlign.y + 185, m_cRighBtmtText);
	}
	////
}

//*********************************************
//					CID_Buffer4
//*********************************************

CID_Buffer4::CID_Buffer4():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0), m_ptLeftTopAlign(0), m_ptLeftBtmAlign(0), m_ptRightTopAlign(0), m_ptRightBtmAlign(0)
{
	sprintf(m_cLeftTopText,"", sizeof(m_cLeftTopText));
	sprintf(m_cLeftBtmText,"", sizeof(m_cLeftBtmText));	
	sprintf(m_cRightTopText,"", sizeof(m_cRightTopText));
	sprintf(m_cRighBtmtText,"", sizeof(m_cRighBtmtText));	
	
}

CID_Buffer4::~CID_Buffer4()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CID_Buffer4::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;
}

void CID_Buffer4::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CID_Buffer4:: DrawImg(CDC &dc)
{
	int i;

	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	
	for(i=0; i<2; i++)
	{
		if(m_bGlass[i] < 0 && m_bSkip[i] <0)
		{
			m_bGlass[i] = 0;
			m_bSkip[i]  = 0;
		}
	}

////amt8580s
	if(m_bGlass[0] == TRUE || m_bSkip[0] == TRUE)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.55 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.38 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/4.1,m_nImgCurPos_Y+m_ImgSize.cy/2.1);
		m_Glass.DrawImg(dc);	
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
	
	if(m_bGlass[1] == TRUE || m_bSkip[1] == TRUE)
	{
		m_Glass2.m_nDegree2 = 0; 
		m_Glass2.m_sizGlassSizeMod2.cx = (long) ( m_Glass2.m_sizGlassSize2.cx  * 0.55 + m_dEqCurPos_Z );
		m_Glass2.m_sizGlassSizeMod2.cy = (long) ( m_Glass2.m_sizGlassSize2.cy  * 0.38 + m_dEqCurPos_Z );
		
		m_Glass2.m_nGlassPos2=CPoint(m_nImgCurPos_X+ m_ImgSize2.cx/1.33,m_nImgCurPos_Y+m_ImgSize2.cy/2.1);
		m_Glass2.DrawImg2(dc);
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

	
	if(m_bGlass[0] == TRUE)
	{
		dc.SetTextColor(m_cLeftTopColor);
		dc.TextOut(rect.left + m_ptLeftTopAlign.x,  rect.top + m_ptLeftTopAlign.y - 30, m_cLeftTopText);
		
		dc.SetTextColor(m_cLeftBtmColor);
		dc.TextOut(rect.left + m_ptLeftBtmAlign.x, rect.top + m_ptLeftBtmAlign.y + 170, m_cLeftBtmText);
	}
	if(m_bGlass[1] == TRUE)
	{		
		dc.SetTextColor(m_cRightTopColor);
		dc.TextOut(rect.left + m_ptRightTopAlign.x,  rect.top + m_ptRightTopAlign.y - 15, m_cRightTopText);
		
		dc.SetTextColor(m_cRightBtmColor);
		dc.TextOut(rect.left + m_ptRightBtmAlign.x, rect.top + m_ptRightBtmAlign.y + 185, m_cRighBtmtText);
	}
	////
}

//*********************************************
//					CID_Buffer5
//*********************************************

CID_Buffer5::CID_Buffer5():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0), m_ptLeftTopAlign(0), m_ptLeftBtmAlign(0), m_ptRightTopAlign(0), m_ptRightBtmAlign(0)
{
	sprintf(m_cLeftTopText,"", sizeof(m_cLeftTopText));
	sprintf(m_cLeftBtmText,"", sizeof(m_cLeftBtmText));	
	sprintf(m_cRightTopText,"", sizeof(m_cRightTopText));
	sprintf(m_cRighBtmtText,"", sizeof(m_cRighBtmtText));	
}

CID_Buffer5::~CID_Buffer5()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CID_Buffer5::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;
}

void CID_Buffer5::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CID_Buffer5:: DrawImg(CDC &dc)
{
	int i;

	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	
	for(i=0; i<2; i++)
	{
		if(m_bGlass[i] < 0 && m_bSkip[i] <0)
		{
			m_bGlass[i] = 0;
			m_bSkip[i]  = 0;
		}
	}

////amt8580s
	if(m_bGlass[0] == TRUE || m_bSkip[0] == TRUE)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.55 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.38 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/4.1,m_nImgCurPos_Y+m_ImgSize.cy/2.1);
		m_Glass.DrawImg(dc);	
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
	
	if(m_bGlass[1] == TRUE || m_bSkip[1] == TRUE)
	{
		m_Glass2.m_nDegree2 = 0; 
		m_Glass2.m_sizGlassSizeMod2.cx = (long) ( m_Glass2.m_sizGlassSize2.cx  * 0.55 + m_dEqCurPos_Z );
		m_Glass2.m_sizGlassSizeMod2.cy = (long) ( m_Glass2.m_sizGlassSize2.cy  * 0.38 + m_dEqCurPos_Z );
		
		m_Glass2.m_nGlassPos2=CPoint(m_nImgCurPos_X+ m_ImgSize2.cx/1.33,m_nImgCurPos_Y+m_ImgSize2.cy/2.1);
		m_Glass2.DrawImg2(dc);
	}
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	


	if(m_bGlass[0] == TRUE)
	{
		dc.SetTextColor(m_cLeftTopColor);
		dc.TextOut(rect.left + m_ptLeftTopAlign.x,  rect.top + m_ptLeftTopAlign.y - 30, m_cLeftTopText);
		
		dc.SetTextColor(m_cLeftBtmColor);
		dc.TextOut(rect.left + m_ptLeftBtmAlign.x, rect.top + m_ptLeftBtmAlign.y + 170, m_cLeftBtmText);
	}
	if(m_bGlass[1] == TRUE)
	{		
		dc.SetTextColor(m_cRightTopColor);
		dc.TextOut(rect.left + m_ptRightTopAlign.x,  rect.top + m_ptRightTopAlign.y - 15, m_cRightTopText);
		
		dc.SetTextColor(m_cRightBtmColor);
		dc.TextOut(rect.left + m_ptRightBtmAlign.x, rect.top + m_ptRightBtmAlign.y + 185, m_cRighBtmtText);
	}
	////	
}

////2014,1125 ysj
//*********************************************
//					CT_Box1
//*********************************************

CT_Box1::CT_Box1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0)
{
}

CT_Box1::~CT_Box1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CT_Box1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CT_Box1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CT_Box1:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;

	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));

	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

////	
}

//*********************************************
//					CT_Box2
//*********************************************

CT_Box2::CT_Box2():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0)
{
}

CT_Box2::~CT_Box2()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CT_Box2::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CT_Box2::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CT_Box2:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;

	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

////	
}

//*********************************************
//					CT_Box3
//*********************************************

CT_Box3::CT_Box3():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0)
{
}

CT_Box3::~CT_Box3()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CT_Box3::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CT_Box3::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CT_Box3:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;

	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));

	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

////	
}

//*********************************************
//					CT_Box4
//*********************************************

CT_Box4::CT_Box4():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0)
{
}

CT_Box4::~CT_Box4()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CT_Box4::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CT_Box4::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CT_Box4:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;

	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

////	
}

//*********************************************
//					CT_Box5
//*********************************************

CT_Box5::CT_Box5():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0)
{
}

CT_Box5::~CT_Box5()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CT_Box5::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CT_Box5::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CT_Box5:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;

	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

////	
}

//*********************************************
//					CT_Box6
//*********************************************

CT_Box6::CT_Box6():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0)
{
}

CT_Box6::~CT_Box6()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CT_Box6::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CT_Box6::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CT_Box6:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;

	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));

	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

////	
}

//*********************************************
//					CT_Box7
//*********************************************

CT_Box7::CT_Box7():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0)
{
}

CT_Box7::~CT_Box7()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CT_Box7::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CT_Box7::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CT_Box7:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;

	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));

	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

////	
}

//*********************************************
//					CT_Box8
//*********************************************

CT_Box8::CT_Box8():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0)
{
}

CT_Box8::~CT_Box8()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CT_Box8::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CT_Box8::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CT_Box8:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;

	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

////	
}

//*********************************************
//					CT_Box9
//*********************************************

CT_Box9::CT_Box9():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0)
{
}

CT_Box9::~CT_Box9()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CT_Box9::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CT_Box9::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CT_Box9:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;

	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));

	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

////	
}

//*********************************************
//					CT_Box10
//*********************************************

CT_Box10::CT_Box10():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0)
{
}

CT_Box10::~CT_Box10()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CT_Box10::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CT_Box10::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CT_Box10:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;

	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

////	
}

//*********************************************
//					CT_Box_Skip1
//*********************************************

CT_Box_Skip1::CT_Box_Skip1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0)
{
}

CT_Box_Skip1::~CT_Box_Skip1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CT_Box_Skip1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CT_Box_Skip1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CT_Box_Skip1:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;

	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));

	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

////	
}


//*********************************************
//					CT_Box_Skip2
//*********************************************

CT_Box_Skip2::CT_Box_Skip2():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0)
{
}

CT_Box_Skip2::~CT_Box_Skip2()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CT_Box_Skip2::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CT_Box_Skip2::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CT_Box_Skip2:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;

	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));

	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

////	
}

//*********************************************
//					CT_Box_Skip3
//*********************************************

CT_Box_Skip3::CT_Box_Skip3():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0)
{
}

CT_Box_Skip3::~CT_Box_Skip3()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CT_Box_Skip3::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CT_Box_Skip3::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CT_Box_Skip3:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;

	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

////	
}

//*********************************************
//					CT_Box_Skip4
//*********************************************

CT_Box_Skip4::CT_Box_Skip4():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0)
{
}

CT_Box_Skip4::~CT_Box_Skip4()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CT_Box_Skip4::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CT_Box_Skip4::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CT_Box_Skip4:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;

	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));

	
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

////	
}

//*********************************************
//					CT_Box_Skip5
//*********************************************

CT_Box_Skip5::CT_Box_Skip5():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0)
{
}

CT_Box_Skip5::~CT_Box_Skip5()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CT_Box_Skip5::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CT_Box_Skip5::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CT_Box_Skip5:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;

	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));

	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

////	
}

//*********************************************
//					CT_Box_Skip6
//*********************************************

CT_Box_Skip6::CT_Box_Skip6():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0)
{
}

CT_Box_Skip6::~CT_Box_Skip6()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CT_Box_Skip6::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CT_Box_Skip6::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CT_Box_Skip6:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;

	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));

	
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

////	
}

//*********************************************
//					CT_Box_Skip7
//*********************************************

CT_Box_Skip7::CT_Box_Skip7():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0)
{
}

CT_Box_Skip7::~CT_Box_Skip7()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CT_Box_Skip7::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CT_Box_Skip7::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CT_Box_Skip7:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;

	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));

	
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

////	
}

//*********************************************
//					CT_Box_Skip8
//*********************************************

CT_Box_Skip8::CT_Box_Skip8():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0)
{
}

CT_Box_Skip8::~CT_Box_Skip8()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CT_Box_Skip8::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CT_Box_Skip8::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CT_Box_Skip8:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;

	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));

	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	


////	
}

//*********************************************
//					CT_Box_Skip9
//*********************************************

CT_Box_Skip9::CT_Box_Skip9():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0)
{
}

CT_Box_Skip9::~CT_Box_Skip9()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CT_Box_Skip9::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CT_Box_Skip9::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CT_Box_Skip9:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;

	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));

	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	


////	
}

//*********************************************
//					CT_Box_Skip10
//*********************************************

CT_Box_Skip10::CT_Box_Skip10():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0)
{
}

CT_Box_Skip10::~CT_Box_Skip10()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CT_Box_Skip10::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CT_Box_Skip10::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CT_Box_Skip10:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;

	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	


////	
}



//*********************************************
//					CID_Buffer_Gripper1
//*********************************************

CID_Buffer_Gripper1::CID_Buffer_Gripper1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CID_Buffer_Gripper1::~CID_Buffer_Gripper1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CID_Buffer_Gripper1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CID_Buffer_Gripper1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CID_Buffer_Gripper1:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
// 	if(m_bGlass < 0)
// 		m_bGlass = 0;
// 	
// 	if(m_bGlass)
// 	{
// 		m_Glass.m_nDegree = 0; 
// 		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 + m_dEqCurPos_Z );
// 		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 + m_dEqCurPos_Z );
// 		
// 		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
// 		m_Glass.DrawImg(dc);
// 	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}



//*********************************************
//					CID_Buffer_Gripper2
//*********************************************

CID_Buffer_Gripper2::CID_Buffer_Gripper2():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CID_Buffer_Gripper2::~CID_Buffer_Gripper2()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CID_Buffer_Gripper2::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CID_Buffer_Gripper2::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CID_Buffer_Gripper2:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
// 	if(m_bGlass < 0)
// 		m_bGlass = 0;
// 	
// 	if(m_bGlass)
// 	{
// 		m_Glass.m_nDegree = 0; 
// 		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 + m_dEqCurPos_Z );
// 		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 + m_dEqCurPos_Z );
// 		
// 		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
// 		m_Glass.DrawImg(dc);
// 	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}



//*********************************************
//					CID_Buffer_Gripper3
//*********************************************

CID_Buffer_Gripper3::CID_Buffer_Gripper3():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CID_Buffer_Gripper3::~CID_Buffer_Gripper3()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CID_Buffer_Gripper3::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CID_Buffer_Gripper3::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CID_Buffer_Gripper3:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
// 	if(m_bGlass < 0)
// 		m_bGlass = 0;
// 	
// 	if(m_bGlass)
// 	{
// 		m_Glass.m_nDegree = 0; 
// 		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 + m_dEqCurPos_Z );
// 		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 + m_dEqCurPos_Z );
// 		
// 		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
// 		m_Glass.DrawImg(dc);
// 	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}


//*********************************************
//					CID_Buffer_Gripper4
//*********************************************

CID_Buffer_Gripper4::CID_Buffer_Gripper4():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CID_Buffer_Gripper4::~CID_Buffer_Gripper4()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CID_Buffer_Gripper4::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CID_Buffer_Gripper4::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CID_Buffer_Gripper4:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
// 	if(m_bGlass < 0)
// 		m_bGlass = 0;
// 	
// 	if(m_bGlass)
// 	{
// 		m_Glass.m_nDegree = 0; 
// 		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 + m_dEqCurPos_Z );
// 		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 + m_dEqCurPos_Z );
// 		
// 		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
// 		m_Glass.DrawImg(dc);
// 	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}

//*********************************************
//					CTRAY_PUSH1
//*********************************************

CTRAY_PUSH1::CTRAY_PUSH1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0)
{
}

CTRAY_PUSH1::~CTRAY_PUSH1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CTRAY_PUSH1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CTRAY_PUSH1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CTRAY_PUSH1:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;


// 	if(m_bGlass < 0)
// 		m_bGlass = 0;
	
// 	if(m_bGlass)
// 	{
// 		m_Glass.m_nDegree = 0; 
// 		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 + m_dEqCurPos_Z );
// 		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 + m_dEqCurPos_Z );
// 		
// 		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
// 		m_Glass.DrawImg(dc);
// 	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}

//*********************************************
//					CID_Move_CV1
//*********************************************

CID_Move_CV1::CID_Move_CV1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0)
{
}

CID_Move_CV1::~CID_Move_CV1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CID_Move_CV1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CID_Move_CV1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CID_Move_CV1:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;

	CRect rect;	 
	rect.left = int(m_nImgCurPos_X + (m_dRadioZ * m_dEqCurPos_Z* 0.5));
	rect.top  = int(m_nImgCurPos_Y + (m_dRadioZ * m_dEqCurPos_Z* 0.5));

	
////amt8580s
	if(m_bGlass[0] == TRUE)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.54 + m_dEqCurPos_Z );//좌우 폭
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.38 + m_dEqCurPos_Z );//상하 폭
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/5.7,m_nImgCurPos_Y+m_ImgSize.cy/2.1);//상하좌우 높이
		m_Glass.DrawImg(dc);	
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	


	if(m_bGlass[1] == TRUE)
	{
		m_Glass2.m_nDegree2 = 0; 
		m_Glass2.m_sizGlassSizeMod2.cx = (long) ( m_Glass2.m_sizGlassSize2.cx  * 0.54 + m_dEqCurPos_Z );
		m_Glass2.m_sizGlassSizeMod2.cy = (long) ( m_Glass2.m_sizGlassSize2.cy  * 0.38 + m_dEqCurPos_Z );
		
		m_Glass2.m_nGlassPos2=CPoint(m_nImgCurPos_X+ m_ImgSize2.cx/1.23,m_nImgCurPos_Y+m_ImgSize2.cy/2.1);
		m_Glass2.DrawImg2(dc);
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

////	
}



















////



//*********************************************
//					CTransferMod_1
//*********************************************

CTransferMod_1::CTransferMod_1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CTransferMod_1::~CTransferMod_1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CTransferMod_1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CTransferMod_1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CTransferMod_1:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
// 	if(m_bGlass < 0)
// 		m_bGlass = 0;
// 	
// 	if(m_bGlass)
// 	{
// 		m_Glass.m_nDegree = 0; 
// 		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 + m_dEqCurPos_Z );
// 		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 + m_dEqCurPos_Z );
// 		
// 		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
// 		m_Glass.DrawImg(dc);
// 	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}

//*********************************************
//					CTransferMod_2
//*********************************************

CTransferMod_2::CTransferMod_2():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CTransferMod_2::~CTransferMod_2()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CTransferMod_2::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CTransferMod_2::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CTransferMod_2:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
// 	if(m_bGlass < 0)
// 		m_bGlass = 0;
// 	
// 	if(m_bGlass)
// 	{
// 		m_Glass.m_nDegree = 0; 
// 		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 + m_dEqCurPos_Z );
// 		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 + m_dEqCurPos_Z );
// 		
// 		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
// 		m_Glass.DrawImg(dc);
// 	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}

//*********************************************
//					CArm
//*********************************************

CArm::CArm():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CArm::~CArm()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CArm::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;

	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CArm::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CArm:: DrawImg(CDC &dc)
{
	int i;

	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	for(i=0; i<2; i++)
	{
		if(m_bGlass[i] < 0)
			m_bGlass[i] = 0;	
	}
		
	if(m_bGlass[0])
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx/4 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy/8 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/7.8,m_nImgCurPos_Y+m_ImgSize.cy/4.25);
		m_Glass.DrawImg(dc);
	}	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

	if(m_bGlass[1])
	{
		m_Glass2.m_nDegree2 = 0; 
		m_Glass2.m_sizGlassSizeMod2.cx = (long) ( m_Glass2.m_sizGlassSize2.cx/4 + m_dEqCurPos_Z );
		m_Glass2.m_sizGlassSizeMod2.cy = (long) ( m_Glass2.m_sizGlassSize2.cy/8 + m_dEqCurPos_Z );
		
		m_Glass2.m_nGlassPos2=CPoint(m_nImgCurPos_X+ m_ImgSize2.cx/7.8,m_nImgCurPos_Y+m_ImgSize2.cy/1.32);
		m_Glass2.DrawImg2(dc);
	}	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
		
}

//*********************************************
//					CPitch1
//*********************************************

CPitch1::CPitch1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CPitch1::~CPitch1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CPitch1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;

	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CPitch1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CPitch1:: DrawImg(CDC &dc)
{
	int i;
	
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	for(i=0; i<2; i++)
	{
		if(m_bGlass[i] < 0)
			m_bGlass[i] = 0;	
	}
	
	if(m_bGlass[0])
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx/5 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy/7 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/5.3);
		m_Glass.DrawImg(dc);
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

	if(m_bGlass[1])
	{
		m_Glass2.m_nDegree2 = 0; 
		m_Glass2.m_sizGlassSizeMod2.cx = (long) ( m_Glass2.m_sizGlassSize2.cx/5 + m_dEqCurPos_Z );
		m_Glass2.m_sizGlassSizeMod2.cy = (long) ( m_Glass2.m_sizGlassSize2.cy/7 + m_dEqCurPos_Z );
		
		m_Glass2.m_nGlassPos2=CPoint(m_nImgCurPos_X+ m_ImgSize2.cx/2,m_nImgCurPos_Y+m_ImgSize2.cy/1.42);
		m_Glass2.DrawImg2(dc);
	}	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);		

}

//*********************************************
//					CPitch2
//*********************************************

CPitch2::CPitch2():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CPitch2::~CPitch2()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CPitch2::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;

	m_ImgSize2.cx = bs.bmWidth;
	m_ImgSize2.cy = bs.bmHeight;

}

void CPitch2::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CPitch2:: DrawImg(CDC &dc)
{
	int i;
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	for(i=0; i<2; i++)
	{
		if(m_bGlass[i] < 0)
			m_bGlass[i] = 0;	
	}
	
	if(m_bGlass[0])
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx/5 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy/7 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/5.3);
		m_Glass.DrawImg(dc);
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

	if(m_bGlass[1])
	{
		m_Glass2.m_nDegree2 = 0; 
		m_Glass2.m_sizGlassSizeMod2.cx = (long) ( m_Glass2.m_sizGlassSize2.cx/5 + m_dEqCurPos_Z );
		m_Glass2.m_sizGlassSizeMod2.cy = (long) ( m_Glass2.m_sizGlassSize2.cy/7 + m_dEqCurPos_Z );
		
		m_Glass2.m_nGlassPos2=CPoint(m_nImgCurPos_X+ m_ImgSize2.cx/2,m_nImgCurPos_Y+m_ImgSize2.cy/1.42);
		m_Glass2.DrawImg2(dc);
	}	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);		
}

//*********************************************
//					CCell1_Up
//*********************************************

CCell1_Up::CCell1_Up():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CCell1_Up::~CCell1_Up()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CCell1_Up::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CCell1_Up::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CCell1_Up:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	if(m_bGlass < 0)
		m_bGlass = 0;
	
	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx/3 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy/7 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}

//*********************************************
//					CCell2_Up
//*********************************************

CCell2_Up::CCell2_Up():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CCell2_Up::~CCell2_Up()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CCell2_Up::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CCell2_Up::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CCell2_Up:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	if(m_bGlass < 0)
		m_bGlass = 0;
	
	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx/4 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy/6 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}

//*********************************************
//					CCell1_Down
//*********************************************

CCell1_Down::CCell1_Down():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CCell1_Down::~CCell1_Down()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CCell1_Down::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CCell1_Down::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CCell1_Down:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	if(m_bGlass < 0)
		m_bGlass = 0;
	
	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx/3 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy/7 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}

//*********************************************
//					CCell2_Down
//*********************************************

CCell2_Down::CCell2_Down():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CCell2_Down::~CCell2_Down()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CCell2_Down::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CCell2_Down::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CCell2_Down:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	if(m_bGlass < 0)
		m_bGlass = 0;
	
	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx/4 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy/6 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z*0.5) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z * 0.52)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth*2)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}

//*********************************************
//					CTrayRobot
//*********************************************

CTrayRobot::CTrayRobot():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CTrayRobot::~CTrayRobot()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CTrayRobot::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CTrayRobot::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CTrayRobot:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	if(m_bGlass < 0)
		m_bGlass = 0;
	
	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 1.44 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.65 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/1.41,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}

//*********************************************
//					CTray1
//*********************************************

CTray1::CTray1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CTray1::~CTray1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CTray1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CTray1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CTray1:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	if(m_bGlass < 0)
		m_bGlass = 0;	

	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 1.44 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.65 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

}

//*********************************************
//					CTray2
//*********************************************

CTray2::CTray2():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CTray2::~CTray2()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CTray2::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;

}

void CTray2::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CTray2:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
	if(m_bGlass < 0)
		m_bGlass = 0;	

	if(m_bGlass)
	{
		m_Glass.m_nDegree = 0; 
		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 1.44 + m_dEqCurPos_Z );
		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.65 + m_dEqCurPos_Z );
		
		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
		m_Glass.DrawImg(dc);
	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	

}

//*********************************************
//					CAlignClamp1
//*********************************************

CAlignClamp1::CAlignClamp1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CAlignClamp1::~CAlignClamp1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CAlignClamp1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CAlignClamp1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CAlignClamp1:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
// 	if(m_bGlass < 0)
// 		m_bGlass = 0;
// 	
// 	if(m_bGlass)
// 	{
// 		m_Glass.m_nDegree = 0; 
// 		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 + m_dEqCurPos_Z );
// 		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 + m_dEqCurPos_Z );
// 		
// 		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
// 		m_Glass.DrawImg(dc);
// 	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}

//*********************************************
//					CAlignClamp2
//*********************************************

CAlignClamp2::CAlignClamp2():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CAlignClamp2::~CAlignClamp2()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CAlignClamp2::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CAlignClamp2::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CAlignClamp2:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
// 	if(m_bGlass < 0)
// 		m_bGlass = 0;
// 	
// 	if(m_bGlass)
// 	{
// 		m_Glass.m_nDegree = 0; 
// 		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 + m_dEqCurPos_Z );
// 		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 + m_dEqCurPos_Z );
// 		
// 		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
// 		m_Glass.DrawImg(dc);
// 	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}

//*********************************************
//					CAlignClamp3
//*********************************************

CAlignClamp3::CAlignClamp3():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CAlignClamp3::~CAlignClamp3()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CAlignClamp3::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CAlignClamp3::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CAlignClamp3:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
// 	if(m_bGlass < 0)
// 		m_bGlass = 0;
// 	
// 	if(m_bGlass)
// 	{
// 		m_Glass.m_nDegree = 0; 
// 		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 + m_dEqCurPos_Z );
// 		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 + m_dEqCurPos_Z );
// 		
// 		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
// 		m_Glass.DrawImg(dc);
// 	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);	
	
}

//*********************************************
//					CWorkbuffer_1
//*********************************************

CWorkbuffer1::CWorkbuffer1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CWorkbuffer1::~CWorkbuffer1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CWorkbuffer1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CWorkbuffer1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CWorkbuffer1:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
// 	if(m_bGlass < 0)
// 		m_bGlass = 0;
// 	
// 	if(m_bGlass)
// 	{
// 		m_Glass.m_nDegree = 0; 
// 		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 1.2 + m_dEqCurPos_Z );
// 		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.25 + m_dEqCurPos_Z );
// 		
// 		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
// 		m_Glass.DrawImg(dc);
// 	}
// 	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CWorkbuffer_2
//*********************************************

CWorkbuffer2::CWorkbuffer2():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CWorkbuffer2::~CWorkbuffer2()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CWorkbuffer2::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CWorkbuffer2::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CWorkbuffer2:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
// 	if(m_bGlass < 0)
// 		m_bGlass = 0;
// 	
// 	if(m_bGlass)
// 	{
// 		m_Glass.m_nDegree = 0; 
// 		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 + m_dEqCurPos_Z );
// 		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 + m_dEqCurPos_Z );
// 		
// 		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
// 		m_Glass.DrawImg(dc);
// 	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CWorkbuffer_3
//*********************************************

CWorkbuffer3::CWorkbuffer3():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CWorkbuffer3::~CWorkbuffer3()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CWorkbuffer3::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CWorkbuffer3::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CWorkbuffer3:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
// 	if(m_bGlass < 0)
// 		m_bGlass = 0;
// 	
// 	if(m_bGlass)
// 	{
// 		m_Glass.m_nDegree = 0; 
// 		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 + m_dEqCurPos_Z );
// 		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 + m_dEqCurPos_Z );
// 		
// 		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
// 		m_Glass.DrawImg(dc);
// 	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CWorkbuffer_4
//*********************************************

CWorkbuffer4::CWorkbuffer4():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CWorkbuffer4::~CWorkbuffer4()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CWorkbuffer4::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CWorkbuffer4::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CWorkbuffer4:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
	
// 	if(m_bGlass < 0)
// 		m_bGlass = 0;
// 	
// 	if(m_bGlass)
// 	{
// 		m_Glass.m_nDegree = 0; 
// 		m_Glass.m_sizGlassSizeMod.cx = (long) ( m_Glass.m_sizGlassSize.cx  * 0.95 + m_dEqCurPos_Z );
// 		m_Glass.m_sizGlassSizeMod.cy = (long) ( m_Glass.m_sizGlassSize.cy  * 0.78 + m_dEqCurPos_Z );
// 		
// 		m_Glass.m_nGlassPos=CPoint(m_nImgCurPos_X+ m_ImgSize.cx/2,m_nImgCurPos_Y+m_ImgSize.cy/2);
// 		m_Glass.DrawImg(dc);
// 	}
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CModuleRbt
//*********************************************

CModuleRbt::CModuleRbt():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CModuleRbt::~CModuleRbt()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CModuleRbt::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CModuleRbt::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CModuleRbt:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CLeftHSRbt
//*********************************************

CLeftHSRbt::CLeftHSRbt():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CLeftHSRbt::~CLeftHSRbt()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CLeftHSRbt::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CLeftHSRbt::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CLeftHSRbt:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CRightHSRbt
//*********************************************

CRightHSRbt::CRightHSRbt():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CRightHSRbt::~CRightHSRbt()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CRightHSRbt::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CRightHSRbt::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CRightHSRbt:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CClipRbt
//*********************************************

CClipRbt::CClipRbt():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CClipRbt::~CClipRbt()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CClipRbt::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CClipRbt::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CClipRbt:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CClampRbt
//*********************************************

CClampRbt::CClampRbt():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CClampRbt::~CClampRbt()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CClampRbt::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CClampRbt::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CClampRbt:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CSorterRbt
//*********************************************

CSorterRbt::CSorterRbt():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CSorterRbt::~CSorterRbt()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CSorterRbt::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CSorterRbt::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CSorterRbt:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CUnloaderRbt
//*********************************************

CUnloaderRbt::CUnloaderRbt():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CUnloaderRbt::~CUnloaderRbt()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CUnloaderRbt::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CUnloaderRbt::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CUnloaderRbt:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CMDStacker1
//*********************************************

CMDStacker1::CMDStacker1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CMDStacker1::~CMDStacker1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CMDStacker1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CMDStacker1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CMDStacker1:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CMDStacker2
//*********************************************

CMDStacker2::CMDStacker2():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CMDStacker2::~CMDStacker2()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CMDStacker2::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CMDStacker2::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CMDStacker2:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CLHSStacker1
//*********************************************

CLHSStacker1::CLHSStacker1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CLHSStacker1::~CLHSStacker1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CLHSStacker1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CLHSStacker1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CLHSStacker1:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CLHSStacker2
//*********************************************

CLHSStacker2::CLHSStacker2():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CLHSStacker2::~CLHSStacker2()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CLHSStacker2::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CLHSStacker2::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CLHSStacker2:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CRHSStacker1
//*********************************************

CRHSStacker1::CRHSStacker1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CRHSStacker1::~CRHSStacker1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CRHSStacker1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CRHSStacker1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CRHSStacker1:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CRHSStacker2
//*********************************************

CRHSStacker2::CRHSStacker2():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CRHSStacker2::~CRHSStacker2()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CRHSStacker2::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CRHSStacker2::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CRHSStacker2:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CClipTube1
//*********************************************

CClipTube1::CClipTube1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CClipTube1::~CClipTube1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CClipTube1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CClipTube1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CClipTube1:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CClipTube2
//*********************************************

CClipTube2::CClipTube2():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CClipTube2::~CClipTube2()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CClipTube2::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CClipTube2::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CClipTube2:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CULDStacker1
//*********************************************

CULDStacker1::CULDStacker1():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CULDStacker1::~CULDStacker1()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CULDStacker1::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CULDStacker1::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CULDStacker1:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CULDStacker2
//*********************************************

CULDStacker2::CULDStacker2():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CULDStacker2::~CULDStacker2()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CULDStacker2::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CULDStacker2::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CULDStacker2:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

//*********************************************
//					CRecject
//*********************************************

CRecject::CRecject():m_pBitDownPad(NULL),m_bDown(0),m_dEqCurPos_Z(0) 
{
	
}

CRecject::~CRecject()
{
	m_BitDownPad.DeleteObject();
	m_BitDownPadMsk.DeleteObject();

}
void CRecject::ComverEqPosToImgPos()
{
	CAxisMod::ComverEqPosToImgPos();

	BITMAP bs;
	if(m_bDown) 
	{
		m_BitDownPad.GetBitmap(&bs);
	}
	else
	{
		m_BitAxis.GetBitmap(&bs);
	}
	
	m_ImgSize.cx = bs.bmWidth;
	m_ImgSize.cy = bs.bmHeight;
}

void CRecject::CreateImg(CDC *pDC, int ImgID, int ImgIDMsk,int ImgDownID, int ImgDownIDMsk)
{
	
	CAxisMod::CreateImg(pDC,ImgID,ImgIDMsk);
	
	m_DcDownPad.CreateCompatibleDC(pDC);
	m_BitDownPad.LoadBitmap(ImgDownID);
	m_pBitDownPad = (CBitmap *)m_DcDownPad.SelectObject(&m_BitDownPad);
	
	m_DcDownPadMsk.CreateCompatibleDC(pDC);
	m_BitDownPadMsk.LoadBitmap(ImgDownIDMsk);
	m_pBitDownPadMsk = (CBitmap *)m_DcDownPadMsk.SelectObject(&m_BitDownPadMsk);
	
	BITMAP bs;
	m_BitDownPad.GetBitmap(&bs);
	m_DownSize.cx = bs.bmWidth;
	m_DownSize.cy = bs.bmHeight;
	
}

void CRecject:: DrawImg(CDC &dc)
{
	ComverEqPosToImgPos();	
	
	m_dRadioZ = 1;
		
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxisMask,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCPAINT);
	
	dc.StretchBlt(int(m_nImgCurPos_X - (m_dRadioZ * m_dEqCurPos_Z) - m_dEqWidth), int(m_nImgCurPos_Y - (m_dRadioZ * m_dEqCurPos_Z)), 
		int(m_ImgSize.cx   + (m_dRadioZ * m_dEqCurPos_Z) + (m_dEqWidth)), int(m_ImgSize.cy   + (m_dRadioZ * m_dEqCurPos_Z)),
		&m_DcAxis,0,0,m_ImgSize.cx,m_ImgSize.cy,SRCAND);
}

void CAnimate::ModuleTrayCheck(CDC *pDC)//, int *nModuleTrayCheckInfo)
{
//	CRect rect;
// 	int nRejectLineStXPos = 20;//1049;
// 	int nRejectLineStYPos = 350;//205;
// 
// 	pDC->SetTextColor(RGB(0,0,255));
// 	
// 	for(int i=0; i < st_basic.mn_lotset_module_load; i++)
// 	{
// 		if(st_modulemap.LoadTray[1][i] == DVC_YES)
// 		{
// 			rect.left = nRejectLineStXPos;
// 			rect.right = nRejectLineStXPos+48;
// 			rect.top = nRejectLineStYPos + (4 * i);
// 			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
// 			pDC->Rectangle(rect);
// 		}
// 		else
// 		{
// 			rect.left = 0;
// 			rect.right = 0;  
// 			rect.top = nRejectLineStYPos + (4 * i);
// 			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
// 			pDC->Rectangle(rect);
// 		}
// 	}
}


void CAnimate::LHSTrayCheck(CDC *pDC)//, int *nModuleTrayCheckInfo)
{
	CRect rect;
	int nRejectLineStXPos = 198;//1049;
	int nRejectLineStYPos = 340;//205;
	
	pDC->SetTextColor(RGB(0,0,255));
	
// 	for(int i=0; i < st_basic.mn_lotset_hs_left; i++)
// 	{
// 		if(st_modulemap.LefthsTray[1][i] == DVC_YES)
// 		{
// 			rect.left = nRejectLineStXPos;
// 			rect.right = nRejectLineStXPos+48;
// 			rect.top = nRejectLineStYPos + (4 * i);
// 			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
// 			pDC->Rectangle(rect);
// 		}
// 		else
// 		{
// 			rect.left = 0;
// 			rect.right = 0;  
// 			rect.top = nRejectLineStYPos + (4 * i);
// 			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
// 			pDC->Rectangle(rect);
// 		}
// 	}
}


void CAnimate::RHSTrayCheck(CDC *pDC)//, int *nModuleTrayCheckInfo)
{
	CRect rect;
	int nRejectLineStXPos = 398;//1049;
	int nRejectLineStYPos = 340;//205;
	
	pDC->SetTextColor(RGB(0,0,255));
	
// 	for(int i=0; i < st_basic.mn_lotset_hs_right; i++)
// 	{
// 		if(st_modulemap.RighthsTray[1][i] == DVC_YES)
// 		{
// 			rect.left = nRejectLineStXPos;
// 			rect.right = nRejectLineStXPos+48;
// 			rect.top = nRejectLineStYPos + (4 * i);
// 			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
// 			pDC->Rectangle(rect);
// 		}
// 		else
// 		{
// 			rect.left = 0;
// 			rect.right = 0;  
// 			rect.top = nRejectLineStYPos + (4 * i);
// 			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
// 			pDC->Rectangle(rect);
// 		}
// 	}
}


void CAnimate::ClipTube1Check(CDC *pDC)//, int *nModuleTrayCheckInfo)
{
	CRect rect;
	int nRejectLineStXPos = 595;//1049;
	int nRejectLineStYPos = 337;//205;
	
	pDC->SetTextColor(RGB(0,0,255));
	
// 	for(int i=0; i < st_basic.mn_lotset_clip; i++)
// 	{
// 		if(st_modulemap.ClipTray[1][i] == DVC_YES)
// 		{
// 			rect.left = nRejectLineStXPos;
// 			rect.right = nRejectLineStXPos+15;
// 			rect.top = nRejectLineStYPos + (4 * i);
// 			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
// 			pDC->Rectangle(rect);
// 		}
// 		else
// 		{
// 			rect.left = 0;
// 			rect.right = 0;  
// 			rect.top = nRejectLineStYPos + (4 * i);
// 			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
// 			pDC->Rectangle(rect);
// 		}
// 	}
}


void CAnimate::ClipTube2Check(CDC *pDC)//, int *nModuleTrayCheckInfo)
{
	CRect rect;
	int nRejectLineStXPos = 635;//1049;
	int nRejectLineStYPos = 337;//205;
	
	pDC->SetTextColor(RGB(0,0,255));
	
// 	for(int i=0; i < st_basic.mn_lotset_clip; i++)
// 	{
// 		if(st_modulemap.ClipTray[1][i] == DVC_YES)
// 		{
// 			rect.left = nRejectLineStXPos;
// 			rect.right = nRejectLineStXPos+15;
// 			rect.top = nRejectLineStYPos + (4 * i);
// 			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
// 			pDC->Rectangle(rect);
// 		}
// 		else
// 		{
// 			rect.left = 0;
// 			rect.right = 0;  
// 			rect.top = nRejectLineStYPos + (4 * i);
// 			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
// 			pDC->Rectangle(rect);
// 		}
// 	}
}


void CAnimate::ULDTrayCheck(CDC *pDC)//, int *nModuleTrayCheckInfo)
{
	CRect rect;
	int nRejectLineStXPos = 699;//1049;
	int nRejectLineStYPos = 350;//205;
	
	pDC->SetTextColor(RGB(0,0,255));
	
// 	for(int i=0; i < st_basic.mn_lotset_unloader; i++)
// 	{
// 		if(st_modulemap.UnloadTray[1][i] == DVC_YES)
// 		{
// 			rect.left = nRejectLineStXPos;
// 			rect.right = nRejectLineStXPos+48;
// 			rect.top = nRejectLineStYPos + (4 * i);
// 			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
// 			pDC->Rectangle(rect);
// 		}
// 		else
// 		{
// 			rect.left = 0;
// 			rect.right = 0;  
// 			rect.top = nRejectLineStYPos + (4 * i);
// 			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
// 			pDC->Rectangle(rect);
// 		}
// 	}
}


void CAnimate::RejectTrayCheck(CDC *pDC)//, int *nModuleTrayCheckInfo)
{
	CRect rect;
	int nRejectLineStXPos = 884;//1049;
	int nRejectLineStYPos = 350;//205;
	
	pDC->SetTextColor(RGB(0,0,255));
	
// 	for(int i=0; i < st_basic.mn_lotset_reject; i++)
// 	{
// 		if(st_modulemap.RejectTray[i] == DVC_YES)
// 		{
// 			rect.left = nRejectLineStXPos;
// 			rect.right = nRejectLineStXPos+48;
// 			rect.top = nRejectLineStYPos + (4 * i);
// 			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
// 			pDC->Rectangle(rect);
// 		}
// 		else
// 		{
// 			rect.left = 0;
// 			rect.right = 0;  
// 			rect.top = nRejectLineStYPos + (4 * i);
// 			rect.bottom = nRejectLineStYPos + (4 * i) + 1;
// 			pDC->Rectangle(rect);
// 		}
// 	}
}

void CAnimate::MDTray1Check(CDC *pDC)
{
	CRect rect;
	int nRejectLineStXPos;//1049;
	int nRejectLineStYPos;//205;

	int n_io_status = {0,};

	nRejectLineStXPos = 108;
	nRejectLineStYPos = 345;

// 	n_io_status = FAS_IO.get_in_bit(st_io.i_m_ld_slide_tray_chk,IO_ON);
// 
// 	if(n_io_status == IO_OFF)
// 	{
// 		rect.left = nRejectLineStXPos-2;
// 		rect.right = nRejectLineStXPos+50;
// 		rect.top = nRejectLineStYPos;
// 		rect.bottom = nRejectLineStYPos + (4) + 108;
// 		pDC->Rectangle(rect);
// 	}
// 	else
// 	{	
// 		rect.left = 0;
// 		rect.right = 0;  
// 		rect.top = nRejectLineStYPos;
// 		rect.bottom = nRejectLineStYPos + (4) + 108;
// 		pDC->Rectangle(rect);
// 	}
}

void CAnimate::LHSTray1Check(CDC *pDC)
{
	CRect rect;
	int nRejectLineStXPos;//1049;
	int nRejectLineStYPos;//205;
	
	int n_io_status = {0,};
	
	nRejectLineStXPos = 298;
	nRejectLineStYPos = 345;

// 	n_io_status = FAS_IO.get_in_bit(st_io.i_hs_fwd_ld_slide_tray_chk,IO_ON);
// 
// 	if(n_io_status == IO_OFF)
// 	{
// 		rect.left = nRejectLineStXPos-2;
// 		rect.right = nRejectLineStXPos+50;
// 		rect.top = nRejectLineStYPos;
// 		rect.bottom = nRejectLineStYPos + (4) + 108;
// 		pDC->Rectangle(rect);
// 	}
// 	else
// 	{	
// 		rect.left = 0;
// 		rect.right = 0;  
// 		rect.top = nRejectLineStYPos;
// 		rect.bottom = nRejectLineStYPos + (4) + 108;
// 		pDC->Rectangle(rect);
// 	}
}

void CAnimate::RHSTray1Check(CDC *pDC)
{
	CRect rect;
	int nRejectLineStXPos;//1049;
	int nRejectLineStYPos;//205;
	
	int n_io_status = {0,};
	
	nRejectLineStXPos = 498;
	nRejectLineStYPos = 345;

// 	n_io_status = FAS_IO.get_in_bit(st_io.i_hs_bwd_ld_slide_tray_chk,IO_ON);
// 
// 	if(n_io_status == IO_OFF)
// 	{
// 		rect.left = nRejectLineStXPos-2;
// 		rect.right = nRejectLineStXPos+50;
// 		rect.top = nRejectLineStYPos;
// 		rect.bottom = nRejectLineStYPos + (4) + 108;
// 		pDC->Rectangle(rect);
// 	}
// 	else
// 	{	
// 		rect.left = 0;
// 		rect.right = 0;  
// 		rect.top = nRejectLineStYPos;
// 		rect.bottom = nRejectLineStYPos + (4) + 108;
// 		pDC->Rectangle(rect);
//	}
}

void CAnimate::ULDTray1Check(CDC *pDC)
{
	CRect rect;
	int nRejectLineStXPos;//1049;
	int nRejectLineStYPos;//205;
	
	int n_io_status = {0,};
	nRejectLineStXPos = 798;
	nRejectLineStYPos = 345;
	
// 	n_io_status = FAS_IO.get_in_bit(st_io.i_uld_uld_slide_tray_chk,IO_ON);
// 	
// 	if(n_io_status == IO_OFF)
// 	{
// 		rect.left = nRejectLineStXPos-2;
// 		rect.right = nRejectLineStXPos+50;
// 		rect.top = nRejectLineStYPos;
// 		rect.bottom = nRejectLineStYPos + (4) + 108;
// 		pDC->Rectangle(rect);
// 	}
// 	else
// 	{	
// 		rect.left = 0;
// 		rect.right = 0;  
// 		rect.top = nRejectLineStYPos;
// 		rect.bottom = nRejectLineStYPos + (4) + 108;
// 		pDC->Rectangle(rect);
// 	}
}


