//-----------------------------------------------------------------------------
// @brief  障害物：動かない.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef _OBSTRUCT_STATIC_H_
#define _OBSTRUCT_STATIC_H_

#include "DxLib.h"
#include "ObstructBase.h"

class ObstructStatic : public ObstructBase
{
public:
	ObstructStatic(int handle);				// コンストラクタ.
	virtual ~ObstructStatic();		// デストラクタ.
};

#endif // _OBSTACLE_BASE_H_

