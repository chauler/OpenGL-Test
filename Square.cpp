#include "Square.h"
#include <cmath>
#include <iostream>

Square::Square(Vector2 pos)
{
    m_position = pos;
    m_size = 0.3f;
}

Vector2* Square::getVertices()
{
    float hyp = sqrtf(m_size * m_size + m_size * m_size);
    Vector2* vertices = new Vector2[4]{Vector2(m_position.x-hyp, m_position.y-hyp), Vector2(m_position.x-hyp, m_position.y+hyp), Vector2(m_position.x+hyp, m_position.y+hyp), Vector2(m_position.x+hyp, m_position.y-hyp)};
    return vertices;
}

void Square::SetPos(Vector2 pos)
{
    m_position = pos;
}