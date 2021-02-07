#ifndef ANDREMOR_HPP
#define ANDREMOR_HPP
#include <aegis.hpp>
class andremor {
public:
    void message_create(aegis::gateway::events::message_create message) noexcept;
};
#endif