#include <iostream>
#include <iomanip>
#include <algorithm>

const int PLANES = 2;
const int TIERS = 3;

// максимальный груз для каждой ступени (кг): [самолёт][ступень]
const double loadLimit[PLANES][TIERS] = {
    {750.0, 1500.0, 2000.0},
    {1000.0, 2000.0, 3000.0}
};

// расход топлива для каждой ступени (л/км): [самолёт][ступень]
const double fuelRate[PLANES][TIERS] = {
    {1.0, 4.0, 7.0},
    {2.0, 4.0, 6.0}
};

// ёмкость основного бака (л)
const double tankCapacity[PLANES] = {300.0, 1000.0};

// ёмкость сбрасываемого временного бака, крепится в точке A (л)
const double dropTankCapacity[PLANES] = {0.0, 100.0};

int findTier(int plane, double cargo) {
    for (int i = 0; i < TIERS; i++) {
        if (cargo <= loadLimit[plane][i]) {
            return i;
        }
    }
    return -1;
}

void printFleet() {
    std::cout << "=== Авиапарк ===\n";
    for (int p = 0; p < PLANES; p++) {
        std::cout << "Самолёт " << p + 1
                  << ": основной бак " << tankCapacity[p] << " л"
                  << ", максимальный груз " << loadLimit[p][TIERS - 1] << " кг";
        if (dropTankCapacity[p] > 0) {
            std::cout << ", временный бак " << dropTankCapacity[p] << " л";
        }
        std::cout << "\n";
        for (int t = 0; t < TIERS; t++) {
            std::cout << "    до " << loadLimit[p][t] << " кг: "
                      << fuelRate[p][t] << " л/км\n";
        }
    }
}

int main() {
    printFleet();

    int plane;
    std::cout << "\nВыберите самолёт (1 или 2): ";
    std::cin >> plane;
    if (plane < 1 || plane > PLANES) {
        std::cout << "Ошибка: такого самолёта в парке нет\n";
        return 1;
    }
    int p = plane - 1;

    double cargo, distAB, distBC;
    std::cout << "Вес груза (кг): ";
    std::cin >> cargo;
    std::cout << "Расстояние A-B (км): ";
    std::cin >> distAB;
    std::cout << "Расстояние B-C (км): ";
    std::cin >> distBC;

    int tier = findTier(p, cargo);
    if (tier == -1) {
        std::cout << "\nСамолёт " << plane << " не поднимает груз " << cargo
                  << " кг (максимум " << loadLimit[p][TIERS - 1] << " кг)\n";
        return 0;
    }

    double rate = fuelRate[p][tier];
    double tank = tankCapacity[p];
    double drop = dropTankCapacity[p];

    double needAB = rate * distAB;
    double needBC = rate * distBC;

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "\nРасход топлива: " << rate << " л/км\n";
    std::cout << "Участок A-B требует: " << needAB << " л\n";

    if (needAB > tank + drop) {
        std::cout << "Невозможно преодолеть расстояние A-B: нужно " << needAB
                  << " л, а взять можно максимум " << tank + drop << " л\n";
        return 0;
    }

    double fromDrop = std::min(drop, needAB);
    double fromMain = needAB - fromDrop;
    double mainLeft = tank - fromMain;

    std::cout << "Участок B-C требует: " << needBC << " л\n";
    if (needBC > tank) {
        std::cout << "Невозможно преодолеть расстояние B-C: нужно " << needBC
                  << " л, а в основной бак помещается максимум " << tank << " л\n";
        return 0;
    }

    double refuel = needBC - mainLeft;
    if (refuel < 0) {
        refuel = 0;
    }

    if (drop > 0) {
        std::cout << "На A-B израсходовано: " << fromDrop << " л из временного бака, "
                  << fromMain << " л из основного\n";
    } else {
        std::cout << "На A-B израсходовано: " << fromMain << " л из основного бака\n";
    }
    std::cout << "В пункте B в основном баке осталось: " << mainLeft << " л\n";
    std::cout << "Минимальная дозаправка в пункте B: " << refuel << " л\n";

    return 0;
}
