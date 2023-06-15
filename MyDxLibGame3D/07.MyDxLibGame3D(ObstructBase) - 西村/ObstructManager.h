#pragma once
#include<vector>

class ObstructBase;
class ObstructManager
{
public:
	ObstructManager();
	~ObstructManager();

	void Load();
	void Create();
	void Delete();
	void Update();
	void Draw();

private:
	std::vector<ObstructBase*> m_Obstruct;
};

