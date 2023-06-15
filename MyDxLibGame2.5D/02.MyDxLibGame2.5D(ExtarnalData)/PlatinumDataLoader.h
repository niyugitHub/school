#pragma once
#include <DxLib.h>
#include <vector>

enum class LayerType {
	BackGround,//�w�i
	Enemies,//�G
	Event,//�C�x���g
};

//�}�b�v�̍L�� * ���C���[���Ȃ̂ŁA2�̃x�N�^�[�����q�ɂ��Ă�
using MapData_t = std::vector < std::vector<unsigned char>>;

//�X�e�[�W�����Ǘ�����N���X
class PlatinumDataLoader
{
public:
	void Load(const TCHAR* filePath);
	const MapData_t& GetMapData()const;
	const int GetChipId(int layerId,int chipX,int chipY)const;
	void GetMapSize(int& width,int& height);
private:
	/// <summary>
	/// �f�[�^�̓��e��XY�]�u����
	/// </summary>
	/// <param name="layerId">���C���[ID</param>
	void TransposeMapData(int layerId);
private:

	MapData_t mapData_;
	int mapWidth_ = 0;
	int mapHeight_ = 0;
};

