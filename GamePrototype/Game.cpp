#include "pch.h"
#include "Game.h"
#include "utils.h"
#include "SVGParser.h"

Game::Game( const Window& window ) 
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_pPlayer = new Character(Point2f(7500.f, 100.f));
	m_pCamera = new Camera(GetViewPort().width, GetViewPort().height);
	m_pMap = new Texture("level1.png");
	m_pPotion = new Texture("Potion_of_Harming_JE3.png");
	m_PotionRect = Rectf(3900, 107, m_pPotion->GetWidth()/5, m_pPotion->GetHeight()/5);
	m_CurrLevel = 1;
	SVGParser::GetVerticesFromSvgFile("level1.svg", m_Landscape);
	m_LevelNr = 1;
	m_BadGuyPos = Point2f(400, 200);
	m_pBadGuy = new Texture("badguy.png");
	m_pBadGuyEnd = new Texture("BadGuyEnd.png");
	m_Yay = new SoundEffect("crowd_small_chil_ec049202_9klCwI6.mp3");
	m_Drinking = new SoundEffect("minecraft-drinking-sound-effect.mp3");
	m_BadGuySeconds = 0;
}

void Game::Cleanup( )
{
	delete m_pPlayer;
	m_pPlayer = nullptr;
	delete m_pCamera;
	delete m_pMap;
	delete m_pPotion;
	delete m_pBadGuy;
	delete m_pBadGuyEnd;
	delete m_Yay;
	delete m_Drinking;
}

void Game::Update( float elapsedSec )
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	m_pPlayer->Update(elapsedSec,m_Landscape,m_pMap);
	m_pPlayer->HandleMovement(elapsedSec, pStates);

	if (m_pPlayer->GetPos().y < 0)
	{
		m_LevelNr = 10;
		m_Yay->Play(0);
	}
	if (m_CurrLevel != m_LevelNr)
	{
		m_CurrLevel = m_LevelNr;
		std::string Level = "level" + std::to_string(m_LevelNr) +".png";
		delete m_pMap;
		m_pMap = new Texture(Level);
		m_pPlayer->SetPos(Point2f(10, 100));
		
	}
	m_BadGuySeconds += elapsedSec;
	if (m_pPlayer->GetPos().x >= 300)
	{
		m_BadGuyPos.x += 300.f * elapsedSec;
	}

	if (m_CanDrink && utils::IsOverlapping(m_pPlayer->GetBounds(), m_PotionRect))
	{
		m_Drinking->Play(0);
		m_CanDrink = 0;
		m_pPlayer->SetDead(true);
	}

	if (m_pPlayer->GetDeadSec() >= 10) ResetAll();
	// Check keyboard state
	//const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	//if ( pStates[SDL_SCANCODE_RIGHT] )
	//{
	//	std::cout << "Right arrow key is down\n";
	//}
	//if ( pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
	//{
	//	std::cout << "Left and up arrow keys are down\n";
	//}
}

void Game::Draw( ) const
{
	ClearBackground( );
	if (m_CurrLevel != 10)
	{
		m_pCamera->Aim(m_pMap->GetWidth(), m_pMap->GetHeight(), m_pPlayer->GetPos());
		m_pMap->Draw();
		m_pBadGuyEnd->Draw(Point2f(3750, 150));
		if (m_CanDrink) m_pPotion->Draw(m_PotionRect);
		m_pPlayer->Draw();
		m_pBadGuy->Draw(m_BadGuyPos);
		m_pCamera->Reset();
	}
	else m_pMap->Draw();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	switch ( e.keysym.sym )
	{
	case SDLK_r:
		ResetAll();
		break;
	
	}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	m_pPlayer->OnKeyUpEvent(e);
	
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::ResetAll()
{
	m_LevelNr = 1;
	m_pPlayer->SetPos(Point2f(50, 100));
	m_BadGuySeconds = 0;
	m_CanDrink = true;
	m_pPlayer->Reset();
	m_BadGuyPos = Point2f(400, 200);
}