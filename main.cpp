#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

struct Chart {
    char date[11];  // 10 znaków na datę, plus 1 na null terminator
    double open;
    double high;
    double low;
    double close;
    double volume;
};

std::vector<Chart> init();
void generateVerticalContinuousPriceChart(const std::vector<Chart> &data, int chartType, int height);
void saveVerticalContinuousPriceChartToFile(const std::vector<Chart> &data, const char *filename, int chartType, int height);

int main() {

    std::cout << "===============================" << std::endl;
    std::cout << "|        Projekt sem           |" << std::endl;
    std::cout << "|          Martnka             |" << std::endl;
    std::cout << "|       NrIndexu: 233123       |" << std::endl;
    std::cout << "===============================" << std::endl;

    std::vector<Chart> data = init();

    // Wczytaj tylko 100 rekordów
    if (data.size() > 100) {
        data.resize(100);
    }

    // Połącz dane z tego samego dnia
    std::vector<Chart> consolidatedData;

    if (!data.empty()) {
        consolidatedData.push_back(data[0]);

        for (size_t i = 1; i < data.size(); ++i) {
            // Jeśli dane są z tego samego dnia, połącz je
            if (strcmp(data[i].date, consolidatedData.back().date) == 0) {
                consolidatedData.back().high = std::max(consolidatedData.back().high, data[i].high);
                consolidatedData.back().low = std::min(consolidatedData.back().low, data[i].low);
                consolidatedData.back().close = data[i].close;  // Ustaw ostatnią cenę zamknięcia
            } else {
                consolidatedData.push_back(data[i]);
            }
        }
    }

    // Wybór, czy jeden chart ma być sumą dzienną (0), jednego tygodnia (1), czy sumą 4 tygodni (2)
    int chartType;
    std::cout << "Wybierz typ wykresu: \n"
              << "0 - Suma dzienna\n"
              << "1 - Suma jednego tygodnia\n"
              << "2 - Suma 4 tygodni\n"
              << "Wybór: ";
    std::cin >> chartType;

    // Wybór wysokości wykresu
    int height = 20;
    std::cout << "Podaj wysokość wykresu domyślnie 20: ";
    std::cin >> height;

    if (height < 0) {
        height = 20;
    }

    // Przykład generowania pionowego ciągłego wykresu cen w konsoli
    generateVerticalContinuousPriceChart(consolidatedData, chartType, height);

    // Przykład zapisu pionowego ciągłego wykresu do pliku
    const char *filename;
    switch (chartType) {
        case 0:
            filename = "vertical_continuous_daily_price_chart.txt";
            break;
        case 1:
            filename = "vertical_continuous_weekly_price_chart.txt";
            break;
        case 2:
            filename = "vertical_continuous_4weekly_price_chart.txt";
            break;
        default:
            std::cerr << "Nieprawidłowy wybór typu wykresu." << std::endl;
            return 1;
    }
    saveVerticalContinuousPriceChartToFile(consolidatedData, filename, chartType, height);

    return 0;
}

std::vector<Chart> init() {
    const char *filename = "intc_us_data.csv";

    // Strumień do odczytu pliku
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Nie można otworzyć pliku: " << filename << std::endl;
    }

    std::vector<Chart> data;

    char line[256];

    file.getline(line, sizeof(line));

    while (file.getline(line, sizeof(line))) {
        Chart chart;

        sscanf(line, "%10[^,],%lf,%lf,%lf,%lf,%lf",
               chart.date, &chart.open, &chart.high, &chart.low, &chart.close, &chart.volume);

        data.push_back(chart);
    }

    file.close();
    return data;
}

void generateVerticalContinuousPriceChart(const std::vector<Chart> &data, int chartType, int height) {
    // Znajdź maksymalną i minimalną cenę
    double minPrice = data[0].low;
    double maxPrice = data[0].high;

    for (const auto &chart : data) {
        if (chart.low < minPrice) {
            minPrice = chart.low;
        }

        if (chart.high > maxPrice) {
            maxPrice = chart.high;
        }
    }

    // Skala wykresu
    int scale = height;

    // Wysokość wykresu zależna od zakresu cen
    double priceRange = maxPrice - minPrice;
    double priceScale = scale / priceRange;

    // Generowanie pionowego ciągłego wykresu w konsoli
    for (int i = 0; i < scale; ++i) {
        // Rysuj daty pod wykresem
        if (i == scale - 1) {
            for (size_t j = 0; j < data.size(); ++j) {
                std::cout << data[j].date + 8 << " ";
            }
            std::cout << std::endl;
        }

        for (size_t j = 0; j < data.size(); ++j) {
            // Oblicz pozycję na wykresie dla ceny
            int position = static_cast<int>((data[j].close - minPrice) * priceScale);

            // Oblicz wysokość świecy zależną od zmiany ceny
            int candleHeight = static_cast<int>((data[j].high - data[j].low) * priceScale);
            int height = std::min(candleHeight, scale - i);

            // Rysuj wykres
            if (i >= position && i < position + height) {
                // Kod do sumy dziennego wykresu
                if (chartType == 0) {
                    if (data[j].close > data[j].open) {
                        std::cout << '0';  // Cały w "0" gdy cena zamknięcia jest wyższa niż otwarcia
                    } else {
                        std::cout << '#';  // Cały w "#" gdy cena zamknięcia jest niższa niż otwarcia
                    }
                }
                    // Kod do sumy tygodniowego wykresu
                else if (chartType == 1) {
                    if (data[j].close > data[j].open) {
                        std::cout << '0';  // Cały w "0" gdy cena zamknięcia jest wyższa niż otwarcia
                    } else {
                        std::cout << '#';  // Cały w "#" gdy cena zamknięcia jest niższa niż otwarcia
                    }
                }
                    // Kod do sumy 4-tygodniowego wykresu
                else if (chartType == 2) {
                    if (data[j].close > data[j].open) {
                        std::cout << '0';  // Cały w "0" gdy cena zamknięcia jest wyższa niż otwarcia
                    } else {
                        std::cout << '#';  // Cały w "#" gdy cena zamknięcia jest niższa niż otwarcia
                    }
                }
            } else {
                std::cout << ' ';
            }
        }
        std::cout << std::endl;
    }
}

void saveVerticalContinuousPriceChartToFile(const std::vector<Chart> &data, const char *filename, int chartType, int height) {
    std::ofstream outFile(filename);

    // Znajdź maksymalną i minimalną cenę
    double minPrice = data[0].low;
    double maxPrice = data[0].high;

    for (const auto &chart : data) {
        if (chart.low < minPrice) {
            minPrice = chart.low;
        }

        if (chart.high > maxPrice) {
            maxPrice = chart.high;
        }
    }

    // Skala wykresu
    int scale = height;

    // Wysokość wykresu zależna od zakresu cen
    double priceRange = maxPrice - minPrice;
    double priceScale = scale / priceRange;

    // Generowanie pionowego ciągłego wykresu do pliku
    for (int i = 0; i < scale; ++i) {
        // Rysuj daty pod wykresem
        if (i == scale - 1) {
            for (size_t j = 0; j < data.size(); ++j) {
                outFile << data[j].date + 8 << " ";
            }
            outFile << std::endl;
        }

        for (size_t j = 0; j < data.size(); ++j) {
            // Oblicz pozycję na wykresie dla ceny
            int position = static_cast<int>((data[j].close - minPrice) * priceScale);

            // Oblicz wysokość świecy zależną od zmiany ceny
            int candleHeight = static_cast<int>((data[j].high - data[j].low) * priceScale);
            int height = std::min(candleHeight, scale - i);

            // Zapisz datę tylko raz na dole osi x
            if (i == scale - 1) {
                outFile << data[j].date + 8 << " ";
            } else if (i >= position && i < position + height) {
                // Kod do sumy dziennego wykresu
                if (chartType == 0) {
                    if (data[j].close > data[j].open) {
                        outFile << '0';  // Cały w "0" gdy cena zamknięcia jest wyższa niż otwarcia
                    } else {
                        outFile << '#';  // Cały w "#" gdy cena zamknięcia jest niższa niż otwarcia
                    }
                }
                    // Kod do sumy tygodniowego wykresu
                else if (chartType == 1) {
                    if (data[j].close > data[j].open) {
                        outFile << '0';  // Cały w "0" gdy cena zamknięcia jest wyższa niż otwarcia
                    } else {
                        outFile << '#';  // Cały w "#" gdy cena zamknięcia jest niższa niż otwarcia
                    }
                }
                    // Kod do sumy 4-tygodniowego wykresu
                else if (chartType == 2) {
                    if (data[j].close > data[j].open) {
                        outFile << '0';  // Cały w "0" gdy cena zamknięcia jest wyższa niż otwarcia
                    } else {
                        outFile << '#';  // Cały w "#" gdy cena zamknięcia jest niższa niż otwarcia
                    }
                }
            } else {
                outFile << ' ';
            }
        }

        // Nowa linia w pliku, aby odseparować datę od świecy
        outFile << std::endl;
    }

    // Zamknij plik
    outFile.close();
}
