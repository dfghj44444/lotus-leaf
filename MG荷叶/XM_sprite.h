#ifndef _WXM_SPRTIE_H
#define _WXM_SPRTIE_H

#include "stdafx.h"
class XM_sprite
{
public:
	XM_sprite();
	~XM_sprite();
	bool LoadFromFile(LPDIRECT3DDEVICE9 pDevice,const char* filename,int width,int height);
	virtual void Draw();
	void SetPosition(double pos_x,double pos_y);
	/**
		设置深度，0-1之间，越小越靠前
	*/
	void SetZ(float pos_z);
	void Clear();
	const int getX();
	const int getY();
	RECT getRect();
	bool CheckFail();
protected:
	ID3DXSprite* _pSprite;
	LPDIRECT3DTEXTURE9  _pTextSprite;
	RECT  _rcSource;
	D3DXVECTOR3 _vPosition;
};

#endif