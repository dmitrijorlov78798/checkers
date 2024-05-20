#pragma once

#include "includes.h"

/**
* @brief ����� ����������� ������ ������� �� ��������� ����� (������ "��������" ��� ����� ������� ������� ����� �����)
*       (����� ������������ ��� "��������" ��� ������ �������� ����)
*/
class position_t
{
private:
    char c_gorizont; // ��������� ����������� �� ��������� ����� �� �����������
    int i_vertical; // �������� ����������� �� ��������� ����� �� ���������
public:
    /**
    * @brief ����������� �� ���������
    */
    position_t() : c_gorizont('a'), i_vertical(1)
    {}
    /**
    * @brief ����������� 
    * 
    * @pamar c_gorizont -- �������� ����������� �� ��������� ����� �� ���������
    * @param i_vertical -- �������� ����������� �� ��������� ����� �� ���������
    */
    position_t(char c_gorizont, int i_vertical);
    /**
    * @brief ����� ���������� �������
    *
    * @pamar c_gorizont -- �������� ����������� �� ��������� ����� �� ���������
    * @param i_vertical -- �������� ����������� �� ��������� ����� �� ���������
    * 
    * @return true -- ������� ��������� �������(� �������� ��������� �����), ������� ���������
    *         false -- ������ �� �������, ������� �������� �������
    */
    bool update(char c_gorizont, int i_vertical);
    /**
    * @brief ����� ���������� ������� �� ���������
    *
    * @pamar direction -- ����������� ��������, ���� �� �������.
    * @param i_steps -- ���������� ������, �� ������� ���������� ������� �� ������� �������
    *
    * @return true -- ������� ��������� �������(� �������� ��������� �����), ������� ���������
    *         false -- ������ �� �������, ������� �������� �������
    */
    bool updateToDiagonal(tipe_direction direction, int i_steps);
    /**
    * @brief ����� �������� ��������� ������������� �������������� ����������
    *
    * @return �������� ������������� �������������� ����������
    */
    int getGorizont() const;
    /**
    * @brief ����� �������� ��������� ������������� ������������ ����������
    *
    * @return �������� ������������� ������������ ����������
    */
    int getVertical() const;
    /**
    * @brief ���������� ��������� ���������
    */
    bool operator == (const position_t& position) const;
    /**
    * @brief ���������� ��������� �����������
    */
    bool operator != (const position_t& position) const;
    /**
    * @brief ���������� ��������� ������
    */
    bool operator < (const position_t& position) const;
    /**
    * @brief ���������� ��������� ������
    */
    bool operator > (const position_t& position) const;
    /**
    * @brief ���������� ��������� ����������
    */
    position_t& operator++();
    /**
    * @brief ���������� ��������� ����������
    */
    position_t& operator--();
    /**
    * @brief ����� ��������� ������� �� ������
    * 
    * @param string -- ������ ��� ��������
    * 
    * @return true -- ������� ����������
    *         false -- ���������� ������� �� ����������
    */
    bool fromString(std::string string);
    /**
    * @brief ����� �������� ������� � ������
    * 
    * @return ������ � ��������
    */
    std::string toString() const;
    /**
    * @brief ����� 
    *
    * @param string -- ������ ��� ��������
    *
    * @return true -- ������� ����������
    *         false -- ���������� ������� �� ����������
    */
    static std::list<position_t> median(position_t first, position_t second);
    // ������ ����������� ����� ���������� ����������
    static position_t end();
    static position_t begin();
    static position_t rbegin();
    static position_t rend();
    // ����������, ��������� �� ������� ����� �� ������ ����������� ��� ����������� � ����� (��� ������)
    bool blackQueen() const;
    // ����������, ��������� �� ������� ����� �� ������ ����������� ��� ����������� � ����� (��� �����)
    bool whiteQueen() const;
    // �������� �������������� ������� � ���������� �� �������(winapi)
    operator COORD();
};

/**
* @brief ����� ����������� ������ ���������� ����(����) ������ ("�������" �������� ������ ����������� ����������)
*/
class step_t
{
private:
    std::list<position_t> step; // ������ �������(2-� ��� ������� ������������, 2-� � ����� ��� ������ ����� ���������� )
public:
    step_t()
    {
        step.push_back(position_t());
        step.push_back(position_t());
    }
    step_t(position_t first, position_t second)
    {
        step.push_front(first);
        step.push_back(second);
    }
    ~step_t()
    {
        step.clear();
    }
    position_t first() const;
    position_t second() const;
    bool find(position_t position) const;
    bool empty() const;
    void push(position_t position);
    position_t pop_back();
    position_t pop_front();
    int size() const;
    bool fromString(std::string string);
    std::string toString() const;
    bool operator == (const step_t& rValueStep) const;
};


/**
* @brief ����� ����������� ������ ��������� �����
*/
class game_field_t
{
private:
    const int W = constants::board::whiteCheck; // ��� ����� ������
    const int B = constants::board::blackCheck; // ��� ������ ������
    int massive[8][8] = // ���������� ������ - ������ �����
    {
       {W, B, W, B, W, B, W, B},
       {B, W, B, W, B, W, B, W},
       {W, B, W, B, W, B, W, B},
       {B, W, B, W, B, W, B, W},
       {W, B, W, B, W, B, W, B},
       {B, W, B, W, B, W, B, W},
       {W, B, W, B, W, B, W, B},
       {B, W, B, W, B, W, B, W}
    };
    int error; // ��� ������ ������� � �������
public:
    game_field_t() : error(-1)
    {}
    // �������� ������� � ������ 
    int& operator[](position_t pos);
};
