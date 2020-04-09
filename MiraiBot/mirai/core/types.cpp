#include "types.h"

namespace mirai
{
    void from_json(const utils::json& json, Group& value)
    {
        json["id"].get_to(value.id);
        json["name"].get_to(value.name);
        json["permission"].get_to(value.permission);
    }

    void from_json(const utils::json& json, Member& value)
    {
        json["id"].get_to(value.id);
        json["memberName"].get_to(value.member_name);
        json["permission"].get_to(value.permission);
        json["group"].get_to(value.group);
    }

    void from_json(const utils::json& json, Friend& value)
    {
        json["id"].get_to(value.id);
        json["nickname"].get_to(value.nickname);
        json["remark"].get_to(value.remark);
    }

    void to_json(utils::json& json, const GroupConfig& value)
    {
        json = {
            { "name", value.name },
            { "announcement", value.announcement },
            { "confessTalk", value.confess_talk },
            { "allowMemberInvite", value.allow_member_invite },
            { "autoApprove", value.auto_approve },
            { "anonymousChat", value.anonymous_chat }
        };
    }

    void from_json(const utils::json& json, GroupConfig& value)
    {
        json["name"].get_to(value.name);
        json["announcement"].get_to(value.announcement);
        json["confessTalk"].get_to(value.confess_talk);
        json["allowMemberInvite"].get_to(value.allow_member_invite);
        json["autoApprove"].get_to(value.auto_approve);
        json["anonymousChat"].get_to(value.anonymous_chat);
    }

    void to_json(utils::json& json, const MemberInfo& value)
    {
        json = {
            { "name", value.name },
            { "specialTitle", value.special_title }
        };
    }

    void from_json(const utils::json& json, MemberInfo& value)
    {
        json["name"].get_to(value.name);
        json["specialTitle"].get_to(value.special_title);
    }

    void to_json(utils::json& json, const SessionConfig& value)
    {
        json = {
            { "cacheSize", value.cache_size },
            { "enableWebsocket", value.enable_websocket }
        };
    }

    void from_json(const utils::json& json, SessionConfig& value)
    {
        json["cacheSize"].get_to(value.cache_size);
        json["enableWebsocket"].get_to(value.enable_websocket);
    }
}
