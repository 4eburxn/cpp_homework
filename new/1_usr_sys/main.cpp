#include <format>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <sstream>
#include <string>

class User;
class Group {
public:
  int id = 0;
  std::set<User *> users{};
  ~Group();
};

class User {
public:
  int id = 0;
  std::string name = "";
  std::string info = "";
  std::set<Group *> groups{};
  User() = default;
  User(int id, std::string name, std::string info)
      : id(id), name(name), info(info) {};
  ~User() {
    for (auto &i : groups) {
      i->users.erase(this);
    }
  }
};

Group::~Group() {
  for (auto &i : users) {
    i->groups.erase(this);
  }
}

enum Modes {
  createUser,
  deleteUser,
  allUsers,
  getUser,
  createGroup,
  deleteGroup,
  allGroups,
  getGroup,
  addUser,
  err__
};
int hash_modes(std::string mode) {
  if (mode == "createUser")
    return createUser;
  if (mode == "deleteUser")
    return deleteUser;
  if (mode == "allUsers")
    return allUsers;
  if (mode == "getUser")
    return getUser;
  if (mode == "createGroup")
    return createGroup;
  if (mode == "addUser")
    return addUser;
  if (mode == "deleteGroup")
    return deleteGroup;
  if (mode == "allGroups")
    return allGroups;
  if (mode == "getGroup")
    return getGroup;
  return err__;
}
std::string get_info() {
  std::string s{};
  char c = std::cin.get();
  if (c == '\n' || !c) {
    return s;
  }
  c = std::cin.get();
  while (c != '\n' && c) {
    s += c;
    c = std::cin.get();
  }
  return s;
}

auto user_info(User &usr) {
  auto it = usr.groups.begin();
  std::stringstream msg;
  if (it != usr.groups.end()) {
    msg << std::format("{}\t{}\t{}\t{}", usr.id, usr.name, (*it)->id, usr.info);
    it++;
    while (it != usr.groups.end()) {
      msg << std::format("\n\t\t{}", (*it)->id);
      it++;
    }
  } else {
    msg << std::format("{}\t{}\tNone\t{}", usr.id, usr.name, usr.info);
  }
  return msg.str();
}

bool clear_cin(char to = '\n') {
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), to);
  std::cout << "something gone wrong" << std::endl;
  return false;
}

int main(int argc, char *argv[]) {
  std::map<int, Group> groups;
  std::map<int, User> users;
  int idu, idg;
  bool status = false;
  std::string mode, name, info;
  while (true) {
    try {
      std::cout << ">>> ";
      std::cin >> mode;
      switch (hash_modes(mode)) {
      case createUser:
        if (!(std::cin >> idu >> name || clear_cin()))
          break;

        if (users.find(idg) != users.end()) {
          std::cout << "ERROR: user already exist" << std::endl;
          break;
        }
        info = get_info();
        users.emplace(idu, User(idu, name, info));
        std::cout << std::format("creating user id:{} name:{} info:\"{}\" ",
                                 idu, name, info)
                  << std::endl;
        break;
      case deleteUser:
        if (!(std::cin >> idu || clear_cin()))
          break;

        if (users.find(idu) != users.end()) {
          User &usr = users[idu];
          std::cout << std::format("deleting user id:{} name:{} info:\"{}\" ",
                                   usr.id, usr.name, usr.info)
                    << std::endl;
          users.erase(idu);
        } else {
          std::cout << "ERROR: user with id " << idu << "does not exists"
                    << std::endl;
        }
        break;
      case allUsers:
        std::cout << "id\tname\tgroups\tinfo\n";
        for (auto &i : users) {
          std::cout << user_info(i.second) << std::endl;
        }
        std::cout << std::endl;
        break;
      case getUser:
        if (!(std::cin >> idu || clear_cin()))
          break;

        if (users.find(idu) != users.end()) {
          std::cout << "id\tname\tgroups\tinfo\n";
          std::cout << user_info(users[idu]) << "\n" << std::endl;
        } else
          std::cout << "ERROR: user with id " << idu << "does not exists"
                    << std::endl;
        break;
      case createGroup:
        if (!(std::cin >> idg || clear_cin()))
          break;
        if (groups.find(idg) != groups.end()) {
          std::cout << "ERROR: group already exist" << std::endl;
          break;
        }
        groups[idg] = Group{idg};
        std::cout << "created group with id:" << idg << std::endl;
        break;
      case addUser:
        if (!(std::cin >> idu >> idg || clear_cin()))
          break;
        if (groups.find(idg) == groups.end()) {
          std::cout << "ERROR: invalid group" << std::endl;
          break;
        }
        groups[idg].users.emplace(&users[idu]);
        users[idu].groups.emplace(&groups[idg]);
        break;
      case deleteGroup:
        if (!(std::cin >> idg || clear_cin()))
          break;
        if (groups.find(idg) == groups.end()) {
          std::cout << "ERROR: group does not exists" << std::endl;
          break;
        }
        groups.erase(idg);
        std::cout << "deleated group" << idg << std::endl;
        break;
      case allGroups:
        if (groups.size()) {
          std::cout << "group:\t users\n";
          for (auto &i : groups) {
            std::cout << i.second.id << ":\t";
            if (i.second.users.size()) {
              for (auto &j : i.second.users) {
                std::cout << " " << j->id;
              }
              std::cout << std::endl;
            } else {
              std::cout << " None" << std::endl;
            }
          }
        } else {
          std::cout << "there is no groups to show" << std::endl;
        }
        break;
      case getGroup:
        if (!(std::cin >> idg || clear_cin()))
          break;

        if (groups.find(idg) != groups.end()) {
          std::cout << "group:\t users\n";
          std::cout << idg << ":\t";
          for (auto &j : groups[idg].users) {
            std::cout << " " << j->id;
          }
          if (!groups[idg].users.size())
            std::cout << " None";
          std::cout << std::endl;
        } else
          std::cout << "ERROR: user with id " << idu << "does not exists"
                    << std::endl;
        break;

      default:
        clear_cin();
        std::cout << "command \"" << mode << "\" is invalid" << std::endl;
      }

    } catch (...) {
    }
  }
}
