#include <iostream>
#include <string>
#include <iomanip>
#include <cctype>

double getDistance() {
    double distance = 0.0;
    while (true) {
        std::cout << "Enter journey distance in miles: ";
        if (std::cin >> distance && distance >= 0.0) {
            return distance;
        }
        std::cout << "Invalid input. Please enter a positive number.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}

int getFuelType() {
    int choice = 0;
    while (true) {
        std::cout << "\nSelect Fuel Type:\n";
        std::cout << "1. Petrol\n";
        std::cout << "2. Diesel\n";
        std::cout << "Enter your choice (1 or 2): ";
        if (std::cin >> choice && (choice == 1 || choice == 2)) {
            return choice;
        }
        std::cout << "Invalid selection. Please choose 1 or 2.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}

double calculateEmissions(double distanceMiles, int fuelType) {
    const double litresPerMile = 9.66 / 100.0;
    const double petrolEmissionRate = 2.31;
    const double dieselEmissionRate = 2.68;

    double fuelConsumedLitres = distanceMiles * litresPerMile;
    double emissionRate = (fuelType == 1) ? petrolEmissionRate : dieselEmissionRate;

    return fuelConsumedLitres * emissionRate;
}

int main() {
    std::cout << "========================================\n";
    std::cout << "      Car Journey CO2 Calculator        \n";
    std::cout << "========================================\n";

    double distance = getDistance();
    int fuelType = getFuelType();
    double totalCO2 = calculateEmissions(distance, fuelType);

    std::string fuelName = (fuelType == 1) ? "Petrol" : "Diesel";

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\n---------------- Journey Summary ----------------\n";
    std::cout << "Distance Traveled : " << distance << " miles\n";
    std::cout << "Fuel Type Used    : " << fuelName << "\n";
    std::cout << "Estimated CO2     : " << totalCO2 << " kg\n";
    std::cout << "------------------------------------------------\n";

    return 0;
}