#pragma once
class SceneInMemory
{
public:
	SceneInMemory();
	virtual ~SceneInMemory();
	
	//something like initialization we call this method when CChildView is created
	void CreateCompatibleDC(CDC& dc);

	//this is the fisrt method that we need to call in case if IsDirty == true
	void PrepareBitmap(const CRect& rc, CDC& dc);
	
	//after we created bitmap we takes memory DC abd pass it to the render function
	//and that's all our scene is ready we only need to mark scene IsDirty = false
	CDC& GetMemoryDC()
	{
		return memDC;
	}


	bool IsDirty() const
	{
		return is_dirty;
	}
	void SetDirty(bool flag)
	{
		is_dirty = flag;
	}


	//this method just copies the content of internal bitmap (corresponded to the given rect) to the target dc 
	void DrawScene(CDC& dc, const CRect& rc);
private:
	CDC memDC;
	CBitmap* p_bitmap;
	CBitmap* old_bitmap;
	bool is_dirty;

};

