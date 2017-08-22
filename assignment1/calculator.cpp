/*************************************

COMP6771 Assignment 1

Programmed by Chunnan Sheng
Student Code: 5100764

**************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <queue>
#include <vector>
#include <regex>
#include <algorithm>
#include <streambuf>
#include <iterator>
#include <ctype.h>
#include <cmath>

using namespace std;

// There are three types of values:
// Int value, double value and string value
enum ValueType
{
    Integer = 0, Double = 1, String = 2
};

// The item could be used by containers like stacks, queues or vectors
class SyntaxItem
{
private:
    // If the item is a string, then type == 0
    // If the item is an integer, then type == 1
    // If the item is a double, then type == 2
    ValueType type;
    // The data may be a string, an integer or a double
    int int_value;
    double double_value;
    string str_value;

public:
    SyntaxItem(int value)
    {
        this->type = Integer;
        this->int_value = value;
    }

    SyntaxItem(double value)
    {
        this->type = Double;
        this->double_value = value;
    }

    SyntaxItem(string value)
    {
        this->type = String;
        this->str_value = value;
    }

    int getIntValue() const
    {
        return this->int_value;
    }

    double getDoubleValue() const
    {
        return this->double_value;
    }

    string getStrValue() const
    {
        return this->str_value;
    }

    ValueType getType() const
    {
        return this->type;
    }
};

// This source code integer version sqrt is adopted from
// WIKIPEDIA (Integer Square Root)
size_t int_sqrt(size_t n)
{
    if (n < 2)
    {
        return n;
    }
    else
    {
        size_t small = int_sqrt(n >> 2) << 1;
        size_t large = small + 1;
        if (large * large > n)
        {
            return small;
        }
        else
        {
            return large;
        }
    }
}

// This function is used to calculate float value of digits after the dot
// For example, 28 of 47.28, 564 of 33.564
int int_pow(int x, size_t n)
{
    int ret = 1;
    for (size_t i = 0; i < n; i++)
    {
        ret *= x;
    }

    return ret;
}

// Read ASCII data from file
// Translate each item into
// an integer
// a double
// or
// a string command
void readFromFile(char * file_name, queue<SyntaxItem> &q_items)
{
    string text;
    ifstream in(file_name); //Open the file

    in.seekg(0, std::ios::end); //Find end of the file
    text.reserve(in.tellg()); //Create size of the string
    in.seekg(0, std::ios::beg);

    text.assign((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());

    //cout << text << endl;
    //cout << endl;

    regex seperator("[ \t\r\n]+");  //([ \t\r\n]+)([^ \t\r\n]*)
    string text_2(regex_replace(text, seperator, "\n"));

    //cout << text_2 << endl;
    in.close();

    //cout << endl;

    // Read lines from the tream
    istringstream string_file(text_2);
    string line;

    while (getline(string_file, line))
    {
        if (line.size() == 0)
        {
            continue;
        }

        bool digits = true;

        if (line.at(0) == '-' || line.at(0) == '+')
        {
            if (1 == line.size())
            {
                digits = false;
            }
            else if (!isdigit(line.at(1)))
            {
                digits = false;
            }
        }
        else if (!isdigit(line.at(0)))
        {
            digits = false;
        }

        if (digits)
        {
            size_t int_end_index;
            int int_value = stoi(line, &int_end_index);
            // If the entire line is an integer
            if (line.size() == int_end_index)
            {
                SyntaxItem item(int_value);
                q_items.push(item);

                //cout << int_value << endl;
            }
            // If the line is a float number
            else if (line.at(int_end_index) == '.')
            {
                size_t after_dot_digits;
                string after_dot = line.substr(int_end_index + 1, line.size() - int_end_index - 1);
                int after_dot_value = stoi(after_dot, &after_dot_digits);

                double double_value = int_value;
                if (0 <= int_value)
                {
                    double_value += (double)after_dot_value / int_pow(10, after_dot_digits);
                }
                else
                {
                    double_value -= (double)after_dot_value / int_pow(10, after_dot_digits);
                }

                SyntaxItem item(double_value);
                q_items.push(item);

                //cout << double_value << endl;
            }
            else
            {
                // Do nothing here
            }
        }
        // If the line is a command
        else
        {
            SyntaxItem item(line);
            q_items.push(item);

            //cout << line << endl;
        }
    }
}

// A template function to deal with calculation of binary inputs like
// a + b
// a - b
// a * b
// a / b
template <typename type1, typename type2, typename type3>
type1 calculate(type2 l, type3 r, const string &cmd)
{
    if ("add" == cmd)
    {
        cout << l << " + " << r << " = " << l + r << endl;
        return l + r;
    }
    else if ("sub" == cmd)
    {
        cout << l << " - " << r << " = " << l - r << endl;
        return l - r;
    }
    else if ("mult" == cmd)
    {
        cout << l << " * " << r << " = " << l * r << endl;
        return l * r;
    }
    else if ("div" == cmd)
    {
        cout << l << " / " << r << " = " << l / r << endl;
        return l / r;
    }
    else
    {
        return 0;
    }
}

// Do calculations if the command is not 'repeat':
// +
// -
// *
// /
// sqrt
// pop
// reverse
bool calculateWithCommand(stack<SyntaxItem> &items, const string &cmd)
{
    if ("add" == cmd || "sub" == cmd || "mult" == cmd || "div" == cmd)
    {
        if (items.size() < 2)
        {
            return false;
        }

        SyntaxItem &l_item = items.top();
        items.pop();
        SyntaxItem &r_item = items.top();
        items.pop();

        if (l_item.getType() == String || r_item.getType() == String)
        {
            return false;
        }

        bool int_left = false;
        bool int_right = false;
        if (l_item.getType() == Integer)
        {
            int_left = true;
        }
        if (r_item.getType() == Integer)
        {
            int_right = true;
        }

        if (int_left && int_right)
        {
            int ret = calculate<int>(l_item.getIntValue(), r_item.getIntValue(), cmd);
            items.push(SyntaxItem(ret));
        }
        else if (int_left)
        {
            double ret = calculate<double>(l_item.getIntValue(), r_item.getDoubleValue(), cmd);
            items.push(SyntaxItem(ret));
        }
        else if (int_right)
        {
            double ret = calculate<double>(l_item.getDoubleValue(), r_item.getIntValue(), cmd);
            items.push(SyntaxItem(ret));
        }
        else
        {
            double ret = calculate<double>(l_item.getDoubleValue(), r_item.getDoubleValue(), cmd);
            items.push(SyntaxItem(ret));
        }
    }
    else if ("sqrt" == cmd)
    {
        if (items.size() < 1)
        {
            return false;
        }

        SyntaxItem &item = items.top();
        items.pop();

        if (item.getType() == String)
        {
            return false;
        }

        if (item.getType() == Integer)
        {
            int ret = int_sqrt(item.getIntValue());
            cout << "sqrt " << item.getIntValue() << " = " << ret << endl;
            items.push(SyntaxItem(ret));
        }
        else
        {
            double ret = sqrt(item.getDoubleValue());
            cout << "sqrt " << item.getDoubleValue() << " = " << ret << endl;
            items.push(SyntaxItem(ret));
        }
    }
    else if ("pop" == cmd)
    {
        if (items.size() < 1)
        {
            return false;
        }

        items.pop();
    }
    else if ("reverse" == cmd)
    {
        if (items.size() < 1)
        {
            return false;
        }

        SyntaxItem &reverse_len_item = items.top();
        items.pop();

        if (reverse_len_item.getType() != Integer)
        {
            return false;
        }

        // Number of items in the stack that should be reversed
        size_t reverse_len = reverse_len_item.getIntValue();
        // If the size of the stack is smaller than reverse_len
        if (reverse_len > items.size())
        {
            return false;
        }

        queue<SyntaxItem> temp_queue;
        for (size_t i = 0; i < reverse_len; i++)
        {
            temp_queue.push(items.top());
            items.pop();
        }
        for (size_t i = 0; i < reverse_len; i++)
        {
            items.push(temp_queue.front());
            temp_queue.pop();
        }
    }
    else
    {
        return false;
    }

    return true;
}

// This is a recursive function dealing with possible
// nested "repeat endrepeat" blocks
bool process(stack<SyntaxItem> &items, queue<SyntaxItem> &q_items)
{
    // Processing and calculation
    while (q_items.size() > 0)
    {
        SyntaxItem item = q_items.front();
        q_items.pop();

        // If the input item is a number (integer or float), add it into the stack
        // If the input item is 'repeat', do the repeat logic (recursion)
        // If the input item is another kind of command, do the corresponding calculation
        switch (item.getType())
        {
        case Integer:
            items.push(item);
            break;

        case Double:
            items.push(item);
            break;

        case String:

            // The input encapsulated in "repeat endrepeat" is considered as a sub problem
            if (item.getStrValue() == "repeat")
            {
                if (items.size() < 1)
                {
                    return false;
                }

                SyntaxItem repeat_times_item = items.top();
                items.pop();

                if (repeat_times_item.getType() != Integer)
                {
                    return false;
                }

                int repeats = repeat_times_item.getIntValue();
                // cout << "Repeats: " << repeats << endl;
                vector<SyntaxItem> sub_list;

                // The corresponding endrepeat should satisfy the balance
                size_t balance = 1;
                while (q_items.size() > 0)
                {
                    SyntaxItem next_item = q_items.front();
                    q_items.pop();

                    if (next_item.getType() == String && next_item.getStrValue() == "repeat")
                    {
                        balance++;
                    }
                    else if (next_item.getType() == String && next_item.getStrValue() == "endrepeat")
                    {
                        balance--;

                        if (balance == 0)
                        {
                            break;
                        }
                    }

                    sub_list.push_back(next_item);
                }

                if (balance != 0)
                {
                    return false;
                }
                // Recursive logic to call process again for sub sequences in "repeat....endrepeat"
                for (int i = 0; i < repeats; i++)
                {
                    queue<SyntaxItem> sub_queue;
                    for (size_t j = 0; j < sub_list.size(); j++)
                    {
                        sub_queue.push(sub_list.at(j));
                    }

                    if (!process(items, sub_queue))
                    {
                        return false;
                    }
                }
            }
            else
            {
                if (!calculateWithCommand(items, item.getStrValue()))
                {
                    return false;
                }
            }

            break;

        default:
            break;
        }
    }

    return true;
}

int main(int argc, char* argv[])
{

    // setup the print out format for the precision required.
    cout.setf(ios::fixed, ios::floatfield);
    cout.precision(3);

    // ASCII data will be translated into syntax blocks stored in q_items
    queue<SyntaxItem> q_items;
    // stack used for calculations
    stack<SyntaxItem> cal_items;

    // Translate ASCII codes into syntax blocks stored in q_items
    readFromFile(argv[1], q_items);

    // Process the syntax blocks
    process(cal_items, q_items);
}

