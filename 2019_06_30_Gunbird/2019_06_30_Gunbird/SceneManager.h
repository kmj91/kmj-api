#pragma once

class Scene;

enum ChangeScene;

class SceneManager
{
public:
	SceneManager();
	virtual ~SceneManager();

	void SceneChangeUpdate();		// 씬 교체 업데이트
	void SceneUpdate();				// 현제 씬 업데이트

private:
	Scene *m_scene;			// 현제 씬

public:
	ChangeScene m_change;
	
};

