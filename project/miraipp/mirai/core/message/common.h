#pragma once

#include "segment.h"

// This header file is not API

namespace mirai
{
    constexpr std::array<std::string_view, std::variant_size_v<msg::Variant>> msg_types
    {
        "At", "AtAll", "Face", "Plain", "Image",
        "FlashImage", "Xml", "Json", "App", "Poke"
    };

    inline bool is_plain(const Segment& node) { return node.type() == SegmentType::plain; }

    inline std::string& get_plain(Segment& node) { return node.get<msg::Plain>().text; }
    inline const std::string& get_plain(const Segment& node) { return node.get<msg::Plain>().text; }

    void combine_adjacent_text(MessageChain& chain);
}
