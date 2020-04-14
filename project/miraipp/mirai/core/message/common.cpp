#include "common.h"

namespace mirai
{
    void combine_adjacent_text(MessageChain& chain)
    {
        size_t offset = 0;
        while (offset < chain.size())
        {
            const auto first = std::find_if(chain.begin() + offset, chain.end(), is_plain);
            if (first == chain.end()) return;
            const auto next = std::next(first);
            const auto last = std::find_if_not(next, chain.end(), is_plain);
            offset = size_t(last - chain.begin());
            std::string& text = get_text(*first);
            for (auto iter = next; iter != last; ++iter) text += get_text(*iter);
            chain.erase(next, last);
        }
    }
}
