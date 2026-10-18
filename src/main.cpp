#include <iostream>

#include <dungeon/dungeon.hpp>
#include <interaction/interaction.hpp>

int main() {
  Dungeon dungeon = ReadDungeon(std::cin, std::cout);
  RunInteraction(dungeon, std::cin, std::cout);
}