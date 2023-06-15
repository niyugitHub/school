#pragma once

//#ifndef _OBSTRUCT_FLOAT_H_
//#define _OBSTRUCT_FLOAT_H_

#include "DxLib.h"
class ObstructBase
{
public:
    ObstructBase(int handle = 0);				// �R���X�g���N�^.
    virtual ~ObstructBase();		// �f�X�g���N�^.

    virtual void Update();					// �X�V.
    virtual void Draw();					// �`��.

    // ���f���n���h���̎擾.
    virtual int GetModelHandle() { return modelHandle; }

    // �|�W�V������getter/setter.
    virtual  const VECTOR& GetPos() const { return pos; }
    virtual void SetPos(const VECTOR set) { pos = set; }

protected:
    int		modelHandle;	// ���f���n���h��.
    VECTOR	pos;			// �|�W�V����.

};

