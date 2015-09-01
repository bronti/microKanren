#include "state_stream.h"

namespace micro_kanren
{

    state_stream::state_stream()
            : is_empty(true)
              , is_lazy(false)
              , head()
              , tail(nullptr)
              , lazy()
    {
    }

    state_stream::state_stream(state st, cptr_state_stream css)
            : is_empty(false)
              , is_lazy(false)
              , head(st)
              , tail(css)
              , lazy()
    {
    }

    state_stream::state_stream(std::function<cptr_state_stream()> lam)
            : is_empty(false)
              , is_lazy(true)
              , head()
              , tail(nullptr)
              , lazy(lam)
    {
    }

    cptr_state_stream pull(cptr_state_stream css)
    {
        while (css->is_lazy)
        {
            css = css->lazy();
        }
        return css;
    }

    cptr_state_stream add_state(state st, cptr_state_stream str)
    {
        return std::make_shared<state_stream>(st, str);
    }

    substitutes empty_substitutes()
    {
        substitutes res{};
        res.second = true;
        return res;
    }

    state empty_state()
    {
        return state(empty_substitutes(), 0);
    }

    cptr_state_stream empty_stream()
    {
        return std::make_shared<state_stream>();
    }

}; // micro_kanren
