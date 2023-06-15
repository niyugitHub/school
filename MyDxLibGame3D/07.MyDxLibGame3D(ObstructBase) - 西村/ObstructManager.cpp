#include "ObstructManager.h"
#include<DxLib.h>
#include"ObstructBase.h"
#include"ObstructFloat.h"
#include"ObstructStatic.h"
ObstructManager::ObstructManager()
{
	
}

ObstructManager::~ObstructManager()
{
	
}

void ObstructManager::Load()
{
}

void ObstructManager::Create()
{
	// áŠQ•¨‚ÌˆÊ’u‚ğ‰Šú‰».
	float band = 10.0f;

	int StaticHandle = MV1LoadModel("data/model/obstructStatic/obstructStatic.pmd");
	int FloatHandle = MV1LoadModel("data/model/obstructFloat/obstructFloat.pmx");

	for (int i = 0; i < 500; i++)
	{
		m_Obstruct.push_back(new ObstructStatic(StaticHandle));

		m_Obstruct.back()->SetPos(VGet(-band + (band * 0), 0, -1.0f));
		band++;
	}

	band = 10.0f;

	for (int i = 0; i < 500; i++)
	{
		m_Obstruct.push_back(new ObstructFloat(FloatHandle));

		m_Obstruct.back()->SetPos(VGet(-band + (band * 0), 0, -1.0f));
		band--;
	}
}

void ObstructManager::Delete()
{
	// áŠQ•¨‚ğíœ.
	m_Obstruct.erase(m_Obstruct.begin(), m_Obstruct.end());
}

void ObstructManager::Update()
{
	for (auto& obstruct : m_Obstruct)
	{
		obstruct->Update();
	}
}

void ObstructManager::Draw()
{
	for (auto& obstruct : m_Obstruct)
	{
		obstruct->Draw();
	}
}
