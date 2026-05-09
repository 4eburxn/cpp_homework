#include <format>
#include <iostream>
#include <string>
#include <vector>

struct Checkpoint {
  std::string name;
  double latitude;
  double longitude;
  bool mandatory;
  double penalty;
};

class Builder {
public:
  virtual ~Builder() = default;
  virtual void addCheckpoint(int index, const std::string &name, double lat,
                             double lon, bool mandatory, double penalty) = 0;
};

class TextBuilder : public Builder {
  std::string result;

public:
  void addCheckpoint(int index, const std::string &name, double lat, double lon,
                     bool mandatory, double penalty) override {
    std::string penaltyStr;
    if (mandatory) {
      penaltyStr = "незачёт";
    } else {
      penaltyStr = std::format("{:.2f}", penalty);
    }
    result += std::format("{}; {}; {:.6f}, {:.6f}; {}\n", index, name, lat, lon,
                          penaltyStr);
  }
  std::string getText() const { return result; }
};

class PenaltyBuilder : public Builder {
  double total = 0.0;

public:
  void addCheckpoint(int index, const std::string &name, double lat, double lon,
                     bool mandatory, double penalty) override {
    if (!mandatory)
      total += penalty;
  }
  double TotalPenalty() const { return total; }
};

class Director {
public:
  void construct(Builder &builder, const std::vector<Checkpoint> &cps) {
    int idx = 1;
    for (const auto &cp : cps) {
      builder.addCheckpoint(idx++, cp.name, cp.latitude, cp.longitude,
                            cp.mandatory, cp.penalty);
    }
  }
};

int main() {
  std::vector<Checkpoint> cps = {
      {"Старт", 55.751244, 37.618423, true,
       0.0}, // Пример: Москва, Красная площадь
      {"Зона 51", 37.235, -115.811111, false, 5.0}, // Легендарная база в Неваде
      {"Бермудский треугольник", 26.629167, -70.883611, true,
       0.0}, // Район с дурной славой в Атлантике
      {"Долина Смерти (Якутия)", 65.0, 111.0, true,
       0.0}, // Елюю Черкечех, мистическое место
      {"Пустыня Наска", -14.716667, -75.133333, false,
       3.0}, // Таинственные геоглифы в Перу
      {"Черный треугольник (Тихий океан)", -10.0, -170.0, false,
       2.5}, // Аномалия у островов Кирибати
      {"Финиш", 59.939095, 30.315868, true, 0.0}
      // Пример: Санкт-Петербург, Исаакиевский собор
  };
  Director dir;
  TextBuilder tb;
  PenaltyBuilder pb;
  dir.construct(tb, cps);
  dir.construct(pb, cps);
  std::cout << tb.getText();
  std::cout << "Суммарный штраф: " << pb.TotalPenalty() << " часов\n";
  return 0;
}
