#pragma once
#include <Json.h>

namespace mechanizm {
    class JsonSerializable {
    public:
        std::string Json() const;
        void SetJson(const std::string value);

        virtual Json::Value JsonValue() const = 0;
        virtual  void SetJsonValue(const Json::Value root) = 0;
    };
}