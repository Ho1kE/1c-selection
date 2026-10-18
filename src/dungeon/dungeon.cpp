#include "dungeon.hpp"

#include <utility>
#include <queue>
#include <limits>

using Tunnel = Dungeon::Tunnel;
using Hall = Dungeon::Hall;

Tunnel::Tunnel(const size_t destination_index,
               const double alive_probability) noexcept
    : destination_hall_index_(destination_index),
      alive_probability_(alive_probability),
      status_(TunnelStatus::UNKNOWN) {
}

size_t Tunnel::DestinationHallIndex() const noexcept {
  return destination_hall_index_;
}

double Tunnel::AliveProbability() const noexcept {
  return alive_probability_;
}

bool Tunnel::IsUnknown() const noexcept {
  return status_ == TunnelStatus::UNKNOWN;
}

bool Tunnel::IsBroken() const noexcept {
  return status_ == TunnelStatus::BROKEN;
}

bool Tunnel::IsOpened() const noexcept {
  return status_ == TunnelStatus::OPENED;
}

void Tunnel::SetStatus(const TunnelStatus status) {
  if ((IsOpened() && status == TunnelStatus::BROKEN) ||
      status == TunnelStatus::UNKNOWN) {
    throw std::runtime_error{"Changind already known status"};
  }
  status_ = status;
}

Hall::Hall(const double profit, std::vector<Tunnel> tunnels)
    : profit_(profit),
      tunnels_(std::move(tunnels)) {
  if (profit_ < 0) {
    throw std::runtime_error{"Wrong profit"};
  }
}

double Hall::Profit() const noexcept {
  return profit_;
}

std::vector<Tunnel>& Hall::Tunnels() noexcept {
  return tunnels_;
}

const std::vector<Tunnel>& Hall::Tunnels() const noexcept {
  return tunnels_;
}

Dungeon::Dungeon(std::vector<Hall> halls, const double day_cost)
    : halls_(std::move(halls)),
      day_cost_(day_cost) {
  if (day_cost_ < 0) {
    throw std::runtime_error{"Wrong day cost"};
  }
  UpdateExploredZone();
}

void Dungeon::ReceiveLookAnswer(const std::vector<TunnelStatus>& statuses) {
  if (!waiting_for_answer_) {
    throw std::runtime_error{"Expected request for action, got statuses"};
  }
  waiting_for_answer_ = false;

  auto& tunnels = halls_[last_looked_hall_].Tunnels();
  if (statuses.size() != tunnels.size()) {
    throw std::runtime_error{"Wrong statuses size"};
  }

  for (size_t tunnel_index = 0; tunnel_index < statuses.size();
       ++tunnel_index) {
    tunnels[tunnel_index].SetStatus(statuses[tunnel_index]);
  }

  UpdateExploredZone();
}

double Dungeon::TotalProfit() const noexcept {
  double result = 0;
  for (const size_t hall_index : explored_zone_) {
    result += halls_[hall_index].Profit();
  }
  return result - halls_cleared_ * day_cost_;
}

Dungeon::Command Dungeon::NextAction() {
  if (waiting_for_answer_) {
    throw std::runtime_error{"Expected answer before next action"};
  }

  for (const size_t hall_index : explored_zone_) {
    for (const auto& tunnel : halls_[hall_index].Tunnels()) {
      if (!tunnel.IsUnknown()) {
        continue;
      }
      waiting_for_answer_ = true;
      last_looked_hall_ = hall_index;
      return {Action::LOOK, hall_index, 0};
    }
  }

  double max_profit = 0.0;
  size_t best_hall_index = 0;
  size_t best_tunnel_index = 0;
  bool selected_tunnel = false;

  for (const size_t hall_index : explored_zone_) {
    for (size_t tunnel_index = 0;
         tunnel_index < halls_[hall_index].Tunnels().size(); ++tunnel_index) {
      const auto& tunnel = halls_[hall_index].Tunnels()[tunnel_index];

      if (!tunnel.IsBroken() ||
          explored_zone_.contains(tunnel.DestinationHallIndex())) {
        continue;
      }

      const double profit = CalculateProfit(tunnel.DestinationHallIndex());

      if (profit <= max_profit) {
        continue;
      }
      max_profit = profit;
      best_hall_index = hall_index;
      best_tunnel_index = tunnel_index;
      selected_tunnel = true;
    }
  }

  if (selected_tunnel && max_profit > 0.0) {
    halls_[best_hall_index].Tunnels()[best_tunnel_index].SetStatus(
        TunnelStatus::OPENED);
    UpdateExploredZone();
    ++halls_cleared_;
    return {Action::CLEAR, best_hall_index, best_tunnel_index};
  }

  return {Action::END, 0, 0};
}

double Dungeon::CalculateProfit(const size_t start_hall_index) {
  constexpr double kInfinity = std::numeric_limits<double>::infinity();

  std::vector<double> min_cost(halls_.size(), kInfinity);
  std::priority_queue<DijkstraNode, std::vector<DijkstraNode>,
                      std::greater<DijkstraNode>>
      dijkstra_heap;

  min_cost[start_hall_index] = 0.0;
  dijkstra_heap.push({0.0, start_hall_index});

  while (!dijkstra_heap.empty()) {
    const auto [current_cost, hall_index] = dijkstra_heap.top();
    dijkstra_heap.pop();

    constexpr double kEpsilon = 1e-7;
    if (current_cost > min_cost[hall_index] + kEpsilon) {
      continue;
    }

    for (const auto& tunnel : halls_[hall_index].Tunnels()) {
      const size_t neighbour_index = tunnel.DestinationHallIndex();
      if (explored_zone_.contains(neighbour_index)) {
        continue;
      }

      const double edge_cost = (1.0 - tunnel.AliveProbability()) * day_cost_;
      const double neighbour_cost = current_cost + edge_cost;

      if (neighbour_cost < min_cost[neighbour_index]) {
        min_cost[neighbour_index] = neighbour_cost;
        dijkstra_heap.push({neighbour_cost, neighbour_index});
      }
    }
  }

  double total_profit = 0.0;
  for (size_t hall_index = 0; hall_index < halls_.size(); ++hall_index) {
    if (!explored_zone_.contains(hall_index) &&
        min_cost[hall_index] != kInfinity) {
      const double hall_profit =
          halls_[hall_index].Profit() - min_cost[hall_index];

      if (hall_profit > 0.0) {
        total_profit += hall_profit;
      }
    }
  }

  return total_profit - day_cost_;
}

void Dungeon::UpdateExploredZone() {
  explored_zone_.clear();
  std::vector<size_t> queue;
  queue.push_back(0);
  explored_zone_.insert(0);

  size_t index_in_queue = 0;
  while (index_in_queue < queue.size()) {
    size_t vertex_index = queue[index_in_queue++];
    for (const auto& tunnel : halls_[vertex_index].Tunnels()) {
      if (tunnel.IsOpened() &&
          !explored_zone_.contains(tunnel.DestinationHallIndex())) {
        explored_zone_.insert(tunnel.DestinationHallIndex());
        queue.push_back(tunnel.DestinationHallIndex());
      }
    }
  }
}

bool Dungeon::DijkstraNode::operator<(
    const DijkstraNode& other) const noexcept {
  return expected_cost < other.expected_cost;
}

bool Dungeon::DijkstraNode::operator>(
    const DijkstraNode& other) const noexcept {
  return (other < *this);
}