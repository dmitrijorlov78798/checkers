#pragma once

#include "positionANDgame_field.h"

/**
 * @brief ����� ����������� ��������� � ������� (����� �������)
 */
// TODO ����������� ����������� ���������. 
class graphics_t
{
private:
    HANDLE h_console;// �������
    COORD h_anchor;// ������� ���������� �� �������
    int countLog;// ������� �����
    game_field_t last_frame;// ��������� �������� ���� �� ������� ����
    game_field_t mask;//�������� ����� ��� ��������� ����� �����
    game_field_t& current_frame;// ������ �� ������� ���� (����� ��� ����)
    // ����� ���������� �������
    void draw(int i_pinctogram, COORD cursore, int color);
    // ��������� ����
    void drawField();
public:
    graphics_t(game_field_t& fldtmp);
    // �������� ����������� �� ������� (� ������� ����� sec - ������, ��� �������� ���������� �������� ��������)
    void updateScreen(int sec);
    // ������� ��� ����������
    void winner(bool black);
    // ������ ���� �� ������
    void updateStep(step_t& step);
    // �������� �������� �����
    void updateMask(step_t step, bool stepBlack);
    // ������ ���� ���� �� ������
    void updateTipegamer(tipe_gamer& gamer);
    // ����� � ������� �����
    void doLog(const char* sz_log);
};
