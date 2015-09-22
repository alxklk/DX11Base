#include "stdafx.h"
#include "Scene.h"


CScene::CScene()
{
}


CScene::~CScene()
{
}

IModel* CScene::GetModel(size_t i)
{
	if(i<models.size())
		return nullptr;
	return models[i];
}

void CScene::AddModel(IModel* model)
{
	models.push_back(model);
}

