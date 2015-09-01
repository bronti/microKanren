#include <cassert>
#include <functional>
#include <string>

#include "entity.h"
#include "state_stream.h"
#include "micro_kanren.h"

namespace micro_kanren
{
    typedef std::function<cptr_state_stream(state const &)> goal;

    cptr_state_stream fail_goal(state const &st)
    {
        return empty_stream();
    }

    cptr_state_stream success_goal(state const &st)
    {
        return add_state(st, empty_stream());
    }

////////////////////////////////////////////////////////////////////////////////////////////////////////////


    cptr_entity walk(cptr_entity var, substitutes const &subst)
    {
        if (var->is_var() && (subst.first.count(var) > 0))
        {
            return subst.first.at(var);
        }
        else
        {
            return var;
        }
    }

    substitutes extern_substitutes(cptr_entity first, cptr_entity second, substitutes subst)
    {
        subst.first[first] = second;
        return subst;
    }

    substitutes unify(cptr_entity first, cptr_entity second, substitutes subst)
    {
        assert(subst.second);
        cptr_entity first_val = walk(first, subst);
        cptr_entity second_val = walk(second, subst);
        if (first_val->is_var() && second_val->is_var() && (*first_val == *second_val))
        {
            return subst;
        }
        else if (first_val->is_var())
        {
            return extern_substitutes(first_val, second_val, subst);
        }
        else if (second_val->is_var())
        {
            return extern_substitutes(second_val, first_val, subst);
        }
        else if (first_val->is_pair() && second_val->is_pair())
        {
            return unify(first_val->pair_tail,
                         second_val->pair_tail,
                         unify(first_val->pair_head, second_val->pair_head, subst));
        }
        else if (*first_val == *second_val)
        {
            return subst;
        }
        subst.first.clear();
        subst.second = false;
        return subst;
    }

    cptr_state_stream stream_plus(cptr_state_stream s1, cptr_state_stream s2)
    {
        if (s1->is_lazy)
        {
            return std::make_shared<state_stream>([s1, s2]() { return stream_plus(s2, s1->lazy()); });
        }
        if (s1->is_empty || s2->is_empty)
        {
            return s1->is_empty ? s2 : s1;
        }
        return add_state(s1->head, stream_plus(s2, s1->tail));
    }

    cptr_state_stream stream_bind(cptr_state_stream s, goal g)
    {
        if (s->is_empty)
        {
            return empty_stream();
        }
        if (s->is_lazy)
        {
            return std::make_shared<state_stream>([s, g]() { return stream_bind(s->lazy(), g); });
        }
        return stream_plus(g(s->head), stream_bind(s->tail, g));
    }

    goal disj(goal g1, goal g2)
    {
        return goal([g1, g2](state const &sc) -> cptr_state_stream
                    {
                        return stream_plus(g1(sc), g2(sc));
                    });
    }

    goal conj(goal g1, goal g2)
    {
        return goal([g1, g2](state const &sc) -> cptr_state_stream
                    {
                        return stream_bind(g1(sc), g2);
                    });
    }

///////////////////////////////////////////////////////////////////////////////////////////

    goal conde(std::initializer_list<std::initializer_list<goal>> list_list)
    {
        return std::accumulate(
                list_list.begin(),
                list_list.end(),
                goal(fail_goal),
                [](goal acc, std::initializer_list<goal> list) -> goal
                {
                    return disj(acc, std::accumulate(
                            list.begin(),
                            list.end(),
                            goal(success_goal),
                            &conj
                    ));
                });
    }

    goal call_fresh(std::function<goal(cptr_entity)> f)
    {
        return goal([f](state const &sc) -> cptr_state_stream
                    {
                        auto var = std::make_shared<entity>(entity::VAR, sc.second);
                        goal g = f(var);
                        return g(state(sc.first, sc.second + 1));
                    });
    }

/////////////////////////////////// sugar ////////////////////////////////////////////////

    cptr_state_stream call_empty_state(goal g)
    {
        return g(empty_state());
    }

    std::vector<state> take_all(cptr_state_stream css)
    {
        std::vector<state> res{};
        while (!css->is_empty)
        {
            auto pulled = pull(css);
            res.push_back(pulled->head);
            css = pulled->tail;
        }
        return res;
    }

    std::vector<state> take_n(cptr_state_stream css, int n)
    {
        std::vector<state> res{};
        for (int i = 0; (i < n) && (!css->is_empty); ++i)
        {
            auto pulled = pull(css);
            res.push_back(pulled->head);
            css = pulled->tail;
        }
        return res;
    }

    std::string state_to_str(state const &s)
    {
        if (!s.first.second)
        {
            return "invalid";
        }
        if (s.first.first.empty())
        {
            return "( )";
        }
        std::string res = "";
        for (auto p: s.first.first)
        {
            res += "(" + p.first->to_string() + " == " + p.second->to_string() + ") ";
        }
        return res;
    }

}; //namespace micro_kanren