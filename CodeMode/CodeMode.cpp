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
};

enum ITEMS
{
    DISH,
    BLUEBERRIES,
    ICE_CREAM
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
    {'1', FLOOR}
};

map<string, ITEMS> itemMap =
{
    {"DISH", DISH},
    {"BLUEBERRIES", BLUEBERRIES},
    {"ICE_CREAM", ICE_CREAM}
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

struct Board
{
    array<array<TILES, 11>, 7> grid;
    vector<Table> usedTables;
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
    Board board;
    Player me, partner;
    vector<Customer> activeCustomers;

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
            cerr << kitchenLine.at(j) << endl;
            board.grid.at(i).at(j) = tileMap.at(kitchenLine.at(j));
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
        board.usedTables.clear();
        for (int i = 0; i < numTablesWithItems; i++) {
            int tableX;
            int tableY;
            string item;
            cin >> tableX >> tableY >> item; cin.ignore();

            board.usedTables.emplace_back();
            board.usedTables.back().row = tableY;
            board.usedTables.back().col = tableX;
            board.usedTables.back().carrying = stringSplit(item, "-");
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


        // MOVE x y
        // USE x y
        // WAIT
        cout << "WAIT" << endl;
    }
}