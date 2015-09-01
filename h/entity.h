#pragma once

#include <memory>
#include <string>

namespace micro_kanren {

    struct entity;
    typedef std::shared_ptr<const entity> cptr_entity;

    struct entity {
        enum TYPE {
            VAR,
            INT,
            BOOL,
            PAIR,
            EMPTY_LIST,
            NOTHING
        };

        TYPE const type;

        int const var_val;
        int const int_val;
        bool const bool_val;
        const cptr_entity pair_head;
        const cptr_entity pair_tail;

//        const static std::unordered_map<string, entity::TYPE> name_types;

        entity();

        entity(int val);

        entity(bool val);

        entity(TYPE type, int val);

//    entity(entity const & other);
        entity(std::shared_ptr<const entity> head,
               std::shared_ptr<const entity> tail);

        bool is_var() const;

        bool is_pair() const;

        bool operator==(entity const &other) const;

        std::string to_string() const;
    };

    struct entity_hash {
        std::size_t operator()(cptr_entity k) const;
    };

    struct entity_eq {
        bool operator()(cptr_entity a, cptr_entity b) const;
    };

    cptr_entity entityfy(bool val);

    cptr_entity entityfy(int val);

    cptr_entity entityfy(cptr_entity val);

}; //namespace micro_kanren