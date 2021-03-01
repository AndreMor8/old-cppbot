#ifndef DB_HPP
#define DB_HPP
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>

class myDb
{
public:
    mongocxx::database * db;
    void set_db(mongocxx::database &db_to_set) noexcept;
    void add_note(std::string user_id, std::string note);
    void delete_note(std::string user_id, int to_delete);
    void delete_all_notes(std::string user_id);
    void edit_note(std::string user_id, int to_edit, std::string note);
    std::vector<std::string> get_notes(std::string user_id);
};
#endif