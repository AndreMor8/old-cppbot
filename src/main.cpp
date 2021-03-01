#include "dotenv.h"
#include <andremor.hpp>
#include <db.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>

using namespace aegis;
using namespace dotenv;

int main(int argc, char* argv[]) {
    env.load_dotenv();
    aegis::core bot(create_bot_t().token(env["DISCORD_TOKEN"]).log_level(spdlog::level::info).intents(32767));
    bot.wsdbg = true;
    mongocxx::instance myInstance{};
    mongocxx::client myMongoClient{mongocxx::uri{env["MONGODB_URI"]}};
    mongocxx::database database_to_set = myMongoClient[env["MONGODB_DB"]];
    andremor andremor_instance;
    myDb myDb_instance;
    andremor_instance.set_bot(bot);
    myDb_instance.set_db(database_to_set);
    andremor_instance.set_mongo_client(myDb_instance);
    bot.set_on_message_create(bind(&andremor::message_create, &andremor_instance, std::placeholders::_1));
    bot.set_on_ready(bind(&andremor::ready, &andremor_instance, std::placeholders::_1));
    
    bot.run();
    bot.yield();
}