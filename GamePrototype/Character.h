#pragma once
#include "utils.h"
#include "Texture.h"
#include "SoundEffect.h"

class Character final
{
private:
	enum class WalkingState {
		none,
		left,
		right,
		midAir
	};
	enum class LookingState {
		left,
		right,
	};

	Point2f m_Pos;
	Vector2f m_Velocity;
	Texture* m_pSpritesheet;
	Rectf m_Bounds;
	WalkingState m_WalkingState;
	LookingState m_LookingState;
	SoundEffect* m_pJumpEffect;
	float m_JumpTime;
	float m_TimeInAir;
	bool m_CanJump;
	bool m_IsOnGround;
	Rectf m_FrameRect;


public:
	explicit Character(const Point2f& startingPos);
	Character(const Character& other) = delete; //Copy constructor afzetten (rule of three)
	~Character();
	void Update(float elapsedSec, const std::vector<std::vector<Point2f>>& landscape, const std::vector<std::vector<Point2f>>& platforms);
	void Draw() const;
	void WalkRight(float elapsedSec, const Uint8* pStates) ;
	void WalkLeft(float elapsedSec, const Uint8* pStates) ;
	void HandleMovement(float elapsedSec, const Uint8* pStates) ;
	void OnKeyUpEvent(const SDL_KeyboardEvent& e);
	void Animate(float elapsedSec);
	Rectf GetCurrFrameRect() const;
	Point2f GetPos() const;
	Rectf GetBounds() const;

	Character& operator=(const Character& rhs) = delete; // asignment= operator afzetten
};

