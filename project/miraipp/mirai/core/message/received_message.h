#pragma once

#include "segment.h"
#include "stringified_message.h"

namespace mirai
{
    /**
     * \brief Represents a message received from others (group or friend)
     */
    struct ReceivedMessage final
    {
        msg::Source source; ///< Source of the message
        std::optional<msg::Quote> quote; ///< If exists, quotation of the message
        StringifiedMessage content; ///< The real message content
    };

    void from_json(const utils::json& json, ReceivedMessage& value);
}
