/*==============================================================================

    ポリゴン描画 [camera.cpp]
                                                       Author : Emu Saito
                                                       Date   : 2016/05/24
--------------------------------------------------------------------------------
    ■Update : None
==============================================================================*/
/*============================================================================
	インクルードファイル
============================================================================*/
#include "main.h"
#include "input.h"
#include "model.h"
#include "camera.h"

/*============================================================================
	定数定義
============================================================================*/
#define CAMERA_WAIT_SPEED (100)				//カメラが自動回り込みするまでの時間
#define CAMERA_ROTATE_SPEED (0.05f)			//カメラが回り込みするスピード
/*============================================================================
	グローバル変数
============================================================================*/
LPDIRECT3DDEVICE9 g_pD3DDeviceCamera;

int g_CameraMode;			//カメラのモード

int g_ModelWrap;			//カメラの回り込みカウント

D3DXVECTOR3 BeforeModelPos;		//モデルの前の位置


D3DXVECTOR3 g_PosAt;		//注視点
D3DXVECTOR3 g_PosEye;		//カメラの座標
D3DXVECTOR3 g_VecUp;		//カメラの上方向のベクトル

float g_FovY;			//画角(視野角)
float g_fAngleCameraEye = 0.0f;
float g_LengthCamera = 15.0f;
float g_fAngleCameraAt = 0.0f;

D3DXVECTOR3 g_VecCamera;
D3DXVECTOR3 g_PosPlus;		//大きさ
D3DXVECTOR3 ModelPos = D3DXVECTOR3(0.0f,0.0f,0.0f);


//ビュー変換の変数
D3DXMATRIX mtxView;
/*============================================================================
	初期化処理
============================================================================*/
void InitCamera(void)
{
	g_CameraMode = 1;				//0がデバッグモード		1が追従モード

	g_pD3DDeviceCamera = GetDevice();

	//ビュー行列を作成
	g_PosAt = D3DXVECTOR3(0.0f,0.0f,0.0f);		//注視点（見たいところ）
	g_PosEye = D3DXVECTOR3(0.0f,10.0f,-15.0f);	//カメラの座標
	g_VecUp = D3DXVECTOR3(0.0f,1.0f,0.0f);		//カメラの上方向ベクトル
	g_FovY = D3DX_PI/3;							//画角(視野角)

	//カメラ専用のビュー行列
	D3DXMatrixLookAtLH(&mtxView,&g_PosEye,&g_PosAt,&g_VecUp);		//上の３つからビュー行列を作る関数

	//ワールドビュー変換の設定
	g_pD3DDeviceCamera->SetTransform(D3DTS_VIEW,&mtxView);		//ビュー行列の設定

	//プロジェクション行列を作成

	//プロジェクションの変数
	D3DXMATRIX mtxProj;
	D3DXMatrixPerspectiveFovLH(&mtxProj,
		D3DX_PI / 3.0f,								//画角(視野角)
		(float)SCREEN_WIDTH/(float)SCREEN_HEIGHT,	//アスペクト比
		1.0f,		//near 0より大きい数字
		1000.0f		//far
		);

	//プロジェクション変換の設定
	g_pD3DDeviceCamera->SetTransform(D3DTS_PROJECTION,&mtxProj);

	
	//カメラを中心とした
	g_fAngleCameraAt = atan2f(g_PosAt.z - g_PosEye.z,g_PosAt.x - g_PosEye.x);

	//注視点を中心とした
	g_fAngleCameraEye = atan2f(g_PosEye.z - g_PosAt.z,g_PosEye.x - g_PosAt.x);
	g_fAngleCameraEye = D3DX_PI;
}
/*============================================================================
	終了処理
============================================================================*/
void UninitCamera(void)
{
	//解放
	/*if(g_pD3DDeviceCamera != NULL)
	{
		g_pD3DDeviceCamera->Release();
		g_pD3DDeviceCamera = NULL;
	}*/
}
/*============================================================================
	描画処理
============================================================================*/
void DrawCamera(void)
{

}
/*============================================================================
	更新処理
============================================================================*/
void UpdateCamera(void)
{
	float fDust;
	D3DXVECTOR3 Speed;
	float fAngle;

	fAngle = GetAngle();

	D3DXVECTOR3 ModelSpeed = D3DXVECTOR3(0.0f,0.0f,0.0f);

	g_pD3DDeviceCamera = GetDevice();

	//ビュー変換の変数
	//D3DXMATRIX mtxView;

	D3DXMatrixLookAtLH(&mtxView,&g_PosEye,&g_PosAt,&g_VecUp);		//上の３つからビュー行列を作る関数

	//ワールドビュー変換の設定
	g_pD3DDeviceCamera->SetTransform(D3DTS_VIEW,&mtxView);		//ビュー行列の設定

	//プロジェクション行列を作成

	//プロジェクションの変数
	D3DXMATRIX mtxProj;
	D3DXMatrixPerspectiveFovLH(&mtxProj,
		D3DX_PI / 3.0f,								//画角(視野角)
		(float)SCREEN_WIDTH/(float)SCREEN_HEIGHT,	//アスペクト比
		1.0f,		//near 0より大きい数字
		1000.0f		//far
		);

	//プロジェクション変換の設定
	g_pD3DDeviceCamera->SetTransform(D3DTS_PROJECTION,&mtxProj);

	//デバッグモード
	if(g_CameraMode == 0)
	{
		if(GetKeyboardPress(DIK_UP))
		{
			g_PosEye.y += 0.2f;
		}
		if(GetKeyboardPress(DIK_DOWN))
		{
			g_PosEye.y -= 0.2f;
		}

		if(GetKeyboardPress(DIK_E))
		{
			g_fAngleCameraEye = atan2f(g_PosEye.z - g_PosAt.z,g_PosEye.x - g_PosAt.x);
			g_fAngleCameraEye += 0.1f;

			//カメラの座標の計算
			g_PosEye.x = g_PosAt.x + cos(g_fAngleCameraEye) * g_LengthCamera;
			g_PosEye.z = g_PosAt.z + sin(g_fAngleCameraEye) * g_LengthCamera;
		}
		if(GetKeyboardPress(DIK_Q))
		{
			g_fAngleCameraEye = atan2f(g_PosEye.z - g_PosAt.z,g_PosEye.x - g_PosAt.x);
			g_fAngleCameraEye -= 0.1f;

			//カメラの座標の計算
			g_PosEye.x = g_PosAt.x + cos(g_fAngleCameraEye) * g_LengthCamera;
			g_PosEye.z = g_PosAt.z + sin(g_fAngleCameraEye) * g_LengthCamera;
		}

		if(GetKeyboardPress(DIK_LEFT))
		{
			g_fAngleCameraAt = atan2f(g_PosAt.z - g_PosEye.z,g_PosAt.x - g_PosEye.x);
			g_fAngleCameraAt -= 0.1f;

			//カメラを中心とした回転
			//注視点の座標の計算
			g_PosAt.x = g_PosEye.x + cos(g_fAngleCameraAt) * g_LengthCamera;
			g_PosAt.z = g_PosEye.z + sin(g_fAngleCameraAt) * g_LengthCamera;
		}
		if(GetKeyboardPress(DIK_RIGHT))
		{
			g_fAngleCameraAt = atan2f(g_PosAt.z - g_PosEye.z,g_PosAt.x - g_PosEye.x);
			g_fAngleCameraAt += 0.1f;

			//カメラを中心とした回転
			//注視点の座標の計算
			g_PosAt.x = g_PosEye.x + cos(g_fAngleCameraAt) * g_LengthCamera;
			g_PosAt.z = g_PosEye.z + sin(g_fAngleCameraAt) * g_LengthCamera;
		}

		//前進
		if(GetKeyboardPress(DIK_W))
		{

			//カメラから注視点に対しての方向ベクトル
			g_VecCamera = g_PosAt - g_PosEye;

			g_VecCamera.y = 0.0f;

			//単位ベクトルをかけて、大きさ１のベクトルを作った。
			D3DXVec3Normalize(&g_VecCamera,&g_VecCamera);

			g_PosEye += g_VecCamera * 1.0f;
			g_PosAt += g_VecCamera * 1.0f;

		}
		//後進
		if(GetKeyboardPress(DIK_S))
		{

			//カメラから注視点に対しての方向ベクトル
			g_VecCamera = g_PosAt - g_PosEye;

			g_VecCamera.y = 0.0f;

			//単位ベクトルをかけて、大きさ１のベクトルを作った。
			D3DXVec3Normalize(&g_VecCamera,&g_VecCamera);

			g_PosEye += g_VecCamera * -1.0f;
			g_PosAt += g_VecCamera * -1.0f;

		}
		
		/*
		//右
		if(GetKeyboardPress(DIK_D))
		{

			//カメラから注視点に対しての方向ベクトル
			g_VecCamera = g_PosAt - g_PosEye;

			//高さは関係ないためYには0を入れる
			g_VecCamera.y = 0.0f;


			//単位ベクトルをかけて、大きさ１のベクトルを作った。
			D3DXVec3Normalize(&g_VecCamera,&g_VecCamera);


			//とりあえずやる
			fDust = g_VecCamera.x;
			g_VecCamera.x = -(g_VecCamera.z);
			g_VecCamera.z = fDust;

			g_PosEye += g_VecCamera * 1.0f;

		}
		//左
		if(GetKeyboardPress(DIK_A))
		{

			//カメラから注視点に対しての方向ベクトル
			g_VecCamera = g_PosAt - g_PosEye;

			g_VecCamera.y = 0.0f;

			//単位ベクトルをかけて、大きさ１のベクトルを作った。
			D3DXVec3Normalize(&g_VecCamera,&g_VecCamera);
			fDust = g_VecCamera.x;

			g_VecCamera.x = -(g_VecCamera.z);
			g_VecCamera.z = fDust;

			g_PosEye += g_VecCamera * -1.0f;

		}*/

	}
	
	//追従カメラモード(カメラ自動回り込みON＆OFF)
	if(g_CameraMode == 1 || g_CameraMode == 2)
	{
		MODEL *Model = GetModelStruct();

		//プレイヤーの移動量が速くなればなるほど、注視点のいちをまえに出すのをつくる


		g_PosAt.x = Model[0].posModel.x + cosf(Model[0].fAngle + D3DX_PI * 0.5) * 5;		//2はモデルの２前の位置を注視点としている
		g_PosAt.z = Model[0].posModel.z + sinf(Model[0].fAngle - D3DX_PI * 0.5) * 5;		//2はモデルの２前の位置を注視点としている

		g_PosAt.x = Model[0].posModel.x;
		g_PosAt.z = Model[0].posModel.z;

		//追従カメラモード（自動回り込みOFF）
		if(g_CameraMode == 1)
		{
			g_PosEye.x = g_PosAt.x + cosf(g_fAngleCameraEye - D3DX_PI * 0.5) * g_LengthCamera;
			g_PosEye.z = g_PosAt.z + sinf(g_fAngleCameraEye + D3DX_PI * 0.5) * g_LengthCamera;
			g_PosEye.y = Model[0].posModel.y + 10;
		}

		
		//追従カメラモード（自動回り込みON）
		if(g_CameraMode == 2)
		{
			//モデルの角度をもらってくる
			MODEL *Model = GetModelStruct();

			g_PosEye.x = g_PosAt.x + cosf(g_fAngleCameraEye - D3DX_PI * 0.5) * g_LengthCamera;
			g_PosEye.z = g_PosAt.z + sinf(g_fAngleCameraEye + D3DX_PI * 0.5) * g_LengthCamera;
			g_PosEye.y = Model[0].posModel.y + 15;
			

			//プレイヤーの位置が前の座標と同じだった場合にインクリメントして少しでも動いたら = 0にする
			if(BeforeModelPos == Model[0].posModel)
			{
				g_ModelWrap++;
			}
			else if(BeforeModelPos != Model[0].posModel)
			{
				g_ModelWrap = 0;
			}

			if(g_ModelWrap >= CAMERA_WAIT_SPEED)
			{
				float fAngleEye,g_fAngleHold;


				//角度を-3.14~0~3.14までのところを0~6.28に変えてる(わかりやすくするため)
				fAngleEye = Model[0].fAngle + D3DX_PI;
				g_fAngleHold = g_fAngleCameraEye + D3DX_PI;

				//モデルの反対側の角度を求める
				fAngleEye = fAngleEye + D3DX_PI;

				//6.28より大きくなってしまった場合
				if(fAngleEye >= D3DX_PI * 2)
				{
					fAngleEye -= D3DX_PI * 2;
				}

				//プラスだったら
				if(fAngleEye - g_fAngleHold > 0)
				{
					if(fAngleEye - g_fAngleHold > D3DX_PI)
					{
						g_fAngleCameraEye -= CAMERA_ROTATE_SPEED;

						if(g_fAngleCameraEye <= -(D3DX_PI))
						{
							g_fAngleCameraEye = D3DX_PI;
						}
					}
					if(fAngleEye - g_fAngleHold < D3DX_PI)
					{
						g_fAngleCameraEye += CAMERA_ROTATE_SPEED;

						if(fAngleEye - D3DX_PI <= g_fAngleCameraEye)
						{
							g_fAngleCameraEye = fAngleEye - D3DX_PI;
						}
					}
				}
				//マイナスだったら
				if(fAngleEye - g_fAngleHold < 0)
				{
					if(fAngleEye - g_fAngleHold < -(D3DX_PI))
					{
						g_fAngleCameraEye += CAMERA_ROTATE_SPEED;

						if(g_fAngleCameraEye >= D3DX_PI)
						{
							g_fAngleCameraEye = -(D3DX_PI);
						}
					}
					if(fAngleEye - g_fAngleHold > -(D3DX_PI))
					{
						g_fAngleCameraEye -= CAMERA_ROTATE_SPEED;

						if(fAngleEye - D3DX_PI >= g_fAngleCameraEye)
						{
							g_fAngleCameraEye = fAngleEye - D3DX_PI;
						}
					}
				}

				//カメラの位置を修正
				g_PosEye.x = g_PosAt.x + cosf(g_fAngleCameraEye - D3DX_PI * 0.5) * 10;
				g_PosEye.z = g_PosAt.z + sinf(g_fAngleCameraEye + D3DX_PI * 0.5) * 10;
				g_PosEye.y = Model[0].posModel.y + 15;
			}

			BeforeModelPos = Model[0].posModel;
		}
	}



	//カメラモード切替
	if(GetKeyboardTrigger(DIK_F2))
	{
		g_CameraMode++;
		if(g_CameraMode == 3)
		{
			g_CameraMode = 0;
		}
	}
	
}
D3DXMATRIX GetView(void)
{

	return mtxView;
}
D3DXVECTOR3 GetCameraPos(void)
{

	return g_PosEye;
}
float GetfAngle(void)
{
	return g_fAngleCameraEye;
}

int GetCameraMode(void)
{
	return g_CameraMode;
}