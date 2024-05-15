#pragma once
#include "BaseGame.h"
#include "Camera.h"
#include "Character.h"
#include "Texture.h"
#include "SoundEffect.h"
#include "PowerUp.h"
class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:
	Character* m_pPlayer;
	Camera* m_pCamera;
	Texture* m_pMap;
	Texture* m_pBadGuy;
	Texture* m_pBadGuyEnd;
	Texture* m_pPotion;
	Texture* m_pTimerString;
	Rectf m_PotionRect;
	std::vector<std::vector<Point2f>> m_Landscape;
	std::vector<PowerUp*> m_pPowerUps;
	int m_LevelNr;
	int m_CurrLevel;
	float m_BadGuySeconds;
	Point2f m_BadGuyPos;
	SoundEffect* m_Yay;
	SoundEffect* m_Drinking;
	bool m_CanDrink{ true };
	float m_Timer;
	bool m_Finished{ true };
	bool m_Results{ false };
	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	void ResetAll();
};