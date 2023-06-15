#pragma once

//#ifndef _OBSTRUCT_FLOAT_H_
//#define _OBSTRUCT_FLOAT_H_

#include "DxLib.h"
class ObstructBase
{
public:
    ObstructBase(int handle = 0);				// コンストラクタ.
    virtual ~ObstructBase();		// デストラクタ.

    virtual void Update();					// 更新.
    virtual void Draw();					// 描画.

    // モデルハンドルの取得.
    virtual int GetModelHandle() { return modelHandle; }

    // ポジションのgetter/setter.
    virtual  const VECTOR& GetPos() const { return pos; }
    virtual void SetPos(const VECTOR set) { pos = set; }

protected:
    int		modelHandle;	// モデルハンドル.
    VECTOR	pos;			// ポジション.

};

