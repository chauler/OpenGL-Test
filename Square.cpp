#include "Square.h"
#include <cmath>
#include <iostream>

Square::Square(float pos)
{
    m_position = pos;
    m_size = 0.6f;
}

Vector2* Square::getVertices()
{
    float hyp = sqrtf(m_size * m_size + m_size * m_size);
    Vector2* vertices = new Vector2[4]{Vector2(m_position-hyp, m_position-hyp), Vector2(m_position-hyp, m_position+hyp), Vector2(m_position+hyp, m_position+hyp), Vector2(m_position+hyp, m_position-hyp)};
    return vertices;
}