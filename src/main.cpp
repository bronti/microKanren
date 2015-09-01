#include "micro_kanren.h"

#include <iostream>

using namespace micro_kanren;

std::string stream_to_str(cptr_state_stream s)
{
    std::string res = "";
    if (s->is_empty)
    {
        return "empty stream";
    }
    while (!s->is_empty)
    {
        res += state_to_str(s->head) + "\n";
        s = s->tail;
    }
    return res;
}

goal fives(cptr_entity x)
{
    return disj(
            equal_equal(x, 5),
            [x](state st) -> cptr_state_stream
            {
                return std::make_shared<state_stream>([st, x]()
                                                      {
                                                          return (fives(x))(st);
                                                      });

            });
}



int main()
{
    using std::cout;
    using std::endl;

    //assert(stream_to_str(call_empty_state(equal_equal(1, 1))) == "empty stream");
    //assert(stream_to_str(call_empty_state(equal_equal(9, std::make_shared<entity>(entity::VAR, 0))))
    //       == "( (VAR 0)  ==  9 ) \n");

//    cout << stream_to_str(
//                call_empty_state(
//                    call_fresh(
//                            [](cptr_entity x) {
//                                return conde({{equal_equal(true, x)},
//                                              {equal_equal(false, false)}});
//                            }
//                    ))) << endl;




    for (auto v: take_n(call_empty_state(call_fresh(fives)), 5))
    {
        cout << state_to_str(v) << endl;
    }
    return 0;
}

