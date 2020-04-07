#pragma once

#include "utils.h"

namespace mirai
{
    enum class TargetType : bool { friend_, group };

    NLOHMANN_JSON_SERIALIZE_ENUM(TargetType, {
            { TargetType::friend_, "friend" },
            { TargetType::group, "group" }
        });

    enum class Permission { member, administrator, owner };

    NLOHMANN_JSON_SERIALIZE_ENUM(Permission, {
            { Permission::member, "MEMBER" },
            { Permission::administrator, "ADMINISTRATOR" },
            { Permission::owner, "OWNER" },
        });

    struct Group final
    {
        int64_t id = 0;
        std::string name;
        Permission permission{};
    };

    struct Member final
    {
        int64_t id = 0;
        std::string member_name;
        Permission permission{};
        Group group;
    };

    struct Friend final
    {
        int64_t id = 0;
        std::string nickname;
        std::string remark;
    };

    struct GroupConfig final
    {
        std::string name;
        std::string announcement;
        bool confess_talk = false;
        bool allow_member_invite = false;
        bool auto_approve = false;
        bool anonymous_chat = false;
    };

    struct MemberInfo final
    {
        std::string name;
        std::string special_title;
    };

    struct SessionConfig final
    {
        size_t cache_size = 4096;
        bool enable_websocket = false;
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
