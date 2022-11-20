#include "Emitter.h"

void Emitter::Initialize()
{
	// �p�[�e�B�N������
	particleMan = std::make_unique<ParticleManager>();
	particleMan->Initialize();
}

void Emitter::Draw()
{
	// �p�[�e�B�N���̕`��
	particleMan->Draw();
}

void Emitter::Update()
{
	particleMan->Update();
}

void Emitter::Create(const XMFLOAT3 center)
{
	for (int i = 0; i < 10; i++)
	{
		//X,Y,Z�S��[-10.0f,+10.0f]�Ń����_���ɕ��z
		const float rnd_pos = 20.0f;
		const float rnd_col = 255.0f;
		XMFLOAT3 pos{};
		pos.x = center.x + (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y = center.y + (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z = center.z + (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		//X,Y,Z�S��[-0.05f,+0.05f]�Ń����_���ɕ��z
		const float rnd_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f,0]�Ń����_���ɕ��z
		XMFLOAT3 acc{};
		const float rnd_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * rnd_acc;

		XMFLOAT4 col{};
		col.x = (float)rand() / RAND_MAX * rnd_col;
		col.y = (float)rand() / RAND_MAX * rnd_col;
		col.z = (float)rand() / RAND_MAX * rnd_col;

		//�ǉ�
		particleMan->Add(60, pos, vel, acc, 1.0f, 0.0f, col);
	}
}