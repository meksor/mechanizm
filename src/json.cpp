#include "json.h"

namespace mechanizm {
    std::string JsonSerializable::Json() const {
        return this->JsonValue().toStyledString();
    }

    void JsonSerializable::SetJson(const std::string value) {
        try {
            const Json::Value root = openshot::stringToJson(value);
            this->SetJsonValue(root);
        } catch (const std::exception& e) {
            throw openshot::InvalidJSON("JSON is invalid (missing keys or invalid data types)");
        }
    };
}