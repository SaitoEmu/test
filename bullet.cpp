/*==============================================================================

	�|���S���`�� [bullet.cpp]
														Author : Emu Saito
														Date   : 2016/5/31
--------------------------------------------------------------------------------
	��Update : None
==============================================================================*/

/*============================================================================
	�C���N���[�h�t�@�C��
============================================================================*/
#include "main.h"
#include "bullet.h"
#include "camera.h"
#include "model.h"
#include "explosion.h"

/*============================================================================
	�萔��`
============================================================================*/
#define BULLET_TEXTURENAME "data/TEXTURE/VS.png"					//�ǂݍ��ރe�N�X�`���t�@�C����	//png jpg tga bmp dds���g����
#define NUM_POLYGON_BULLET (2)											//�o���b�g�̃|���S����
#define BULLET_WIDTH (30.0f)											//�|���S���̕�
#define BULLET_HEIGHT (30.0f)											//�|���S���̂�����
#define MAX_BULLET (256)												//�e�̍ő吔
#define ROUT2 (1.4142)													//��Q�̏���
/*============================================================================
	�\���̒�`
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
	�O���[�o���ϐ�
============================================================================*/
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;	//�e�N�X�`���[�C���^�[�t�F�[�X�ւ̃|�C���^�[
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBullet = NULL;

BULLET g_aBullet[MAX_BULLET];

/*============================================================================
	�v���g�^�C�v�錾
============================================================================*/
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);

/*============================================================================
	����������
============================================================================*/
void InitBullet(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//g_pD3DDevice�̎擾

	//�f�B�X�N����e�N�X�`���̓ǂݍ���
	if(FAILED(D3DXCreateTextureFromFile(pDevice,BULLET_TEXTURENAME,&g_pTextureBullet)))			//���strue ����false
	{
		MessageBox(NULL,"�ǂݍ��݂Ɏ��s���܂���","TEXTURE",MB_OK|MB_ICONHAND);
	}

	MakeVertexBullet(pDevice);						//���_�̍쐬

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
	�I������
============================================================================*/
void UninitBullet(void)
{
	if(g_pTextureBullet != NULL)		//�e�N�X�`���C���^�[�t�F�[�X�̂��Еt��
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}
	if(g_pVtxBufferBullet != NULL)		//���Еt��
	{
		g_pVtxBufferBullet->Release();
		g_pVtxBufferBullet = NULL;
	}
}

/*============================================================================
	�X�V����
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



	//�����蔻��
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
	�`�揈��
============================================================================*/
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//g_pD3DDevice�̎擾

	D3DXMATRIX	mtxViewInverse,mtxView;
	mtxView = GetView();

	//Z�\�[�g����
	pDevice->SetRenderState(D3DRS_ZENABLE,TRUE);		//Z�o�b�t�@��ON/OFF

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);		//Z�o�b�t�@�ւ̏�������ON/OFF

	D3DXMATRIX mtxWorld,mtxRotY,mtxTrans,mtxScale;

	


	for(int nCnt = 0;nCnt < MAX_BULLET;nCnt++)
	{
		if(g_aBullet[nCnt].bUse == true)
		{
			g_aBullet[nCnt].rot += 0.5f;


		D3DXMatrixIdentity(&mtxWorld);		//�P�ʍs������

		//��]�s������(D3DXToRadian��1�x�̃��W�A���������Ă���)
		D3DXMatrixRotationYawPitchRoll(&mtxRotY,0.0f,0.0f,g_aBullet[nCnt].rot);

		//�s��̍���
		D3DXMatrixMultiply(&mtxWorld,&mtxWorld,&mtxRotY);

	//�]�n�s��
	D3DXMatrixTranspose(&mtxViewInverse,&mtxView);
	mtxViewInverse._14 = 0.0f;
	mtxViewInverse._24 = 0.0f;
	mtxViewInverse._34 = 0.0f;

	mtxWorld = mtxViewInverse * mtxWorld;

	//���s�ړ��s������
	D3DXMatrixTranslation(&mtxTrans,g_aBullet[nCnt].BulletPos.x,g_aBullet[nCnt].BulletPos.y,g_aBullet[nCnt].BulletPos.z);

	//���s�ړ��s������[���h�ɔ��f������
	mtxWorld = mtxWorld * mtxTrans;


	//���[���h�ϊ��̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD,&mtxWorld);

	pDevice->SetStreamSource(
		0,
		g_pVtxBufferBullet,
		0,								//�I�t�Z�b�g(�o�C�g)
		sizeof(VERTEX_3D)				//�X�g���C�h��
		);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0,g_pTextureBullet);

			//�|���S��(�v���~�e�B�u)�l�p�`�̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//�|���S��(�v���~�e�B�u)�̎��	�l�p�`
				nCnt * 4,			//�I�t�Z�b�g
				NUM_POLYGON_BULLET);				//�v���~�e�B�u��
		}

	}

	//���ɖ߂�
	pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);		//�f�X�g(�w�i)

	//Z�\�[�g�����ɖ߂�
	pDevice->SetRenderState(D3DRS_ZENABLE,FALSE);		//Z�o�b�t�@��ON/OFF

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE,TRUE);		//Z�o�b�t�@�ւ̏�������ON/OFF

}

/*============================================================================
	���_�̍쐬
============================================================================*/
HRESULT MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)//return E_FAIL���������^�ɕς���
{
	if(FAILED(pDevice->CreateVertexBuffer(
	sizeof(VERTEX_3D) * NUM_VERTEX * MAX_BULLET,				//�쐬���������_�o�b�t�@�̃T�C�Y(1�̒��_�f�[�^*���_��)
	D3DUSAGE_WRITEONLY,							//����
	FVF_VERTEX_3D,
	D3DPOOL_MANAGED,							//�������̊Ǘ����@(MANAGED�̓f�o�C�X�ɂ��C��)
	&g_pVtxBufferBullet,			//��Еt��������
	NULL)))
	{
		return E_FAIL;			//�G���[�`�F�b�N
	}

	VERTEX_3D *pVtx;		//���z�A�h���X���擾���邽�߂̃|�C���^�[(�����ɉ��z�A�h���X���炤��)

	//���_�o�b�t�@�[�����b�N���ĉ��z�A�h���X���擾����
	g_pVtxBufferBullet->Lock(0,0,(void**)&pVtx,0);					//0,0,����͂���ƑS�����b�N����    **&pVtx �A�h���X�������Ă���蒠�̃A�h���X
	
	int nCnt;
	for(nCnt = 0;nCnt < MAX_BULLET;nCnt++)
	{


		//���_�J���[�̐ݒ�(0�`255�̐����l)
		pVtx[0].color = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[1].color = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[2].color = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[3].color = D3DCOLOR_RGBA(255,255,255,255);

		//�e�N�X�`���̃o�b�t�@�[
		pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

		//�e�N�X�`��
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


	//���_�o�b�t�@�[�̃J�M���J����
	g_pVtxBufferBullet->Unlock();

	return S_OK;
}

/*
void SetVertexBullet(int bulletIndex)
{
	VERTEX_3D *pVtx;		//���z�A�h���X���擾���邽�߂̃|�C���^�[(�����ɉ��z�A�h���X���炤��)

	//���_�o�b�t�@�[�����b�N���ĉ��z�A�h���X���擾����
	g_pVtxBufferBullet->Lock(0,0,(void**)&pVtx,0);					//0,0,����͂���ƑS�����b�N����    **&pVtx �A�h���X�������Ă���蒠�̃A�h���X
	
	pVtx += bulletIndex * NUM_VERTEX;
	pVtx[0].pos = D3DXVECTOR3(-1.0f,1.0f,0.0f);
	pVtx[1].pos = D3DXVECTOR3(1.0f,1.0f,0.0f);
	pVtx[2].pos = D3DXVECTOR3(-1.0f,-1.0f,0.0f);
	pVtx[3].pos = D3DXVECTOR3(1.0f,-1.0f,0.0f);

	//���_�o�b�t�@�[�̃J�M���J����
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
	//�Ίp���̊p�x(���W�A���p)�̎Z�o
	g_fAnglePolygon = atan2f(BULLET_HEIGHT,BULLET_WIDTH);

	//�����X
	g_fLeftTopX = g_posPolygon.x + cos(g_rotPolygon + D3DX_PI + g_fAnglePolygon) * g_fLengthPolygon;
	//�����Y
	g_fLeftTopY = g_posPolygon.y + sin(g_rotPolygon + D3DX_PI + g_fAnglePolygon) * g_fLengthPolygon;
	//�E���X
	g_fRightTopX = g_posPolygon.x + cos(g_rotPolygon - g_fAnglePolygon) * g_fLengthPolygon;
	//�E���Y
	g_fRightTopY = g_posPolygon.y + sin(g_rotPolygon - g_fAnglePolygon) * g_fLengthPolygon;
	//�E����X
	g_fRightBottomX = g_posPolygon.x + cos(g_rotPolygon + g_fAnglePolygon) * g_fLengthPolygon;
	//�E����Y
	g_fRightBottomY = g_posPolygon.y + sin(g_rotPolygon + g_fAnglePolygon) * g_fLengthPolygon;
	//������X
	g_fLeftBottomX = g_posPolygon.x + cos(g_rotPolygon - D3DX_PI - g_fAnglePolygon) * g_fLengthPolygon;
	//������Y
	g_fLeftBottomY = g_posPolygon.y + sin(g_rotPolygon - D3DX_PI - g_fAnglePolygon) * g_fLengthPolygon;
	*/
			break;
		}
	}
}