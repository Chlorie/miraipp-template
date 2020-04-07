#pragma once

#include <vector>
#include <string>
#include <variant>
#include "utils.h"

namespace mirai
{
    class MessageChainNode;
    using Message = std::vector<MessageChainNode>;

    namespace msg
    {
        struct Source final
        {
            int32_t id = 0;
            int32_t time = 0;
        };

        struct Quote final
        {
            int32_t id = 0;
            int64_t group_id = 0;
            int64_t sender_id = 0;
            int64_t target_id = 0;
            Message origin;
        };

        struct At final
        {
            int32_t target = 0;
            std::string display;
        };

        struct AtAll final {};

        struct Face final
        {
            std::optional<int32_t> face_id = 0;
            std::optional<std::string> name;
        };

        struct Plain final { std::string text; };

        struct Image final
        {
            std::optional<std::string> image_id;
            std::optional<std::string> url;
            std::optional<std::string> path;
        };

        struct FlashImage final
        {
            std::optional<std::string> image_id;
            std::optional<std::string> url;
            std::optional<std::string> path;
        };

        struct Xml final { std::string xml; };
        struct Json final { std::string json; };
        struct App final { std::string content; };
        struct Poke final { std::string name; };

        void to_json(utils::json& json, const Source& value);
        void from_json(const utils::json& json, Source& value);
        void to_json(utils::json& json, const Quote& value);
        void from_json(const utils::json& json, Quote& value);
        void to_json(utils::json& json, const At& value);
        void from_json(const utils::json& json, At& value);
        void to_json(utils::json& json, const AtAll& value);
        void from_json(const utils::json& json, AtAll& value);
        void to_json(utils::json& json, const Face& value);
        void from_json(const utils::json& json, Face& value);
        void to_json(utils::json& json, const Plain& value);
        void from_json(const utils::json& json, Plain& value);
        void to_json(utils::json& json, const Image& value);
        void from_json(const utils::json& json, Image& value);
        void to_json(utils::json& json, const FlashImage& value);
        void from_json(const utils::json& json, FlashImage& value);
        void to_json(utils::json& json, const Xml& value);
        void from_json(const utils::json& json, Xml& value);
        void to_json(utils::json& json, const Json& value);
        void from_json(const utils::json& json, Json& value);
        void to_json(utils::json& json, const App& value);
        void from_json(const utils::json& json, App& value);
        void to_json(utils::json& json, const Poke& value);
        void from_json(const utils::json& json, Poke& value);

        using Variant = std::variant<Source, Quote, At, AtAll, Face, Plain,
            Image, FlashImage, Xml, Json, App, Poke>;
    }

    class MessageChainNode final
    {
    private:
        msg::Variant data_;
    public:
        enum class Type
        {
            source, quote, at, at_all, face, plain,
            image, flash_image, xml, json, app, poke
        };
        template <typename F, typename D>
        using enable_if_visitable_t = std::void_t<decltype(std::visit(std::declval<F&&>(), std::declval<D>()))>;
    public:
        MessageChainNode() = default;
        MessageChainNode(const msg::Variant& variant) :data_(variant) {}
        MessageChainNode(msg::Variant&& variant) :data_(std::move(variant)) {}
        MessageChainNode& operator=(const msg::Variant& variant)
        {
            data_ = variant;
            return *this;
        }
        MessageChainNode& operator=(msg::Variant&& variant)
        {
            data_ = std::move(variant);
            return *this;
        }

        Type type() const { return Type(data_.index()); }
        template <Type T> auto& get() { return std::get<size_t(T)>(data_); }
        template <Type T> const auto& get() const { return std::get<size_t(T)>(data_); }
        template <typename T> T& get() { return std::get<T>(data_); }
        template <typename T> const T& get() const { return std::get<T>(data_); }

        msg::Variant& data() { return data_; }
        const msg::Variant& data() const { return data_; }

        template <typename F, typename = enable_if_visitable_t<F, msg::Variant&>>
        decltype(auto) apply(F&& func)
        {
            return std::visit(std::forward<F>(func), data_);
        }
        template <typename F, typename = enable_if_visitable_t<F, const msg::Variant&>>
        decltype(auto) apply(F&& func) const
        {
            return std::visit(std::forward<F>(func), data_);
        }
    };

    /**
     * \brief Construct a plain text message object
     * \param text The message content
     * \return The message
     */
    Message plain_text(std::string_view text);

    void to_json(utils::json& json, const MessageChainNode& value);
    void from_json(const utils::json& json, MessageChainNode& value);
}
