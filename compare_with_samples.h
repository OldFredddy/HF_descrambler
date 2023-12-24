#ifndef COMPARE_WITH_SAMPLES_H
#define COMPARE_WITH_SAMPLES_H

#include <vector>

class compare_with_samples {
public:
    compare_with_samples();

    // Объявление других методов класса (если они есть)
};

// Объявление функций за пределами класса
std::vector<int> kmp_table(const std::vector<char>& word);
int kmp_search(const std::vector<char>& text, const std::vector<char>& word, const std::vector<int>& T);
int compare_with_samples_go(std::vector<char> signal, std::vector<char> sample, int length, int probability);
int compare_with_samples_one_one(const std::vector<char>& zero_file, const std::vector<char>& sample);
#endif // COMPARE_WITH_SAMPLES_H
