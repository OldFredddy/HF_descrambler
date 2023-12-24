#include "compare_with_samples.h"
#include <vector>
#include <cmath>
using namespace std;
compare_with_samples::compare_with_samples()
{

}

vector<int> kmp_table(const vector<char>& word) {
    vector<int> T(word.size() + 1, -1);
    for (size_t i = 1; i <= word.size(); i++) {
        int pos = T[i - 1];
        while (pos != -1 && word[pos] != word[i - 1]) {
            pos = T[pos];
        }
        T[i] = pos + 1;
    }
    return T;
}

int kmp_search(const vector<char>& text, const vector<char>& word, const vector<int>& T) {
    int m = 0; // начало текущего совпадения в text
    int i = 0; // позиция текущего символа в word
    while (m + i < text.size()) {
        if (word[i] == text[m + i]) {
            if (i == word.size() - 1) {
                return m;
            }
            i++;
        } else {
            m += i - T[i];
            if (i > 0) {
                i = T[i];
            }
        }
    }
    return -1; // не найдено
}

int compare_with_samples_go(vector<char> signal, vector<char> sample, int length, int probability) {
    // Рассчитать минимальное количество символов, которые должны совпасть
    int min_match_count = std::ceil(length * (probability / 100.0));

    for (size_t i = 0; i <= sample.size() - length; i++) {
        vector<char> word(sample.begin() + i, sample.begin() + i + length);
        vector<int> T = kmp_table(word);
        int match_count = 0; // Счетчик совпадений

        int m = 0; // начало текущего совпадения в signal
        int j = 0; // позиция текущего символа в word

        // Измененный поиск с учетом вероятности
        while (m + j < signal.size()) {
            if (word[j] == signal[m + j]) {
                match_count++;
                if (match_count >= min_match_count) {
                    return m; // Достаточное количество совпадений
                }
                j++;
                if (j == word.size()) {
                    break; // Достигнут конец слова, переход к следующему
                }
            } else {
                m += j - T[j];
                if (j > 0) {
                    j = T[j];
                }
                match_count = 0; // Сброс счетчика совпадений
            }
        }
    }
    return -1; // не найдено
}
int compare_with_samples_one_one(const std::vector<char>& zero_file, const std::vector<char>& sample) {
    if (sample.empty() || zero_file.size() < sample.size()) {
        return -1; // Если образец пуст или больше исходного вектора
    }
    std::vector<int> T = kmp_table(sample);
    return kmp_search(zero_file, sample, T);
}
