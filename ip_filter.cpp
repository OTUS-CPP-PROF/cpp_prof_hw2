#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define UNUSED(v) (void)(v)

using namespace std;
using ip_type = vector<string>;
using ip_pool_type = vector<ip_type>;

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]

auto is_valid_ip(const ip_type &ip)
{
    if(ip.size() != 4) {
        return false;
    }
    for(auto &s : ip) {
        if(static_cast<unsigned>(stoi(s)) > 255) {
            return false;
        }
    }
    return true;
}

ip_type split(const string &str, char d)
{
    vector<string> r;

    string::size_type start = 0;
    string::size_type stop = str.find_first_of(d);
    while(stop != string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

// print IPs
void print_ip_pool(const ip_pool_type &ip_pool)
{
    for(const auto &ip : ip_pool) {
        for(const auto &ip_part : ip) {
            if(&ip_part != &ip[0]) { // one probably blame me paying with pointers... it's just to avoid multiplying entities
                cout << ".";
            }
            cout << ip_part;
        }
        cout << endl;
    }
}

void print_ip_pool(const ip_pool_type &&ip_pool) {
    ip_pool_type _ip_pool{ move(ip_pool) };
    print_ip_pool(_ip_pool);
}

// compare IPs
bool operator < (const ip_type &a, const ip_type &b)
{
    auto it_b = b.begin();
    for(const auto &s_a : a) {
        if(it_b == b.end()) {
            return false; // common parts of a == b
        }
        auto n_a = stoi(s_a);
        auto n_b = stoi(*it_b);
        if(n_a < n_b) {
            return true; // a < b
        } else if(n_a > n_b) {
            return false; // a > b
        }
        ++it_b;
    }
    return false; // common parts of a == b
}

bool ip_contains(const ip_type &ip, unsigned char n)
{
    for(auto &s : ip) {
        if(static_cast<unsigned char>(stoi(s)) == n) {
            return true;
        }
    }
    return false;
}

ip_pool_type filter_ip_any(const ip_pool_type &ip_pool, unsigned char n)
{
    ip_pool_type filtered;

    for(const auto &ip : ip_pool) {
        if(ip_contains(ip, n)) {
            filtered.push_back(ip);
        }
    }
    return filtered;
}

ip_pool_type filter_ip(const ip_pool_type &ip_pool, unsigned char n1)
{
    ip_pool_type filtered;

    for(const auto &ip : ip_pool) {
        if(static_cast<unsigned char>(stoi(ip[0])) == n1) {
            filtered.push_back(ip);
        }
    }
    return filtered;
}

ip_pool_type filter_ip(const ip_pool_type &ip_pool, unsigned char n1, unsigned char n2)
{
    ip_pool_type filtered;

    for(const auto &ip : ip_pool) {
        if(static_cast<unsigned char>(stoi(ip[0])) == n1 &&
            static_cast<unsigned char>(stoi(ip[1])) == n2) {
            filtered.push_back(ip);
        }
    }
    return filtered;
}

int main(int argc, char const *argv[])
{
UNUSED(argc);
UNUSED(argv);

    try
    {
        ip_pool_type ip_pool;

        for(string line; getline(cin, line);)
        {
            ip_type v = split(split(line, '\t').at(0), '.');
            if(is_valid_ip(v)) {
                ip_pool.push_back(v);
            }
        }

        // TODO reverse lexicographically sort
        sort(ip_pool.begin(), ip_pool.end(), [](const auto &a, const auto &b) {return b < a;} );
        print_ip_pool(ip_pool);

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first byte and output
        // ip = filter(1)
        cout << '\n' << "filter(1)" << endl;
        print_ip_pool(filter_ip(ip_pool, 1));

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)
        cout << '\n' << "filter(46, 70)" << endl;
        print_ip_pool(filter_ip(ip_pool, 46, 70));

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        // ip = filter_any(46)
        cout << '\n' << "filter_any(46)" << endl;
        print_ip_pool(filter_ip_any(ip_pool, 46));

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch(const exception &e)
    {
        cerr << e.what() << endl;
    }

    return 0;
}
