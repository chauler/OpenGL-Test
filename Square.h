#include "Vector2.h"

class Square
{
    public:
    Square()
    {
        *this = Square(Vector2(0.0, 0.0));
    }
    Square(Vector2 pos);
    ~Square(){}
    Vector2* getVertices();
    void SetPos(Vector2);
    Vector2 getPos(){return m_position;}

    private:
    Vector2 m_position;
    float m_size;
};