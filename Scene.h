#pragma once

#include "Model.h"

class CScene
{
	std::vector<IModel*> models;
public:
	CScene();
	IModel* GetModel(size_t i);
	void AddModel(IModel*);
	~CScene();
};

