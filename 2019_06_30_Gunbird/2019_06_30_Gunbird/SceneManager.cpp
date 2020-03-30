#include "stdafx.h"
#include "SceneManager.h"
#include "SceneTitleAndSelect.h"
#include "SceneMainGame.h"

SceneManager::SceneManager()
{
	// 초기 씬
	m_change = SCENE_TITLE;
}


SceneManager::~SceneManager()
{
	if (m_scene != NULL) {
		delete m_scene;
	}
}

void SceneManager::SceneChangeUpdate()
{
	// 교체할 씬이 있으면
	if (m_change != SCENE_EMPT) {
		if (m_scene != NULL) {
			delete m_scene;
		}
		// 씬 교체
		switch (m_change)
		{
		case SCENE_TITLE:
			m_scene = new SceneTitleAndSelect;
			break;
		case SCENE_GAME:
			m_scene = new SceneMainGame;
			break;
		default:
			break;
		}
		m_change = SCENE_EMPT;
	}
}

void SceneManager::SceneUpdate()
{
	m_scene->Update();
}
