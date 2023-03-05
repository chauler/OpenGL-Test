#include "Vector2.h"

class Square
{
    public:
    Square(){*this = Square(0);}
    Square(float pos);
    ~Square(){}
    Vector2* getVertices();

    private:
    float m_position;
    float m_size;
};