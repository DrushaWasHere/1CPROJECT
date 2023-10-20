#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <unordered_set>

#include <locale>
#include <iostream>


struct colon_is_space : std::ctype<char> {
  colon_is_space() : std::ctype<char>(get_table()) {}
  static mask const* get_table()
  {
    static mask rc[table_size];
    rc[','] = std::ctype_base::space;
    rc['\n'] = std::ctype_base::space;
    rc[' '] = std::ctype_base::space;
    return &rc[0];
  }
};


enum Dirs {
  U = 0,
  R = 1,
  D = 2,
  L = 3
};

int LT(int dir) {
  return (dir + 3) % 4;
}

int RT(int dir) {
  return (dir + 1) % 4;
}

bool IsValid(int i, int j, int k) {
  return i >= 0 && j >= 0 && i < k && j < k;
}

struct Way {
  int cur_dir;
  int i;
  int j;
  std::string way;
  int time;
};

bool operator<(const Way& f, const Way& s) {
  return f.time > s.time;
}


class Minotaurus {
 public:
  int map_size_ = 3;
  int time_ = 0;
  int dir_;
  int i_;
  int j_;
  int go_;
  int rot_;
  int fire_;
  int k_;
  std::vector<std::vector<char>> map_;

  Minotaurus(int x, int y, int x1, int y1, int go, int rot, int fire, int k) : i_(map_size_ / 2), j_(map_size_ / 2), go_(go), rot_(rot), fire_(fire),
                                                                               k_(k), map_(map_size_, std::vector<char>(map_size_, '#')) {
    if (x1 == x - 1) {
      dir_ = L;
    } else if (x1 == x + 1) {
      dir_ = R;
    } else if (y1 == y + 1) {
      dir_ = U;
    } else if (y1 == y - 1) {
      dir_ = D;
    }
    map_[i_][j_] = '0';
  }

  void Fire() {
    time_ += fire_;
    std::cout << 3 << "\n";
    std::vector<std::vector<char>> lighted(2 * k_ + 3, std::vector<char>(2 * k_ + 3, '#'));
    for (int i = 1; i < k_ * 2 + 2; ++i) {
      for (int j = 1; j < k_ * 2 + 2; ++j) {
        std::cin >> lighted[i][j];
        if (lighted[i][j] == '#') {
          if (!(lighted[i + 1][j] == '_' || lighted[i - 1][j] == '_' || lighted[i][j - 1] == '_' || lighted[i][j + 1] == '_')) {
            lighted[i][j] = '#';
          }
        }
      }
    }
    for (int i_l = 1; i_l < k_ * 2 + 2; ++i_l) {
      for (int j_l = 1; j_l < k_ * 2 + 2; ++j_l) {
//          int j_m = i_ - (k_ - j_l) - 1;
//          int i_m = map_size_ - (j_ + (k_ - i_l)) - 2;
        int j_m = j_ - (k_ - j_l) - 1;
        int i_m = i_ - (k_ - i_l) - 1;
        if (!IsValid(i_m, j_m, map_size_)) {
          Resize();
          j_m = j_ - (k_ - j_l) - 1;
          i_m = i_ - (k_ - i_l) - 1;
        }
        if (map_[i_m][j_m] == '#' && lighted[i_l][j_l] == '_') {
          map_[i_m][j_m] = '_';
        }
      }
    }
  }

  std::pair<int, int> GetNext() {
    if (dir_ == L) {
      return {i_, j_ - 1};
    }
    if (dir_ == R) {
      return {i_, j_ + 1};
    }
    if (dir_ == D) {
      return {i_ + 1, j_};
    }
    return {i_ - 1, j_};
  }

  void Go() {
    time_ += go_;
    std::cout << 1 << "\n";
    int succ;
    std:: cin >> succ;
    auto coord = GetNext();
    if (!IsValid(coord.first, coord.second, map_size_)) {
      Resize();
      coord = GetNext();
    }
    if (succ == 0) {
      map_[coord.first][coord.second] = '#';
    } else {
      i_ = coord.first;
      j_ = coord.second;
      map_[coord.first][coord.second] = '0';
    }
  }

  void Turn(int clockwise) {
    time_ += rot_;
    std::cout << 2 << ", " << clockwise << "\n";
    int one;
    std::cin >> one;
    if (clockwise) {
      dir_ = (dir_ + 1) % 4;
    } else {
      dir_ = (dir_ + 3) % 4;
    }
  }

  void Resize() {
    i_ = map_size_ + i_;
    j_ = map_size_ + j_;
    std::vector<std::vector<char>> new_map(map_size_ * 3, std::vector<char>(map_size_  * 3, '#'));
    for (int i = 0; i < map_size_; ++i) {
      for (int j = 0; j < map_size_; ++j) {
        new_map[i + map_size_][j + map_size_] = map_[i][j];
      }
    }
    map_ = new_map;
    map_size_ *= 3;
  }

  std::string Dijkstra() {
    std::priority_queue<Way> heap;
    std::vector<std::vector<std::unordered_set<int>>> used_(map_size_, std::vector<std::unordered_set<int>>(map_size_));
    heap.push({dir_, i_, j_, "", 0});
    while (!heap.empty()) {
      Way top = heap.top();
      heap.pop();
      if (!IsValid(top.i, top.j, map_size_) || map_[top.i][top.j] == '#' || (used_[top.i][top.j].count(top.cur_dir) != 0)) {
        continue;
      }
      if (map_[top.i][top.j] == '_') {
        return top.way;
      }
      if (used_[top.i][top.j].size() == 4) {
        continue;
      }
      Way w1;
      if (top.cur_dir == U) {
        w1 = {U, top.i - 1, top.j, top.way + "G", top.time + go_};
      }
      if (top.cur_dir == D) {
        w1 = {D, top.i + 1, top.j, top.way + "G", top.time + go_};
      }
      if (top.cur_dir == L) {
        w1 = {L, top.i, top.j - 1, top.way + "G", top.time + go_};
      }
      if (top.cur_dir == R) {
        w1 = {R, top.i, top.j + 1, top.way + "G", top.time + go_};
      }
      heap.push(w1);
      heap.push({LT(top.cur_dir), top.i, top.j, top.way + "L", top.time + rot_});
      heap.push({RT(top.cur_dir), top.j, top.j, top.way + "R", top.time + rot_});
      used_[top.i][top.j].insert(top.cur_dir);
    }
    return "END";
  }

  void Play() {
    Fire();
    std::string new_way = Dijkstra();
    while (new_way != "END") {
      for (char symb : new_way) {
        if (symb == 'G') {
          Go();
        } else if (symb == 'L') {
          Turn(0);
        } else if (symb == 'R') {
          Turn(1);
        }
      }
//      PrintMap();
      Fire();
      new_way = Dijkstra();
    }
  }

  void PrintMap() {
    for (int i = 0; i < map_size_; ++i) {
      for (int j = 0; j < map_size_; ++j) {
        if (i == i_ && j == j_) {
          std::cout << "M" << " ";
        } else {
          std::cout << map_[i][j] << " ";
        }
      }
      std::cout << "\n";
    }
    std::cout << "\n";
  }
};



int main() {
  std::cin.imbue(std::locale(std::cin.getloc(), new colon_is_space));
  int x, y, x1, y1, a, b, c, k;
  std::cin >> x >> y >> x1 >> y1 >> a >> b >> c >> k;
  Minotaurus minotaurus(x, y, x1, y1, a, b, c, k);
  minotaurus.Play();
  std::cout << 4 << " " << minotaurus.time_;
}

//1 1 1 0 3 1 0 1
//###
//__#
//###
//1
//1
//###
//#__
//###
