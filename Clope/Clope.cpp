#include <iostream>
#include <windows.h>
#include <fstream>

#include "Settings.h"
#include "DataReaderMushroom.h"
#include "Processor.h"

void WriteResults(const std::vector<std::vector<size_t>>& result)
{
    std::ofstream out("results.csv");
    for (size_t i = 0; i < result.size(); ++i)
        if (!result[i].empty())
        {
            out << i + 1 << ',' << result[i].size();
            for (size_t j = 0; j < result[i].size(); ++j)
            {
                out << ',' << result[i][j];
            }
            out << std::endl;
        }
    out.close();
}

void WriteEdibility(const std::vector<std::vector<size_t>>& result, const std::vector<char>& edibility)
{
    std::ofstream out("resultsEd.csv");
    for (size_t i = 0; i < result.size(); ++i)
        if (!result[i].empty())
        {
            out << i + 1 << ',' << result[i].size();
            for (size_t j = 0; j < result[i].size(); ++j)
            {
                out << ',' << edibility[result[i][j]];
            }
            out << std::endl;
        }
    out.close();
}

bool ReadArgument(char* arg)
{
    if (arg[1] == 0)
        return false;

    if (arg[0] == 'r')
    {
        double r = strtod(arg + 2, nullptr);
        if (r == 0)
            return false;
        Settings::r = r;
        return true;
    }

    if (arg[0] == 'n')
    {
        int n = atoi(arg + 2);
        if (n == 0)
            return false;
        Settings::numObjs = n;
        return true;
    }

    if (arg[0] == 'i')
    {
        int n = atoi(arg + 2);
        if (n == 0)
            return false;
        Settings::maxIterations = n;
        return true;
    }

    if (arg[0] == 'p')
    {
        Settings::path = arg + 2;
        return true;
    }
    return false;
}

void ShowHelp()
{
    std::cout << "Аргументы командной строки:\n";
    std::cout << " r:CLOPE коэффициент отталкивания\n";
    std::cout << " n:количество объектов\n";
    std::cout << " i:максимальное количество итераций\n";
    std::cout << " p:путь к файлу данных\n\n";
    std::cout << "Значения по умолчанию: r = " << Settings::r << ";  n = " << Settings::numObjs << ";  i = " << Settings::maxIterations << ";  p = \"" << Settings::path << "\"\n\n";
}

int main(int argc, char* argv[])
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    
    Settings::r = 2.7;
    Settings::numObjs = 126;
    Settings::maxIterations = 1;
    Settings::path = "d:\\agaricus-lepiota.data";

    try
    {
        if (argc > 1)
        {
            if (argc == 2 && argv[1][0] == '?')
            {
                ShowHelp();
                return 0;
            }

            for (size_t i = 1; i < argc; ++i)
            {
                if (!ReadArgument(argv[i]))
                {
                    std::cout << "Некорректный аргумент " << argv[i] << "\n\n";
                    ShowHelp();
                    return 0;
                }
            }
        }

        DataReaderMushroom dataReader(Settings::path);
        Processor proc(dataReader);
        if (!proc.Process(Settings::maxIterations))
        {
            std::cout << "Неизвестная ошибка!\n\n";
            return 1;
        }

        std::vector<std::vector<size_t>> result;
        proc.GetResults(result);
        WriteResults(result);

        std::vector<char> edibility = dataReader.GetEdibility();
        WriteEdibility(result, edibility);
    }
    catch (const std::exception& err)
    {
        std::cout << err.what() << "\n\n";
        return 1;
    }

    return 0;
}
