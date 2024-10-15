#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

// формула 3.2
double correlation(const std::vector<int>& x, const std::vector<int>& y) {
    int N = x.size();
    double sum = 0.0;

    for (int n = 0; n < N; n++) {
        sum += x[n] * y[n];
    }

    return sum;
}

// формула 3.3
double normalizedCorrelation(const std::vector<int>& x,
                             const std::vector<int>& y) {
    int N = x.size();
    double sumXY = 0.0, sumX2 = 0.0, sumY2 = 0.0;

    for (int n = 0; n < N; n++) {
        sumXY += x[n] * y[n];
        sumX2 += x[n] * x[n];
        sumY2 += y[n] * y[n];
    }

    return sumXY / (std::sqrt(sumX2) * std::sqrt(sumY2));
}

void printCorrelationTable(const std::string& title, double corr_ab,
                           double corr_ac, double corr_bc,
                           bool normalized = false) {
    std::cout << title << std::endl;
    std::cout << std::setw(6) << " " << " | " << std::setw(12) << "a"
              << std::setw(12) << "b" << std::setw(12) << "c" << std::endl;

    if (normalized) {
        std::cout << std::fixed << std::setprecision(6);
    }

    std::cout << std::setw(6) << "a" << " | " << std::setw(12) << "-"
              << std::setw(12) << corr_ab << std::setw(12) << corr_ac
              << std::endl;
    std::cout << std::setw(6) << "b" << " | " << std::setw(12) << corr_ab
              << std::setw(12) << "-" << std::setw(12) << corr_bc << std::endl;
    std::cout << std::setw(6) << "c" << " | " << std::setw(12) << corr_ac
              << std::setw(12) << corr_bc << std::setw(12) << "-" << std::endl;

    if (normalized) {
        std::cout.unsetf(std::ios::fixed);
    }
}

int main() {
    std::vector<int> a = {7, 3, 2, -2, -2, -4, 1, 5};
    std::vector<int> b = {8, 6, 4, 0, -5, -6, 0, 3};
    std::vector<int> c = {2, -1, 3, -9, -2, -8, 4, -1};

    // корреляция между массивами
    double corr_ab = correlation(a, b);
    double corr_ac = correlation(a, c);
    double corr_bc = correlation(b, c);

    // нормализация
    double norm_corr_ab = normalizedCorrelation(a, b);
    double norm_corr_ac = normalizedCorrelation(a, c);
    double norm_corr_bc = normalizedCorrelation(b, c);

    printCorrelationTable("Correlation between a, b and c:", corr_ab, corr_ac,
                          corr_bc);
    printCorrelationTable("Normalized correlation between a, b and c:",
                          norm_corr_ab, norm_corr_ac, norm_corr_bc, true);

    return 0;
}
