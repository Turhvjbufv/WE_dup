#include "WE_path.h"
namespace fs = std::filesystem;
enum answer { no, yes };

WE_path::WE_path() {
  check_path_to_myprojects();
  check_path_to_workshop();
}
void WE_path::add_lines_of_WE() {
  if (!fs::exists("WE.txt")) {
    return;
  }
  std::fstream listWE("WE.txt");
  std::string lines_of_WE{""};
  while (getline(listWE, lines_of_WE)) {
    // putting all recorded projects into a string since
    // it is much faster to check if a string is in another string
    // than to open a file, go all over his lines and check if the string
    // matches
    // the "\n" is in case of an overlap(pretty much 0 chance but jsut to make
    // sure)
    list_of_items.insert(list_of_items.length(), "\n");
    list_of_items.insert(list_of_items.length(), lines_of_WE);
  }
  listWE.close();
}
void WE_path::check_path_to_myprojects() {
  if (!fs::exists("WE_path_to_myprojects.txt")) {
    std::cout << "file does not exist, creating file...\n";
    std::ofstream listWE_create_the_file_if_doesnt_exist;
    listWE_create_the_file_if_doesnt_exist.open("WE_path_to_workshop.txt");
    listWE_create_the_file_if_doesnt_exist.close();
    if (std::filesystem::exists(
            "C:\\Program Files (x86)\\Steam\\SteamApps\\common\\wallpaper_"
            "engine\\projects\\myprojects") == 1) {

      std::cout
          << "We have detected you have steam workdshop content that is in\n"
             " C:\\Program Files (x86)\\Steam\\SteamApps\\common\\wallpaper_"
             "engine\\projects\\myprojects \nEnter "
             "1 if that is correct and 0 if it is not/you want to choose "
             "a different path\n";

      int yes_or_no_myprojects{};
      std::cin >> yes_or_no_myprojects;
      while (yes_or_no_myprojects) {
        switch (yes_or_no_myprojects) {
        case no: {
          // 0(not the correct path)
          // should even get in but just in case someone miss types
          break;
        }
        case yes: {
          // 1(the correct path)
          path_to_myprojects =
              "C:\\Program Files (x86)\\Steam\\SteamApps\\common\\wallpaper_"
              "engine\\projects\\myprojects";
          std::ofstream listWE_myprojects("WE_path_to_myprojects.txt",
                                          std::ios::out);
          listWE_myprojects << path_to_myprojects;
          listWE_myprojects.close();
          return;
        }
        default: {
          // no 0/1
          std::cout
              << " ERROR command not found\nPlease enter 0 if NOT"
                 "correct, or 1 if correct(if you don't know what to do enter "
                 "1)";
          break;
        }
        }
      }
    }
    std::cout
        << "Enter your workshop steam path \nshould look something like "
           "this:\n"
           "C:\\Program Files "
           "(x86)\\Steam\\SteamApps\\common\\wallpaper_"
           "engine\\projects\\myprojects"
           "\nYou can check this yourself by going to wallpaper engine "
           "and "
           "right clicking on a wallpaper YOU uploded/created for wallpaper "
           "engine and choose \"open in explorer\" and then go back one "
           "folder (the folder you go back to should be named "
           "\"myprojects\")copy the path and paste it here\n";
    // gets the path and puts it in WE_path_to_myprojects.txt
    getline(std::cin, path_to_myprojects);
    std::ofstream listWE_myprojects("WE_path_to_myprojects.txt", std::ios::out);
    listWE_myprojects << path_to_myprojects;
    listWE_myprojects.close();
  } else {
    // file exists
    // reads the path from WE_path_to_myprojects.txt
    std::fstream listWE_myprojects("WE_path_to_myprojects.txt", std::ios::in);
    path_to_myprojects.assign(std::istreambuf_iterator<char>(listWE_myprojects),
                              std::istreambuf_iterator<char>());
    listWE_myprojects.close();
  }
}
void WE_path::check_path_to_workshop() {
  if (!fs::exists("WE_path_to_workshop.txt")) {
    std::cout << "file does not exist, creating file...\n";
    std::ofstream listWE_create_the_file_if_doesnt_exist;
    listWE_create_the_file_if_doesnt_exist.open("WE_path_to_workshop.txt");
    listWE_create_the_file_if_doesnt_exist.close();

    // if does not exist
    if (std::filesystem::exists(
            "C:\\Program Files "
            "(x86)\\Steam\\SteamApps\\workshop\\content\\431960")) {
      // checks if you have the path above

      std::cout << "we have detected your steam workdshop content it is "
                   "C:\\Program "
                   "Files (x86)\\Steam\\SteamApps\\workshop\\content\\431960"
                   "\nEnter 1 if that path is correct"
                   "\nOr 0 if it is not/you want to choose a different path\n";
      int yes_or_no_workshop{};
      std::cin >> yes_or_no_workshop;
      while (yes_or_no_workshop) {
        switch (yes_or_no_workshop) {
        case no: {
          // 0(not the correct path)
          // should even get in but just in case someone miss types
          break;
        }
        case yes: {
          // 1(the correct path)
          path_to_workshop =
              "C:\\Program Files "
              "(x86)\\Steam\\SteamApps\\workshop\\content\\431960";
          std::ofstream listWE_workshop("WE_path_to_workshop.txt",
                                        std::ios::out);
          listWE_workshop << path_to_workshop;
          listWE_workshop.close();
          check_length_of_oldpath =
              static_cast<int>(path_to_workshop.length()) + 1;
          return;
        }
        default: {
          // no 0/1
          std::cout
              << " ERROR command not found\nPlease enter 0 if NOT"
                 "correct, or 1 if correct(if you don't know what to do enter "
                 "1)";
          break;
        }
        }
      }
    }
    std::cout << "Enter your workshop steam path it should look something like "
                 "this:\n"
                 "C:\\Program Files "
                 "(x86)\\Steam\\SteamApps\\workshop\\content\\431960"
                 "\nYou can check this yourself by going to wallpaper engine "
                 "and "
                 "right clicking on a wallpaper you subscribed to(downloaded) "
                 "and choose \"open in explorer\" and then go back one folder "
                 "(the folder you go back to should be 431960) "
                 "copy the path and paste it here\n";
    // gets the path and puts it in WE_path_to_workshop.txt
    getline(std::cin, path_to_workshop);
    std::ofstream listWE_workshop("WE_path_to_workshop.txt", std::ios::out);
    listWE_workshop << path_to_workshop;
    listWE_workshop.close();
  } else {
    // file exists
    // reads the path from WE_path_to_workshop.txt
    std::fstream listWE_workshop("WE_path_to_workshop.txt", std::ios::in);
    path_to_workshop.assign(std::istreambuf_iterator<char>(listWE_workshop),
                            std::istreambuf_iterator<char>());
    listWE_workshop.close();
  }
  check_length_of_oldpath = static_cast<int>(path_to_workshop.length()) + 1;
}
std::string WE_path::get_path_to_workshop() const { return path_to_workshop; }
std::string WE_path::get_path_to_myprojects() const {
  return path_to_myprojects;
}
std::string WE_path::get_list_of_items() const { return list_of_items; }
void WE_path::add_list_of_items(std::string add_project) {
  list_of_items.insert(list_of_items.length(), "\n");
  list_of_items.insert(list_of_items.length(), add_project);
}
int WE_path::get_check_length_of_oldpath() const {
  return check_length_of_oldpath;
}
int WE_path::get_check_if_changed() const { return check_if_changed; }
void WE_path::add_check_if_changed() { ++check_if_changed; }
void WE_path::minus_check_if_changed() { --check_if_changed; }
void WE_path::constructor_copy_or_not_this_session() {
  // if running the program for the first time this session
  std::cout
      << "before we start copying, would you like to be able to choose "
         "what to copy or for the program to just copy everything it "
         "finds that is not copied yet, enter 1 to be able to choose "
         "or 0 to not choose(copy all new wallpapers)\nYou can also "
         "choose 2 which will just record all new wallpapers so that they "
         "wont show up again in the future"
         "otherwise\n";

  std::cin >> copy_or_not_this_session;
  bool to_copy{true};
  while (to_copy) {
    switch (copy_or_not_this_session) {
    // does the same thing for 0 or 1 or 2( im switch at least)
    case 0:
    case 1:
    case 2: {
      to_copy = false;
      break;
    }
    default:
      std::cout
          << " ERROR command not found\nPlease enter 0 to not "
             "choose if to copy, 1 to choose to record or add or \"skip\" "
             "and 2 to record all";
      break;
    }
  }
}
void WE_path::set_copy_or_not_this_session(int set) {
  copy_or_not_this_session = set;
}
int WE_path::get_copy_or_not_this_session() const {
  return copy_or_not_this_session;
}