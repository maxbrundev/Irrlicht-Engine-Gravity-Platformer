#pragma once
#include "Window.h"
#include "SharedContext.h"
#include "Scene.h"
#include "UserInterface.h"

class Game
{
public:
	Game();
	~Game();

	void UpdateDeltaTime();

	void Run();
	void Update();
	void Draw() const;
	void UpdateScores();

private:
	SharedContext	m_sharedContext;
	EventManager*	m_eventManager;
	Window*			m_window;
	Scene*			m_scene;
	UserInterface*	m_userInterface;
};
