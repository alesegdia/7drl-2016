#pragma once

#include <alligator/util/matrix.h>

#include "../core/facing.h"

class PlacementMap
{
public:

    void reset( int w, int h )
    {
        m_matrix.reset(new Matrix2D<uint8_t>(w, h));
    }

    bool isUsed( int x, int y, Facing facing )
    {
        uint8_t facing_bit = (uint8_t)facing;
        uint8_t mask = 1 << facing_bit;
        uint8_t cell = m_matrix->get(x, y);
        return (cell & mask) != 0;
    }

    void setUsedValue( int x, int y, Facing facing, bool new_value )
    {
        uint8_t facing_bit = (uint8_t)facing;
        uint8_t cell = m_matrix->get(x, y);
        cell = new_value ? cell | (1 << facing_bit)
                         : cell & (1 << facing_bit);
        m_matrix->set(x, y, cell);
    }

private:

    Matrix2D<uint8_t>::SharedPtr m_matrix;

};
