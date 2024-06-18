#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <algorithm> // Для std::sort

using namespace std;
using namespace chrono;

struct Key {
    int s;
    struct Group {
        char l;
        int num;
    } group;
    string last_name;  // Фамилия
    string first_name; // Имя
    string middle_name; // Отчество
    struct Direction {
        string title;
    } dir;
};

// Функция чтения данных из файла
void read_file(vector<Key>& arr, const string& filename) {
    ifstream in(filename);
    if (in.is_open()) {
        string line;
        while (getline(in, line)) {
            stringstream ss(line);
            Key k;

            ss >> k.last_name >> k.first_name >> k.middle_name;

            string group;
            ss >> group;
            k.group.l = group[0];
            k.group.num = stoi(group.substr(1));

            ss >> k.dir.title;

            if (ss.fail()) {
                cerr << "Error parsing line: " << line << endl;
                continue;
            }

            // Считываем последнее число из строки как s
            string last_number;
            ss >> last_number;
            k.s = stoi(last_number);

            arr.push_back(k);
        }
        in.close();
    }
    else {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }
}

void write_file(const vector<Key>& arr, duration<double> diff, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& k : arr) {
            file << "Line number: " << k.s << "\n";
            file << "Line: " << k.last_name << " " << k.first_name << " " << k.middle_name << " "
                << k.group.l << k.group.num << " " << k.dir.title << "\n";

            std::cout << "Line number: " << k.s << "\n";
            std::cout << "Line: " << k.last_name << " " << k.first_name << " " << k.middle_name << " "
                << k.group.l << k.group.num << " " << k.dir.title << "\n";
        }
        file << fixed << setprecision(6) << diff.count() << " s" << "\n";
        file.close();
    }
    else {
        cerr << "Error opening file: " << filename << endl;
        exit(1);
    }
}

// Функция для сравнения Key по полю s для сортировки
bool compareByKey(const Key& a, const Key& b) {
    return a.s < b.s;
}

int binary_search_non_recursive(const vector<Key>& arr, int target) {
    int left = 0, right = arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid].s == target) {
            return mid;
        }
        else if (arr[mid].s < target) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }
    return -1; // Если элемент не найден
}

int linear_search_with_sentinel(vector<Key>& arr, int target) {
    Key sentinel;
    sentinel.s = target;
    arr.push_back(sentinel); // Добавляем барьер

    int i = 0;
    while (arr[i].s != target) {
        ++i;
    }
    arr.pop_back(); // Убираем барьер

    if (i == arr.size()) {
        return -1; // Если элемент не найден
    }
    return i;
}

void Call_Binary_Search() {
    vector<Key> arr;
    vector<Key> answer;
    read_file(arr, "input.txt");

    // Сортируем массив по полю s
    sort(arr.begin(), arr.end(), compareByKey);

    int const pattern_s = 14903; // Поиск по последнему числу s

    auto start = high_resolution_clock::now();
    int index_s = binary_search_non_recursive(arr, pattern_s);
    auto end = high_resolution_clock::now();
    duration<double> diff = end - start;

    if (index_s != -1) {
        answer.push_back(arr[index_s]);
    }
    write_file(answer, diff, "outBinarySearch.txt");
}

// Вызов линейного поиска с барьером
void Call_Linear_Search_With_Sentinel() {
    vector<Key> arr;
    vector<Key> answer;
    read_file(arr, "input.txt");

    int const pattern_s = 14903; // Поиск по последнему числу s

    auto start = high_resolution_clock::now();
    int index_s = linear_search_with_sentinel(arr, pattern_s);
    auto end = high_resolution_clock::now();
    duration<double> diff = end - start;

    if (index_s != -1) {
        answer.push_back(arr[index_s]);
    }
    write_file(answer, diff, "outLinearSearchWithSentinel.txt");
}

int main() {
    Call_Binary_Search();
    Call_Linear_Search_With_Sentinel();
    return 0;
}
