#include "dungeon.hpp"

#include <iostream>

int main() {
  using Hall = Dungeon::Hall;
  using Tunnel = Dungeon::Tunnel;
  using Status = Dungeon::TunnelStatus;

  Hall start(0, {
                    Tunnel{1, 0.1},
                    Tunnel{2, 0.3},
                });
  Hall h1(15, {});
  Hall h2(20, {});

  Dungeon d({start, h1, h2}, 7);
  std::cout << static_cast<size_t>(d.NextAction().action);
  d.ReceiveLookAnswer({Status::BROKEN, Status::OPENED});
  std::cout << static_cast<size_t>(d.NextAction().action);
  std::cout << std::endl << "Total profit: " << d.TotalProfit() << std::endl;
}