/*==============================================================================

    �|���S���`�� [camera.cpp]
                                                       Author : Emu Saito
                                                       Date   : 2016/05/24
--------------------------------------------------------------------------------
    ��Update : None
==============================================================================*/
/*============================================================================
	�C���N���[�h�t�@�C��
============================================================================*/
#include "main.h"
#include "input.h"
#include "model.h"
#include "camera.h"

/*============================================================================
	�萔��`
============================================================================*/
#define CAMERA_WAIT_SPEED (100)				//�J������������荞�݂���܂ł̎���
#define CAMERA_ROTATE_SPEED (0.05f)			//�J��������荞�݂���X�s�[�h
/*============================================================================
	�O���[�o���ϐ�
============================================================================*/
LPDIRECT3DDEVICE9 g_pD3DDeviceCamera;

int g_CameraMode;			//�J�����̃��[�h

int g_ModelWrap;			//�J�����̉�荞�݃J�E���g

D3DXVECTOR3 BeforeModelPos;		//���f���̑O�̈ʒu


D3DXVECTOR3 g_PosAt;		//�����_
D3DXVECTOR3 g_PosEye;		//�J�����̍��W
D3DXVECTOR3 g_VecUp;		//�J�����̏�����̃x�N�g��

float g_FovY;			//��p(����p)
float g_fAngleCameraEye = 0.0f;
float g_LengthCamera = 15.0f;
float g_fAngleCameraAt = 0.0f;

D3DXVECTOR3 g_VecCamera;
D3DXVECTOR3 g_PosPlus;		//�傫��
D3DXVECTOR3 ModelPos = D3DXVECTOR3(0.0f,0.0f,0.0f);


//�r���[�ϊ��̕ϐ�
D3DXMATRIX mtxView;
/*============================================================================
	����������
============================================================================*/
void InitCamera(void)
{
	g_CameraMode = 1;				//0���f�o�b�O���[�h		1���Ǐ]���[�h

	g_pD3DDeviceCamera = GetDevice();

	//�r���[�s����쐬
	g_PosAt = D3DXVECTOR3(0.0f,0.0f,0.0f);		//�����_�i�������Ƃ���j
	g_PosEye = D3DXVECTOR3(0.0f,10.0f,-15.0f);	//�J�����̍��W
	g_VecUp = D3DXVECTOR3(0.0f,1.0f,0.0f);		//�J�����̏�����x�N�g��
	g_FovY = D3DX_PI/3;							//��p(����p)

	//�J������p�̃r���[�s��
	D3DXMatrixLookAtLH(&mtxView,&g_PosEye,&g_PosAt,&g_VecUp);		//��̂R����r���[�s������֐�

	//���[���h�r���[�ϊ��̐ݒ�
	g_pD3DDeviceCamera->SetTransform(D3DTS_VIEW,&mtxView);		//�r���[�s��̐ݒ�

	//�v���W�F�N�V�����s����쐬

	//�v���W�F�N�V�����̕ϐ�
	D3DXMATRIX mtxProj;
	D3DXMatrixPerspectiveFovLH(&mtxProj,
		D3DX_PI / 3.0f,								//��p(����p)
		(float)SCREEN_WIDTH/(float)SCREEN_HEIGHT,	//�A�X�y�N�g��
		1.0f,		//near 0���傫������
		1000.0f		//far
		);

	//�v���W�F�N�V�����ϊ��̐ݒ�
	g_pD3DDeviceCamera->SetTransform(D3DTS_PROJECTION,&mtxProj);

	
	//�J�����𒆐S�Ƃ���
	g_fAngleCameraAt = atan2f(g_PosAt.z - g_PosEye.z,g_PosAt.x - g_PosEye.x);

	//�����_�𒆐S�Ƃ���
	g_fAngleCameraEye = atan2f(g_PosEye.z - g_PosAt.z,g_PosEye.x - g_PosAt.x);
	g_fAngleCameraEye = D3DX_PI;
}
/*============================================================================
	�I������
============================================================================*/
void UninitCamera(void)
{
	//���
	/*if(g_pD3DDeviceCamera != NULL)
	{
		g_pD3DDeviceCamera->Release();
		g_pD3DDeviceCamera = NULL;
	}*/
}
/*============================================================================
	�`�揈��
============================================================================*/
void DrawCamera(void)
{

}
/*============================================================================
	�X�V����
============================================================================*/
void UpdateCamera(void)
{
	float fDust;
	D3DXVECTOR3 Speed;
	float fAngle;

	fAngle = GetAngle();

	D3DXVECTOR3 ModelSpeed = D3DXVECTOR3(0.0f,0.0f,0.0f);

	g_pD3DDeviceCamera = GetDevice();

	//�r���[�ϊ��̕ϐ�
	//D3DXMATRIX mtxView;

	D3DXMatrixLookAtLH(&mtxView,&g_PosEye,&g_PosAt,&g_VecUp);		//��̂R����r���[�s������֐�

	//���[���h�r���[�ϊ��̐ݒ�
	g_pD3DDeviceCamera->SetTransform(D3DTS_VIEW,&mtxView);		//�r���[�s��̐ݒ�

	//�v���W�F�N�V�����s����쐬

	//�v���W�F�N�V�����̕ϐ�
	D3DXMATRIX mtxProj;
	D3DXMatrixPerspectiveFovLH(&mtxProj,
		D3DX_PI / 3.0f,								//��p(����p)
		(float)SCREEN_WIDTH/(float)SCREEN_HEIGHT,	//�A�X�y�N�g��
		1.0f,		//near 0���傫������
		1000.0f		//far
		);

	//�v���W�F�N�V�����ϊ��̐ݒ�
	g_pD3DDeviceCamera->SetTransform(D3DTS_PROJECTION,&mtxProj);

	//�f�o�b�O���[�h
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

			//�J�����̍��W�̌v�Z
			g_PosEye.x = g_PosAt.x + cos(g_fAngleCameraEye) * g_LengthCamera;
			g_PosEye.z = g_PosAt.z + sin(g_fAngleCameraEye) * g_LengthCamera;
		}
		if(GetKeyboardPress(DIK_Q))
		{
			g_fAngleCameraEye = atan2f(g_PosEye.z - g_PosAt.z,g_PosEye.x - g_PosAt.x);
			g_fAngleCameraEye -= 0.1f;

			//�J�����̍��W�̌v�Z
			g_PosEye.x = g_PosAt.x + cos(g_fAngleCameraEye) * g_LengthCamera;
			g_PosEye.z = g_PosAt.z + sin(g_fAngleCameraEye) * g_LengthCamera;
		}

		if(GetKeyboardPress(DIK_LEFT))
		{
			g_fAngleCameraAt = atan2f(g_PosAt.z - g_PosEye.z,g_PosAt.x - g_PosEye.x);
			g_fAngleCameraAt -= 0.1f;

			//�J�����𒆐S�Ƃ�����]
			//�����_�̍��W�̌v�Z
			g_PosAt.x = g_PosEye.x + cos(g_fAngleCameraAt) * g_LengthCamera;
			g_PosAt.z = g_PosEye.z + sin(g_fAngleCameraAt) * g_LengthCamera;
		}
		if(GetKeyboardPress(DIK_RIGHT))
		{
			g_fAngleCameraAt = atan2f(g_PosAt.z - g_PosEye.z,g_PosAt.x - g_PosEye.x);
			g_fAngleCameraAt += 0.1f;

			//�J�����𒆐S�Ƃ�����]
			//�����_�̍��W�̌v�Z
			g_PosAt.x = g_PosEye.x + cos(g_fAngleCameraAt) * g_LengthCamera;
			g_PosAt.z = g_PosEye.z + sin(g_fAngleCameraAt) * g_LengthCamera;
		}

		//�O�i
		if(GetKeyboardPress(DIK_W))
		{

			//�J�������璍���_�ɑ΂��Ă̕����x�N�g��
			g_VecCamera = g_PosAt - g_PosEye;

			g_VecCamera.y = 0.0f;

			//�P�ʃx�N�g���������āA�傫���P�̃x�N�g����������B
			D3DXVec3Normalize(&g_VecCamera,&g_VecCamera);

			g_PosEye += g_VecCamera * 1.0f;
			g_PosAt += g_VecCamera * 1.0f;

		}
		//��i
		if(GetKeyboardPress(DIK_S))
		{

			//�J�������璍���_�ɑ΂��Ă̕����x�N�g��
			g_VecCamera = g_PosAt - g_PosEye;

			g_VecCamera.y = 0.0f;

			//�P�ʃx�N�g���������āA�傫���P�̃x�N�g����������B
			D3DXVec3Normalize(&g_VecCamera,&g_VecCamera);

			g_PosEye += g_VecCamera * -1.0f;
			g_PosAt += g_VecCamera * -1.0f;

		}
		
		/*
		//�E
		if(GetKeyboardPress(DIK_D))
		{

			//�J�������璍���_�ɑ΂��Ă̕����x�N�g��
			g_VecCamera = g_PosAt - g_PosEye;

			//�����͊֌W�Ȃ�����Y�ɂ�0������
			g_VecCamera.y = 0.0f;


			//�P�ʃx�N�g���������āA�傫���P�̃x�N�g����������B
			D3DXVec3Normalize(&g_VecCamera,&g_VecCamera);


			//�Ƃ肠�������
			fDust = g_VecCamera.x;
			g_VecCamera.x = -(g_VecCamera.z);
			g_VecCamera.z = fDust;

			g_PosEye += g_VecCamera * 1.0f;

		}
		//��
		if(GetKeyboardPress(DIK_A))
		{

			//�J�������璍���_�ɑ΂��Ă̕����x�N�g��
			g_VecCamera = g_PosAt - g_PosEye;

			g_VecCamera.y = 0.0f;

			//�P�ʃx�N�g���������āA�傫���P�̃x�N�g����������B
			D3DXVec3Normalize(&g_VecCamera,&g_VecCamera);
			fDust = g_VecCamera.x;

			g_VecCamera.x = -(g_VecCamera.z);
			g_VecCamera.z = fDust;

			g_PosEye += g_VecCamera * -1.0f;

		}*/

	}
	
	//�Ǐ]�J�������[�h(�J����������荞��ON��OFF)
	if(g_CameraMode == 1 || g_CameraMode == 2)
	{
		MODEL *Model = GetModelStruct();

		//�v���C���[�̈ړ��ʂ������Ȃ�΂Ȃ�قǁA�����_�̂������܂��ɏo���̂�����


		g_PosAt.x = Model[0].posModel.x + cosf(Model[0].fAngle + D3DX_PI * 0.5) * 5;		//2�̓��f���̂Q�O�̈ʒu�𒍎��_�Ƃ��Ă���
		g_PosAt.z = Model[0].posModel.z + sinf(Model[0].fAngle - D3DX_PI * 0.5) * 5;		//2�̓��f���̂Q�O�̈ʒu�𒍎��_�Ƃ��Ă���

		g_PosAt.x = Model[0].posModel.x;
		g_PosAt.z = Model[0].posModel.z;

		//�Ǐ]�J�������[�h�i������荞��OFF�j
		if(g_CameraMode == 1)
		{
			g_PosEye.x = g_PosAt.x + cosf(g_fAngleCameraEye - D3DX_PI * 0.5) * g_LengthCamera;
			g_PosEye.z = g_PosAt.z + sinf(g_fAngleCameraEye + D3DX_PI * 0.5) * g_LengthCamera;
			g_PosEye.y = Model[0].posModel.y + 10;
		}

		
		//�Ǐ]�J�������[�h�i������荞��ON�j
		if(g_CameraMode == 2)
		{
			//���f���̊p�x��������Ă���
			MODEL *Model = GetModelStruct();

			g_PosEye.x = g_PosAt.x + cosf(g_fAngleCameraEye - D3DX_PI * 0.5) * g_LengthCamera;
			g_PosEye.z = g_PosAt.z + sinf(g_fAngleCameraEye + D3DX_PI * 0.5) * g_LengthCamera;
			g_PosEye.y = Model[0].posModel.y + 15;
			

			//�v���C���[�̈ʒu���O�̍��W�Ɠ����������ꍇ�ɃC���N�������g���ď����ł��������� = 0�ɂ���
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


				//�p�x��-3.14~0~3.14�܂ł̂Ƃ����0~6.28�ɕς��Ă�(�킩��₷�����邽��)
				fAngleEye = Model[0].fAngle + D3DX_PI;
				g_fAngleHold = g_fAngleCameraEye + D3DX_PI;

				//���f���̔��Α��̊p�x�����߂�
				fAngleEye = fAngleEye + D3DX_PI;

				//6.28���傫���Ȃ��Ă��܂����ꍇ
				if(fAngleEye >= D3DX_PI * 2)
				{
					fAngleEye -= D3DX_PI * 2;
				}

				//�v���X��������
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
				//�}�C�i�X��������
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

				//�J�����̈ʒu���C��
				g_PosEye.x = g_PosAt.x + cosf(g_fAngleCameraEye - D3DX_PI * 0.5) * 10;
				g_PosEye.z = g_PosAt.z + sinf(g_fAngleCameraEye + D3DX_PI * 0.5) * 10;
				g_PosEye.y = Model[0].posModel.y + 15;
			}

			BeforeModelPos = Model[0].posModel;
		}
	}



	//�J�������[�h�ؑ�
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