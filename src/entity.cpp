#include "entity.h"
#include <cassert>

namespace micro_kanren
{
    entity::entity()
            : type(NOTHING)
              , var_val(0)
              , int_val(0)
              , bool_val(false)
              , pair_head(nullptr)
              , pair_tail(nullptr)
    {
    }

    entity::entity(int val)
            : type(INT)
              , var_val(0)
              , int_val(val)
              , bool_val(false)
              , pair_head(nullptr)
              , pair_tail(nullptr)
    {
    }

    entity::entity(bool val)
            : type(BOOL)
              , var_val(0)
              , int_val(0)
              , bool_val(val)
              , pair_head(nullptr)
              , pair_tail(nullptr)
    {
    }

    entity::entity(TYPE type, int val)
            : type(type)
              , var_val(val)
              , int_val(0)
              , bool_val(false)
              , pair_head(nullptr)
              , pair_tail(nullptr)
    {
        assert((type == VAR));
    }

//    entity(entity const & other)
//            : type(other.type)
//            , var_val(other.var_val)
//            , int_val(other.int_val)
//            , bool_val(other.bool_val)
//            , pair_head(other.pair_head)
//            , pair_tail(other.pair_tail)
//    {
//    }

    entity::entity(std::shared_ptr<const entity> head,
                   std::shared_ptr<const entity> tail
    )
            : type(PAIR)
              , var_val(0)
              , int_val(0)
              , bool_val(0)
              , pair_head(head)
              , pair_tail(tail)
    {
    }

    bool entity::is_var() const
    {
        return type == VAR;
    }

    bool entity::is_pair() const
    {
        return type == PAIR;
    }

    bool entity::operator==(entity const &other) const
    {
        return ((type == other.type)
                and (var_val == other.var_val)
                and (int_val == other.int_val)
                and (bool_val == other.bool_val)
                and ((not pair_head && not other.pair_head)
                     || (pair_head
                         && other.pair_head
                         && (*pair_head == *other.pair_head)))
                and ((not pair_tail && not other.pair_tail)
                     || (pair_tail
                         && other.pair_tail
                         && (*pair_tail == *other.pair_tail))));
    }

    std::string entity::to_string() const
    {
        std::string val = "";
        switch (type)
        {
            case VAR:
            {
                return " (VAR " + std::to_string(var_val) + ") ";
            }
            case INT:
            {
                return " " + std::to_string(int_val) + " ";
            }
            case BOOL:
            {
                return bool_val ? " true " : " false ";
            }
            case PAIR:
            {
                return " (" + pair_head->to_string() + ", " + pair_tail->to_string() + ") ";
            }
            default:
            {
                return "";
            }
        }
    }


//    const std::unordered_map<string, entity::TYPE> entity::name_types = {{"i", entity::INT},
//                                                                         {"b", entity::BOOL}};
//{"6entity", entity::VAR}};

    std::size_t entity_hash::operator()(cptr_entity k) const
    {
        using std::hash;
        if (!k)
        {
            return 0;
        }
        else
        {
            return ((hash<int>()(k->type) ^ (hash<int>()(k->var_val) << 1)) >> 1)
                   ^ ((hash<int>()(k->int_val) ^ (hash<bool>()(k->bool_val) << 1)) >> 1)
                   ^ ((entity_hash()(k->pair_head) ^ (entity_hash()(k->pair_tail) << 1)) >> 1);
        }
    }

    bool entity_eq::operator()(cptr_entity a, cptr_entity b) const
    {
        return *a == *b;
    }


cptr_entity entityfy(bool val)
{
    return std::make_shared<entity>(val);
}

cptr_entity entityfy(int val)
{
    return std::make_shared<entity>(val);
}

cptr_entity entityfy(cptr_entity val)
{
    return val;
}
}; //namespace micro_kanren