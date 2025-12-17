#include "knight.h"
#include "elf.h"
#include "dragon.h"

Knight::Knight(int x, int y, const std::string& name) : 
    NPC(KnightType, x, y, name) {}
Knight::Knight(std::istream &is) : NPC(KnightType, is) {}

void Knight::print()
{
    std::cout << *this;
}

void Knight::save(std::ostream &os)
{
    os << std::endl << KnightType << std::endl;
    NPC::save(os);
}

bool Knight::accept(std::shared_ptr<NPC> visitor)
{
    return visitor->fight(std::dynamic_pointer_cast<Knight>(shared_from_this()));
}

bool Knight::fight(std::shared_ptr<Dragon> other)
{
    fight_notify(other, true);
    return true;
}

bool Knight::fight(std::shared_ptr<Elf> other)
{
    fight_notify(other, false);
    return false;
}

bool Knight::fight(std::shared_ptr<Knight> other)
{
    fight_notify(other, false);
    return false;
}

std::ostream &operator<<(std::ostream &os, Knight &knight)
{
    os << "Странствующий рыцарь '" << knight.name << "': " 
       << *static_cast<NPC *>(&knight) << std::endl;
    return os;
}