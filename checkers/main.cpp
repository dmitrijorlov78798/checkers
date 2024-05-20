// ���� �������� main, ����� �������(��������), � ����� ����� ����

#include "graphics.h"
#include "checkers.h"


/**
 * @brief ����������� ����� ������  
 */
// TODO ������� � ��������� ����
class gamer_t
{
public:
    /**
    * @brief ����������� ����� - ������� ���
    * 
    * @param possibleSteps -- ������ ��������� �����
    * @param step -- ��������� ���
    */
    virtual void makeStep(std::list<step_t> possibleSteps, step_t& step) = 0;
    /**
    * @brief ����������� ����������
    */
    virtual ~gamer_t()
    {}
};
/**
 * @brief ����� ����� - �������
 */
 // TODO ������� � ��������� ����
class human_t : public gamer_t
{
private:
    graphics_t& graphics; // ������ �� ����� ����������� ��������� � �������������
public:
    /**
    * @brief �����������
    * 
    * @param graphics -- ����� ���������� � �������������� (����� ��� ����)
    */
    human_t(graphics_t& graphics) : graphics(graphics)
    {}
    /**
    * @brief ����� - ������� ���
    *
    * @param possibleSteps -- ������ ��������� �����
    * @param step -- ��������� ���
    */
    void makeStep(std::list<step_t> possibleSteps, step_t& step) override
    {
        while (true) // ���� �� ������� �������� ����� �� ������
        {
            step_t tempStep;
            graphics.updateStep(tempStep);
            for (auto& it : possibleSteps)
                if (tempStep == it) // �������� �� ����������
                {
                    step = tempStep;
                    return;
                }
        }
    }
};
/**
 * @brief ����� ����� - �� (���� �� ����� � ����� ������, �� ����� ����������� ��������� �������� �������)
 */
 // TODO ������� � ��������� ����
class AI_t : public gamer_t
{
private:
    game_field_t& gameField; // ����� ����������� ������� ����
    /**
    * @brief ����� - ������� ��������� ���
    *
    * @param possibleSteps -- ������ ��������� �����
    * @param step -- ��������� ���
    */
    void makeRandomStep(std::list<step_t> possibleSteps, step_t& step)
    {
        step = possibleSteps.front();
        size_t i = 0, randomValue = rand() % possibleSteps.size() - 1;
        for (auto& it : possibleSteps)
            if (randomValue == i++) step = it;
    }
public:
    /**
    * @brief �����������
    *
    * @param field -- ����� ����������� ������� ���� (����� ��� ����)
    */
    AI_t(game_field_t& field) : gameField(field)
    {
        srand(time(NULL));;
    }
    /**
    * @brief ����� - ������� ���
    *
    * @param possibleSteps -- ������ ��������� �����
    * @param step -- ��������� ���
    */
    void makeStep(std::list<step_t> possibleSteps, step_t& step) override
    {// ���� ����� ������
        makeRandomStep(possibleSteps, step);
    }
};

/**
 * @brief ����� ����������� �������� ������ ����
 */
class game_checkers_t
{
private:
    tipe_gamer gamer; // ��� ����
    gamer_t* p_gamerWhite, * p_gamerBlack; // ��� ������
    game_field_t gameField; // ������� ����
    graphics_t graphics; // ��������� � �������
    step_t stepWhite, stepBlack; // ������� ���� �������
    std::list<checker_t> l_checkersWhite, l_checkersBlack; // ������ ����� �������
    std::list<step_t> l_possibleStepWhite, l_possibleStepBlack; // ������ ��������� ����� �������
    bool end_game, b_stepBlack; // ����� - ����� ����, ��� ������0
    /**
    * @brief ����� �������� ������
    */
    void game_cicle()
    {
        if (!l_checkersWhite.empty() && !l_checkersBlack.empty() &&
            makePossibleStep(b_stepBlack ? l_possibleStepBlack : l_possibleStepWhite))
        { // ���������, �������� �� � ������� ����� � ���� �� � ��� ��������� ����
            gameLogic(); // ��������� ������ ����
            graphics.updateMask(b_stepBlack ? stepBlack : stepWhite, b_stepBlack); // ���������� ����������
            graphics.updateScreen(3); 
            b_stepBlack = !b_stepBlack; // �������� ��� ������� ������
        }
        else // ���� ������� �� �����������
        {
            end_game = true; // ����� ����
            graphics.winner(!b_stepBlack); // ������� ����������
        }
    }

    /**
    * @brief ����� ��������� ������� ������
    */
    void gameLogic()
    {
        gamer_t* p_gamer = b_stepBlack ? p_gamerBlack : p_gamerWhite; // ��� ���?
        p_gamer->makeStep(b_stepBlack ? l_possibleStepBlack : l_possibleStepWhite,
            b_stepBlack ? stepBlack : stepWhite); // ����� �������� ���
        steping(b_stepBlack ? l_checkersBlack : l_checkersWhite,
            b_stepBlack ? stepBlack : stepWhite); // �����
        deleteChecker(!b_stepBlack ? l_checkersBlack : l_checkersWhite); // ������� ��������� �����
    }
    /**
    * @brief ����� �������� ������ ��������� �����
    * 
    * @param possibleStep -- ������ ��������� �����
    * 
    * @return true -- ������ ��������� ����� ������
    *         false -- ��� ��������� �����
    */
    bool makePossibleStep(std::list<step_t>& possibleStep)
    {
        int sizeStep = 0; // ������ ����
        bool attack = false; // ���� ����������� �����
        std::list<step_t> l_possibleStep_temp; 
        std::list<checker_t> l_chekers_temp;
        possibleStep.clear(); // ���������� ������ ��������� �����
        for (auto& it : b_stepBlack ? l_checkersBlack : l_checkersWhite)
        { // ��� ������ ����� �� ������ ���� ����� ������
            bool checkAttack = false; // ���� �������� �� ����� 
            l_possibleStep_temp = it.getPossibleStep(checkAttack); // ���������� ��������� ���� ������ ����� 
            if (!attack && checkAttack) // ���� ������� ���������� ��� � ������
            {// ���������� ������ ��������
                possibleStep.clear();
                attack |= checkAttack; 
            }
            if (l_possibleStep_temp.empty())
                continue; // ���� ������ ����, ������� �����
            if (attack && !checkAttack)
                continue; // ���� ��� ������������ ���� � ������, � � ������ ����� ������� ����� �� ���, ������� �����
            if (l_possibleStep_temp.front().size() < sizeStep)
                continue; // ���� � ���������� ������ �����, ���-�� ������ ����� ���������� ������ ��� � �������, ������� �����
            else if (l_possibleStep_temp.front().size() > sizeStep)
            { // �����, ���� ���-�� ������ ����� ���������� � ������� ������ ������, �������� ������� ������
                sizeStep = l_possibleStep_temp.front().size();
                possibleStep.clear();
            }
            while (!l_possibleStep_temp.empty())
            { // ���� ��� �������� ������, ���������� ������ ������� ����� � ����� ������ ��������� �����
                possibleStep.push_front(l_possibleStep_temp.front());
                l_possibleStep_temp.pop_front();
            }
        }
        return !possibleStep.empty(); // ���� ���� ��� �� ��������, �� ������ true
    }
    /**
    * @brief ����� ���������� ����
    * 
    * @param l_checker -- ������ �����
    * @param step -- ���, ������� ����� ���������
    */
    void steping(std::list<checker_t >& l_checker, step_t step)
    {// ���� �� ������, ���� ������� ����� � � ���� ���������, �����
        for (auto& it : l_checker) if (it.checkPosition(step.first())) it.moveChecker(step);
    }
    /**
    * @brief ����� �������� ������ �����
    *
    * @param l_checker -- ������ �����
    */
    void deleteChecker(std::list<checker_t>& l_checker)
    {// ���� �� ������ �����
        std::list<checker_t>::iterator l_iter = l_checker.begin();
        for (; l_iter != l_checker.end();)
            if (l_iter->getKill()) // ���� ����� �������� ��� ��������(�����)
                l_iter = l_checker.erase(l_iter); // �������
            else ++l_iter;

    }
public:
    /**
    * @brief ����������� 
    */
    game_checkers_t() : graphics(gameField), end_game(false), b_stepBlack(false), p_gamerBlack(NULL), p_gamerWhite(NULL)
    {
        graphics.updateTipegamer(gamer); // ������ ���� ���� 
        switch (gamer) // � ����������� �� ���� ���� ������� ��������� ���������� ������������ ������
        { 
        case tipe_gamer::HvsH: // ������� ������ ��������
            p_gamerWhite = new human_t(graphics); 
            p_gamerBlack = new human_t(graphics);
            break;
        case tipe_gamer::HvsAI: // ������� ������ ��
            p_gamerWhite = new human_t(graphics);
            p_gamerBlack = new AI_t(gameField);
            break;
        case tipe_gamer::AIvsAI: // �� ������ ��
            p_gamerWhite = new AI_t(gameField);
            p_gamerBlack = new AI_t(gameField);
            break;
        default:
            break;
        }
        size_t count = 12; // ������� ����� �����, 12 ����
        for (position_t it = position_t::begin(); it != position_t::end() && count > 0; ++it)
            if (gameField[it] == 0)
            {
                l_checkersWhite.push_front(checker_t(gameField, it, false));
                --count;
            }

        count = 12; // ������� ������ �����, 12 ����
        for (position_t it = position_t::rbegin(); it != position_t::rend() && count > 0; --it)
            if (gameField[it] == 0)
            {
                l_checkersBlack.push_front(checker_t(gameField, it, true));
                --count;
            }
        graphics.updateScreen(0); // ��������� ���������
    }
    /**
    * @brief ����������
    */
    ~game_checkers_t()
    {
        l_checkersBlack.clear();
        l_checkersWhite.clear();
        l_possibleStepWhite.clear();
        l_possibleStepBlack.clear();
        delete p_gamerWhite;
        delete p_gamerBlack;
    }
    /**
    * @brief ����� - ������ �������� �����
    */
    void playing()
    {
        while (!end_game) game_cicle();
    }
};


int main()
{
    game_checkers_t game;
    game.playing();
    return 0;
}