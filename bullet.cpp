/*==============================================================================

	ポリゴン描画 [bullet.cpp]
														Author : Emu Saito
														Date   : 2016/5/31
--------------------------------------------------------------------------------
	■Update : None
==============================================================================*/

/*============================================================================
	インクルードファイル
============================================================================*/
#include "main.h"
#include "bullet.h"
#include "camera.h"
#include "model.h"
#include "explosion.h"

/*============================================================================
	定数定義
============================================================================*/
#define BULLET_TEXTURENAME "data/TEXTURE/VS.png"					//読み込むテクスチャファイル名	//png jpg tga bmp ddsが使える
#define NUM_POLYGON_BULLET (2)											//バレットのポリゴン数
#define BULLET_WIDTH (30.0f)											//ポリゴンの幅
#define BULLET_HEIGHT (30.0f)											//ポリゴンのたかさ
#define MAX_BULLET (256)												//弾の最大数
#define ROUT2 (1.4142)													//√２の少数
/*============================================================================
	構造体定義
============================================================================*/
typedef struct
{
	D3DXVECTOR3 BulletPos;
	D3DXVECTOR3 Speed;
	float Lifespan;
	D3DXCOLOR color;
	D3DXVECTOR3 CenterPos;
	float rot;
	bool bUse;
}BULLET;
/*============================================================================
	グローバル変数
============================================================================*/
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;	//テクスチャーインターフェースへのポインター
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBullet = NULL;

BULLET g_aBullet[MAX_BULLET];

/*============================================================================
	プロトタイプ宣言
============================================================================*/
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);

/*============================================================================
	初期化処理
============================================================================*/
void InitBullet(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//g_pD3DDeviceの取得

	//ディスクからテクスチャの読み込み
	if(FAILED(D3DXCreateTextureFromFile(pDevice,BULLET_TEXTURENAME,&g_pTextureBullet)))			//失敗true 成功false
	{
		MessageBox(NULL,"読み込みに失敗しました","TEXTURE",MB_OK|MB_ICONHAND);
	}

	MakeVertexBullet(pDevice);						//頂点の作成

	int nCnt;

	for(nCnt = 0;nCnt < MAX_BULLET;nCnt++)
	{
		g_aBullet[nCnt].BulletPos.x = 0;
		g_aBullet[nCnt].BulletPos.y = 0;
		g_aBullet[nCnt].BulletPos.z = 0;
		g_aBullet[nCnt].color.a = 0;
		g_aBullet[nCnt].color.r = 0;
		g_aBullet[nCnt].color.b = 0;
		g_aBullet[nCnt].color.g = 0;
		g_aBullet[nCnt].Lifespan = 0;
		g_aBullet[nCnt].Speed.x = 0;
		g_aBullet[nCnt].Speed.y = 0;
		g_aBullet[nCnt].Speed.z = 0;
		g_aBullet[nCnt].bUse = false;

	}

}

/*============================================================================
	終了処理
============================================================================*/
void UninitBullet(void)
{
	if(g_pTextureBullet != NULL)		//テクスチャインターフェースのお片付け
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}
	if(g_pVtxBufferBullet != NULL)		//お片付け
	{
		g_pVtxBufferBullet->Release();
		g_pVtxBufferBullet = NULL;
	}
}

/*============================================================================
	更新処理
============================================================================*/
void UpdateBullet(void)
{
	int nCnt;
	D3DXVECTOR3 ModelPos = GetModelPos();

	float fModelAngle = GetAngle();

	for(nCnt = 0;nCnt < MAX_BULLET;nCnt++)
	{
		if(g_aBullet[nCnt].bUse == true)
		{
			/*g_aBullet[nCnt].Speed.x = -(g_aBullet[nCnt].Speed.x) * (ModelPos.x - g_aBullet[nCnt].BulletPos.x);
			g_aBullet[nCnt].Speed.y = -(g_aBullet[nCnt].Speed.y) * (ModelPos.y - g_aBullet[nCnt].BulletPos.y);
			g_aBullet[nCnt].Speed.z = -(g_aBullet[nCnt].Speed.z) * (ModelPos.z - g_aBullet[nCnt].BulletPos.z);
			*/
			g_aBullet[nCnt].BulletPos += g_aBullet[nCnt].Speed;
			g_aBullet[nCnt].CenterPos += g_aBullet[nCnt].Speed;
			g_aBullet[nCnt].Lifespan--;
			if(g_aBullet[nCnt].Lifespan <= 0)
			{
				g_aBullet[nCnt].bUse = false;
				break;
			}
		}

	}



	//あたり判定
	for(int nCnt = 0;nCnt < MAX_BULLET;nCnt++)
	{
		if(g_aBullet[nCnt].bUse == true)
		{
			if(g_aBullet[nCnt].BulletPos.z >= 40.0f)
			{
				g_aBullet[nCnt].bUse = false;
				SetExplosion(g_aBullet[nCnt].BulletPos);
				break;
			}

			if(g_aBullet[nCnt].BulletPos.z <= -40.0f)
			{
				g_aBullet[nCnt].bUse = false;
				SetExplosion(g_aBullet[nCnt].BulletPos);
				break;
			}

			if(g_aBullet[nCnt].BulletPos.x >= 40.0f)
			{
				g_aBullet[nCnt].bUse = false;
				SetExplosion(g_aBullet[nCnt].BulletPos);
				break;
			}

			if(g_aBullet[nCnt].BulletPos.x <= -40.0f)
			{
				g_aBullet[nCnt].bUse = false;
				SetExplosion(g_aBullet[nCnt].BulletPos);
				break;
			}
		}
	}



}

/*============================================================================
	描画処理
============================================================================*/
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//g_pD3DDeviceの取得

	D3DXMATRIX	mtxViewInverse,mtxView;
	mtxView = GetView();

	//Zソートする
	pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);		//ZバッファのON/OFF

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);		//Zバッファへの書き込みON/OFF

	D3DXMATRIX mtxWorld,mtxRotY,mtxTrans,mtxScale;

	


	for(int nCnt = 0;nCnt < MAX_BULLET;nCnt++)
	{
		if(g_aBullet[nCnt].bUse == true)
		{
			g_aBullet[nCnt].rot += 0.5f;


		D3DXMatrixIdentity(&mtxWorld);		//単位行列を作る

		//回転行列を作る(D3DXToRadianは1度のラジアンが入っている)
		D3DXMatrixRotationYawPitchRoll(&mtxRotY,0.0f,0.0f,g_aBullet[nCnt].rot);

		//行列の合成
		D3DXMatrixMultiply(&mtxWorld,&mtxWorld,&mtxRotY);

	//転地行列
	D3DXMatrixTranspose(&mtxViewInverse,&mtxView);
	mtxViewInverse._14 = 0.0f;
	mtxViewInverse._24 = 0.0f;
	mtxViewInverse._34 = 0.0f;

	mtxWorld = mtxViewInverse * mtxWorld;

	//平行移動行列を作る
	D3DXMatrixTranslation(&mtxTrans,g_aBullet[nCnt].BulletPos.x,g_aBullet[nCnt].BulletPos.y,g_aBullet[nCnt].BulletPos.z);

	//平行移動行列をワールドに反映させる
	mtxWorld = mtxWorld * mtxTrans;


	//ワールド変換の設定
	pDevice->SetTransform(D3DTS_WORLD,&mtxWorld);

	pDevice->SetStreamSource(
		0,
		g_pVtxBufferBullet,
		0,								//オフセット(バイト)
		sizeof(VERTEX_3D)				//ストライド量
		);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0,g_pTextureBullet);

			//ポリゴン(プリミティブ)四角形の描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//ポリゴン(プリミティブ)の種類	四角形
				nCnt * 4,			//オフセット
				NUM_POLYGON_BULLET);				//プリミティブ数
		}

	}

	//元に戻す
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);		//デスト(背景)

	//Zソートを元に戻す
	pDevice->SetRenderState(D3DRS_ZENABLE,FALSE);		//ZバッファのON/OFF

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);		//Zバッファへの書き込みON/OFF

}

/*============================================================================
	頂点の作成
============================================================================*/
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)//return E_FAILが許される型に変える
{
	if(FAILED(pDevice->CreateVertexBuffer(
	sizeof(VERTEX_3D) * NUM_VERTEX * MAX_BULLET,				//作成したい頂点バッファのサイズ(1個の頂点データ*頂点数)
	D3DUSAGE_WRITEONLY,							//爆速
	FVF_VERTEX_3D,
	D3DPOOL_MANAGED,							//メモリの管理方法(MANAGEDはデバイスにお任せ)
	&g_pVtxBufferBullet,			//後片付けをする
	NULL)))
	{
		return E_FAIL;			//エラーチェック
	}

	VERTEX_3D *pVtx;		//仮想アドレスを取得するためのポインター(ここに仮想アドレスもらうよ)

	//頂点バッファーをロックして仮想アドレスを取得する
	g_pVtxBufferBullet->Lock(0,0,(void**)&pVtx,0);					//0,0,を入力すると全部ロックする    **&pVtx アドレスが書いてある手帳のアドレス
	
	int nCnt;
	for(nCnt = 0;nCnt < MAX_BULLET;nCnt++)
	{


		//頂点カラーの設定(0～255の整数値)
		pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

		//テクスチャのバッファー
		pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

		//テクスチャ
		pVtx[0].normal = D3DXVECTOR3(0.0f,0.0f,0.0f);
		pVtx[1].normal = D3DXVECTOR3(0.0f,0.0f,0.0f);
		pVtx[2].normal = D3DXVECTOR3(0.0f,0.0f,0.0f);
		pVtx[3].normal = D3DXVECTOR3(0.0f,0.0f,0.0f);

		pVtx[0].pos = D3DXVECTOR3(-0.5f,0.5f,0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.5f,0.5f,0.0f);
		pVtx[2].pos = D3DXVECTOR3(-0.5f,-0.5f,0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.5f,-0.5f,0.0f);

		pVtx+=4;
	}


	//頂点バッファーのカギを開ける
	g_pVtxBufferBullet->Unlock();

	return S_OK;
}

/*
void SetVertexBullet(int bulletIndex)
{
	VERTEX_3D *pVtx;		//仮想アドレスを取得するためのポインター(ここに仮想アドレスもらうよ)

	//頂点バッファーをロックして仮想アドレスを取得する
	g_pVtxBufferBullet->Lock(0,0,(void**)&pVtx,0);					//0,0,を入力すると全部ロックする    **&pVtx アドレスが書いてある手帳のアドレス
	
	pVtx += bulletIndex * NUM_VERTEX;
	pVtx[0].pos = D3DXVECTOR3(-1.0f,1.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(1.0f,1.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(-1.0f,-1.0f,0.0f);
	pVtx[3].pos = D3DXVECTOR3(1.0f,-1.0f,0.0f);

	//頂点バッファーのカギを開ける
	g_pVtxBufferBullet->Unlock();
}
*/
void SetBullet(D3DXVECTOR3 pos,D3DXVECTOR3 Speed,float lifespan,D3DXCOLOR color)
{
	int nCnt;

	for(nCnt = 0;nCnt < MAX_BULLET;nCnt++)
	{
		if(g_aBullet[nCnt].bUse == false)
		{
			g_aBullet[nCnt].BulletPos.x = pos.x;
			g_aBullet[nCnt].BulletPos.y = pos.y + 1.0f;
			g_aBullet[nCnt].BulletPos.z = pos.z;
			g_aBullet[nCnt].Speed.x = Speed.x;
			g_aBullet[nCnt].Speed.y = Speed.y;
			g_aBullet[nCnt].Speed.z = Speed.z;
			g_aBullet[nCnt].Lifespan = lifespan;
			g_aBullet[nCnt].color = color;
			g_aBullet[nCnt].bUse = true;

	/*float g_fLengthPolygon,g_fAnglePolygon;
	g_fLengthPolygon = sqrtf(BULLET_WIDTH * BULLET_WIDTH + BULLET_HEIGHT * BULLET_HEIGHT) * 0.5f;
	//対角線の角度(ラジアン角)の算出
	g_fAnglePolygon = atan2f(BULLET_HEIGHT,BULLET_WIDTH);

	//左上のX
	g_fLeftTopX = g_posPolygon.x + cos(g_rotPolygon + D3DX_PI + g_fAnglePolygon) * g_fLengthPolygon;
	//左上のY
	g_fLeftTopY = g_posPolygon.y + sin(g_rotPolygon + D3DX_PI + g_fAnglePolygon) * g_fLengthPolygon;
	//右上のX
	g_fRightTopX = g_posPolygon.x + cos(g_rotPolygon - g_fAnglePolygon) * g_fLengthPolygon;
	//右上のY
	g_fRightTopY = g_posPolygon.y + sin(g_rotPolygon - g_fAnglePolygon) * g_fLengthPolygon;
	//右下のX
	g_fRightBottomX = g_posPolygon.x + cos(g_rotPolygon + g_fAnglePolygon) * g_fLengthPolygon;
	//右下のY
	g_fRightBottomY = g_posPolygon.y + sin(g_rotPolygon + g_fAnglePolygon) * g_fLengthPolygon;
	//左下のX
	g_fLeftBottomX = g_posPolygon.x + cos(g_rotPolygon - D3DX_PI - g_fAnglePolygon) * g_fLengthPolygon;
	//左下のY
	g_fLeftBottomY = g_posPolygon.y + sin(g_rotPolygon - D3DX_PI - g_fAnglePolygon) * g_fLengthPolygon;
	*/
			break;
		}
	}
}