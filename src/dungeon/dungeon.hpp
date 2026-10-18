#pragma once

#include <cstddef>
#include <vector>
#include <unordered_set>

class Dungeon {
 public:
  enum class Action { LOOK, CLEAR, END };

  enum class TunnelStatus {
    UNKNOWN,
    OPENED,
    BROKEN,
  };

  struct Command {
    Action action;
    size_t hall_index = 0;
    size_t tunnel_index = 0;
  };

  class Tunnel {
   public:
    Tunnel(size_t destination_index, double alive_probability) noexcept;

    size_t DestinationHallIndex() const noexcept;

    double AliveProbability() const noexcept;

    bool IsUnknown() const noexcept;

    bool IsBroken() const noexcept;

    bool IsOpened() const noexcept;

    void SetStatus(TunnelStatus status);

   private:
    size_t destination_hall_index_;
    double alive_probability_;
    TunnelStatus status_;
  };

  class Hall {
   public:
    Hall(double profit, std::vector<Tunnel> tunnels);

    double Profit() const noexcept;

    std::vector<Tunnel>& Tunnels() noexcept;

    const std::vector<Tunnel>& Tunnels() const noexcept;

   private:
    double profit_;
    std::vector<Tunnel> tunnels_;
  };

 public:
  Dungeon(std::vector<Hall> halls, double day_cost);

  void ReceiveLookAnswer(const std::vector<TunnelStatus>& statuses);

  double TotalProfit() const noexcept;

  Command NextAction();

 private:
  struct DijkstraNode {
    double expected_cost;
    size_t hall_index;

    bool operator<(const DijkstraNode& other) const noexcept;
    bool operator>(const DijkstraNode& other) const noexcept;
  };

  double CalculateProfit(size_t start_hall_index);

  void UpdateExploredZone();

 private:
  const double day_cost_;
  bool waiting_for_answer_ = false;
  size_t last_looked_hall_ = 0;
  size_t halls_cleared_ = 0;

  std::unordered_set<size_t> explored_zone_;
  std::vector<Hall> halls_;
};
