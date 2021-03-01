#include <db.hpp>
#include <cstdint>
#include <iostream>
#include <vector>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;

using namespace mongocxx;

void myDb::set_db(database &db_to_set) noexcept
{
    this->db = &db_to_set;
    return;
}

void myDb::add_note(std::string user_id, std::string note)
{
    if(note.length() > 230) throw "Only 230 characters per note.";
    cursor notes = this->db->collection("notes").find(document{} << "user_id" << open_document << "$eq" << user_id << close_document << finalize);
    int quantity = 0;
    for(auto doc : notes) {
        quantity++;
        if(quantity > 24) break;
    }
    if(quantity >= 25) throw "Only 25 notes per user";
    this->db->collection("notes").insert_one(document{} << "user_id" << user_id << "note" << note << finalize);
    return;
}

void myDb::delete_note(std::string user_id, int to_delete)
{
    if(to_delete < 1) throw "Invalid note number";
    if(to_delete > 25) throw "Invalid note number";
    cursor notes = this->db->collection("notes").find(document{} << "user_id" << open_document << "$eq" << user_id << close_document << finalize);
    int quantity = 0;
    bool deleted = false;
    for(auto doc : notes) {
        quantity++;
        if(quantity == to_delete) {
            this->db->collection("notes").delete_one(document{} << "_id" << open_document << "$eq" << doc["_id"].get_oid().value << close_document << finalize);
            deleted = true;
            break;
        } else if(quantity > to_delete) break;
        else continue;
    }
    if(deleted == true) return;
    else throw "That note doesn't exist";
}

void myDb::delete_all_notes(std::string user_id) {
    this->db->collection("notes").delete_many(document{} << "user_id" << user_id << finalize);
    return;
}

void myDb::edit_note(std::string user_id, int to_edit, std::string note) {
    if(to_edit < 1) throw "Invalid note number";
    if(to_edit > 25) throw "Invalid note number";
    cursor notes = this->db->collection("notes").find(document{} << "user_id" << open_document << "$eq" << user_id << close_document << finalize);
    int quantity = 0;
    bool edited = false;
    for(auto doc : notes) {
        quantity++;
        if(quantity == to_edit) {
            this->db->collection("notes").update_one(document{} << "_id" << open_document << "$eq" << doc["_id"].get_oid().value << close_document << finalize, document{} << "$set" << open_document << "note" << note << close_document << finalize);
            edited = true;
            break;
        } else if(quantity > to_edit) break;
        else continue;
    }
    if(edited == true) return;
    else throw "That note doesn't exist";
}

std::vector<std::string> myDb::get_notes(std::string user_id) {
    cursor notes = this->db->collection("notes").find(document{} << "user_id" << open_document << "$eq" << user_id << close_document << finalize);
    std::vector<std::string> to_send;
    for(auto doc : notes) {
        to_send.push_back(doc["note"].get_utf8().value.to_string());
    }
    return to_send;
}