#pragma once

#include <numeric>
#include <vector>
#include <initializer_list>

#include "entity.h"
#include "state_stream.h"

namespace micro_kanren
{
    typedef std::function<cptr_state_stream(state const &)> goal;

    cptr_state_stream fail_goal(state const &st);

    cptr_state_stream success_goal(state const &st);

////////////////////////////////////////////////////////////////////////////////////////////////////////////

    cptr_entity walk(cptr_entity var, substitutes const &subst);

    substitutes extern_substitutes(cptr_entity first,
                                   cptr_entity second,
                                   substitutes subst);

    substitutes unify(cptr_entity first, cptr_entity second, substitutes subst);

    template<class L, class R>
    goal equal_equal(L l, R r)
    {
        cptr_entity left = entityfy(l);
        cptr_entity right = entityfy(r);

        return goal([left, right](state const &sc) -> cptr_state_stream
                    {
                        auto subst = unify(left, right, sc.first);
                        if (!subst.second)
                        {
                            return empty_stream();
                        }
                        return add_state(state(subst, sc.second), empty_stream());
                    });
    }

    cptr_state_stream stream_plus(cptr_state_stream s1, cptr_state_stream s2);

    cptr_state_stream stream_bind(cptr_state_stream s, goal g);

    goal disj(goal g1, goal g2);

    goal conj(goal g1, goal g2);

///////////////////////////////////////////////////////////////////////////////////////////

    goal conde(std::initializer_list<std::initializer_list<goal>> list_list);

    goal call_fresh(std::function<goal(cptr_entity)> f);

/////////////////////////////////// sugar ////////////////////////////////////////////////

    cptr_state_stream call_empty_state(goal g);

    std::vector<state> take_all(cptr_state_stream css);

    std::vector<state> take_n(cptr_state_stream css, int n);

    std::string state_to_str(state const &s);

}; //namespace micro_kanren