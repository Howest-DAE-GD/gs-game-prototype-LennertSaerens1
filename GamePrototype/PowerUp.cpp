#include "pch.h"
#include "PowerUp.h"
#include "Texture.h"
#include "utils.h"


PowerUp::PowerUp(const Point2f& center)
{
	m_pTexture = new Texture("6850730-spuit-met-blauwe-vloeistof-flast-stijl-vector.png");
	m_Bounds = Rectf{ center.x,center.y,m_pTexture->GetWidth()/10, m_pTexture->GetHeight()/10 };
	m_IsCollected = false;
}



PowerUp::~PowerUp()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void PowerUp::Draw()
{
	if (!m_IsCollected)
	{
		m_pTexture->Draw(m_Bounds);
	}
}

bool PowerUp::IsOverlapping(const Rectf& rect)
{
	return utils::IsOverlapping(rect, m_Bounds);
}
