#include "watches.hpp"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <unistd.h>

namespace
{
    constexpr std::size_t sidebarWidth = 28;
    constexpr std::size_t detailsWidth = 43;

    void drawWatch(const Watch& watch)
    {
        std::cout << '\n' << watch.brand << " " << watch.name << '\n'
                  << R"(
             ||=======||
             ||       ||
             ||=======||
          .-'-----------'-.
        .'       12        '.
       /                     \
      |    10          2      |
      |       \      /        |
      | 9      \    /       3 |--.
      |         \  /          |  |
      |          o            |--'
      |    8           4      |
       \         6           /
        '.                 .'
          '-.___________.-'
             ||=======||
             ||       ||
             ||=======||
                        )";
    }

    std::string fit(const std::string& text, std::size_t width)
    {
        if (text.size() > width)
    	{
            return text.substr(0, width - 3) + "...";
        }
        return text + std::string(width - text.size(), ' ');
    }

    void displaySidebar(const Collection& collection, std::size_t selected)
    {
        const auto& watches = collection.getWatches();
        std::vector<std::string> sidebar;
        std::vector<std::string> details;
        if (watches.empty())
	    {
            sidebar.push_back("No watches yet.");
            details.push_back("Your collection is empty.");
        } else
	    {
            for (std::size_t i = 0; i < watches.size(); ++i)
	        {
                sidebar.push_back(std::string(i == selected ? "> " : "  ")
                    + std::to_string(i + 1) + ". " + watches[i].name);
            }
            const auto& watch = watches[selected];
            std::ostringstream price;
            price << '$' << std::fixed << std::setprecision(2) << watch.price;
            details = {
                "Name: " + watch.name,
                "Brand: " + watch.brand,
                "Price: " + price.str(),
                "Year: " + std::to_string(watch.year),
                "Movement: " + watch.movement
            };
        }

        const std::string border = "+" + std::string(sidebarWidth + 2, '-')
            + "+" + std::string(detailsWidth + 2, '-') + "+\n";
        const auto row = [](const std::string& left, const std::string& right)
	    {
            std::cout << "| " << fit(left, sidebarWidth)
                    << " | " << fit(right, detailsWidth) << " |\n";
        };
        std::cout << border;
        row("Collection (" + std::to_string(collection.getCount()) + ")", "Watch details");
        std::cout << border;
        const auto rows = std::max<std::size_t>(8, std::max(sidebar.size(), details.size()));
        for (std::size_t i = 0; i < rows; ++i)
	    {
            row(i < sidebar.size() ? sidebar[i] : "",
                i < details.size() ? details[i] : "");
        }
        std::cout << border;
    }
} // namespace

// Read one watch per line, with quoted name, brand, and movement fields.
// The map is keyed by watch name; later entries replace earlier duplicates.
std::map<std::string, Watch> convertInput(std::istream& input)
{
    std::map<std::string, Watch> watches;
    std::string line;
    std::size_t lineNumber = 0;
    while (std::getline(input, line))
    {
        ++lineNumber;
        if (line.find_first_not_of(" \t\r") == std::string::npos)
        {
            continue;
        }
        std::istringstream iss(line);
        Watch watch;
        if (!(iss >> std::quoted(watch.name) >> std::quoted(watch.brand)
                  >> watch.price >> watch.year >> std::quoted(watch.movement))
            || !(iss >> std::ws).eof())
        {
            throw std::runtime_error("Invalid watch data on line "
                + std::to_string(lineNumber));
        }
        watches[watch.name] = watch;
    }

    return watches;
}

int main()
{
    Collection collection("My collection");
    std::ifstream terminal;
    std::istream* commands = &std::cin;
    if (!isatty(STDIN_FILENO))
    {
        try
        {
            for (const auto& entry : convertInput(std::cin))
            {
                collection.addWatch(entry.second);
            }
        }
        catch (const std::exception& error)
        {
            std::cerr << error.what() << '\n';
            return 1;
        }

        // The pipe reaches EOF after the watch data. Read menu commands
        // from the controlling terminal instead, when one is available.
        if (isatty(STDOUT_FILENO))
        {
            terminal.rdbuf()->pubsetbuf(nullptr, 0);
            terminal.open("/dev/tty");
        }
        if (!terminal.is_open())
        {
            collection.displayCollection();
            return 0;
        }
        commands = &terminal;
    }
    std::size_t selected = 0;
    std::string input;
    std::string status;
    bool showDrawing = false;

    std::cout << "\033[?1049h" << std::flush;
    while (true)
    {
        std::cout << "\033[2J\033[H";
        std::cout << "Watch Collection\n\n";
        displaySidebar(collection, selected);
        if (showDrawing && collection.getCount() > 0)
	{
            drawWatch(collection.getWatches()[selected]);
        }
        std::cout << "\nEnter a watch number to select it, draw to draw it, or quit to exit.\n";
        if (!status.empty())
	{
            std::cout << status << '\n';
        }
        std::cout << "> " << std::flush;
        if (!std::getline(*commands, input) || input == "quit" || input == "q")
	{
            break;
        }
        if (input.empty())
	{
            continue;
        }
        if (input == "draw")
	{
            showDrawing = collection.getCount() > 0;
            status = showDrawing ? "" : "There are no watches to draw yet.";
            continue;
        }
        std::istringstream selection(input);
        std::size_t number = 0;
        if ((selection >> number) && (selection >> std::ws).eof()
            && number > 0 && number <= collection.getCount())
        {
            selected = number - 1;
            status.clear();
        } else
        {
            status = collection.getCount() == 0
                ? "There are no watches to select yet."
                : "Enter a number from 1 to " + std::to_string(collection.getCount())
                    + ", draw, or quit.";
        }
    }
    // Restore the original terminal screen.
    std::cout << "\033[?1049l" << std::flush;
    return 0;
}
