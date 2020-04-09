#pragma once

#include <optional>
#include <variant>
#include "types.h"
#include "message.h"
#include "../utils/variant_wrapper.h"
#include "../utils/json_extensions.h"

namespace mirai // TODO: needs documentation
{
    struct GroupMessage final
    {
        Message message; ///< The message
        Member sender; ///< Sender of the message
    };

    struct FriendMessage final
    {
        Message message;
        Friend sender;
    };

    struct BotOnlineEvent final { int64_t qq = 0; };
    struct BotOfflineEventActive final { int64_t qq = 0; };
    struct BotOfflineEventForce final { int64_t qq = 0; };
    struct BotOfflineEventDropped final { int64_t qq = 0; };
    struct BotReloginEvent final { int64_t qq = 0; };

    struct GroupRecallEvent final
    {
        int64_t author_id = 0;
        int32_t message_id = 0;
        int32_t time = 0;
        Group group;
        std::optional<Member> operator_;
    };

    struct FriendRecallEvent final
    {
        int64_t author_id = 0;
        int32_t message_id = 0;
        int32_t time = 0;
        int64_t operator_ = 0;
    };

    struct BotGroupPermissionChangeEvent final
    {
        Permission origin{};
        Permission current{};
        Group group;
    };

    struct BotMuteEvent final
    {
        int32_t duration_seconds = 0;
        Member operator_;
    };

    struct BotUnmuteEvent final { Member operator_; };
    struct BotJoinGroupEvent final { Group group; };

    struct GroupNameChangeEvent final
    {
        std::string origin;
        std::string current;
        Group group;
        bool is_by_bot = false;
    };

    struct GroupEntranceAnnouncementChangeEvent final
    {
        std::string origin;
        std::string current;
        Group group;
        Member operator_;
    };

    struct GroupMuteAllEvent final
    {
        bool origin = false;
        bool current = false;
        Group group;
        std::optional<Member> operator_;
    };

    struct GroupAllowAnonymousChatEvent final
    {
        bool origin = false;
        bool current = false;
        Group group;
        std::optional<Member> operator_;
    };

    struct GroupAllowConfessTalkEvent final
    {
        bool origin = false;
        bool current = false;
        Group group;
        bool is_by_bot = false;
    };

    struct GroupAllowMemberInviteEvent final
    {
        bool origin = false;
        bool current = false;
        Group group;
        std::optional<Member> operator_;
    };

    struct MemberJoinEvent final { Member member; };

    struct MemberLeaveEventKick final
    {
        Member member;
        std::optional<Member> operator_;
    };

    struct MemberLeaveEventQuit final { Member member; };

    struct MemberCardChangeEvent final
    {
        std::string origin;
        std::string current;
        Member member;
        std::optional<Member> operator_;
    };

    struct MemberSpecialTitleChangeEvent final
    {
        std::string origin;
        std::string current;
        Member member;
    };

    struct MemberPermissionChangeEvent final
    {
        Permission origin{};
        Permission current{};
        Member member;
    };

    struct MemberMuteEvent final
    {
        int32_t duration_seconds = 0;
        Member member;
        std::optional<Member> operator_;
    };

    struct MemberUnmuteEvent final
    {
        int32_t duration_seconds = 0;
        Member member;
        std::optional<Member> operator_;
    };

    void from_json(const utils::json& json, GroupMessage& value);
    void from_json(const utils::json& json, FriendMessage& value);
    void from_json(const utils::json& json, BotOnlineEvent& value);
    void from_json(const utils::json& json, BotOfflineEventActive& value);
    void from_json(const utils::json& json, BotOfflineEventForce& value);
    void from_json(const utils::json& json, BotOfflineEventDropped& value);
    void from_json(const utils::json& json, BotReloginEvent& value);
    void from_json(const utils::json& json, GroupRecallEvent& value);
    void from_json(const utils::json& json, FriendRecallEvent& value);
    void from_json(const utils::json& json, BotGroupPermissionChangeEvent& value);
    void from_json(const utils::json& json, BotMuteEvent& value);
    void from_json(const utils::json& json, BotUnmuteEvent& value);
    void from_json(const utils::json& json, BotJoinGroupEvent& value);
    void from_json(const utils::json& json, GroupNameChangeEvent& value);
    void from_json(const utils::json& json, GroupEntranceAnnouncementChangeEvent& value);
    void from_json(const utils::json& json, GroupMuteAllEvent& value);
    void from_json(const utils::json& json, GroupAllowAnonymousChatEvent& value);
    void from_json(const utils::json& json, GroupAllowConfessTalkEvent& value);
    void from_json(const utils::json& json, GroupAllowMemberInviteEvent& value);
    void from_json(const utils::json& json, MemberJoinEvent& value);
    void from_json(const utils::json& json, MemberLeaveEventKick& value);
    void from_json(const utils::json& json, MemberLeaveEventQuit& value);
    void from_json(const utils::json& json, MemberCardChangeEvent& value);
    void from_json(const utils::json& json, MemberSpecialTitleChangeEvent& value);
    void from_json(const utils::json& json, MemberPermissionChangeEvent& value);
    void from_json(const utils::json& json, MemberMuteEvent& value);
    void from_json(const utils::json& json, MemberUnmuteEvent& value);

    using EventVariant = std::variant<
        GroupMessage, FriendMessage,
        BotOnlineEvent, BotOfflineEventActive, BotOfflineEventForce, BotOfflineEventDropped,
        BotReloginEvent, GroupRecallEvent, FriendRecallEvent, BotGroupPermissionChangeEvent,
        BotMuteEvent, BotUnmuteEvent, BotJoinGroupEvent, GroupNameChangeEvent,
        GroupEntranceAnnouncementChangeEvent, GroupMuteAllEvent, GroupAllowAnonymousChatEvent,
        GroupAllowConfessTalkEvent, GroupAllowMemberInviteEvent, MemberJoinEvent,
        MemberLeaveEventKick, MemberLeaveEventQuit, MemberCardChangeEvent, MemberSpecialTitleChangeEvent,
        MemberPermissionChangeEvent, MemberMuteEvent, MemberUnmuteEvent
    >;

    enum class EventType
    {
        group_message, friend_message,
        bot_online_event, bot_offline_event_active, bot_offline_event_force, bot_offline_event_dropped,
        bot_relogin_event, group_recall_event, friend_recall_event, bot_group_permission_change_event,
        bot_mute_event, bot_unmute_event, bot_join_group_event, group_name_change_event,
        group_entrance_announcement_change_event, group_mute_all_event, group_allow_anonymous_chat_event,
        group_allow_confess_talk_event, group_allow_member_invite_event, member_join_event,
        member_leave_event_kick, member_leave_event_quit, member_card_change_event,
        member_special_title_change_event, member_permission_change_event, member_mute_event,
        member_unmute_event
    };

    /**
     * \brief The event type containing every kind of event,
     * plus the two message receiving "events"
     */
    using Event = utils::VariantWrapper<EventVariant, EventType>;

    void from_json(const utils::json& json, Event& value);
}
