#include "stdafx.h"
#include "SceneInMemory.h"


SceneInMemory::SceneInMemory()
{
	p_bitmap = NULL;
	old_bitmap = NULL;
	is_dirty = true;
}


SceneInMemory::~SceneInMemory()
{
	delete p_bitmap;
}

void SceneInMemory::CreateCompatibleDC(CDC& dc)
{
	memDC.CreateCompatibleDC(&dc);
}

void  SceneInMemory::PrepareBitmap(const CRect& rc, CDC& dc)
{
	memDC.SelectObject(old_bitmap);
	delete p_bitmap;
	p_bitmap = new CBitmap();
	p_bitmap->CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
	old_bitmap = memDC.SelectObject(p_bitmap);
}

void  SceneInMemory::DrawScene(CDC& dc, const CRect& rc)
{
	dc.BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
}
