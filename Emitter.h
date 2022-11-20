#include"ParticleManager.h"
#include<memory>
#include<DirectXMath.h>
using namespace DirectX;
#pragma once
class Emitter
{
private:

	std::unique_ptr<ParticleManager>particleMan;

public:

	void Initialize();
	void Create(const XMFLOAT3 center);
	void Update();
	void Draw();

private:

};