#pragma once
#include "BaseGame.h"
#include "Camera.h"
#include "Character.h"
#include "Texture.h"
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
	std::vector<std::vector<Point2f>> m_Landscape;
	std::vector<std::vector<Point2f>> m_Platforms;


	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;
};