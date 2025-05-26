#include <iostream>
#include <fstream>
#include <string>

class Grep {
public:
	Grep(const std::string& cm) : command(cm) {
		name = get_word();
		pattern = get_word();
		file_name = get_word();

		std::fstream file(file_name);

		if (!file) {
			throw std::runtime_error("File can't be opened");
		}

		char ch;
		while (file.get(ch)) {
			content += ch;
		}

		check();

	}

	void check() {
		if (name.compare(0, 4, "grep") != 0) {
			throw std::runtime_error("Command should start with word 'grep'");
		}

		std::string line;
		bool matched = false;

		while (content_index < content.size()) {
			line.clear();

			while (content_index < content.size() && content[content_index] != '\n') {
				line += content[content_index];
				++content_index;
			}
			if (content_index < content.size() && content[content_index] == '\n') {
				++content_index;
			}

			std::string output;
			size_t line_index = 0;

			while (line_index < line.size()) {
				size_t found = line.find(pattern, line_index);

				if (found == std::string::npos) {
					output += line.substr(line_index);
					break;
				}

				output += line.substr(line_index, found - line_index);
				output += "\033[31m" + pattern + "\033[0m";
				line_index = found + pattern.size();
				matched = true;
			}

			std::cout << output << std::endl;
			
		}

		if (!matched) {
			std::cout << "Pattern not found." << std::endl;
		}
	}


private:
	std::string content;
	std::string command;
	std::string name;
	std::string pattern;
	std::string file_name;
	size_t content_index = 0;
	size_t command_index = 0;
	size_t pattern_index = 0;

	void skip_spaces(const std::string& str, size_t& index) {
		while (index < str.size() && str[index] == ' ') {
			++index;
		}
	}

	std::string get_word() {
		std::string result;
		skip_spaces(command, command_index);

		if (command_index < command.size() && command[command_index] == '\"') {
			++command_index;

			while (command_index < command.size() && command[command_index] != '\"') {
				result += command[command_index];
				++command_index;
			}

			if (command_index < command.size() && command[command_index] == '\"') {
				++command_index;
			}
		}

		else {
			while (command_index < command.size() && command[command_index] != ' ') {
				result += command[command_index];
				++command_index;
			}
		}

		return result;
	}

};

int main() {
	try {
		std::cout << "Enter command" << std::endl;
		std::string command;
		std::getline(std::cin, command);
		Grep gp(command);
	}

	catch (const std::exception& e){
		std::cout << e.what() << std::endl;
	}
	return 0;
}
