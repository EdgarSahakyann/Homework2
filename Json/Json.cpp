#include <iostream>
#include <fstream>
#include <variant>
#include <vector>
#include <string>
#include <map>


class Json;

using Json_values_arr = std::variant<int, bool, std::string>;
using Json_values = std::variant<int, bool, std::string, std::vector<Json_values_arr>, Json>;

class Json {
public:
    Json(const std::string& file_name) {
        std::ifstream file(file_name);
        if (!file) {
            throw std::runtime_error("File can't be opened");
        }
        char ch;
        while (file.get(ch)) {
            content += ch;
        }
        parse();
    }

    Json() = default;

    Json(const Json& other)
        : elements(other.elements),
        content(other.content),
        index(other.index) {
    }

    Json(Json&& other) noexcept
        : elements(std::move(other.elements)),
        content(std::move(other.content)),
        index(other.index) {
        other.index = 0;
    }

    Json& operator=(const Json& other) {
        if (this != &other) {
            elements = other.elements;
            content = other.content;
            index = other.index;
        }
        return *this;
    }

    void add(const std::string& key, const Json_values& value) {
        elements[key] = value;
    }

    void print(int indent = 0) const {
        std::string indent_str(indent * 2, ' ');
        size_t count = 0;
        std::cout << indent_str << "{" << std::endl;

        for (const auto& el : elements) {
            std::cout << indent_str << "  \"" << el.first << "\" : ";

            std::visit([&](const auto& value) {
                if constexpr (std::is_same_v<std::decay_t<decltype(value)>, int>) {
                    std::cout << value;
                }
                else if constexpr (std::is_same_v<std::decay_t<decltype(value)>, bool>) {
                    std::cout << (value ? "true" : "false");
                }
                else if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::string>) {
                    std::cout << "\"" << value << "\"";
                }
                else if constexpr (std::is_same_v<std::decay_t<decltype(value)>, std::vector<Json_values_arr>>) {
                    std::cout << "[ ";

                    for (size_t i = 0; i < value.size(); ++i) {
                        std::visit([&](const auto& val) {
                            if constexpr (std::is_same_v<std::decay_t<decltype(val)>, int>) {
                                std::cout << val;
                            }
                            else if constexpr (std::is_same_v<std::decay_t<decltype(val)>, bool>) {
                                std::cout << (val ? "true" : "false");
                            }
                            else if constexpr (std::is_same_v<std::decay_t<decltype(val)>, std::string>) {
                                std::cout << "\"" << val << "\"";
                            }
                            }, value[i]);

                        if (i < value.size() - 1) {
                            std::cout << ", ";
                        }
                    }

                    std::cout << " ]";
                }
                else if constexpr (std::is_same_v<std::decay_t<decltype(value)>, Json>) {
                    value.print(indent + 1);
                }
                }, el.second);

            if (count < elements.size() - 1) {
                std::cout << ",";
            }

            std::cout << std::endl;
            ++count;
        }

        std::cout << indent_str << "}" << std::endl;
    }

private:
    std::map<std::string, Json_values> elements;
    std::string content;
    size_t index = 0;

    void skip_spaces() {
        while (index < content.size() && isspace(content[index])) {
            ++index;
        }
    }

    std::string parse_string() {
        std::string result;
        if (index < content.size() && content[index] == '\"') {
            ++index;
            while (index < content.size() && content[index] != '\"') {
                result += content[index];
                ++index;
            }
            if (index < content.size() && content[index] == '\"') {
                ++index;
            }
            else {
                throw std::runtime_error("Unterminated string");
            }
        }
        else {
            throw std::runtime_error("Expected a string");
        }
        return result;
    }

    std::vector<Json_values_arr> parse_array() {
        std::vector<Json_values_arr> arr;
        if (index < content.size() && content[index] == '[') {
            ++index;
            skip_spaces();
            while (index < content.size() && content[index] != ']') {
                skip_spaces();
                if (isdigit(content[index])) {
                    std::string result;
                    while (index < content.size() && isdigit(content[index])) {
                        result += content[index];
                        ++index;
                    }
                    arr.push_back(std::stoi(result));
                }
                else if (content.compare(index, 4, "true") == 0) {
                    arr.push_back(true);
                    index += 4;
                }
                else if (content.compare(index, 5, "false") == 0) {
                    arr.push_back(false);
                    index += 5;
                }
                else if (content[index] == '\"') {
                    arr.push_back(parse_string());
                }
                else {
                    throw std::runtime_error("Invalid array!");
                }
                skip_spaces();
                if (content[index] == ',') {
                    ++index;
                    skip_spaces();
                }
            }
            if (index < content.size() && content[index] == ']') {
                ++index;
            }
            else {
                throw std::runtime_error("Expected ']'");
            }
        }
        else {
            throw std::runtime_error("Expected '['");
        }
        return arr;
    }

    Json_values parse_val() {
        skip_spaces();

        if (index < content.size() && isdigit(content[index])) {
            std::string result;

            while (index < content.size() && isdigit(content[index])) {
                result += content[index];
                ++index;
            }

            return std::stoi(result);
        }
        else if (content.compare(index, 4, "true") == 0) {
            index += 4;
            return true;
        }
        else if (content.compare(index, 5, "false") == 0) {
            index += 5;
            return false;
        }
        else if (index < content.size() && content[index] == '\"') {
            return parse_string();
        }
        else if (index < content.size() && content[index] == '[') {
            return parse_array();
        }
        else if (index < content.size() && content[index] == '{') {
            Json nested_json;
            nested_json.content = content;
            nested_json.index = index;
            nested_json.parse();
            index = nested_json.index;
            return nested_json;
        }

        throw std::runtime_error("Invalid type of value");
    }

    void parse() {
        std::string key;
        Json_values value;
        skip_spaces();

        if (index < content.size() && content[index] == '{') {
            ++index;
            skip_spaces();

            while (index < content.size() && content[index] != '}') {
                key = parse_string();
                skip_spaces();

                if (index < content.size() && content[index] == ':') {
                    ++index;
                    skip_spaces();
                    value = parse_val();
                    elements[key] = value;
                    skip_spaces();

                    if (index < content.size() && content[index] == ',') {
                        ++index;
                        skip_spaces();
                    }
                    else if (index < content.size() && content[index] != '}') {
                        throw std::runtime_error("Expected ',' or '}'");
                    }
                }
                else {
                    throw std::runtime_error("Expected ':'");
                }
            }
            if (index < content.size() && content[index] == '}') {
                ++index;
            }
            else {
                throw std::runtime_error("Expected '}'");
            }
        }
        else {
            throw std::runtime_error("Expected '{'");
        }
    }

    };

    int main() {
        try {
            Json js("example.json");
            js.add("age", 28);
            js.add("skills", std::vector<Json_values_arr>{"C++", "Python", "JavaScript"});

            Json nested;
            nested.add("city", std::string("Yerevan"));
            nested.add("zip", 1111);
            js.add("info", nested);

            js.print();
        }

        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }

        return 0;
    }
