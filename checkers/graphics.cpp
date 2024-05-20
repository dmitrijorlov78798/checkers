#include "graphics.h"

void graphics_t::draw(int i_pinctogram, COORD cursore, int color)
{
    if (color) SetConsoleTextAttribute(h_console, color); // ��������� �����
    char c_pinctogram;
    switch (i_pinctogram)
    {// � ����������� �� ����, ����� �������
    case constants::board::whiteCheck:
        c_pinctogram = ' ';
        break;
    case constants::board::blackCheck:
        c_pinctogram = '*';
        break;
    case constants::checker::white:
        c_pinctogram = 'w';
        break;
    case constants::checker::black:
        c_pinctogram = 'b';
        break;
    case constants::checker::whiteQueen:
        c_pinctogram = 'W';
        break;
    case constants::checker::blackQueen:
        c_pinctogram = 'B';
        break;
    default:
        c_pinctogram = 'E';
        break;
    }
    cursore.X += h_anchor.X; // � ����������� ��������� �������� ������� �����
    cursore.Y += h_anchor.Y;
    SetConsoleCursorPosition(h_console, cursore);
    putchar(c_pinctogram);
    if (color) SetConsoleTextAttribute(h_console, 15);
}

void graphics_t::drawField()
{
    COORD cursor;
    cursor.X = h_anchor.X - 1;
    SetConsoleTextAttribute(h_console, 14);
    for (int y = 8; y > 0; y--) // ������ �������� ���������
    {

        cursor.Y = h_anchor.Y + y - 1;
        SetConsoleCursorPosition(h_console, cursor);
        putchar(9 - y + (int)'0');
    }
    cursor.Y = h_anchor.Y + 8;
    for (int x = 0; x < 8; x++) // ������ ��������� �����������
    {
        cursor.X = h_anchor.X + x;
        SetConsoleCursorPosition(h_console, cursor);
        putchar((char)x + 'a');
    }
    SetConsoleTextAttribute(h_console, 15);
    for (position_t it = position_t::begin(); it < position_t::end(); ++it) // ����� �������� ����
        draw(current_frame[it], it, 0);
}

graphics_t::graphics_t(game_field_t& fldtmp) : current_frame(fldtmp)
{
    h_console = GetStdHandle(-11);
    h_anchor.X = 3;
    h_anchor.Y = 0;
    drawField();
}

void graphics_t::updateScreen(int sec)
{
    game_field_t tempFrame = last_frame;
    // ���� �� ����� ����, ���� ���������, ������������ �� ��������������� ������ � ������� � �������
    for (position_t it = position_t::begin(); it < position_t::end(); ++it) 
        if (last_frame[it] != current_frame[it]
            || mask[it] > constants::mask::null)
        {
            last_frame[it] = current_frame[it];
            draw(current_frame[it], it, mask[it]);
        }

    h_anchor.X += 12;
    for (position_t it = position_t::begin(); it < position_t::end(); ++it) // ����� ����������� ����, ��� �������
        draw(tempFrame[it], it, mask[it]);
    h_anchor.X -= 12;
    // �����
    Sleep(sec * 1000);
}

void graphics_t::winner(bool black)
{
    COORD cursor;
    cursor.X = h_anchor.X;
    cursor.Y = h_anchor.Y + 10;
    SetConsoleCursorPosition(h_console, cursor);
    std::cout << "                                                                               ";
    SetConsoleCursorPosition(h_console, cursor);
    if (black) std::cout << "WINNER BLACK";
    else std::cout << "WINNER WHITE";
    std::cout << std::endl << std::endl;
}

void graphics_t::updateStep(step_t& step)
{
    COORD cursor;
    cursor.X = h_anchor.X;
    cursor.Y = h_anchor.Y + 10;
    while (true) // ���� ��� ��������� ����� �� ������, ���������� ���
    {
        std::string tempString;
        SetConsoleCursorPosition(h_console, cursor);
        std::cout << "                                                                            ";
        SetConsoleCursorPosition(h_console, cursor);
        std::cout << "Your step?\"example:'a3-b4'\": "; std::cin >> tempString;
        if (step.fromString(tempString)) return;
    }
}

void graphics_t::updateMask(step_t step, bool stepBlack)
{   // ������� ������� ���������
    for (position_t it = position_t::begin(); it < position_t::end(); ++it)
        if (mask[it] != constants::mask::null)
        {
            draw(last_frame[it], it, 0);
            mask[it] = constants::mask::null;
        }

    position_t lastPos = position_t::end();
    while (!step.empty()) // ���������� ����� ����� ������� ����, ������ ���������� �� ������� ������/�����
    {
        position_t currentPos = step.pop_front();
        mask[currentPos] = stepBlack ? // ��������� ������� �����
            constants::mask::black : constants::mask::white;

        if (lastPos != position_t::end())
        {
            std::list<position_t> l_tempPos = position_t::median(lastPos, currentPos);
            while (!l_tempPos.empty())
            {
                mask[l_tempPos.front()] = stepBlack ? // ��������� �������, ����� ������� ������������� �����
                    constants::mask::black : constants::mask::white;
                l_tempPos.pop_front();
            }
        }

        lastPos = currentPos;

    }
    // �������� �� ���������� ������ ���� � ����������� �����
    for (position_t it = position_t::begin(); it != position_t::end(); ++it)
        if (last_frame[it] != current_frame[it]
            || mask[it] > constants::mask::null)
        {
            if (mask[it] == constants::mask::null)
                mask[it] = stepBlack ?
                constants::mask::black : constants::mask::white;
            if (mask[it] == constants::mask::white) // ���� �� �����, ������ ��������������� ����������  
            {
                if (last_frame[it] == constants::checker::black
                    || last_frame[it] == constants::checker::black)
                    mask[it] = constants::color::red; // ����� ����� �����
                else if (last_frame[it] == constants::checker::white
                    || last_frame[it] == constants::checker::whiteQueen)
                    mask[it] = constants::color::blue; // ������ �������, �� ���� ������ ��������
                else if (last_frame[it] == constants::board::blackCheck
                    && (current_frame[it] == constants::checker::white
                        || current_frame[it] == constants::checker::whiteQueen))
                    mask[it] = constants::color::orange; // ��������� �������, ���� �� ������
                else mask[it] = constants::color::green; // ������������� �������
            }
            if (mask[it] == constants::mask::black) // �� ��, ���� �� ������
            {
                if (last_frame[it] == constants::checker::white
                    || last_frame[it] == constants::checker::whiteQueen)
                    mask[it] = constants::color::red;
                else if (last_frame[it] == constants::checker::black
                    || last_frame[it] == constants::checker::blackQueen)
                    mask[it] = constants::color::blue;
                else if (last_frame[it] == constants::board::blackCheck
                    && (current_frame[it] == constants::checker::black
                        || current_frame[it] == constants::checker::blackQueen))
                    mask[it] = constants::color::orange;
                else mask[it] = constants::color::green;
            }
        }
}

void graphics_t::updateTipegamer(tipe_gamer& gamer)
{
    COORD cursor;
    char temp;
    bool b_stop = false;
    while (!b_stop) // ���� ��� ��������� ������ �� ������, ���������� ���
    {
        cursor.X = h_anchor.X;
        cursor.Y = h_anchor.Y + 10;
        SetConsoleCursorPosition(h_console, cursor);
        std::cout << "Your tipe game? 1 - HvsH, 2 - HvsAI, 3 - AIvsAI";
        ++cursor.Y;
        SetConsoleCursorPosition(h_console, cursor);
        std::cin >> temp;
        if (temp == '1') { gamer = tipe_gamer::HvsH; b_stop = true; }
        if (temp == '2') { gamer = tipe_gamer::HvsAI; b_stop = true; }
        if (temp == '3') { gamer = tipe_gamer::AIvsAI; b_stop = true; }
        SetConsoleCursorPosition(h_console, cursor);
        std::cout << ' ';
    }
}

void graphics_t::doLog(const char* sz_log)
{
    COORD cursor;
    cursor.X = h_anchor.X;
    cursor.Y = h_anchor.Y + 15 + countLog++;
    SetConsoleCursorPosition(h_console, cursor);
    std::cout << "log: " << sz_log;
}