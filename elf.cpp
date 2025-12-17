#include "elf.h"
#include "knight.h"
#include "dragon.h"

Elf::Elf(int x, int y, const std::string& name) : 
    NPC(ElfType, x, y, name) {}
Elf::Elf(std::istream &is) : NPC(ElfType, is) {}

void Elf::print()
{
    std::cout << *this;
}

void Elf::save(std::ostream &os)
{
    os << std::endl << ElfType << std::endl;
    NPC::save(os);
}

bool Elf::accept(std::shared_ptr<NPC> visitor)
{
    return visitor->fight(std::dynamic_pointer_cast<Elf>(shared_from_this()));
}

bool Elf::fight(std::shared_ptr<Knight> other)
{
    fight_notify(other, true);
    return true;
}

bool Elf::fight(std::shared_ptr<Dragon> other)
{
    fight_notify(other, false);
    return false;
}

bool Elf::fight(std::shared_ptr<Elf> other)
{
    fight_notify(other, false);
    return false;
}

std::ostream &operator<<(std::ostream &os, Elf &elf)
{
    os << "Эльф '" << elf.name << "': " 
       << *static_cast<NPC *>(&elf) << std::endl;
    return os;
}