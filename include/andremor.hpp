#ifndef ANDREMOR_HPP
#define ANDREMOR_HPP
#include <aegis.hpp>
#include <db.hpp>
class andremor
{
public:
    aegis::core * _bot;
    void set_bot(aegis::core &bot) noexcept;
    void message_create(aegis::gateway::events::message_create message) noexcept;
    void ready(aegis::gateway::events::ready obj) noexcept;
    std::mutex m_ping_test;
    std::int64_t checktime = 0;
    std::int64_t ws_checktime = 0;
    std::condition_variable cv_ping_test;
    myDb * _mongo_client;
    void set_mongo_client(myDb &the_client) noexcept;
};
#endif