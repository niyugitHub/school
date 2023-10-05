// 2023 Takeru Yui All Rights Reserved.
#include<vector>
#include "Map.h"
#include "DxLib.h"
#include "WorldSprite.h"

const int Map::Stage1Data[StageDataColNum][StageDataRowNum] =
{
	{73,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65},
};

const float Map::ChipSize = 0.725f;
const int Map::ChipPixelSize = 32;

/// <summary>
/// �R���X�g���N�^
/// </summary>
Map::Map()
	: chipGraph(-1)
{
	currentData.clear();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Map::~Map()
{
	for (const auto& sprite : sprites)
	{
		if (sprite != nullptr)
		{
			delete sprite;
		}
	}
	sprites.clear();
}

/// <summary>
/// ���[�h
/// </summary>
void Map::Load()
{
	// �Ƃ肠�����}�b�v���[�h
	chipGraph = LoadGraph("data/map.png");

	// WorldSprite���̐ݒ�ƈʒu������
	VECTOR chipLeftTopPos = VGet(0, StageDataColNum * ChipSize, 0);			// �}�b�v�̕`��J�n�ʒu�i����j
	for (int i = 0; i < StageDataColNum; i++)
	{
		for (int j = 0; j < StageDataRowNum; j++)
		{
			auto sprite = new WorldSprite();
			sprite->Initialize(chipGraph, ChipPixelSize, Stage1Data[i][j]);
			VECTOR chipHalfOffset = VGet(-Map::ChipSize * 0.5f, -Map::ChipSize * 0.5f, 0);					// �}�b�v�`�b�v�̔����T�C�Y�����ɂ��炷�I�t�Z�b�g
			VECTOR chipPos = VAdd(VGet(j * Map::ChipSize,  (-i - 1) * Map::ChipSize, 0), chipHalfOffset);	// �^�񒆃s�{�b�g�Ȃ̂Ń}�b�v�`�b�v�����T�C�Y���炷+�n�ʂȂ̂ň����
			chipPos = VAdd(chipPos, chipLeftTopPos);
			sprite->SetTransform(chipPos, Map::ChipSize);
			sprites.push_back(sprite);
		}
	}

	// ��X�Ⴄ�f�[�^�𓮓I�ɐ؂�ւ��邽�߁AcurrentData�ɃR�s�[
	const int(*targetDataRows)[StageDataRowNum] = Stage1Data;

	currentData.clear();
	for (int i = 0; i < StageDataColNum; i++)
	{
		std::vector<int> newColData;
		for (int j = 0; j < StageDataColNum; j++)
		{
			newColData.push_back(targetDataRows[i][j]);
		}
		currentData.push_back(newColData);
	}
}

/// <summary>
/// �X�V
/// </summary>
void Map::Update()
{
	// �����Ȃ�
}

/// <summary>
/// �`��
/// </summary>
void Map::Draw()
{
	// �䂭�䂭�̓J�����������Ă��āA�J�����͈͈ȊO�\�����Ȃ��悤��
	for (const auto& sprite : sprites)
	{
		sprite->Draw();
	}
}


