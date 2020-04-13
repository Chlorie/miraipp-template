#pragma once

#include <optional>
#include <variant>
#include "types.h"
#include "message.h"
#include "../utils/variant_wrapper.h"
#include "../utils/json_extensions.h"

namespace mirai // TODO: finish the documentation
{
    /**
     * \brief Event for receiving a group message
     */
    struct GroupMessage final
    {
        Message message; ///< The message
        Member sender; ///< Sender of the message
    };

    /**
     * \brief Event for receiving a friend message
     */
    struct FriendMessage final
    {
        Message message; ///< The messgae
        Friend sender;
    };

    /**
     * \brief Event received when the bot goes online
     */
    struct BotOnlineEvent final
    {
        int64_t qq = 0; ///< QQ of the bot
    };

    /**
     * \brief Event received when the bot gets offline actively
     */
    struct BotOfflineEventActive final
    {
        int64_t qq = 0; ///< QQ of the bot
    };

    /**
     * \brief Event received when the bot is forced offline
     */
    struct BotOfflineEventForce final
    {
        int64_t qq = 0; ///< QQ of the bot
    };

    /**
     * \brief Event received when the bot is disconnected
     */
    struct BotOfflineEventDropped final
    {
        int64_t qq = 0; ///< QQ of the bot
    };

    /**
     * \brief Event received when the bot actively re-login
     */
    struct BotReloginEvent final
    {
        int64_t qq = 0; ///< QQ of the bot
    };

    /**
     * \brief Event received when a group message is recalled by someone
     */
    struct GroupRecallEvent final
    {
        int64_t author_id = 0; ///< The sender of the recalled message
        int32_t message_id = 0; ///< The ID of the message
        int32_t time = 0; ///< Timestamp when the message is sent
        Group group; ///< The group in which the message is recalled
        std::optional<Member> operator_; ///< The operator who recalled the message, null if it's the bot
    };

    /**
     * \brief Event received when a friend message is recalled by someone
     */
    struct FriendRecallEvent final
    {
        int64_t author_id = 0; ///< The sender of the recalled message
        int32_t message_id = 0; ///< The ID of the message
        int32_t time = 0; ///< Timestamp when the message is sent
        int64_t operator_ = 0; ///< QQ of the operator who recalled the message
    };

    /**
     * \brief Event received when the bot's permission is changed in some group
     * \remarks The operator can only be the group owner
     */
    struct BotGroupPermissionChangeEvent final
    {
        Permission origin{}; ///< The original permission
        Permission current{}; ///< The permission now
        Group group; ///< The group in which the bot's permission is changed
    };

    /**
     * \brief Event received when the bot is muted in some group
     */
    struct BotMuteEvent final
    {
        int32_t duration_seconds = 0; ///< The duration of the mute
        Member operator_; ///< The operator who muted the bot
    };

    /**
     * \brief Event received when the bot is unmuted in some group
     */
    struct BotUnmuteEvent final
    {
        Member operator_; ///< The operator who unmuted the bot
    };

    /**
     * \brief Event received when the bot joins any group
     */
    struct BotJoinGroupEvent final
    {
        Group group; ///< The group that the bot joined
    };

    /**
     * \brief Event received when some group's name is changed
     */
    struct GroupNameChangeEvent final
    {
        std::string origin; ///< The original group name
        std::string current; ///< The group name now
        Group group; ///< The group of which name is changed
        bool is_by_bot = false; ///< Whether it's the bot who changed the name
    };

    struct GroupEntranceAnnouncementChangeEvent final
    {
        std::string origin;
        std::string current;
        Group group;
        std::optional<Member> operator_;
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

    struct MemberJoinEvent final
    {
        Member member;
    };

    struct MemberLeaveEventKick final
    {
        Member member;
        std::optional<Member> operator_;
    };

    struct MemberLeaveEventQuit final
    {
        Member member;
    };

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

    using MessageEventVariant = std::variant<GroupMessage, FriendMessage>;
    using NonMessageEventVariant = std::variant<
        BotOnlineEvent, BotOfflineEventActive, BotOfflineEventForce, BotOfflineEventDropped,
        BotReloginEvent, GroupRecallEvent, FriendRecallEvent, BotGroupPermissionChangeEvent,
        BotMuteEvent, BotUnmuteEvent, BotJoinGroupEvent, GroupNameChangeEvent,
        GroupEntranceAnnouncementChangeEvent, GroupMuteAllEvent, GroupAllowAnonymousChatEvent,
        GroupAllowConfessTalkEvent, GroupAllowMemberInviteEvent, MemberJoinEvent,
        MemberLeaveEventKick, MemberLeaveEventQuit, MemberCardChangeEvent,
        MemberSpecialTitleChangeEvent, MemberPermissionChangeEvent, MemberMuteEvent, MemberUnmuteEvent
    >;

    using EventVariant = utils::apply_template_t<std::variant, utils::concat_list_t<
        utils::extract_types_t<MessageEventVariant>, utils::extract_types_t<NonMessageEventVariant>>>;

    /**
     * \brief Enum corresponding to every type of an event, including the two message events
     */
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
