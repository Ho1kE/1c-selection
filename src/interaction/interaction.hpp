#pragma once

#include <dungeon/dungeon.hpp>

#include <iostream>

Dungeon ReadDungeon(std::istream& input_stream, std::ostream& output_stream);

void RunInteraction(Dungeon& dungeon, std::istream& input_stream,
                    std::ostream& output_stream);