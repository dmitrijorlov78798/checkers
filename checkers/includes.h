// �������� ������������ ������������ �����,
// ����� � ���������� ���������� ��� ������
// ����� �������.

#pragma once

#ifndef INCLUDES
#define INCLUDES

// ������������ ������������ �����

#include <windows.h>
#include <list>
#include <map>
#include <string>
#include <iostream>

// �����

/**
 * @brief ��� �������
 *
 * @param HvsH -- ����� ������ ������
 * @param HvsAI -- ����� ������ ����������
 * @param AIvsAI -- ��������� ������ ����������
 */
enum class tipe_gamer
{
    HvsH, // ����� ������ ������
    HvsAI, // ����� ������ ����������
    AIvsAI // ��������� ������ ����������
};

/**
 * @brief ��� ����������� ����������� �����
 *
 * @param q -- ����� ������� ���� ������������ �����
 * @param e -- ������ ������� ���� ������������ �����
 * @param z -- ����� ������ ���� ������������ �����
 * @param c -- ������ ������ ���� ������������ �����
 * @param end -- ��������� �������,��������� ��� ��������
 */
enum class tipe_direction
{
    q, // ����� ������� ���� ������������ ����� 
    e, // ������ ������� ���� ������������ �����
    z, // ����� ������ ���� ������������ �����
    c, // ������ ������ ���� ������������ �����
    end // ��������� �������,��������� ��� ��������

};

/**
 * @brief �������� ����������������� ����������� ����������� �����
 *
 * @param direction -- �������� ������
 *
 * @return ������ ����� ����������
 */
inline tipe_direction& operator++ (tipe_direction& direction)
{
    if (direction != tipe_direction::end) // ���� �� �� � ����� ������ �����������
        direction = (tipe_direction)((int)direction + 1); // �� ����� �������������� ����� ����������� �� ������� ������� ��������
    return direction;
}

/**
 * @brief �������� �������������� �����������
 *
 * @param direction -- �������� ������
 *
 * @return ������ ����� ��������������
*/
inline tipe_direction operator! (tipe_direction direction)
{
    switch (direction) // ����������� ������ �� ���������������
    { // ��� �������������� ����� ��������� �� ���������� � ��������������� �������
    case tipe_direction::q:
        direction = tipe_direction::c;
        break;
    case tipe_direction::e:
        direction = tipe_direction::z;
        break;
    case tipe_direction::z:
        direction = tipe_direction::e;
        break;
    case tipe_direction::c:
        direction = tipe_direction::q;
        break;
    default:
        break;
    }
    return direction;
}

inline char int2char(int value)
{
    return (char)(value + (int)'0');
}
inline int char2int(char value)
{
    return ((int)value - (int)'0');
}


// ���������� ���������

/**
 * @brief ����� ������� ��� ��������
 *
 */
class constants
{
public:
    struct checker
    {
        static const int white = 1;
        static const int black = 2;
        static const int whiteQueen = 3;
        static const int blackQueen = 4;
        static const int killed = 5;
    };
    struct board
    {
        static const int whiteCheck = -1;
        static const int blackCheck = 0;
        static const int minVertical = 1;
        static const int maxVertical = 8;
        static const char minGorizontal = 'a';
        static const char maxGorizontal = 'h';
    };
    struct mask
    {
        static const int null = 0;
        static const int white = 10;
        static const int black = 20;
    };
    struct color
    {
        static const int white = 15;
        static const int yellow = 14;
        static const int green = 2;
        static const int blue = 3;
        static const int red = 4;
        static const int orange = 6;
    };
};


#endif // INCLUDES