
#include "afxwin.h"
class CMyButton :
	public CButton
{
public:
	CMyButton();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual ~CMyButton();
};
