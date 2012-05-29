#include "stdafx.h"


XM_sprite::XM_sprite():
_vPosition(0.0f, 0.0f, 0.0f),
_pTextSprite( NULL ),
_pSprite( NULL )
{
	_rcSource.top=0,
	_rcSource.left=0;
	_rcSource.right=0;
	_rcSource.bottom=0;
}

XM_sprite::~XM_sprite()
{
	_RELEASE(_pTextSprite);
	_RELEASE(_pSprite);
}

void XM_sprite::Clear()
{
	_RELEASE(_pTextSprite);
	_RELEASE(_pSprite);
}

bool XM_sprite::LoadFromFile(LPDIRECT3DDEVICE9 pDevice,const char* filename,int width,int height)
{
	//äÖÈ¾¾«Áé
	D3DXIMAGE_INFO d3dxImageInfo;
	D3DXCreateTextureFromFileEx(pDevice,filename,
								width,height,
								1,0,D3DFMT_UNKNOWN,D3DPOOL_MANAGED,
								D3DX_DEFAULT,D3DX_DEFAULT,
								D3DCOLOR_COLORVALUE(255.0f,255.0f,255.0f,255.0f),
								&d3dxImageInfo,
								NULL,
								&_pTextSprite);
	D3DXCreateSprite( pDevice, &_pSprite );
	_rcSource.right=width;
	_rcSource.bottom=height;
	return TRUE;

}

void XM_sprite::Draw()
{
	if(_pSprite==NULL) return;
	_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	_pSprite->Draw(_pTextSprite,&_rcSource, 0, &_vPosition, 0xffffffff);
	_pSprite->End();
}

void XM_sprite::SetPosition(double pos_x, double pos_y)
{
	_vPosition.x=pos_x;
	_vPosition.y=pos_y;
}

void XM_sprite::SetZ(float pos_z)
{
	_vPosition.z=pos_z;
}

const int XM_sprite::getX()
{
	return _vPosition.x;
}

const int XM_sprite::getY()
{
	return _vPosition.y;
}

RECT XM_sprite::getRect()
{
	RECT _rc;
	_rc.top   = _vPosition.y;
	_rc.left  = _vPosition.x;
	_rc.right = _vPosition.x+_rcSource.right;
	_rc.bottom= _vPosition.y+_rcSource.bottom;
	return _rc;
}
bool XM_sprite::CheckFail()
{
	if (_vPosition.x+PIC_WIDTH-1>=OFF_LEFT+20*PIC_WIDTH||_vPosition.x<OFF_LEFT||_vPosition.y<OFF_TOP||_vPosition.y+PIC_HEIGHT-1>=OFF_TOP+14*PIC_HEIGHT)
		return true;
	else return false;
}