// 2023 Takeru Yui All Rights Reserved.
#pragma once
#include <vector>

class WorldSprite;

/// <summary>
/// 2D�̔w�i�}�b�v
/// </summary>
class Map
{
public:
	Map();
	~Map();

	void Load();
	void Update();
	void Draw();

	// �}�b�v�`�b�v�̃T�C�Y
	static const float	ChipSize;
	static const int	ChipPixelSize;
	static const int	StageDataColNum = 16;		// �}�b�v�s
	static const int	StageDataRowNum = 20;		// �}�b�v��

private:
	static const int Stage1Data[StageDataColNum][StageDataRowNum];
	
	std::vector<std::vector<int>> currentData;
	std::vector<WorldSprite*> sprites;
	int chipGraph;
};

