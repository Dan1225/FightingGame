#include "../Scene/scene.h"
//**********************************************
//
//		シーンの管理
//
//**********************************************

void SceneManager::Update(float elapsedTime)
{
	currentScene->Update(elapsedTime);
}

void SceneManager::Render()
{
	currentScene->Render();
}

void SceneManager::ChangeScene(Scene* new_scene)
{
	currentScene.reset(new_scene);
	//currentScene->Initialize();
	// 2 重初期化されないようにする。
	if (!currentScene->initialized)
	{
		currentScene->Initialize();
		currentScene->initialized = true;
	}
}
