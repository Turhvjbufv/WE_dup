#include "WE_path.h"
#include <cctype>
#include <string>
#include <windows.h>

using namespace std;
namespace fs = std::filesystem;
// this project follows bool, meaning 1 is true and 0 is false(even when
// returning from functions)
enum answer { no, yes, always_no, always_yes };
//  easier to see the input

int check_if_file_exist();
int transfer_files(const string &new_project_transfer, WE_path &paths,
                   const int choose_override);
void copy_files(const string &path_old_for_file,
                const string &path_new_for_file);
void check_for_new_project(WE_path &paths);
int check_if_in_file(const string &new_project_to_check, WE_path &paths,
                     const int &choose_override, const string &title);
void unpkg(const string &new_path_to_old, const string &new_path_to_projects);
void run_system_commands(const string &use_to_command);
void send_to_search(WE_path &paths);
bool search_Projects(WE_path &paths, int copy_all_or_choose);
void tolower_loop(string &Tolower);
string find_title(const string &path, const string &project,
                  const string &check_project);
void add_to_WE(const string &project, const string &title);
void bad_input();
// project == wallpaper
int main() {
  try {
    WE_path paths;
    if (check_if_file_exist()) {
      // check if file exist also returns true in case user wants to add all
      // currrect wallpapers(first time)
      // or running the program after the first time(WE.txt exists)
      cout << "file found!, checking sending you to search...\n";
      send_to_search(paths);
      cout << "checking if updates are needed \n";
      check_for_new_project(paths);
      switch (paths.get_projects_added()) {
      case 0: {
        cout << "nothing changed";
        break;
      }
      default: {
        cout << "number of added projects is " << paths.get_projects_added();
        break;
      }
      }
    } else {
      // run if the function: check_if_file_exist(). returned false and WE.txt
      // file desnt exist(which means that the user chose to record all
      // wallpapers) this gets all the names of current files in the directory
      // and puts them in the newly created WE.txt file
      int dont_make_new_line{0};

      for (const auto &entry :
           fs::directory_iterator(paths.get_path_to_workshop())) {

        string path_of_new_project = entry.path().string();
        int check_length{static_cast<int>(path_of_new_project.length() -
                                          paths.get_check_length_of_oldpath())};
        string new_project = path_of_new_project.substr(
            paths.get_check_length_of_oldpath(), check_length);

        switch (dont_make_new_line) {
        case 0: {
          fstream listWE("WE.txt");
          string title{
              find_title(paths.get_path_to_workshop(), new_project, "")};
          listWE << new_project << "-" << title;
          dont_make_new_line++;
          listWE.close();
          continue;
        }
        default: {
          string title{
              find_title(paths.get_path_to_workshop(), new_project, "")};
          add_to_WE(new_project, title);
        }
        }
      }
    }
  } catch (const char *exception) {
    cout << exception;
    return -1;
  }
  return 0;
}

int check_if_file_exist() {
  // check if WE.txt exists and if it is not then creates it
  // and asks if you want to copy all new projects or not
  // if it does exist returns 1

  if (!fs::exists("WE.txt")) {
    cout << "file does not exist, creating file...\n";
    ofstream listWE_create_the_file_if_doesnt_exist;
    listWE_create_the_file_if_doesnt_exist.open("WE.txt");
    listWE_create_the_file_if_doesnt_exist.close();

    string copy_or_transfer{};
    // TODO fix the yes/no here
    // first time creating WE.txt it will ask if you
    // want to transer all current downloaded projects
    ofstream listWE("WE.txt", ios::out);
    string yes_or_no;
    cout << "would you like to copy all of your current downloaded projects?"
            "\nEnter yes or no(no means that all current downloaded projects "
            "will just be recorded(running this program again will allow "
            "you to search for a specific project)\n";
    cin >> yes_or_no;
    if (yes_or_no != "yes" && yes_or_no != "no") {

      cout << "try again, only one try because I am lazy, if "
              "it also failes it will default to not copying\n";
      cin >> yes_or_no;
    }
    if (yes_or_no == "yes") {
      return 1;
    }

    return 0;
  }
  return 1;
}
void check_for_new_project(WE_path &paths) {
  paths.constructor_copy_or_not_this_session();
  // decides if to copy all wallpapers(1) or letting the user choose which to
  // copy(0) or to just record all the current new wallpapers(2)
  string new_project{""};
  // the string for the new project
  for (const auto &entry :
       fs::directory_iterator(paths.get_path_to_workshop())) {
    string path_of_new_project{entry.path().string()};
    // gets the the project's path

    int check_length{static_cast<int>(path_of_new_project.length() -
                                      paths.get_check_length_of_oldpath())};
    new_project = {path_of_new_project.substr(
        paths.get_check_length_of_oldpath(), check_length)};
    // gets the length and then removes the path except for the project number
    // and inserts it in new_project(this is for cleaner look mostly)

    if (paths.get_list_of_items().find(new_project) != string::npos) {
      // if it's already recorded
      continue;
    }

    if (paths.get_copy_or_not_this_session() == 2) {
      // if wants to only record all new wallpapers
      string title{find_title(paths.get_path_to_workshop(), new_project, "")};
      add_to_WE(new_project, title);
      continue;
    }
    if (transfer_files(new_project, paths, 0)) {
      paths.add_projects_added();
    }
  }
}

int transfer_files(const string &new_project_transfer, WE_path &paths,
                   const int choose_override) {

  string path_to_old{paths.get_path_to_workshop() + "\\" +
                     new_project_transfer};
  string title{
      find_title(paths.get_path_to_workshop(), new_project_transfer, "")};
  string path_to_projects{paths.get_path_to_myprojects() + "\\" + title};
  string path_to_old_json{path_to_old + "\\project.json"};
  // gets the path to folder and copies the project to myproject
  // sets the path for the projects
  static int show_or_not{4};
  // for later to ask the user if he wants to see this wallapaper in the
  // future if they said they do not want to copy.
  // 4 so that it wont accidentely register with value 0-3
  int copy_or_not{0};
  // copy this wallpaper or not
  ifstream project_name(path_to_old_json);
  switch (paths.get_copy_or_not_this_session()) {
  case 1: {
    // if user choose to copy this session, gets the tile name and asks the user
    // if he want to copy it, will sometimes print strange characters beacuse
    // it only shows ascii ones(basic english and numbers)
    string title_in_json{
        find_title(paths.get_path_to_workshop(), new_project_transfer, "")};

    cout << "Would you like to copy\n" + title_in_json +
                "\nEnter 1 to copy or 0 to not copy\n";
    cin >> copy_or_not;
    bad_input();
    bool bcopy_or_not{true};
    while (bcopy_or_not) {

      switch (copy_or_not) {
      case no: {
        // 0, chose to not copy, if it can show up in the future
        // asks the user if he want to see it in future sessions
        // read the cout to know more
        if (paths.get_list_of_items().find(new_project_transfer) !=
            string::npos) {
          return 0;
        }
        while (true) {
          switch (show_or_not) {
          default: {
            if (!choose_override) {
              // if not searching
              return 0;
            }
            if ((choose_override &&
                 !check_if_in_file(new_project_transfer, paths, choose_override,
                                   title_in_json))) {
              // if searching but chose to not potentally ovderride
              return 0;
            }
            cout
                << "Would you like this project:\n" + title_in_json +
                       "\nTo not show anymore in options for copying"
                       "\nEnter 0 for it to show up in future sessions"
                       "\n1 for it to NOT show up in the future sessions"
                       "(will not show up again in this session anyway)"
                       "\nEnter 2-always no, which means that for "
                       "all projects this session it will not record them "
                       "and you will be able to see them in future sessions if "
                       "you don't copy them"
                       "\nEnter 3-always yes, will record them(if you don't "
                       "copy), and you won't see "
                       "them in future sessions unless you use search"
                       "\n(Entring 2 or 3 is only for this session, while 0 or "
                       "1 is only for this wallpaper, any other number will "
                       "result in this showing up again)\n";
            cin >> show_or_not;
            bad_input();
            break;
          }
          case no: {
            // 0(chose that this wallpaper will show up in future sessions)
            return 0;
          }
          case yes: {
            // 1(chose that this wallpaper will NOT show up in future
            //  "scan" sessions) this puts it in WE.txt file to record it
            add_to_WE(new_project_transfer, title);
            return 0;
          }
          case always_no: {
            // 2(if user chooses not to copy a wallpaper always make it show up
            // in future sessions)
            return 0;
          }
          case always_yes: {
            // 3(if user chooses not to copy a wallpaper always make it NOT show
            // up in future sessions unless using search)
            add_to_WE(new_project_transfer, title);
            return 0;
          }
          }
        }
        break;
      }
      case yes: {
        // 1, chose to copy
        bcopy_or_not = false;
        break;
      }
      default: {
        // any other number
        cout << "Please Enter again\n" + title_in_json +
                    "\nWrite 1 if you want to copy and 0 if you don't\n";
        cin >> copy_or_not;
        bad_input();
        break;
      }
      }
    }
  }
  }
  // puts the newproject into added projects(records them in WE.txt)
  add_to_WE(new_project_transfer, title);
  // calls the function to copy the folder
  copy_files(path_to_old, path_to_projects);
  // calls the function to upack the pkg file if there is one
  unpkg(path_to_old, path_to_projects);
  return 1;
}
int check_if_in_file(const string &new_project_to_check, WE_path &paths,
                     const int &choose_override, const string &title) {
  if (fs::exists(
          paths.get_path_to_myprojects() + "\\" +
          find_title(paths.get_path_to_workshop(), new_project_to_check, ""))) {
    switch (choose_override) {
    case 1: {
      // this choise only appears when searching
      cout << "This project:\n" + title +
                  "\nAlready exists if you choose to continue then "
                  "it will wipe that folder\nEnter 1 to continue and 0 to "
                  "not:\n";
      int Continue{0};
      cin >> Continue;
      bad_input();
      bool overriding{true};
      while (overriding) {
        switch (Continue) {
        case 0:
          // if wallapper is already in WE.txt and chose to skip this one
          return 0;
        case 1:
          // found word but chose to continue
          paths.add_list_of_items(new_project_to_check);
          return 1;
          break;
        default:
          cout << " ERROR command not found\nPlease enter 0 to NOT "
                  "(ponetially)override or 1 to (ponetially)override";
          break;
        }
      }
      break;
    }
    default:
      cout << "This project's:\n" + title + "\nFile already exists";
      // if wallapper exists and user is not searching
      return 0;
    }
  }
  // if it not in WE.txt
  return 1;
}
void copy_files(const string &path_old_for_file,
                const string &path_new_for_file) {

  // copies files
  fs::copy(path_old_for_file.c_str(), path_new_for_file.c_str(),
           std::filesystem::copy_options::overwrite_existing |
               std::filesystem::copy_options::recursive);
}

void unpkg(const string &new_path_to_old, const string &path_to_projects) {
  // gets all the path pkg and zip
  // then uses commands to copy the pkg, unpack it to a zip then uzip it
  string path_to_old_pkg{new_path_to_old + "\\scene.pkg"};
  string current_path{fs::current_path().string()};
  string path_to_projects_pkg{path_to_projects + "\\scene.pkg"};
  string path_to_old_zip(new_path_to_old + "\\scene.zip");
  string path_to_projects_zip{path_to_projects + "\\scene.zip"};
  string command_to_unpkg{"pkg2zip.exe -pkg2zip \"" + path_to_projects_pkg +
                          "\" " + "\"" + path_to_projects_zip + "\""};
  string command_to_unzip{"tar -xkvf \"" + path_to_projects_zip + "\" -C \"" +
                          path_to_projects + "\""};
  if (!fs::exists(path_to_old_pkg)) {
    // if there is no scene.pkg
    if (fs::exists(new_path_to_old + "\\gifscene.pkg")) {
      // if there is a gifscene.pkg
      path_to_old_pkg = new_path_to_old + "\\gifscene.pkg";
      path_to_projects_pkg = path_to_projects + "\\gifscene.pkg";
      path_to_old_zip = new_path_to_old + "\\gifscene.zip";
      path_to_projects_zip = path_to_projects + "\\gifscene.zip";
      command_to_unpkg = "pkg2zip.exe -pkg2zip \"" + path_to_projects_pkg +
                         "\" " + "\"" + path_to_projects_zip + "\"";
      command_to_unzip = "tar -xvf \"" + path_to_projects_zip + "\" -C \"" +
                         path_to_projects + "\"";
    } else {
      // if there is no scene.pkg nor gifscene.pkg
      return;
    }
  }
  // runs the commands
  run_system_commands(command_to_unpkg);
  // upacks the .pkg to .zip in the projects directory
  run_system_commands(command_to_unzip);
  // upcks the .zip to normal files and folders in the projects directory
  remove(path_to_projects_pkg.c_str());
  remove(path_to_projects_zip.c_str());
  // deletes the .zip and .pkg file as they are no longer needed
}

void run_system_commands(const string &use_to_command) {
  // creates a child prossesor to run the unzip and upack commands
  // honestly I barely know what is happening in this function
  // but it works
  cout << use_to_command << " runs"
       << "\n";
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ZeroMemory(&pi, sizeof(pi));
  LPSTR use_for_commands = const_cast<char *>(use_to_command.c_str());
  // Start the child process.
  CreateProcess(NULL,             // No module name (use command line)
                use_for_commands, // Command line
                NULL,             // Process handle not inheritable
                NULL,             // Thread handle not inheritable
                FALSE,            // Set handle inheritance to FALSE
                0,                // No creation flags
                NULL,             // Use parent's environment block
                NULL,             // Use parent's starting directory
                &si,              // Pointer to STARTUPINFO structure
                &pi);             // Pointer to PROCESS_INFORMATION structure

  // Wait until child process exits.
  WaitForSingleObject(pi.hProcess, INFINITE);
  // Close process and thread handles.
  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);
}

void send_to_search(WE_path &paths) {
  bool search{true};
  while (search) {
    cout << "Would you like to search for a project to copy(only in english)?"
            "\nAlso choose if more than 1 project is found will it copy "
            "every option or just what you want to copy(will ask you)"
            "\nEnter 0 to skip this(and go over every project)"
            "\nEnter 1 for it to copy every project it finds that matches"
            "\nEnter 2 for you to choose what to copy(if found 1 or more)"
            "\nThis is case insensitive search (bla == BLA)";
    int option;
    cin >> option;
    bad_input();
    switch (option) {
      // didnt put enum since it does not match
    case 0: {
      search = false;
      break;
    }
    case 1: {
      if (!search_Projects(paths, 1)) {
        cout << "Project not found\n";
      }
      break;
    }
    case 2: {
      if (!search_Projects(paths, 2)) {
        cout << "Project not found or you chose not to copy every time\n";
      }
      break;
    }
    default:
      cout << "ERROR command not found\n";
      break;
    }
  }
}
bool search_Projects(WE_path &paths, int copy_all_or_choose) {
  string new_project{""};
  // goes over every project in workshop and then checks title to see if it
  // matches if it does depending on what the user choose before then he can
  // choose what to copy or it copies all
  // if not found and or chose to not copy everything returns false
  cout << "enter the name of the wallpaper you want to copy and I'll search "
          "for it, you can only use numbers or english to search as only "
          "ascii support\nDo note that with this you can potentally override a "
          "project that alredy exists, you wll get the option to choose if "
          "this happens\n";

  string check_project;
  cin.get();
  getline(cin, check_project);
  tolower_loop(check_project);
  bool return_if_found{false};
  string line_in_json{""};
  for (const auto &entry :
       fs::directory_iterator(paths.get_path_to_workshop())) {
    // gets the the project's path
    string path_of_new_project{entry.path().string()};
    // gets the length and then removes the path except for the project
    // number and inserts it in new_project
    int check_length{static_cast<int>(path_of_new_project.length() -
                                      paths.get_check_length_of_oldpath())};
    new_project = {path_of_new_project.substr(
        paths.get_check_length_of_oldpath(), check_length)};
    string temp_check_project =
        find_title(paths.get_path_to_workshop(), new_project, check_project);
    if (temp_check_project != "") {
      // if found a match
      return_if_found = true;
      if (copy_all_or_choose == 2) {
        paths.set_copy_or_not_this_session(1);
        // if they chose to be able to choose if to copy or
        // not(copy_all_or_choose == 2)
        if (transfer_files(new_project, paths, 1)) {
          // if it does not exist yet then copy it and
          // add one to number of added proejcts and insert it to list
          // then returns the newly added project
          paths.add_list_of_items(new_project);
          paths.add_projects_added();
        }
        // if chose to not copy
        return_if_found = false;
      } else {
        // if chose to copy all(copy_all_or_choose == 1)
        if (transfer_files(new_project, paths, 1)) {
          paths.add_list_of_items(new_project);
          paths.add_projects_added();
          // if it does not exist yet then copy it and
          // add one to number of added proejcts and insert it to list
          // then returns the newly added project
        }
        // if it does exist
      }
    }
  }

  // if did not found and or chose to not copy every option
  cout << "search finished/n";
  return return_if_found;
}
void tolower_loop(string &Tolower) {
  for (auto &c : Tolower) {
    // makes it all lower case to not make it case sensitive
    c = tolower(c);
  }
}
string find_title(const string &path, const string &project,
                  const string &check_project) {
  string json_path{path + "\\" + project + "\\project.json"};
  ifstream project_name(json_path);
  string line_in_json("");
  while (getline(project_name, line_in_json)) {
    if (line_in_json.find("title") == string::npos) {
      // if it is not the line with the title
      continue;
    }
    tolower_loop(line_in_json);
    if (check_project == "" ||
        line_in_json.find(check_project) != string::npos) {
      line_in_json = line_in_json.substr(12, line_in_json.size() - 14);
      return line_in_json;
    }
  }
  return "";
}
void bad_input() {
  if (cin.fail()) {
    throw "ERROR -- You did not enter an integer";
  }
}
void add_to_WE(const string &project, const string &title) {
  fstream listWE2("WE.txt", ios::app);
  listWE2 << "\n" << project << "-" << title;
  listWE2.close();
}