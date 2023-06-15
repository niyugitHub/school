﻿//-----------------------------------------------------------------------------
// @brief  障害物：浮遊する.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef _OBSTRUCT_FLOAT_H_
#define _OBSTRUCT_FLOAT_H_

#include "DxLib.h"
#include "ObstructBase.h"

class ObstructFloat : public ObstructBase
{
public:
	ObstructFloat(int handle);				// コンストラクタ.
	virtual ~ObstructFloat();		// デストラクタ.

    void Update() override;					// 更新.

private:
	float rad;						// sin移動用のラジアン関数.
};

#endif // _OBSTRUCT_FLOAT_H_

