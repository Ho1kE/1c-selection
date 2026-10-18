#include "interaction.hpp"

#include <iostream>
#include <vector>

Dungeon ReadDungeon(std::istream& input_stream, std::ostream& output_stream) {
  size_t halls_count = 0;
  double day_cost = 0.0;

  output_stream << "Enter halls count: " << std::endl;
  input_stream >> halls_count;

  output_stream << "Enter daily workers cost: " << std::endl;
  input_stream >> day_cost;

  std::vector<Dungeon::Hall> halls;
  halls.reserve(halls_count);

  output_stream << std::endl;

  for (size_t hall_index = 0; hall_index < halls_count; ++hall_index) {
    double profit = 0.0;
    size_t tunnels_count = 0;

    output_stream << "Setting data for " << hall_index << "th hall"
                  << std::endl;

    output_stream << "Enter profit and tunnels count: " << std::endl;
    input_stream >> profit >> tunnels_count;

    if (tunnels_count > 0) {
      output_stream << "Enter tunnels: " << std::endl;
    }

    std::vector<Dungeon::Tunnel> tunnels;
    tunnels.reserve(tunnels_count);

    for (size_t tunnel_index = 0; tunnel_index < tunnels_count;
         ++tunnel_index) {
      size_t destination = 0;
      double alive_probability = 0.0;

      input_stream >> destination >> alive_probability;

      tunnels.emplace_back(destination, alive_probability);
    }

    halls.emplace_back(profit, std::move(tunnels));
  }

  return Dungeon{std::move(halls), day_cost};
}

void RunInteraction(Dungeon& dungeon, std::istream& input_stream,
                    std::ostream& output_stream) {
  using Action = Dungeon::Action;
  using Status = Dungeon::TunnelStatus;

  for (auto command = dungeon.NextAction();; command = dungeon.NextAction()) {
    switch (command.action) {
      case Action::LOOK: {
        output_stream << "Want to look at " << command.hall_index << "th hall"
                      << std::endl;
        std::vector<Status> statuses;
        std::string status;
        for (size_t i = 0; i < command.tunnel_index; ++i) {
          input_stream >> status;
          statuses.push_back(status == "OK" ? Status::OPENED : Status::BROKEN);
        }
        dungeon.ReceiveLookAnswer(statuses);
      } break;
      case Action::CLEAR: {
        output_stream << "Clearing " << command.tunnel_index
                      << "th tunnel from " << command.hall_index << "th hall"
                      << std::endl;
      } break;
      case Action::END:
        output_stream << "Total money earned: " << dungeon.TotalProfit()
                      << std::endl;
        return;
    }
  }
}