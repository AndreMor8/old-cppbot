#include "lib/CPUSnapshot.h"
#include <string>
#include <vector>
#include <chrono>
#include <sstream>
#include <future>
using namespace std;

vector<string> vector_slice(vector<string> arr, int i) {
    return vector<string>(arr.begin() + i, arr.end());
}

string string_round(float var)
{
    char str[40];
    sprintf(str, "%.2f", var); 
    return str;
}

future<string> get_cpu_usage()
{
    return async(launch::async, []() {
        CPUSnapshot previousSnap;
        this_thread::sleep_for(chrono::seconds(1));
        CPUSnapshot curSnap;

        const float ACTIVE_TIME = curSnap.GetActiveTimeTotal() - previousSnap.GetActiveTimeTotal();
        const float IDLE_TIME = curSnap.GetIdleTimeTotal() - previousSnap.GetIdleTimeTotal();
        const float TOTAL_TIME = ACTIVE_TIME + IDLE_TIME;
        float usage = 100.f * ACTIVE_TIME / TOTAL_TIME;
        return string_round(usage);
    });
}

int64_t to_number(string str, int length)
{
    string myThing = "Invalid number!";
    if (str.length() > length)
        throw myThing;
    std::istringstream iss(str);
    int64_t i64;
    iss >> i64;
    if (iss.fail())
        throw myThing;
    else
        return i64;
}

vector<string> split(string str, string splitBy)
{
    vector<string> tokens;
    tokens.push_back(str);
    size_t splitAt;
    size_t splitLen = splitBy.size();
    string frag;
    while (true)
    {
        frag = tokens.back();
        splitAt = frag.find(splitBy);
        if (splitAt == string::npos)
            break;
        tokens.back() = frag.substr(0, splitAt);
        tokens.push_back(frag.substr(splitAt + splitLen, frag.size() - (splitAt + splitLen)));
    }
    return tokens;
}

string join(vector<string> arr, string joinwith)
{
    string tosend;
    int i = 0;
    for (string thing : arr)
    {
        tosend += thing + ((i - 1) == arr.size() ? "" : joinwith);
        i++;
    }
    return tosend;
}

string reverse_string(string line)
{
    string::reverse_iterator it;
    string result;
    for (it = line.rbegin(); it < line.rend(); it++)
    {
        result += *it;
    }
    return result;
}

string date(int64_t timestamp)
{
    chrono::system_clock::time_point starttime = chrono::system_clock::from_time_t(timestamp / 1000);
    chrono::time_point<std::chrono::system_clock> now_t = chrono::system_clock::now();
    auto time_is = now_t - starttime;

    using seconds = chrono::duration<int, ratio<1, 1>>;
    using minutes = chrono::duration<int, ratio<60, 1>>;
    using hours = chrono::duration<int, ratio<3600, 1>>;
    using days = chrono::duration<int, ratio<24 * 3600, 1>>;
    using months = chrono::duration<int, ratio<24 * 3600 * 30, 1>>;
    using years = chrono::duration<int, ratio<24 * 3600 * 365, 1>>;

    std::stringstream ss;
    auto y = chrono::duration_cast<years>(time_is).count();
    time_is -= years(y);
    auto mo = chrono::duration_cast<months>(time_is).count();
    time_is -= months(mo);
    auto d = chrono::duration_cast<days>(time_is).count();
    time_is -= days(d);
    auto h = chrono::duration_cast<hours>(time_is).count();
    time_is -= hours(h);
    auto m = chrono::duration_cast<minutes>(time_is).count();
    time_is -= minutes(m);
    auto s = chrono::duration_cast<seconds>(time_is).count();

    if (y)
        ss << y << " years, ";
    if (mo)
        ss << mo << " months, ";
    if (d)
        ss << d << " days, ";
    if (h)
        ss << h << " hours, ";
    if (m)
        ss << m << " minutes, ";
    ss << s << " seconds ";
    return ss.str();
}