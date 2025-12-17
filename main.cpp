#include "npc.h"
#include "knight.h"
#include "elf.h"
#include "dragon.h"
#include "battleManager.h"

#include <ctime>
#include <vector>
#include <set>

class TextObserver : public IFightObserver
{
public:
    void on_fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win)
    {
        if (win) {
            attacker->print();
            std::cout << " убивает ";
            defender->print();
            std::cout << std::endl;
        }
    }
};

class FileObserver : public IFightObserver
{
private:
    std::ofstream logFile;

public:
    FileObserver(const std::string& fileName) {
        logFile.open(fileName, std::ios_base::app);
    }

    void on_fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win)
    {
        if (win && logFile.is_open()) {
            logFile << *attacker << " убивает " << *defender << std::endl;
        }
    }
};

std::shared_ptr<TextObserver> textObs = std::make_shared<TextObserver>();
std::shared_ptr<FileObserver> fileObs = std::make_shared<FileObserver>("log.txt");

std::shared_ptr<NPC> factory(std::istream &is)
{
    std::shared_ptr<NPC> result;
    int type{0};
    if (is >> type)
    {
        switch (type)
        {
            case KnightType:
                result = std::make_shared<Knight>(is);
                break;
            case ElfType:
                result = std::make_shared<Elf>(is);
                break;
            case DragonType:
                result = std::make_shared<Dragon>(is);
                break;
            default:
                break;
        }
    }

    if (result) {
        result->subscribe(textObs);
        result->subscribe(fileObs);
    }

    return result;
}

std::string generate_name(NpcType type, int id) {
    static std::vector<std::string> knightNames = {"Артемиус", "Гланцелот", "Рыдцарек"};
    static std::vector<std::string> elfNames = {"Ласлего", "Эльфиус", "Эдуард"};
    static std::vector<std::string> dragonNames = {"Бакуганиус", "Драгон", "Виверникс"};
    
    std::string name;
    if (type == KnightType) 
        name = knightNames[std::rand() % knightNames.size()] + "_" + std::to_string(id);
    else if (type == ElfType)
        name = elfNames[std::rand() % elfNames.size()] + "_" + std::to_string(id);
    else
        name = dragonNames[std::rand() % dragonNames.size()] + "_" + std::to_string(id);
    
    return name;
}

std::shared_ptr<NPC> factory(NpcType type, int x, int y, int id)
{
    std::shared_ptr<NPC> result;
    std::string name = generate_name(type, id);

    switch (type) {
        case KnightType:
            result = std::make_shared<Knight>(x, y, name);
            break;
        case ElfType:
            result = std::make_shared<Elf>(x, y, name);
            break;
        case DragonType:
            result = std::make_shared<Dragon>(x, y, name);
            break;
        default:
            break;
    }

    if (result) {
        result->subscribe(textObs);
        result->subscribe(fileObs);
    }

    return result;
}

void save(const set_t &array, const std::string &fileName)
{
    std::ofstream fs(fileName);
    fs << array.size() << std::endl;
    for (auto &n : array) {
        n->save(fs);
    }
    fs.flush();
    fs.close();
}

set_t load(const std::string &fileName)
{
    set_t result;
    std::ifstream is(fileName);
    if (is.good() && is.is_open())
    {
        int count;
        is >> count;

        for (int i = 0; i < count; ++i) {
            result.insert(factory(is));
        }

        is.close();
    }

    return result;
}

std::ostream &operator<<(std::ostream &os, const set_t &array)
{
    for (auto &n : array) {
        n->print();
    }
    return os;
}

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    set_t array;

    for (size_t i = 0; i < 10; ++i) {
        int type = std::rand() % 3 + 1;
        int x = std::rand() % 500 + 1;
        int y = std::rand() % 500 + 1;
        array.insert(factory(NpcType(type), x, y, i));
    }

    save(array, "dungeon.txt");

    std::cout << array;

    array.clear();

    array = load("dungeon.txt");

    for (size_t distance = 50; distance <= 200 && !array.empty(); distance += 50) {
        std::cout << "\nДистанция боя: " << distance << std::endl;
        
        auto dead_list = fight(array, distance);
        for (auto &d : dead_list) {
            array.erase(d);
        }
        
        std::cout << "Убито: " << dead_list.size() 
                  << ", Осталось: " << array.size() << std::endl;
    }

    std::cout << "\nВыжившие:" << std::endl;
    if (!array.empty()) {
        std::cout << array;
    } else {
        std::cout << "Нет" << std::endl;
    }
    
    return 0;
}