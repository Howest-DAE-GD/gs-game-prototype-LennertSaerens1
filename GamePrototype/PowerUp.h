#pragma once
class Texture;
class PowerUp
{
public:
	
	PowerUp(const Point2f& pos);
	~PowerUp();
	void Draw();
	bool IsOverlapping(const Rectf& rect);
private:
	Rectf m_Bounds;
	const Texture* m_pTexture;
	bool m_IsCollected;

};
	

