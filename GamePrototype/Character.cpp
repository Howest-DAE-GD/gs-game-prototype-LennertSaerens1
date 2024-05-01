#include "pch.h"
#include "Character.h"
#include "iostream"

Character::Character(const Point2f& startingPos)
	:m_Pos{startingPos}
	,m_Velocity{}
	,m_WalkingState{WalkingState::none}
	,m_LookingState{ LookingState::right }
	, m_JumpTime{ 0.4f }
	,m_TimeInAir{0}
	, m_CanJump{ 1 }
	,m_IsOnGround{  }
	,m_FrameRect{ Rectf(2, 341, 178, 336) }
	,m_IsDead{ false }
	,m_CanPlayClang{true}
	,m_CanPlaySad{ true }
	,m_DeadSeconds{ 0 }
{
	m_pSpritesheet = new Texture("character.png");
	m_Bounds = Rectf(0, 0, GetCurrFrameRect().width/2, GetCurrFrameRect().height/2);
	m_pJumpEffect = new SoundEffect("maro-jump-sound-effect_1.mp3");
	m_Sad = new SoundEffect("tf_nemesis.mp3");
	m_MetalClang = new SoundEffect("metal-pipe-clang.mp3");
}

Character::~Character()
{
	delete m_pSpritesheet;
	m_pSpritesheet = nullptr;
	delete m_pJumpEffect;
	m_pJumpEffect = nullptr; 
	delete m_Sad;
	delete m_MetalClang;
}


void Character::Update(float elapsedSec, const std::vector<std::vector<Point2f>>& landscape, const Texture* map)
{

	const float gravity{ -7.f };
	const float friction{ -1.4f };

	const Point2f leftTop{ m_Pos.x+50, m_Pos.y + m_Bounds.height };
	const Point2f leftTopMiddle{ m_Pos.x + 50, m_Pos.y + m_Bounds.height / 4*3 };

	const Point2f leftBottom{ m_Pos.x+50, m_Pos.y };
	const Point2f leftBottomMiddle{ m_Pos.x + 50, m_Pos.y + m_Bounds.height / 4 };

	const Point2f rightTop{ m_Pos.x + m_Bounds.width-50, m_Pos.y + m_Bounds.height };
	const Point2f rightTopMiddle{ m_Pos.x + m_Bounds.width-50, m_Pos.y + m_Bounds.height /4*3};

	const Point2f rightBottom{ m_Pos.x + m_Bounds.width-50, m_Pos.y };
	const Point2f rightBottomMiddle{ m_Pos.x + m_Bounds.width-50, m_Pos.y + m_Bounds.height / 4 };


	const Point2f lowerLeft{ m_Pos.x, m_Pos.y + m_Bounds.height / 4 };
	const Point2f lowerMiddle{ m_Pos.x + m_Bounds.width/2, m_Pos.y + m_Bounds.height / 4 };
	const Point2f lowerRight{ m_Pos.x + m_Bounds.width, m_Pos.y + m_Bounds.height / 4 };

	utils::HitInfo hitInfo{};

	m_Pos.x += m_Velocity.x * elapsedSec;
	m_Pos.y += m_Velocity.y * elapsedSec;
	if (m_Velocity.x < 1.f && m_Velocity.x >-1.f) m_Velocity.x = 0;
	m_Bounds = Rectf(m_Pos.x, m_Pos.y, GetCurrFrameRect().width / 2, GetCurrFrameRect().height / 2);


	for (int idx{ 0 }; idx < landscape.size(); ++idx)
	{

		const std::vector <Point2f>& collissionShape{ landscape[idx] };
		
		if ((utils::Raycast(collissionShape, leftTop, leftTopMiddle, hitInfo) || utils::Raycast(collissionShape, rightTop, rightTopMiddle, hitInfo)) && m_Velocity.y > 0)
		{
			m_Velocity.y = 0;
			m_Pos.y = hitInfo.intersectPoint.y-m_Bounds.height;
		}
		else
		{
			m_Velocity.y += gravity;
		}
		if ((utils::Raycast(collissionShape, leftBottom, leftBottomMiddle, hitInfo) || utils::Raycast(collissionShape, rightBottom, rightBottomMiddle, hitInfo)) && m_Velocity.y < 0)
		{
			m_Velocity.y = 0;
			m_Pos.y = hitInfo.intersectPoint.y;
			m_TimeInAir = 0;
			m_CanJump = 1;
			m_IsOnGround = 1;
		}
		if ((utils::Raycast(collissionShape, lowerLeft, lowerMiddle, hitInfo)))
		{
			if (hitInfo.normal.Normalized().x >= 0.3)
			{
				if (m_Velocity.x <= 0)
				{
					m_Velocity.x = 0;
					m_Pos.x = hitInfo.intersectPoint.x;
				}

			}
		}
		if ((utils::Raycast(collissionShape, lowerRight, lowerMiddle, hitInfo)))
		{
			if (hitInfo.normal.Normalized().x <= -0.3)
			{
				if (m_Velocity.x >= 0)
				{
					m_Velocity.x = 0;
					m_Pos.x = hitInfo.intersectPoint.x - m_Bounds.width;
				}
			}
		}
		
		
		if (m_Velocity.y > 5)
		{
			m_IsOnGround = 0; 
		}
		if (m_Velocity.y < -100)
		{
			m_IsOnGround = 0;
			m_CanJump = false;
		}
		if (m_Velocity.x > 0 && m_IsOnGround == 1) m_Velocity.x += friction;
		if (m_Velocity.x < 1.f && m_Velocity.x >-1.f) m_Velocity.x = 0;
		if (m_Velocity.x < 0 && m_IsOnGround == 1) m_Velocity.x -= friction;
		if (m_Velocity.x < 1.f && m_Velocity.x >-1.f) m_Velocity.x = 0;
		


	}
	if (m_Pos.x <= 0)
	{ 
		m_Pos.x = 1;
		m_Velocity.x = 0;
	}
	if (m_Pos.x + m_Bounds.width >= map->GetWidth())
	{ 
		m_Pos.x = map->GetWidth() - 1 - m_Bounds.width;
		m_Velocity.x = 0;
	}
	if (m_IsDead)
	{
		m_DeadSeconds += elapsedSec;
	}
	Animate(elapsedSec);


}

void Character::Draw() const
{
	/*glPushMatrix();
	glTranslatef(m_Pos.x, m_Pos.y, 0);
	m_pTexture->Draw();
	glPopMatrix();*/

	Rectf drawRect{ m_Pos.x,m_Pos.y, m_FrameRect.width/2,m_FrameRect.height/2};
	m_pSpritesheet->Draw(drawRect, m_FrameRect);
}

void Character::WalkRight(float elapsedSec, const Uint8* pStates)
{
	if (m_Velocity.x <= 140.f && m_Velocity.x >= 0.f && m_IsOnGround == 1) m_Velocity.x += 1050.f * elapsedSec;
	else if (m_Velocity.x <= 150.f && m_IsOnGround == 0)m_Velocity.x += 2600.f * elapsedSec;
	m_WalkingState = WalkingState::right;
	m_LookingState = LookingState::right;
	
	
}

void Character::WalkLeft(float elapsedSec, const Uint8* pStates)
{
	
	if (m_Velocity.x >= -140.f && m_Velocity.x <= 0.f && m_IsOnGround == 1) m_Velocity.x -= 1050.f * elapsedSec;
	else if (m_Velocity.x >= -150.f && m_IsOnGround == 0)m_Velocity.x -= 2500.f * elapsedSec;
	
	m_WalkingState = WalkingState::left;
	m_LookingState = LookingState::left;
}


void Character::HandleMovement(float elapsedSec, const Uint8* pStates)
{
	// Aparte functions voor elke movement method want ze worden lang
	if (!m_IsDead)
	{
		if (m_IsOnGround == 1)
		{
			m_WalkingState = WalkingState::none;

		}
		if (pStates[SDL_SCANCODE_RIGHT])
		{
			WalkRight(elapsedSec, pStates);
		}
		else if (pStates[SDL_SCANCODE_LEFT])
		{
			WalkLeft(elapsedSec, pStates);
		}



		if (pStates[SDL_SCANCODE_SPACE])
		{
			if (m_IsOnGround == 1)
			{
				m_pJumpEffect->Play(0);
				m_IsOnGround = 0;
			}

			if (m_CanJump == 1)
			{
				m_Velocity.y = 400.f;
				m_TimeInAir += elapsedSec;
			}



		}


		if (m_IsOnGround == 0) (m_WalkingState = WalkingState::midAir);
		if (m_TimeInAir >= m_JumpTime)
		{
			m_CanJump = 0;
		}
	}
}



Rectf Character::GetCurrFrameRect() const
{
	return m_FrameRect;
}

Point2f Character::GetPos() const
{
	return m_Pos;
}

Rectf Character::GetBounds() const
{
	return m_Bounds;
}

void Character::Animate(float elapsedSec)
{
	if (m_WalkingState == WalkingState::none) m_FrameRect = Rectf(1, 354, 181, 345);
	else if (m_WalkingState == WalkingState::left)
	{
		m_FrameRect = Rectf(738, 380, 205, 311);
	}
	else if (m_WalkingState == WalkingState::right)
	{
		m_FrameRect = Rectf(533, 365, 185, 323);
	}

	else if (m_WalkingState == WalkingState::midAir && m_LookingState == LookingState::left)
	{
		m_FrameRect = Rectf(361, 339, 152, 391);
	}
	else if (m_WalkingState == WalkingState::midAir && m_LookingState == LookingState::right)
	{
		m_FrameRect = Rectf(194, 342, 153, 391);
	}
	if (m_IsDead) m_FrameRect = Rectf(1, 354, 181, 345);
	if (m_IsDead && m_DeadSeconds >= 3)	m_FrameRect = Rectf(954, 94, 341, 180);
	if (m_IsDead && m_DeadSeconds >= 3 && m_CanPlayClang)
	{
		m_MetalClang->Play(0);
		m_CanPlayClang = false;
	}
	if (m_IsDead && m_DeadSeconds >= 7 && m_CanPlaySad)
	{
		m_Sad->Play(0);
		m_CanPlaySad = false;
	}
	
}

void Character::SetPos(const Point2f& pos)
{
	m_Pos = pos;
}

void Character::SetDead(const bool& isDead)
{
	m_IsDead = isDead;
}

void Character::Reset()
{
	m_IsDead = false;
	m_DeadSeconds = 0;
	m_CanPlayClang = true;
	m_CanPlaySad = true;
}

float Character::GetDeadSec()
{
	return m_DeadSeconds;
}

void Character::OnKeyUpEvent(const SDL_KeyboardEvent& e)
{
	switch (e.keysym.sym)
	{
	case SDLK_SPACE:
		m_CanJump = false;
		break;
	}
}
