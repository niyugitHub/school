// 2023 Takeru Yui All Rights Reserved.
#include<vector>
#include "Map.h"
#include "DxLib.h"
#include "WorldSprite.h"
#include "PlatinumDataLoader.h"

const int Map::Stage1Data[StageData1ColNum][StageData1RowNum] =
{
	{65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65},
	{73,73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
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
	{73, 0,73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65},
};

const int Map::Stage2Data[StageData2ColNum][StageData2RowNum] =
{
	{65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65},
	{73,73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73, 0,73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,65},
	{73,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65,65},
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
	stage = new PlatinumDataLoader;
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
void Map::Load(int stageNo)
{
	stage->Load("data/stage1.fmf");
	int width = 0;
	int height = 0;

	stage->GetMapSize(width, height);

	currentData = stage->GetMapData();
	//////////////////////////////////////////////////////
	// �O�������int�l���͂ŁAStage1Data���g����Stage2Data���g�����؂�ւ���
	// ����������ƁA�σf�[�^�ł���currentData��
	// Stage1Data��Stage2Data�̓��e���y�ǂݎ���đ�����Ă���z
	// �������O���t�@�C���u�O���t�@�C���̓��e��ǂݎ���đ������v�`�ɂ����
	// �O���f�[�^���͊���
	//////////////////////////////////////////////////////
	/*currentData.clear();*/
	std::vector<int> newColData;

	/*for (int i = 0; i < height; i++)
	{
		newColData.clear();
		for (int j = 0; j < width; j++)
		{
			newColData.push_back(stageData[i][j]);
		}
		currentData.push_back(newColData);
	}*/


	/*switch (stageNo)
	{
	case 0:
		dataColNum = StageData1ColNum;
		dataRowNum = StageData1RowNum;
		for (int i = 0; i < dataColNum; i++)
		{
			newColData.clear();
			for (int j = 0; j < dataRowNum; j++)
			{
				newColData.push_back(Stage1Data[i][j]);
			}
			currentData.push_back(newColData);
		}
		break;
	case 1:
		dataColNum = StageData2ColNum;
		dataRowNum = StageData2RowNum;
		for (int i = 0; i < dataColNum; i++)
		{
			newColData.clear();
			for (int j = 0; j < dataRowNum; j++)
			{
				newColData.push_back(Stage2Data[i][j]);
			}
			currentData.push_back(newColData);
		}
		break;
	}*/
	//////////////////////////////////////////////////////

	// �Ƃ肠�����}�b�v���[�h
	chipGraph = LoadGraph("data/map.png");

	// WorldSprite���̐ݒ�ƈʒu������
	VECTOR chipLeftTopPos = VGet(0, height * ChipSize, 0);			// �}�b�v�̕`��J�n�ʒu�i����j
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			/*int MapData = stage->GetChipId(0, j, i);*/
			auto sprite = new WorldSprite();
			sprite->Initialize(chipGraph, ChipPixelSize, currentData[0][i + j * height]);
			VECTOR chipHalfOffset = VGet(-Map::ChipSize * 0.5f, -Map::ChipSize * 0.5f, 0);					// �}�b�v�`�b�v�̔����T�C�Y�����ɂ��炷�I�t�Z�b�g
			VECTOR chipPos = VAdd(VGet(j * Map::ChipSize,  (-i - 1) * Map::ChipSize, 0), chipHalfOffset);	// �^�񒆃s�{�b�g�Ȃ̂Ń}�b�v�`�b�v�����T�C�Y���炷+�n�ʂȂ̂ň����
			chipPos = VAdd(chipPos, chipLeftTopPos);
			sprite->SetTransform(chipPos, Map::ChipSize);
			sprites.push_back(sprite);
		}
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


