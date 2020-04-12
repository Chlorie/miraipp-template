#pragma once

#include <string>

namespace mirai::utils
{
    /**
     * \brief Convert encoding of text
     * \param text The text to be converted in source encoding
     * \param src_enc A C-string describing the source encoding
     * \param dst_enc A C-string describing the target encoding
     * \param capability_factor The maximum ratio of byte-length expansion
     * \return Converted text, in target encoding
     */
    std::string convert_encoding(std::string_view text,
        const char* src_enc, const char* dst_enc, float capability_factor = 2.0f);

    /**
     * \brief Convert text from local encoding to UTF-8
     * \param text The text to be converted in local encoding
     * \return Converted text, in UTF-8
     */
    std::string local_to_utf8(std::string_view text);

    /**
     * \brief Convert text from UTF-8 to local encoding
     * \param text The text to be converted in UTF-8
     * \return Converted text, in local encoding
     */
    std::string utf8_to_local(std::string_view text);
}
