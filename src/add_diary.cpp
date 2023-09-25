/**
 * @file add_diary.cpp
 * @author Mes (mes900903@gmail.com)
 * @brief add new student into the ncuma diary
 * @version 0.1
 * @date 2023-09-26
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "nlohmann/json.hpp"

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

int main()
{
  /**
   * @brief generate new data.json
   *
   */

  // read raw_new.txt
  std::ifstream raw_new("raw_new.txt");
  if (raw_new.fail()) {
    std::cerr << "read raw_new.txt failed\n";
    return 1;
  }

  // read student
  int senior_semester, junior_semester;
  std::string line;
  std::string senior_name, junior_name;
  std::stringstream ss;
  std::getline(raw_new, line);
  ss << line;
  ss >> senior_semester >> junior_semester;
  ss.clear();
  ss.str("");

  // generate the json for every junior student
  ordered_json total_family_data = ordered_json::parse("{\"data\":[]}");

  while (std::getline(raw_new, line)) {
    ss << line;
    ss >> senior_name >> junior_name;
    ss.clear();
    ss.str("");

    ordered_json single_junior_student;
    single_junior_student["name"] = "(" + std::to_string(junior_semester) + ")" + junior_name;
    single_junior_student["grade"] = junior_semester;
    single_junior_student["nickname"] = json::array();
    single_junior_student["title"] = json::array();
    single_junior_student["status"] = "";
    single_junior_student["demo"] = single_junior_student["name"];
    single_junior_student["senior"] = "(" + std::to_string(senior_semester) + ")" + senior_name;
    single_junior_student["junior"] = json::array();
    single_junior_student["width"] = 0;

    total_family_data["data"].push_back(std::move(single_junior_student));
  }
  raw_new.close();

  // read data.json, and set the senior student
  std::ifstream old_data("data.json");

  if (old_data.fail()) {
    std::cerr << "read data.json failed\n";
    return 1;
  }

  // set the junior of senior student, and append it to total_family_data
  ordered_json old_family_data = ordered_json::parse(old_data);
  old_data.close();

  for (auto &old_student : old_family_data["data"]) {
    // set the junior of senior student
    if (old_student["grade"] == senior_semester) {
      // find the junior student, who is the junior of senior student
      for (auto &junior : total_family_data["data"]) {
        if (junior["senior"] == old_student["name"])
          old_student["junior"].push_back(junior["name"]);

        if (junior["grade"] < senior_semester + 1)
          break;
      }
    }

    // append the old student to total_family_data
    total_family_data["data"].push_back(std::move(old_student));
  }

  std::ofstream out("new_data.json");
  out << total_family_data.dump(4);
  out.close();

  /**
   * @brief generate new search.json
   *
   */

  // generate the json for every junior student
  ordered_json total_search_data = ordered_json::parse("{\"data\":[]}");
  for (const auto &student : total_family_data["data"]) {
    if (student["grade"] != junior_semester)
      break;

    ordered_json single_search_student;
    single_search_student["name"] = student["name"];
    single_search_student["grade"] = student["grade"];

    total_search_data["data"].push_back(std::move(single_search_student));
  }

  // read search.json
  std::ifstream old_search("search.json");
  if (old_search.fail()) {
    std::cerr << "read search.json failed\n";
    return 1;
  }

  ordered_json old_search_data = ordered_json::parse(old_search);
  old_search.close();

  // append the old student to total_search_data
  for (const auto &student : old_search_data["data"])
    total_search_data["data"].push_back(std::move(student));

  std::ofstream out_search("new_search.json");
  out_search << total_search_data.dump(4);
}