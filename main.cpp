#include "andremor.hpp"
using namespace aegis;

int main(int argc, char* argv[]) {

    core bot;
    bot.wsdbg = true;
    andremor andremor_instance;

    bot.set_on_message_create(bind(&andremor::message_create, &andremor_instance, std::placeholders::_1));
    bot.set_on_ready([&](aegis::gateway::events::ready obj)
    {
        bot.log->info(obj.user.username + "#" + obj.user.discriminator + " is running!");
    });

    bot.run();
    bot.yield();
}