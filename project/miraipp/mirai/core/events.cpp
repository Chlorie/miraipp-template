#include "events.h"

namespace mirai
{
    void from_json(const utils::json& json, GroupMessage& value)
    {
        json.at("messageChain").get_to(value.message);
        json.at("sender").get_to(value.sender);
    }

    void from_json(const utils::json& json, FriendMessage& value)
    {
        json.at("messageChain").get_to(value.message);
        json.at("sender").get_to(value.sender);
    }

    void from_json(const utils::json& json, TempMessage& value)
    {
        json.at("messageChain").get_to(value.message);
        json.at("sender").get_to(value.sender);
    }

    void from_json(const utils::json& json, BotOnlineEvent& value)
    {
        json.at("qq").get_to(value.qq);
    }

    void from_json(const utils::json& json, BotOfflineEventActive& value)
    {
        json.at("qq").get_to(value.qq);
    }

    void from_json(const utils::json& json, BotOfflineEventForce& value)
    {
        json.at("qq").get_to(value.qq);
    }

    void from_json(const utils::json& json, BotOfflineEventDropped& value)
    {
        json.at("qq").get_to(value.qq);
    }

    void from_json(const utils::json& json, BotReloginEvent& value)
    {
        json.at("qq").get_to(value.qq);
    }

    void from_json(const utils::json& json, GroupRecallEvent& value)
    {
        json.at("authorId").get_to(value.author_id);
        json.at("messageId").get_to(value.message_id);
        json.at("time").get_to(value.time);
        json.at("group").get_to(value.group);
        json.at("operator").get_to(value.operator_);
    }

    void from_json(const utils::json& json, FriendRecallEvent& value)
    {
        json.at("authorId").get_to(value.author_id);
        json.at("messageId").get_to(value.message_id);
        json.at("time").get_to(value.time);
        json.at("operator").get_to(value.operator_);
    }

    void from_json(const utils::json& json, BotGroupPermissionChangeEvent& value)
    {
        json.at("origin").get_to(value.origin);
        json.at("current").get_to(value.current);
        json.at("group").get_to(value.group);
    }

    void from_json(const utils::json& json, BotMuteEvent& value)
    {
        json.at("durationSeconds").get_to(value.duration_seconds);
        json.at("operator").get_to(value.operator_);
    }

    void from_json(const utils::json& json, BotUnmuteEvent& value)
    {
        json.at("operator").get_to(value.operator_);
    }

    void from_json(const utils::json& json, BotJoinGroupEvent& value)
    {
        json.at("group").get_to(value.group);
    }

    void from_json(const utils::json& json, GroupNameChangeEvent& value)
    {
        json.at("origin").get_to(value.origin);
        json.at("current").get_to(value.current);
        json.at("group").get_to(value.group);
        json.at("isByBot").get_to(value.is_by_bot);
    }

    void from_json(const utils::json& json, GroupEntranceAnnouncementChangeEvent& value)
    {
        json.at("origin").get_to(value.origin);
        json.at("current").get_to(value.current);
        json.at("group").get_to(value.group);
        json.at("operator").get_to(value.operator_);
    }

    void from_json(const utils::json& json, GroupMuteAllEvent& value)
    {
        json.at("origin").get_to(value.origin);
        json.at("current").get_to(value.current);
        json.at("group").get_to(value.group);
        json.at("operator").get_to(value.operator_);
    }

    void from_json(const utils::json& json, GroupAllowAnonymousChatEvent& value)
    {
        json.at("origin").get_to(value.origin);
        json.at("current").get_to(value.current);
        json.at("group").get_to(value.group);
        json.at("operator").get_to(value.operator_);
    }

    void from_json(const utils::json& json, GroupAllowConfessTalkEvent& value)
    {
        json.at("origin").get_to(value.origin);
        json.at("current").get_to(value.current);
        json.at("group").get_to(value.group);
        json.at("isByBot").get_to(value.is_by_bot);
    }

    void from_json(const utils::json& json, GroupAllowMemberInviteEvent& value)
    {
        json.at("origin").get_to(value.origin);
        json.at("current").get_to(value.current);
        json.at("group").get_to(value.group);
        json.at("operator").get_to(value.operator_);
    }

    void from_json(const utils::json& json, MemberJoinEvent& value)
    {
        json.at("member").get_to(value.member);
    }

    void from_json(const utils::json& json, MemberLeaveEventKick& value)
    {
        json.at("member").get_to(value.member);
        json.at("operator").get_to(value.operator_);
    }

    void from_json(const utils::json& json, MemberLeaveEventQuit& value)
    {
        json.at("member").get_to(value.member);
    }

    void from_json(const utils::json& json, MemberCardChangeEvent& value)
    {
        json.at("origin").get_to(value.origin);
        json.at("current").get_to(value.current);
        json.at("member").get_to(value.member);
        json.at("operator").get_to(value.operator_);
    }

    void from_json(const utils::json& json, MemberSpecialTitleChangeEvent& value)
    {
        json.at("origin").get_to(value.origin);
        json.at("current").get_to(value.current);
        json.at("member").get_to(value.member);
    }

    void from_json(const utils::json& json, MemberPermissionChangeEvent& value)
    {
        json.at("origin").get_to(value.origin);
        json.at("current").get_to(value.current);
        json.at("member").get_to(value.member);
    }

    void from_json(const utils::json& json, MemberMuteEvent& value)
    {
        json.at("durationSeconds").get_to(value.duration_seconds);
        json.at("member").get_to(value.member);
        json.at("operator").get_to(value.operator_);
    }

    void from_json(const utils::json& json, MemberUnmuteEvent& value)
    {
        json.at("member").get_to(value.member);
        json.at("operator").get_to(value.operator_);
    }

    void from_json(const utils::json& json, NewFriendRequestEvent& value)
    {
        json.at("eventId").get_to(value.event_id);
        json.at("fromId").get_to(value.from_id);
        const int64_t group_id = json.at("groupId").get<int64_t>();
        if (group_id != 0) value.group_id = group_id;
        json.at("nick").get_to(value.nick);
    }

    void from_json(const utils::json& json, MemberJoinRequestEvent& value)
    {
        json.at("eventId").get_to(value.event_id);
        json.at("fromId").get_to(value.from_id);
        json.at("groupId").get_to(value.group_id);
        json.at("nick").get_to(value.nick);
        json.at("groupName").get_to(value.group_name);
    }

    namespace
    {
        constexpr std::array<std::string_view, std::variant_size_v<EventVariant>> event_types
        {
            "GroupMessage", "FriendMessage", "TempMessage",
            "BotOnlineEvent", "BotOfflineEventActive", "BotOfflineEventForce", "BotOfflineEventDropped",
            "BotReloginEvent", "GroupRecallEvent", "FriendRecallEvent", "BotGroupPermissionChangeEvent",
            "BotMuteEvent", "BotUnmuteEvent", "BotJoinGroupEvent", "GroupNameChangeEvent",
            "GroupEntranceAnnouncementChangeEvent", "GroupMuteAllEvent", "GroupAllowAnonymousChatEvent",
            "GroupAllowConfessTalkEvent", "GroupAllowMemberInviteEvent", "MemberJoinEvent",
            "MemberLeaveEventKick", "MemberLeaveEventQuit", "MemberCardChangeEvent",
            "MemberSpecialTitleChangeEvent", "MemberPermissionChangeEvent", "MemberMuteEvent",
            "MemberUnmuteEvent", "NewFriendRequestEvent", "MemberJoinRequestEvent"
        };

        template <size_t... I>
        void event_from_json_impl(const utils::json& json, Event& value,
            std::index_sequence<I...>)
        {
            const std::string& type = json.at("type").get_ref<const std::string&>();
            ((type == event_types[I]
                  ? value = Event(json.get<std::variant_alternative_t<I, EventVariant>>())
                  : (void)0), ...);
        }
    }

    void from_json(const utils::json& json, Event& value)
    {
        event_from_json_impl(json, value,
            std::make_index_sequence<std::variant_size_v<EventVariant>>{});
    }
}
