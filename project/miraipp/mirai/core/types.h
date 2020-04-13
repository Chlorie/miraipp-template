#pragma once

#include "../utils/json_extensions.h"

namespace mirai // TODO: needs documentation
{
    /**
     * \brief Whether the target is friend or group
     */
    enum class TargetType : bool { friend_, group };

    NLOHMANN_JSON_SERIALIZE_ENUM(TargetType, {
        { TargetType::friend_, "friend" },
        { TargetType::group, "group" }
        });

    /**
     * \brief Permission of a group member
     */
    enum class Permission { member, administrator, owner };

    NLOHMANN_JSON_SERIALIZE_ENUM(Permission, {
        { Permission::member, "MEMBER" },
        { Permission::administrator, "ADMINISTRATOR" },
        { Permission::owner, "OWNER" },
        });

    /**
     * \brief Information about a group
     */
    struct Group final
    {
        int64_t id = 0; ///< ID of the group
        std::string name; ///< Name of the group
        Permission permission{}; ///< The permission of the bot in the group
    };

    /**
     * \brief Information about a group member
     */
    struct Member final
    {
        int64_t id = 0; ///< ID of the member
        std::string member_name; ///< Name of the member
        Permission permission{}; ///< The permission of the group member
        Group group; ///< Information about the group
    };

    /**
     * \brief Information about a friend
     */
    struct Friend final
    {
        int64_t id = 0; ///< ID of the friend
        std::string nickname; ///< Nickname of the friend
        std::string remark; ///< Remark of the friend
    };

    /**
     * \brief Group configuration
     */
    struct GroupConfig final
    {
        std::string name; ///< The group name
        std::string announcement; ///< The group entrance announcement
        bool confess_talk = false; ///< Whether to enable confess talk
        bool allow_member_invite = false; ///< Whether to allow inviting other people
        bool auto_approve = false; ///< Whether to approve join requests automatically
        bool anonymous_chat = false; ///< Whether to enable anonymous chat
    };

    /**
     * \brief Other information about a group member
     */
    struct MemberInfo final
    {
        std::string name; ///< Name of the member
        std::string special_title; ///< Special title of the member
    };

    /**
     * \brief Session configuration
     */
    struct SessionConfig final
    {
        size_t cache_size = 4096; ///< Cache size of the session
        bool enable_websocket = false; ///< Whether websocket is enabled for this session
    };

    void from_json(const utils::json& json, Group& value);
    void from_json(const utils::json& json, Member& value);
    void from_json(const utils::json& json, Friend& value);

    void to_json(utils::json& json, const GroupConfig& value);
    void from_json(const utils::json& json, GroupConfig& value);
    void to_json(utils::json& json, const MemberInfo& value);
    void from_json(const utils::json& json, MemberInfo& value);
    void to_json(utils::json& json, const SessionConfig& value);
    void from_json(const utils::json& json, SessionConfig& value);
}
