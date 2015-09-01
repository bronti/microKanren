#pragma once

#include <unordered_map>
#include <memory>

#include "entity.h"

namespace micro_kanren
{

// true -> valid state; false -> substitutes unmeetable (eg after unify 9 and 8)
    typedef std::pair<std::unordered_map<cptr_entity, cptr_entity, entity_hash, entity_eq>, bool> substitutes;
    typedef std::pair<substitutes, unsigned> state;

    struct state_stream;
    typedef std::shared_ptr<const state_stream> cptr_state_stream;

    struct state_stream
    {
        const bool is_empty;
        const bool is_lazy;
        const state head;
        const std::shared_ptr<const state_stream> tail;
        std::function<cptr_state_stream()> lazy;

        state_stream();

        state_stream(state st, cptr_state_stream css);

        state_stream(std::function<cptr_state_stream()> lam);
    };

    cptr_state_stream pull(cptr_state_stream css);

    cptr_state_stream add_state(state st, cptr_state_stream str);

    substitutes empty_substitutes();

    state empty_state();

    cptr_state_stream empty_stream();

}; // namespace mikro_kanren
