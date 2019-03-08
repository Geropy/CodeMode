#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <array>
#include <map>

using namespace std;

enum TILES
{
    FLOOR,
    TABLE,
    DISHWASHER,
    WINDOW,
    BLUEBERRY_CRATE,
    ICE_CREAM_CRATE,
    STRAWBERRY_CRATE,
    CHOPPING_BOARD
};

enum ITEMS
{
    NONE,
    DISH,
    BLUEBERRIES,
    ICE_CREAM,
    STRAWBERRIES,
    CHOPPED_STRAWBERRIES
};

map<char, TILES> tileMap =
{
    {'.', FLOOR},
    {'D', DISHWASHER},
    {'W', WINDOW},
    {'B', BLUEBERRY_CRATE},
    {'I', ICE_CREAM_CRATE},
    {'#', TABLE},
    {'0', FLOOR},
    {'1', FLOOR},
    {'S', STRAWBERRY_CRATE},
    {'C', CHOPPING_BOARD}
};

map<string, ITEMS> itemMap =
{
    {"DISH", DISH},
    {"BLUEBERRIES", BLUEBERRIES},
    {"ICE_CREAM", ICE_CREAM}
};

struct Position
{
    int row;
    int col;
};

struct Player
{
    int row;
    int col;
    vector<ITEMS> carrying;
};

struct Table
{
    int row;
    int col;
    vector<ITEMS> carrying;
};

struct Customer
{
    int points;
    vector<ITEMS> order;
};

vector<ITEMS> stringSplit(const string& str, const string& delim)
{

    vector<ITEMS> items;
    if (str == "NONE")
    {
        return items;
    }
    size_t prev = 0, pos = 0;

    do
    {
        pos = str.find(delim, prev);

        if (pos == string::npos)
        {
            pos = str.length();
        }

        string token = str.substr(prev, pos - prev);

        if (!token.empty())
        {
            items.push_back(itemMap.at(token));
        }

        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());

    return items;

}

int main()
{
    array<array<TILES, 11>, 7> grid;
    vector<Table> usedTables;
    Position dishwasher;
    Position window;
    Position blueberryCrate;
    Position iceCreamCrate;
    Player me, partner;
    vector<Customer> activeCustomers;

    map<ITEMS, Position*> equipmentMap =
    {
        {DISH, &dishwasher},
        {BLUEBERRIES, &blueberryCrate},
        {ICE_CREAM, &iceCreamCrate}
    };

    int numAllCustomers;
    cin >> numAllCustomers; cin.ignore();
    for (int i = 0; i < numAllCustomers; i++) {
        string customerItem; // the food the customer is waiting for
        int customerAward; // the number of points awarded for delivering the food
        cin >> customerItem >> customerAward; cin.ignore();
    }
    for (int i = 0; i < 7; i++) {
        string kitchenLine;
        getline(cin, kitchenLine);
        for (int j = 0; j < kitchenLine.size(); ++j)
        {
            auto tile = tileMap.at(kitchenLine.at(j));
            grid.at(i).at(j) = tile;
            if (tile == DISHWASHER)
            {
                dishwasher.row = i;
                dishwasher.col = j;
            }
            else if (tile == WINDOW)
            {
                window.row = i;
                window.col = j;
            }
            else if (tile == BLUEBERRY_CRATE)
            {
                blueberryCrate.row = i;
                blueberryCrate.col = j;
            }
            else if (tile == ICE_CREAM_CRATE)
            {
                iceCreamCrate.row = i;
                iceCreamCrate.col = j;
            }
        }
    }

    // game loop
    while (1) {
        int turnsRemaining;
        cin >> turnsRemaining; cin.ignore();
        int playerX;
        int playerY;
        string playerItem;
        cin >> playerX >> playerY >> playerItem; cin.ignore();

        me.row = playerY;
        me.col = playerX;
        me.carrying = stringSplit(playerItem, "-");


        int partnerX;
        int partnerY;
        string partnerItem;
        cin >> partnerX >> partnerY >> partnerItem; cin.ignore();

        partner.row = partnerY;
        partner.col = partnerX;
        partner.carrying = stringSplit(partnerItem, "-");


        int numTablesWithItems; // the number of tables in the kitchen that currently hold an item
        cin >> numTablesWithItems; cin.ignore();
        usedTables.clear();
        for (int i = 0; i < numTablesWithItems; i++) {
            int tableX;
            int tableY;
            string item;
            cin >> tableX >> tableY >> item; cin.ignore();

            usedTables.emplace_back();
            usedTables.back().row = tableY;
            usedTables.back().col = tableX;
            usedTables.back().carrying = stringSplit(item, "-");
        }
        string ovenContents; // ignore until wood 1 league
        int ovenTimer;
        cin >> ovenContents >> ovenTimer; cin.ignore();


        int numCustomers; // the number of customers currently waiting for food
        cin >> numCustomers; cin.ignore();
        activeCustomers.clear();
        for (int i = 0; i < numCustomers; i++) {
            string customerItem;
            int customerAward;
            cin >> customerItem >> customerAward; cin.ignore();

            activeCustomers.emplace_back();
            activeCustomers.back().points = customerAward;
            activeCustomers.back().order = stringSplit(customerItem, "-");
        }

        // Start basic
        // Create the last active recipe
        auto& order = activeCustomers.back().order;

        // See what item I need next
        int orderIndex = 0;
        ITEMS nextItem = NONE;
        bool restart = false;

        while (orderIndex < order.size())
        {
            auto item = order[orderIndex];
            if (me.carrying.size() > orderIndex)
            {
                // I don't need to pick up this item
                // I either need the next item, or I need to start over
                if (me.carrying[orderIndex] == item)
                {
                    // Move to next
                    ++orderIndex;
                }
                else
                {
                    restart = true;
                    break;
                }
            }
            else
            {
                nextItem = item;
                break;
            }
        }

        // If I need to restart, use the dishwasher to clear the plate
        if (restart)
        {
            cout << "USE " << dishwasher.col << " " << dishwasher.row << endl;
            continue;
        }

        // If the next item is NONE, the dish is complete
        if (nextItem == NONE)
        {
            cout << "USE " << window.col << " " << window.row << endl;
            continue;
        }

        // Get the next required item
        Position* pos = equipmentMap.at(nextItem);
        cout << "USE " << pos->col << " " << pos->row << endl;
    }
}