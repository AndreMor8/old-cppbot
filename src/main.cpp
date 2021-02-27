#include "dotenv.h"
#include <andremor.hpp>

using namespace aegis;
using namespace dotenv;

int main(int argc, char* argv[]) {
    env.load_dotenv();
    core bot(create_bot_t().token(env["DISCORD_TOKEN"]).log_level(spdlog::level::info).intents(32767));
    bot.wsdbg = true;
    andremor andremor_instance;
    andremor_instance.set_bot(bot);
    bot.set_on_message_create(bind(&andremor::message_create, &andremor_instance, std::placeholders::_1));
    bot.set_on_ready(bind(&andremor::ready, &andremor_instance, std::placeholders::_1));
    
    bot.run();
    bot.yield();
}