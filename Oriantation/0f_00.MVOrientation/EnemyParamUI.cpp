#include "EnemyParam.h"
#include "EnemyParamModel.h"
#include "EnemyUiHp.h"
#include "EnemyParamUI.h"

/// <summary>
/// �`��
/// </summary>
void EnemyParamUI::Draw()
{
	hpUi.Draw(paramModel.GetParam());
}

void EnemyParamUI::OnDamage(int damage)
{
	auto param = paramModel.GetParam();
	param.Life = param.Life - damage;
	paramModel.OnChangeParam(param);
}