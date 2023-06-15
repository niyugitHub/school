#include "ObstructBase.h"



ObstructBase::ObstructBase(int handle)
{
	// ３Ｄモデルの読み込み
	modelHandle = MV1DuplicateModel(handle);
	if (modelHandle < 0)
	{
		printfDx("ObstructFloat:データ読み込みに失敗");
	}

	pos = VGet(0, 0, 0);
}

ObstructBase::~ObstructBase()
{
	// モデルのアンロード.
	MV1DeleteModel(modelHandle);
}

void ObstructBase::Update()
{
	// ３Dモデルのポジション設定
	MV1SetPosition(modelHandle, pos);
}

void ObstructBase::Draw()
{
	// ３Ｄモデルの描画
	MV1DrawModel(modelHandle);
}
