#include "andremor.hpp"
#include "util.hpp"
#include <aegis.hpp>
#include <aegis/utility.hpp>
//Yes, this is using more than 1 namespace.
using namespace aegis;
using namespace std;
using namespace utility;

void andremor::message_create(gateway::events::message_create message) {
        string content = message.msg.get_content();
        if (content.length() == 0) return;
        string PREFIX = "g!";
        size_t found = content.find(PREFIX);
        if (found == string::npos) return;
        if (found != 0) return;
        content.erase(0, PREFIX.length());
        if (content.length() == 0) return;
        vector<string> args = split(content, " ");
        string command = args[0];
        args.erase(args.begin());
        AEGIS_TRACE(bot.log, command + "\n");
        try {
            core &bot = message.channel.get_guild().get_bot();
            if (command == "server") {
                guild &server = message.msg.get_guild();
                string role_list;
                string channel_list;
                using namespace gateway::objects;
                for (pair<snowflake, role> role : server.get_roles()) {
                    if (role_list.length() > 987) break;
                    if (role.first.gets() == server.guild_id.gets()) continue;
                    role_list += "<@&" + role.first.gets() + "> ";
                }
                if(role_list.length() == 0) role_list = "*no roles*";
                for (pair<snowflake, aegis::channel *> channel : server.get_channels()) {
                    if(channel_list.length() > 987) break;
                    channel_list += "<#" + channel.first.gets() + "> ";
                }
                embed embed;
                embed.title("Information about " + server.get_name());
                embed.color(0x00ff00);
                thumbnail icon;
                icon.url = "https://cdn.discordapp.com/icons/" + server.guild_id.gets() + "/" + server.get_icon() + ".png?size=4096";
                embed.thumbnail(icon);
                vector<field> fields;
                field server_id; server_id.name("ID").value(server.guild_id.gets()).is_inline(true); fields.push_back(server_id);
                field owner; owner.name("Owner").value("<@!" + server.get_owner().gets() + ">").is_inline(true); fields.push_back(owner);
                field membercount; membercount.name("Member Count").value(to_string(server.get_member_count())).is_inline(true); fields.push_back(membercount);
                field roles; roles.name("Roles (" + to_string(server.get_roles().size() - 1) + ")").value(role_list); fields.push_back(roles);
                field channels; channels.name("Channels").value(channel_list); fields.push_back(channels);
                field creation; creation.name("Server creation time").value(to_iso8601(server.guild_id.get_time()) + "\n\n" + date(server.guild_id.get_time())).is_inline(true); fields.push_back(creation);
                field region; region.name("Region").value(server.get_region()).is_inline(true); fields.push_back(region);
                field voicestates; voicestates.name("Voice states").value(to_string(server.get_voicestates().size())).is_inline(true); fields.push_back(voicestates);
                embed.fields(fields);
                message.channel.create_message(create_message_t().content("Server information").embed(embed));
            }
            if (command == "userinfo") {
                user &usuario = message.get_user();
                using namespace gateway::objects;
                embed embed;
                embed.title("Information about " + usuario.get_full_name());
                embed.color(0x21afb4);
                thumbnail avatar;
                avatar.url = "https://cdn.discordapp.com/avatars/" + usuario.get_id().gets() + "/"+ usuario.get_avatar() +".png?size=4096";
                embed.thumbnail(avatar);
                string voicestatustext;
                vector<field> fields;
                field id; id.name("ID").value(usuario.get_id().gets()).is_inline(true); fields.push_back(id);
                field is_bot; is_bot.name("Bot?").value(usuario.is_bot() ? "Yes" : "No").is_inline(true); fields.push_back(is_bot);
                field created; created.name("Account create time").value(to_iso8601(usuario.get_id().get_time()) + "\n\n" + date(usuario.get_id().get_time())).is_inline(true); fields.push_back(created);
                aegis::user::guild_info member = usuario.get_guild_info(message.channel.get_guild().guild_id);
                voicestatustext += "Muted: " + string(member.mute ? "Yes" : "No") + "\nDeafened: " + string(member.deaf ? "Yes" : "No");
                field nickname; nickname.name("Server nick").value(member.nickname.has_value() ? member.nickname.value() : "*none*").is_inline(true); fields.push_back(nickname);
                field joined; joined.name("Joined time").value(to_iso8601(member.id.get_time()) + "\n\n" + date(member.id.get_time())).is_inline(true); fields.push_back(joined);
                field voicestatus; voicestatus.name("Voice status").value(voicestatustext).is_inline(true); fields.push_back(voicestatus);
                string rolelist;
                for(int i = 0; i < member.roles.size(); i++) {
                    if(rolelist.length() > 987) break;
                    if(member.roles[i].gets() == message.channel.get_guild().guild_id.gets()) continue;
                    rolelist += "<@&" + member.roles[i].gets() + "> ";
                }
                if(rolelist.length() == 0) rolelist = "*no roles*";
                field rolefield; rolefield.name("Roles (" + to_string(member.roles.size() - 1) + ")").value(rolelist); fields.push_back(rolefield);
                embed.fields(fields);
                message.channel.create_message(create_message_t().embed(embed));
            }
            if (command == "hola") {
                message.channel.create_message("Hola :)");
            }
            if (command == "dm") {
                bot.create_dm_message(message.get_user().get_id(), "Hola :)", 0L);
            }
            if (command == "file") {
                rest::aegis_file archivo;
                archivo.name = "hola.txt";
                archivo.data = { 'U', 'n', ' ', 'a', 'r', 'c', 'h', 'i', 'v', 'o', ' ', 'e', 'n', ' ', 'C', '+', '+' };
                message.channel.create_message(create_message_t().file(archivo));
            }
            if (command == "stats") {
                stringstream thing;
                thing << "Time: " << bot.uptime_str() << endl;
                thing << "RAM Usage: " << format_bytes(getCurrentRSS()) << endl;
                message.channel.create_message(create_message_t().content(thing.str()));
            }
            if (command == "random") {
                if(args.empty() ? false : (args[0].length() > 4)) message.channel.create_message(create_message_t().content("Invalid number!"));
                else {
                    std::istringstream iss(args.empty() ? "10" : args[0]);
                    int64_t i64;
                    iss >> i64;
                    if(!iss.fail()) {
                        size_t length = i64;
                        if(!(length > 1950)) {
                            if(!(length < 1)) {
                                string result = random_string(length);
                                message.channel.create_message(create_message_t().content(result));
                            } else message.channel.create_message(create_message_t().content("Invalid length"));
                        } else message.channel.create_message(create_message_t().content("Invalid length! Must be 1950 or less"));
                    } else message.channel.create_message(create_message_t().content("Invalid number"));
                }
            }
            
        } catch (std::exception &err) {
            string err_string = err.what();
            message.channel.create_message("Error: " + err_string);
            cout << err_string << endl;
        }
        return;
}
void andremor::ready(gateway::events::ready obj) {
    cout << obj.user.username << "#" << obj.user.discriminator << " is running!" << endl; 
}