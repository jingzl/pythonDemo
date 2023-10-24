#ifndef _AFFINE_TRANS_H_
#define _AFFINE_TRANS_H_



class CAffineTrans
{
public:	
	CAffineTrans();
	~CAffineTrans();
	
	double m_mx[3][3];
	
	BOOL TransDisplayPointToAffineSpace(float& x, float& y, POINT* pPtDes);
};

#endif
