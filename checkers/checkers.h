#pragma once

#include "positionANDgame_field.h"

/**
 * @brief ����� ����������� ������ �����
 */
class checker_t
{
private:
    game_field_t& game_field; // ������ �� ������� ����
    position_t position; // ������� ������� �����
    bool b_black; // ���� - ����� ������
    bool b_queen; // ���� - ����� �����
    
    /**
    * @brief ����� - ������� ����� ������
    */
    void setQueen(); 
    /**
    * @brief ����� �������� ������� (������������)  
    *
    * @param position -- ������� ��� ��������
    * 
    * @return ��� ������� ����������� ������� ������� ���� 0, ���� ������� �����   
    */
    int checkCollision(position_t position) const;
    /**
    * @brief ����� �������� � ���������� ������ ��������� ���� ������ �����
    * (����������� �������� ����� � �������)
    *
    * @param l_PossibleStep -- ������ ���� ��� ����������
    *
    * @return true -- ��������� ����� �������
    *         false -- ����� �� �������
    */
    bool checkAttack(std::list<step_t>& l_PossibleStep) const;
    /**
    * @brief ����������� ����� ������ ��������� ����� (��� ���������� ������ � �������)
    *
    * @param PossibleStep -- map ��������� ����� ��� ������ (���� - ���������� ������ ����� ����������)
    * @param step -- ��� ����������� ������������ ������
    * @param direction -- ����������� ���� ����������� ������������ ������
    *
    * @return true --  ����� ��� ���������� ������������ ������ �������
    *         false -- ����� �� �������, ������ ����������� ����� ���������
    */
    bool checkAttackRecursion(std::map<int, std::list<step_t>>& PossibleStep,
        step_t step, tipe_direction direction) const;
    /**
    * @brief ����� - ��������� ������� ������� ����� � ������� ����
    */
    void savePosition();
    /**
    * @brief ����� - ����� ����� � ������ �������
    * 
    * @param position -- ������� �� ������� ���������� ����� ����� ����������
    */
    void killChecker(position_t position) const;
public:
    /**
    * @brief �����������
    * 
    * @param game_field -- ������ �� ������� ���� (����� ��� ���� �����)
    * @param position -- �������, �� ������� ��������� �����
    * @param b_black -- ���� 1, ����� ������
    */
    checker_t(game_field_t& game_field, position_t position, bool b_black);
    /**
    * @brief ����� - ����������� �����
    *
    * @param position -- ������� �� ������� ���������� ����������� �����
    */
    void moveChecker(step_t step);
    /**
    * @brief ����� �������� ����� ������ �����
    *
    * @return true -- ����� �����
    *         false -- ����� ����
    */
    bool getKill() const;
    /**
    * @brief ����� �������� ������� ����� ������� 
    *
    * @param position -- ������� ��� ��������
    * 
    * @return true -- ����� ��������� �� ������ �������
    *         false -- ����� ��� �� ������ �������
    */
    bool checkPosition(position_t position) const;
    /**
    * @brief ����� �������� � ���������� ������ ��������� ����� ������ �����
    *
    * @param b_checkAttak -- ���� ��� ������ - ���� �� ������� ����������� ��� �����
    *
    * @return ������ ��������� �����
    */
    std::list<step_t> getPossibleStep(bool& b_checkAttak) const;
};