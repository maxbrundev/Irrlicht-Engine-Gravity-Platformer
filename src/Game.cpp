#include "Game.h"
#include "Player.h"
#include <fstream>
#include <algorithm>

Game::Game()
{
	UpdateScores();

	m_eventManager = new EventManager(&m_sharedContext);
	m_sharedContext.eventManager = m_eventManager;

	m_window = new Window(m_eventManager);
	m_sharedContext.window = m_window;

	m_scene = new Scene(&m_sharedContext);
	m_sharedContext.scene = m_scene;

	m_scene->Setup();

	m_userInterface = new UserInterface(&m_sharedContext);
}

Game::~Game()
{
	delete m_eventManager;
	delete m_window;
	delete m_userInterface;
	delete m_scene;
}

void Game::UpdateDeltaTime()
{
	m_sharedContext.currentTime = m_window->GetDevice()->getTimer()->getTime();
	m_sharedContext.deltaTime = (m_sharedContext.currentTime - m_sharedContext.lastTime) / 1000.f;
	m_sharedContext.lastTime = m_window->GetDevice()->getTimer()->getTime();
}


void Game::Run()
{
	while (m_sharedContext.window->GetDevice()->run())
	{
		Update();
		Draw();

		if (m_sharedContext.gameInfo.playerFailed)
		{
			m_scene->Close();
			m_sharedContext.Reset();
			UpdateScores();
			m_scene->Setup();
		}
	}
}

void Game::Update()
{
	UpdateDeltaTime();
	irr::core::stringw titre(m_window->GetDriver()->getFPS());
	m_window->GetDevice()->setWindowCaption(titre.c_str());
	m_eventManager->Update();
	m_scene->Update();
	m_userInterface->Update();

	if (m_scene->GetPlayer()->IsPlaying())
	{
		if (m_sharedContext.gameInfo.currentScore >= 0.f)
		{
			m_sharedContext.gameInfo.currentScore -= 500.f * m_sharedContext.deltaTime;

			if (m_scene->GetPlayer()->IsShooting())
				m_sharedContext.gameInfo.currentScore -= 5000.f * m_sharedContext.deltaTime;

			if (m_scene->GetPlayer()->IsLighting())
				m_sharedContext.gameInfo.currentScore -= 1000.f * m_sharedContext.deltaTime;
		}
		else
		{
			m_sharedContext.gameInfo.currentScore = 0;
		}
	}
}

void Game::Draw() const
{
	m_window->GetDriver()->beginScene(true, true, m_window->GetBackgroundColor());

	m_scene->GetSceneManager().drawAll();

	m_userInterface->Draw();

	m_window->GetDriver()->endScene();
}

void Game::UpdateScores()
{
	std::ifstream myReadFile;
	myReadFile.open("../assets/scores/data.txt");
	std::vector<uint32_t> scores;

	uint32_t output;

	if (myReadFile.is_open())
	{
		while (!myReadFile.eof())
		{
			myReadFile >> output;
			scores.push_back(output);
		}
	}

	myReadFile.close();

	if (!scores.empty())
	{
		m_sharedContext.lastScore = scores[scores.size() - 1];
		std::sort(scores.begin(), scores.end());
		m_sharedContext.highestScore = scores[scores.size() - 1];
	}
}
