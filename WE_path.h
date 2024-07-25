#include <filesystem>
#include <fstream>
#include <iostream>

class WE_path {
  // makes it easier since they are grouped togther
  std::string path_to_workshop{""};
  std::string path_to_myprojects{""};
  std::string list_of_items{""};
  // used to check if the project already exists, much fsater than reading a
  // file every time
  int check_length_of_oldpath;
  // keeps the length of workshop so to only add the number of the project to
  // the WE.txt file(and also for checking if its in file)
  int projects_added{0};
  // used to check if there are wallaper that were added
  // and the number of them to wallpaper engine
  int copy_or_not_this_session;
  void add_lines_of_WE();
  void check_path_to_myprojects();
  void check_path_to_workshop();

public:
  WE_path();
  void constructor_copy_or_not_this_session();
  void set_copy_or_not_this_session(int set);
  int get_copy_or_not_this_session() const;
  int get_projects_added() const;
  void minus_projects_added();
  void add_projects_added();
  int get_check_length_of_oldpath() const;
  std::string get_list_of_items() const;
  void add_list_of_items(std::string);
  std::string get_path_to_workshop() const;
  std::string get_path_to_myprojects() const;
};