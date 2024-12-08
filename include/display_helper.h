#ifndef DISPLAY_H
#define DISPLAY_H

#include <optional>
#include <vector>
#include <string>
#include <conio.h>
#include <iostream>
#include <set>

#define UP 72
#define DOWN 80
#define SPACE_BAR 32
#define ENTER 13
#define MAX_DISPLAY_COUNT 5

// 한개의 아이템을 만드는 메뉴창을 띄웁니다.
// 파라미터: 아이템, 아이템을 string으로 반환하는 함수, 타이틀, 입력창 프롬포트
template <typename T, typename M>
std::optional<T> select_one(const std::vector<T> &items, M mapper, const std::string &title)
{
    std::optional<T> result;
    std::set<int> selected_indecies;
    display_scrollable_menu(items, mapper, MAX_DISPLAY_COUNT, selected_indecies, [selected_indecies](int selected_index) {
        return selected_indecies.empty();
    }, title);
    if (selected_indecies.empty()) return std::nullopt;
    else items[*selected_indecies.begin()];
}

// 여러개의 아이템을 만드는 메뉴창을 띄웁니다.
// 파라미터: 아이템, 아이템을 string으로 반환하는 함수, 최대 선택, 타이틀, 입력창 프롬포트
// 최대 선택의 값이 0이면 선택 제한이 없습니다.
template <typename T, typename M>
std::vector<T> select_multiple(const std::vector<T> &items, M mapper, const int max_item, const std::string &title)
{
    std::vector<T> result;
    std::set<int> selected_indecies;
    display_scrollable_menu(items, mapper, MAX_DISPLAY_COUNT, selected_indecies, [selected_indecies, max_item](int selected_index) {
        return selected_indecies.size() < max_item;
    }, title);
    for (int index : selected_indecies)
    {
        result.push_back(items[index]);
    }
    return result;
}

// 메뉴를 출력합니다.
// 파라미터: 아이템, 아이템을 string으로 반환하는 함수, 표시할 메뉴수, 선택된 아이템인덱스, 선택 트리거(파라미터: 인덱스, 선택하려면 true 반환), 타이틀, 입력창 프롬포트
template <typename T, typename M, typename V>
void display_scrollable_menu(const std::vector<T> &items, M mapper, const int &visible_cout, std::set<int>& selected_index, V on_select, const std::string &title)
{
    int current_pos = 0;
    while (true)
    {
        int start_index = (current_pos + visible_cout < items.size()) ? current_pos : static_cast<int>(items.size()) - visible_cout;
        int end_index = std::min(static_cast<int>(items.size() - 1), current_pos + visible_cout);
        system("cls");
        std::cout << title << std::endl;
        if (items.empty())
        {
            std::cout << "No item" << std::endl;
            std::cout << "Press any key." << std::endl;
        }
        else
        {
            for (int i = start_index; i <= end_index; i++)
            {
                if (i == current_pos)
                    std::cout << "->";
                if (selected_index.count(i) > 0)
                    std::cout << "[" << i + 1 << "]";
                else
                    std::cout << i + 1 << ".";
                std::cout << mapper(items[i]) << std::endl;
            }
        }
        std::cout << "Press Spacebar to select, Enter to confirm, Arrow keys to navigate." << std::endl;
        char input = _getch();

        if (items.empty())
            return;

        if (input == UP)
        {
            current_pos = std::max(current_pos - 1, 0);
        }
        else if (input == DOWN)
        {
            current_pos = std::min(current_pos + 1, static_cast<int>(items.size() - 1));
        }
        else if (input == SPACE_BAR)
        {
            if (selected_index.count(current_pos) > 0)
                selected_index.erase(current_pos);
            else if (on_select(current_pos)) 
                selected_index.insert(current_pos);
        }
        else if (input == ENTER) // Enter
        {
            return;
        }
    }
}

// 사용자로부터 인풋을 받습니다.
std::string get_input(const std::string &prompt)
{
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

#endif