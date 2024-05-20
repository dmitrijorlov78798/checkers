// файл содержит main, класс игроков(временно), и класс самой игры

#include "graphics.h"
#include "checkers.h"


/**
 * @brief абстрактный класс игрока  
 */
// TODO вынести в отдельный файл
class gamer_t
{
public:
    /**
    * @brief виртуальный метод - сделать ход
    * 
    * @param possibleSteps -- список возможных ходов
    * @param step -- выбранный ход
    */
    virtual void makeStep(std::list<step_t> possibleSteps, step_t& step) = 0;
    /**
    * @brief виртуальный деструктор
    */
    virtual ~gamer_t()
    {}
};
/**
 * @brief класс игрок - человек
 */
 // TODO вынести в отдельный файл
class human_t : public gamer_t
{
private:
    graphics_t& graphics; // ссылка на класс реализующий интерфейс с пользователем
public:
    /**
    * @brief конструктор
    * 
    * @param graphics -- класс интерфейса с полдьзователем (общий для всех)
    */
    human_t(graphics_t& graphics) : graphics(graphics)
    {}
    /**
    * @brief метод - сделать ход
    *
    * @param possibleSteps -- список возможных ходов
    * @param step -- выбранный ход
    */
    void makeStep(std::list<step_t> possibleSteps, step_t& step) override
    {
        while (true) // пока не получим валидный ответ от игрока
        {
            step_t tempStep;
            graphics.updateStep(tempStep);
            for (auto& it : possibleSteps)
                if (tempStep == it) // проверка на валидность
                {
                    step = tempStep;
                    return;
                }
        }
    }
};
/**
 * @brief класс игрок - ИИ (пока ии дурак и ходит наугад, но будут реализованы алгоритмы принятия решений)
 */
 // TODO вынести в отдельный файл
class AI_t : public gamer_t
{
private:
    game_field_t& gameField; // класс реализцющий игровое поле
    /**
    * @brief метод - сделать случайный ход
    *
    * @param possibleSteps -- список возможных ходов
    * @param step -- выбранный ход
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
    * @brief конструктор
    *
    * @param field -- класс реализцющий игровое поле (общее для всех)
    */
    AI_t(game_field_t& field) : gameField(field)
    {
        srand(time(NULL));;
    }
    /**
    * @brief метод - сделать ход
    *
    * @param possibleSteps -- список возможных ходов
    * @param step -- выбранный ход
    */
    void makeStep(std::list<step_t> possibleSteps, step_t& step) override
    {// пока ходим наугад
        makeRandomStep(possibleSteps, step);
    }
};

/**
 * @brief класс реализующий основную логику игры
 */
class game_checkers_t
{
private:
    tipe_gamer gamer; // тип игры
    gamer_t* p_gamerWhite, * p_gamerBlack; // два игрока
    game_field_t gameField; // игровое поле
    graphics_t graphics; // интерфейс с игроком
    step_t stepWhite, stepBlack; // текущие шаги игроков
    std::list<checker_t> l_checkersWhite, l_checkersBlack; // список шашок игроков
    std::list<step_t> l_possibleStepWhite, l_possibleStepBlack; // список возможных ходов игроков
    bool end_game, b_stepBlack; // флаги - конец игры, ход черных0
    /**
    * @brief метод основной работы
    */
    void game_cicle()
    {
        if (!l_checkersWhite.empty() && !l_checkersBlack.empty() &&
            makePossibleStep(b_stepBlack ? l_possibleStepBlack : l_possibleStepWhite))
        { // проверяем, остались ли у игроков шашки и есть ли у них доступные шаги
            gameLogic(); // запускаем логику игры
            graphics.updateMask(b_stepBlack ? stepBlack : stepWhite, b_stepBlack); // обновления интерфейса
            graphics.updateScreen(3); 
            b_stepBlack = !b_stepBlack; // передаем ход другому игроку
        }
        else // если условия не выполнились
        {
            end_game = true; // конец игры
            graphics.winner(!b_stepBlack); // выводим победителя
        }
    }

    /**
    * @brief метод отработки игровой логики
    */
    void gameLogic()
    {
        gamer_t* p_gamer = b_stepBlack ? p_gamerBlack : p_gamerWhite; // чей ход?
        p_gamer->makeStep(b_stepBlack ? l_possibleStepBlack : l_possibleStepWhite,
            b_stepBlack ? stepBlack : stepWhite); // игрок выбирает ход
        steping(b_stepBlack ? l_checkersBlack : l_checkersWhite,
            b_stepBlack ? stepBlack : stepWhite); // ходим
        deleteChecker(!b_stepBlack ? l_checkersBlack : l_checkersWhite); // удаляем съеденные шашки
    }
    /**
    * @brief метод создания списка возможных ходов
    * 
    * @param possibleStep -- список возможных ходов
    * 
    * @return true -- список возможных шагов создан
    *         false -- нет возможных шагов
    */
    bool makePossibleStep(std::list<step_t>& possibleStep)
    {
        int sizeStep = 0; // размер шага
        bool attack = false; // флаг возможности атаки
        std::list<step_t> l_possibleStep_temp; 
        std::list<checker_t> l_chekers_temp;
        possibleStep.clear(); // подготовка списка возможных шагов
        for (auto& it : b_stepBlack ? l_checkersBlack : l_checkersWhite)
        { // для каждой шашки из списка всех шашок игрока
            bool checkAttack = false; // флаг проверки на атаку 
            l_possibleStep_temp = it.getPossibleStep(checkAttack); // возвращаем возможные ходы данной шашки 
            if (!attack && checkAttack) // если впервые встретился шаг с атакой
            {// предыдущий список затираем
                possibleStep.clear();
                attack |= checkAttack; 
            }
            if (l_possibleStep_temp.empty())
                continue; // если список пуст, пропуск цикла
            if (attack && !checkAttack)
                continue; // если уже присутствуют шаги с атакой, а в списке шагов текущей шашки из нет, пропуск цикла
            if (l_possibleStep_temp.front().size() < sizeStep)
                continue; // если в предыдущем списке шагов, кол-во взятых шашек противника больше чем в текущем, пропуск цикла
            else if (l_possibleStep_temp.front().size() > sizeStep)
            { // иначе, если кол-во взятых шашек противника в текущем списке больше, затираем прошлый список
                sizeStep = l_possibleStep_temp.front().size();
                possibleStep.clear();
            }
            while (!l_possibleStep_temp.empty())
            { // если все проверки прошли, записываем список текущей шашки в общий список возможных ходов
                possibleStep.push_front(l_possibleStep_temp.front());
                l_possibleStep_temp.pop_front();
            }
        }
        return !possibleStep.empty(); // если хоть что то записали, то вернем true
    }
    /**
    * @brief метод совершения хода
    * 
    * @param l_checker -- список шашек
    * @param step -- шаг, который нужно выполнить
    */
    void steping(std::list<checker_t >& l_checker, step_t step)
    {// идем по списку, если позиция шашки и в шаге совпадают, ходим
        for (auto& it : l_checker) if (it.checkPosition(step.first())) it.moveChecker(step);
    }
    /**
    * @brief метод удаления взятой шашки
    *
    * @param l_checker -- список шашек
    */
    void deleteChecker(std::list<checker_t>& l_checker)
    {// идем по списку шашек
        std::list<checker_t>::iterator l_iter = l_checker.begin();
        for (; l_iter != l_checker.end();)
            if (l_iter->getKill()) // если шашка помечена как съеденая(убита)
                l_iter = l_checker.erase(l_iter); // удаляем
            else ++l_iter;

    }
public:
    /**
    * @brief конструктор 
    */
    game_checkers_t() : graphics(gameField), end_game(false), b_stepBlack(false), p_gamerBlack(NULL), p_gamerWhite(NULL)
    {
        graphics.updateTipegamer(gamer); // запрос типа игры 
        switch (gamer) // в зависимотси от типа игры создаем различные реализации абстрактного игрока
        { 
        case tipe_gamer::HvsH: // человек против человека
            p_gamerWhite = new human_t(graphics); 
            p_gamerBlack = new human_t(graphics);
            break;
        case tipe_gamer::HvsAI: // человек против ИИ
            p_gamerWhite = new human_t(graphics);
            p_gamerBlack = new AI_t(gameField);
            break;
        case tipe_gamer::AIvsAI: // ИИ против ИИ
            p_gamerWhite = new AI_t(gameField);
            p_gamerBlack = new AI_t(gameField);
            break;
        default:
            break;
        }
        size_t count = 12; // создаем белые шашки, 12 штук
        for (position_t it = position_t::begin(); it != position_t::end() && count > 0; ++it)
            if (gameField[it] == 0)
            {
                l_checkersWhite.push_front(checker_t(gameField, it, false));
                --count;
            }

        count = 12; // создаем черные шашки, 12 штук
        for (position_t it = position_t::rbegin(); it != position_t::rend() && count > 0; --it)
            if (gameField[it] == 0)
            {
                l_checkersBlack.push_front(checker_t(gameField, it, true));
                --count;
            }
        graphics.updateScreen(0); // обновляем интерфейс
    }
    /**
    * @brief деструктор
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
    * @brief метод - запуск игрового цикла
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