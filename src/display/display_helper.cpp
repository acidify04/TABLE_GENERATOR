#include "display_helper.h"

// 사용자로부터 인풋을 받습니다.
std::string get_input(const std::string &prompt)
{
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

int get_integer_input(const std::string prompt)
{
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    try
    {
        return std::stoi(input);
    }
    catch (const std::invalid_argument &)
    {
        return 0;
    }
    catch (const std::out_of_range)
    {
        return 0;
    }
}

std::string limited_str(const std::string &str, int limit)
{
    limit -= 3;
    if (limit < 1)
        return "...";
    if (str.length() < limit)
        return str;
    return str.substr(0, limit) + "...";
}

void draw_table(std::string title, std::vector<ColumnItem> column, std::vector<RowItem> rows)
{
    size_t column_width = 15; // 기본 최소 너비

    size_t visible_columns = 5; // 최대 열
    size_t start_column = 0;
    size_t table_width = column_width * (visible_columns + 1) + visible_columns;
    // 테이블 그리는 부분
    while (true)
    {
        system("cls");
        std::cout << "\n" << std::string(table_width, '=') << "\n";
        std::cout << std::setw(table_width / 2 - title.length()) << std::left << title << "\n";
        std::cout << std::string(table_width, '=') << "\n";

        std::cout << std::setw(column_width) << ""; // Row 부분
        for (size_t i = start_column; i < std::min(column.size(), start_column + visible_columns); i++)
        {
            std::cout << std::setw(column_width + 1) << std::left << column[i].column;
        }
        std::cout << "\n" << std::string(table_width, '-') << "\n";

        for (const auto &row : rows)
        {
            std::cout << std::setw(column_width) << std::left << row.row;
            for (size_t i = start_column; i < std::min(row.columns.size(), start_column + visible_columns); i++)
            {
                std::cout << std::setw(column_width + 1) << std::left << limited_str(row.columns[i], column_width);
            }
            std::cout << "\n";
        }

        std::cout << std::string(table_width, '=') << "\n";

        std::cout << "Use Left/Right Arrow Keys to scroll, 'q' to quit." << std::endl;
        char input = _getch();
        if (input == LEFT)
        {
            if (start_column > 0)
                start_column--;
        }
        else if (input == RIGHT)
        {
            if (start_column + visible_columns < column.size())
                start_column++;
        }
        else if (input == 'q' || input == 'Q')
            return;
    }
}