#include "session.h"
#include "utils.h"

namespace mirai
{
    void Session::release() const noexcept
    {
        if (qq_ == 0) return; // Moved-from or default init
        try
        {
            utils::post_json("/release", {
                    { "sessionKey", key_ },
                    { "qq", qq_ }
                });
        }
        catch (...) { std::abort(); }
    }

    Session::Session(const std::string_view auth_key, const int64_t qq)
    {
        // Authorize
        {
            const auto res = utils::post_json("/auth", {
                    { "authKey", std::string(auth_key) }
                });
            utils::check_response(res);
            res["session"].get_to(key_);
        }
        // Verify
        {
            const auto res = utils::post_json("/verify", {
                    { "sessionKey", key_ },
                    { "qq", qq }
                });
            utils::check_response(res);
            qq_ = qq; // QQ ID set (not 0) means the initialization has completed
        }
    }

    Session::~Session() noexcept { release(); }

    Session::Session(Session&& other) noexcept :
        qq_(std::exchange(other.qq_, 0)),
        key_(std::move(other.key_)) {}

    Session& Session::operator=(Session&& other) noexcept
    {
        release();
        qq_ = std::exchange(other.qq_, 0);
        key_ = std::move(other.key_);
        return *this;
    }

    int32_t Session::send_friend_message(const int64_t target,
        const Message& msg, const OptionalParam<int32_t> quote) const
    {
        utils::json json{
            { "sessionKey", key_ },
            { "target", target },
            { "messageChain", msg }
        };
        if (quote.has_value()) json["quote"] = *quote;
        const auto res = utils::post_json("/sendFriendMessage", json);
        utils::check_response(res);
        return res["messageId"].get<int32_t>();
    }

    int32_t Session::send_friend_message(const int64_t target,
        const std::string_view msg, const OptionalParam<int32_t> quote) const
    {
        return send_friend_message(target, plain_text(msg), quote);
    }

    int32_t Session::send_group_message(const int64_t target,
        const Message& msg, const OptionalParam<int32_t> quote) const
    {
        utils::json json{
            { "sessionKey", key_ },
            { "target", target },
            { "messageChain", msg }
        };
        if (quote.has_value()) json["quote"] = *quote;
        const auto res = utils::post_json("/sendGroupMessage", json);
        utils::check_response(res);
        return res["messageId"].get<int32_t>();
    }

    int32_t Session::send_group_message(const int64_t target,
        const std::string_view msg, const OptionalParam<int32_t> quote) const
    {
        return send_group_message(target, plain_text(msg), quote);
    }
}
