//=============================================================================
//
// 入力処理 [camera.h]
// Author : EMU SAITO
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

/*============================================================================
	プロトタイプ宣言
============================================================================*/
void InitCamera(void);
void UninitCamera(void);
void DrawCamera(void);
void UpdateCamera(void);
D3DXMATRIX GetView(void);
D3DXVECTOR3 GetCameraPos(void);
int GetCameraMode(void);
float GetfAngle(void);

#endif _CAMERA_H_