﻿//-----------------------------------------------------------------------------
// @brief  プレイヤークラス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "Player.h"

//-----------------------------------------------------------------------------
// @brief  コンストラクタ.
//-----------------------------------------------------------------------------
Player::Player()
{
	// ３Ｄモデルの読み込み
	modelHandle = MV1LoadModel("data/model/player/hackadoll.pmx");

	pos = VGet(0, 0, 0);
}

//-----------------------------------------------------------------------------
// @brief  デストラクタ.
//-----------------------------------------------------------------------------
Player::~Player()
{
	// モデルのアンロード.
	MV1DeleteModel(modelHandle);
}

//-----------------------------------------------------------------------------
// @brief  更新.
//-----------------------------------------------------------------------------
void Player::Update()
{
	// キー入力取得
	int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// 上を押していたら上に進む
	if (key & PAD_INPUT_UP)
	{
		pos.z += 1;
	}

	// 下を押していたら下に進む
	if (key & PAD_INPUT_DOWN)
	{
		pos.z -= 1;
	}

	// 右を押していたら右に進む
	if (key & PAD_INPUT_RIGHT)
	{
		pos.x += 1;
	}

	// 左を押していたら左に進む
	if (key & PAD_INPUT_LEFT)
	{
		pos.x -= 1;
	}

	// ３Dモデルのポジション設定
	MV1SetPosition(modelHandle, pos);
}

//-----------------------------------------------------------------------------
// @brief  描画.
//-----------------------------------------------------------------------------
void Player::Draw()
{
	// ３Ｄモデルの描画
	MV1DrawModel(modelHandle);
}