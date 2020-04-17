#include "types.h"

namespace mirai
{
    void from_json(const utils::json& json, Group& value)
    {
        json.at("id").get_to(value.id);
        json.at("name").get_to(value.name);
        json.at("permission").get_to(value.permission);
    }

    void from_json(const utils::json& json, Member& value)
    {
        json.at("id").get_to(value.id);
        json.at("memberName").get_to(value.member_name);
        json.at("permission").get_to(value.permission);
        json.at("group").get_to(value.group);
    }

    void from_json(const utils::json& json, Friend& value)
    {
        json.at("id").get_to(value.id);
        json.at("nickname").get_to(value.nickname);
        json.at("remark").get_to(value.remark);
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
        json.at("name").get_to(value.name);
        json.at("announcement").get_to(value.announcement);
        json.at("confessTalk").get_to(value.confess_talk);
        json.at("allowMemberInvite").get_to(value.allow_member_invite);
        json.at("autoApprove").get_to(value.auto_approve);
        json.at("anonymousChat").get_to(value.anonymous_chat);
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
        json.at("name").get_to(value.name);
        json.at("specialTitle").get_to(value.special_title);
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
        json.at("cacheSize").get_to(value.cache_size);
        json.at("enableWebsocket").get_to(value.enable_websocket);
    }
}
