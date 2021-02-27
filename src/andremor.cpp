#include <andremor.hpp>
#include <util.hpp>
#include <aegis.hpp>
#include <aegis/utility.hpp>

//Yes, this is using more than 1 namespace.
using namespace aegis;
using namespace std;
using namespace utility;

void andremor::set_bot(aegis::core &bot) noexcept
{
    this->_bot = &bot;
}

void andremor::message_create(gateway::events::message_create message) noexcept
{
    core & bot = *_bot;
    if (message.get_user().is_bot())
    {
        if (message.get_user().get_id() == bot.self()->get_id())
        {
            if (message.msg.nonce == checktime)
            {
                ws_checktime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count() - checktime;
                {
                    std::lock_guard<std::mutex> lk(m_ping_test);
                    cv_ping_test.notify_all();
                }
                return;
            }
        }
        return;
    }
    string content = message.msg.get_content();
    if (content.empty())
        return;
    string PREFIX = "g!";
    size_t found = content.find(PREFIX);
    if (found == string::npos)
        return;
    if (found != 0)
        return;
    content.erase(0, PREFIX.length());
    if (content.empty())
        return;
    vector<string> args = split(content, " ");
    string command = args[0];
    args.erase(args.begin());
    bot.log->info(command);
    try
    {
        if (command == "server")
        {
            aegis::guild &pre_server = message.msg.get_guild();
            aegis::future<gateway::objects::guild> promise = pre_server.get_guild();
            gateway::objects::guild server = promise.get();
            string role_list;
            string channel_list;
            using namespace gateway::objects;
            for (role role : server.roles)
            {
                if (role_list.length() > 987)
                    break;
                if (role.role_id.gets() == server.guild_id.gets())
                    continue;
                role_list += role.get_mention();
            }
            if (role_list.empty())
                role_list = "*no roles*";
            for (pair<snowflake, aegis::channel *> channel : pre_server.get_channels())
            {
                if (channel_list.length() > 987)
                    break;
                channel_list += "<#" + channel.first.gets() + "> ";
            }
            gateway::objects::embed embed;
            embed.title("Information about " + server.name);
            embed.color(0x00ff00);
            thumbnail icon;
            icon.url = "https://cdn.discordapp.com/icons/" + server.guild_id.gets() + "/" + server.icon + ".png?size=4096";
            embed.thumbnail(icon);
            vector<string> verlevels = {"None", "Low", "Medium", "High", "Very high"};
            vector<string> explev = {"Disabled", "Only members without roles", "All members"};
            vector<field> fields;
            field server_id;
            server_id.name("ID").value(server.guild_id.gets()).is_inline(true);
            fields.push_back(server_id);
            field owner;
            owner.name("Owner").value("<@!" + server.owner_id.gets() + ">").is_inline(true);
            fields.push_back(owner);
            field membercount;
            membercount.name("Member Count").value(to_string(pre_server.get_member_count())).is_inline(true);
            fields.push_back(membercount);
            field roles;
            roles.name("Roles (" + to_string(server.roles.size() - 1) + ")").value(role_list);
            fields.push_back(roles);
            field channels;
            channels.name("Channels (" + to_string(server.channels.size()) + ")").value(channel_list);
            fields.push_back(channels);
            field creation;
            creation.name("Server creation time").value(to_iso8601(server.guild_id.get_time()) + "\n\n" + date(server.guild_id.get_time())).is_inline(true);
            fields.push_back(creation);
            field region;
            region.name("Region").value(server.region).is_inline(true);
            fields.push_back(region);
            field voicestates;
            voicestates.name("Voice states").value(to_string(server.voice_states.size())).is_inline(true);
            fields.push_back(voicestates);
            field verification;
            verification.name("Server Verification").value(verlevels[server.verification_level]).is_inline(true);
            fields.push_back(verification);
            field explici;
            explici.name("Explicit content filter").value(explev[server.explicit_content_filter]).is_inline(true);
            fields.push_back(explici);
            string featurestext = join(server.features, ", ");
            field features;
            features.name("Features").value(featurestext.empty() ? "*Without features*" : featurestext).is_inline(true);
            fields.push_back(features);
            embed.fields(fields);

            message.channel.create_message(create_message_t().content("Server information").embed(embed));
        }
        if (command == "userinfo")
        {
            snowflake id_user = get_snowflake(args.empty() ? message.msg.get_author_id().gets() : args[0], message.msg.get_guild());
            auto usuario = message.msg.get_guild().find_member(id_user);
            if (usuario == nullptr)
                message.channel.create_message("Invalid user!");
            else
            {
                using namespace gateway::objects;
                embed embed;
                embed.title("Information about " + usuario->get_full_name());
                embed.color(0x21afb4);
                thumbnail avatar;
                if (usuario->get_avatar().empty())
                {
                    avatar.url = "https://cdn.discordapp.com/embed/avatars/" + to_string(usuario->get_discriminator() % 5) + ".png";
                    embed.thumbnail(avatar);
                }
                else
                {
                    avatar.url = "https://cdn.discordapp.com/avatars/" + usuario->get_id().gets() + "/" + usuario->get_avatar() + ".png?size=4096";
                    embed.thumbnail(avatar);
                }
                string voicestatustext;
                vector<field> fields;
                field id;
                id.name("ID").value(usuario->get_id().gets()).is_inline(true);
                fields.push_back(id);
                field is_bot;
                is_bot.name("Bot?").value(usuario->is_bot() ? "Yes" : "No").is_inline(true);
                fields.push_back(is_bot);
                field created;
                created.name("Account create time").value(to_iso8601(usuario->get_id().get_time()) + "\n\n" + date(usuario->get_id().get_time())).is_inline(true);
                fields.push_back(created);
                aegis::user::guild_info member = usuario->get_guild_info(message.channel.get_guild().guild_id);
                voicestatustext += "Muted: " + string(member.mute ? "Yes" : "No") + "\nDeafened: " + string(member.deaf ? "Yes" : "No");
                field nickname;
                nickname.name("Server nick").value(member.nickname.has_value() ? member.nickname.value() : "*none*").is_inline(true);
                fields.push_back(nickname);
                field joined;
                joined.name("Joined time").value(to_iso8601(member.id.get_time()) + "\n\n" + date(member.id.get_time())).is_inline(true);
                fields.push_back(joined);
                field voicestatus;
                voicestatus.name("Voice status").value(voicestatustext).is_inline(true);
                fields.push_back(voicestatus);
                string rolelist;
                for (int i = 0; i < member.roles.size(); i++)
                {
                    if (rolelist.length() > 987)
                        break;
                    if (member.roles[i].gets() == message.channel.get_guild().guild_id.gets())
                        continue;
                    rolelist += "<@&" + member.roles[i].gets() + "> ";
                }
                if (rolelist.empty())
                    rolelist = "*no roles*";
                field rolefield;
                rolefield.name("Roles (" + to_string(member.roles.size() - 1) + ")").value(rolelist);
                fields.push_back(rolefield);
                embed.fields(fields);
                message.channel.create_message(create_message_t().embed(embed));
            }
        }
        if (command == "hola")
        {
            message.channel.create_message("Hola :)");
        }
        if (command == "dm")
        {
            bot.create_dm_message(message.get_user().get_id(), "Hola :)", 0L);
        }
        if (command == "file")
        {
            if (args.empty())
                message.channel.create_message("Usage: `file <name> <args...>`");
            else
            {
                vector<string> final_text = vector_slice(args, 1);
                if (final_text.empty())
                    message.channel.create_message("Put something for your file");
                rest::aegis_file archivo;
                archivo.name = args[0] + ".txt";
                archivo.data = to_char_vector(join(final_text, " "));
                message.channel.create_message(create_message_t().file(archivo));
            }
        }
        if (command == "stats")
        {
            stringstream thing;
            thing << "Time: " << bot.uptime_str() << endl;
            thing << "RAM Usage: " << format_bytes(getCurrentRSS()) << endl;
            thing << "CPU Usage: " << get_cpu_usage().get() << "%" << endl;
            message.channel.create_message(create_message_t().content(thing.str()));
        }
        if (command == "random")
        {
            int64_t i64 = to_number(args.empty() ? "10" : args[0], 4);
            size_t length = i64;
            if (length < 1950)
            {
                if (length > 1)
                {
                    string result = random_string(length);
                    message.channel.create_message(create_message_t().content(result));
                }
                else
                    message.channel.create_message(create_message_t().content("Invalid length"));
            }
            else
                message.channel.create_message(create_message_t().content("Invalid length! Must be 1950 or less"));
        }

        if (command == "reverse")
        {
            if (!(args.empty()))
            {
                string reversed = reverse_string(join(args, " "));
                message.channel.create_message(create_message_t().content(reversed));
            }
            else
                message.channel.create_message(create_message_t().content("Put something"));
        }
        if (command == "say")
        {
            if (!(args.empty()))
            {
                string tosay = join(args, " ");
                message.channel.create_message(create_message_t().content(tosay));
            }
            else
                message.channel.create_message(create_message_t().content("Put something"));
        }
        if (command == "kick")
        {
            auto bot_perms = message.msg.get_guild().perms();
            auto perms = message.msg.get_guild().get_permissions(message.get_user(), message.channel);
            if (perms.can_kick() && bot_perms.can_kick())
            {
                if (args.empty())
                    message.channel.create_message("Mention or put someone's ID.");
                else
                {
                    guild &server = message.msg.get_guild();
                    snowflake userID = get_snowflake(args[0], message.msg.get_guild());
                    if (userID.gets() == "0")
                        message.channel.create_message("Invalid ID!");
                    else
                    {
                        auto promise = server.remove_guild_member(userID);
                        auto res = promise.get();
                        if (res.success())
                            message.channel.create_message("I've kicked that member.");
                        else
                            message.channel.create_message(res.content);
                    }
                }
            }
            else
            {
                message.channel.create_message("You or I don't have permissions.");
            }
        }
        if (command == "ban")
        {
            auto bot_perms = message.msg.get_guild().perms();
            auto perms = message.msg.get_guild().get_permissions(message.get_user(), message.channel);
            if (perms.can_ban() && bot_perms.can_ban())
            {
                if (args.empty())
                    message.channel.create_message("Mention or put someone's ID.");
                else
                {
                    guild &server = message.msg.get_guild();
                    snowflake userID = get_snowflake(args[0], message.msg.get_guild());
                    if (userID.gets() == "0")
                        message.channel.create_message("Invalid ID!");
                    else
                    {
                        vector<string> vector_reason = vector_slice(args, 1);
                        auto promise = server.create_guild_ban(userID, 0, vector_reason.empty() ? "" : join(vector_reason, " "));
                        auto res = promise.get();
                        if (res.success())
                            message.channel.create_message("I've banned that member.");
                        else
                            message.channel.create_message(res.content);
                    }
                }
            }
            else
            {
                message.channel.create_message("You or I don't have permissions.");
            }
        }
        if (command == "purge")
        {
            auto bot_perms = message.channel.perms();
            auto perms = message.msg.get_guild().get_permissions(message.get_user(), message.channel);
            if (perms.can_manage_messages() && bot_perms.can_manage_messages())
            {
                if (args.empty())
                    message.channel.create_message("Usage: purge <number>");
                else
                {
                    int16_t _amount = to_i16(args[0], 3);
                    size_t amount = _amount;
                    if (amount > 100)
                        message.channel.create_message("Only 100 messages at a time");
                    else if (amount < 1)
                        message.channel.create_message("Invalid amount!");
                    else
                    {
                        auto algo = message.msg.delete_message();
                        algo.wait();
                        message.channel.create_message("Mañana hago este comando");
                    }
                }
            }
            else
            {
                message.channel.create_message("You or I don't have permissions.");
            }
        }
        if (command == "ping")
        {
            unique_lock<mutex> lk(m_ping_test);
            checktime = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count();

            auto msg = message.channel.create_message("Pong", checktime).get();
            string to_edit = fmt::format("Ping from REST: {}ms", (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now().time_since_epoch()).count() - checktime));
            msg.edit(to_edit);
            if (cv_ping_test.wait_for(lk, 5s) == std::cv_status::no_timeout)
                msg.edit(fmt::format("{}\n\nPing from WS: {}ms", to_edit, ws_checktime));
            else
                msg.edit(fmt::format("{}\n\nPing from WS: Not available", to_edit));
        }
    }
    catch (aegis::exception const *err)
    {
        string err_string = err->what();
        message.channel.create_message("Error: " + err_string);
        bot.log->error(err_string);
    }
    catch (std::exception const *err)
    {
        string err_string = err->what();
        message.channel.create_message("Error: " + err_string);
        bot.log->error(err_string);
    }
    catch (string err)
    {
        message.channel.create_message(err);
    }
    catch (...)
    {
        message.channel.create_message(create_message_t().content("Some unknown error happened!"));
        bot.log->error("Some unknown error happened");
    }
    return;
}

void andremor::ready(gateway::events::ready obj) noexcept {
    core & bot = *_bot;
    bot.log->info(obj.user.username + "#" + obj.user.discriminator + " is running!");
}