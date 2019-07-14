#ifndef _MODELLISTCLASS_H_
#define _MODELLISTCLASS_H_

#include <D3DX10math.h>
#include <stdlib.h>
#include <time.h>

class ModelListClass
{
private:
	struct ModelInfoType
	{
		D3DXVECTOR4 color;
		float positionX, positionY, positionZ;
	};

public:
	ModelListClass();
	~ModelListClass();

	bool Initialize(int numModels);
	void Shutdown();

	int GetModelCount();
	void GetData(int index, float& positionX, float& positionY, float& positionZ, D3DXVECTOR4& color);

private:
	int m_modelCount;
	ModelInfoType* m_ModelInfoList;
};

#endif
