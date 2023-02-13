#include "hw3.h"

using namespace std;

AccessControl::AccessControl(int table1Size, int table2Size) {
  regUsers.resize(table1Size);
  activeUsers.resize(table2Size);
  user_pass_count.resize(table1Size);
  int i,j;
  for(i=0; i<table1Size; i++) {
    regUsers[i].first = EMPTY;
    regUsers[i].second = EMPTY;
    user_pass_count[i].first = EMPTY;
    user_pass_count[i].second = 0;
  }
  for(j=0; j<table2Size;j++) {
    activeUsers[j] = EMPTY;
  }
  load_factor_reg = 0.0;
  load_factor_active = 0.0;
  regtableSize = table1Size;
  activetableSize = table2Size;
  user_count = 0;
  reg_count = 0;
  active_count = 0;
}

AccessControl::~AccessControl() {

}

int AccessControl::addUser(std::string username, std::string pass) {
  int index = 0, counter = 1;
  int hash = hashFunction(username,regtableSize,index);
  int new_hash;
  while (regUsers[hash].first == EMPTY || regUsers[hash].first == DELETED || regUsers[hash].first != username  ) {
    if (index > regtableSize)
    {
      return 0;
    }

    if (regUsers[hash].first == EMPTY || regUsers[hash].first == DELETED)
    {
      regUsers[hash].first = username;
      regUsers[hash].second = pass;
      reg_count ++;
      pass_counter(username);
      user_count++;
      load_factor_reg = float(reg_count) / float(regtableSize);
      if ( (load_factor_reg ) > MAX_LOAD_FACTOR  ) {
        rehash_table_reg();
      }
      hash = hashFunction(username,regtableSize,index);
      return 1;
    }

    else if(regUsers[hash].first != username)
    {
      index ++;
      hash = hashFunction(username,regtableSize, index);
      continue;
    }

    else
    {
      return 0;
    }
  }
}
int AccessControl::addUsers(std::string filePath) {
    ifstream file;
    string line;
    file.open(filePath.c_str());
    getline(file, line);
    string delimiter = " ";
    string name,pass;
    int count = 0;
    while(file.good()) {
        size_t pos = 0;
        pos = line.find(delimiter);
        name =line.substr(0,pos);
        line.erase(0, pos + delimiter.length());
        pos = line.find(delimiter);
        pass = line.substr(0,pos);
        line.erase(0, pos + delimiter.length());
        count += addUser(name,pass);
        getline(file,line);
    }
    file.close();
    return count;
}

int AccessControl::delUser(std::string username, std::vector<std::string>& oldPasswords) {
    int hash;
    int i=0;
    while (i < regtableSize) {
        hash = hashFunction(username,regtableSize,i);
        if (regUsers[hash].first == username) {
            oldPasswords.push_back(regUsers[hash].second);
            regUsers[hash].first = DELETED;
            regUsers[hash].second = DELETED;
            i++;
        }
        else {
            i++;
        }
    }
    if (oldPasswords.size() == 0) {
        return 0;
    }
    for(i=0; i < user_count; i++) {
        if (user_pass_count[i].first == username) {
            user_pass_count[i].first = DELETED;
            user_pass_count[i].second = 0;
            break;
        }
    }
    logout(username);
    reg_count--;
    user_count--;
    return 1;

}
int AccessControl::changePass(std::string username, std::string oldpass, std::string newpass) {
  int i,hash,index;
  for(i=0; i < user_count; i++)
  {
    if (user_pass_count[i].first == username) { break   ; }
    else                                      { continue; }
  }
    if (user_pass_count[i].first != username)   { return 0; }
    else {
      index = user_pass_count[i].second - 1;
      hash = hashFunction(username,regtableSize,index);
      while(index < regtableSize) {
          if(regUsers[hash].second != oldpass) {
              index++;
              hash = hashFunction(username,regtableSize,index);
          }
          else {
              break;
          }
      }
      while(index < regtableSize ) {

        if (regUsers[hash].second == EMPTY) {
          regUsers[hash].second = newpass;
          regUsers[hash].first = username;
          reg_count++;
          pass_counter(username);
          load_factor_reg = float(reg_count) / float(regtableSize);
          if ( (load_factor_reg ) > MAX_LOAD_FACTOR  ) {
            rehash_table_reg();
          }
          return 1;
        }
        index++;
        hash = hashFunction(username, regtableSize,index);
      }
    }
    return 0;

}

int AccessControl::login(std::string username, std::string pass) {
 int i,hash,hash_active,index,index_active;
  for(i=0; i < user_count; i++)
  {
    if (user_pass_count[i].first == username) { break   ; }
    else                                      { continue; }
  }

  if (user_pass_count[i].first != username)   {return 0; }
  else if (check_login(username) == 1)        {return 0; }
  else
  {
    index = user_pass_count[i].second - 1;
    hash = hashFunction(username,regtableSize,index);
    while(regUsers[hash].first != EMPTY || regUsers[hash].first != DELETED) {
        if(regUsers[hash].first != username) {
            index++;
            hash = hashFunction(username,regtableSize,index);
        }
        else {
            break;
        }
    }
    index_active = 0;
    if (regUsers[hash].second != pass)   {return 0; }
    while (index_active < activetableSize) {
        hash_active = hashFunction(username,activetableSize,index_active);
        if (activeUsers[hash_active] == EMPTY || activeUsers[hash_active] == DELETED)
        {

          activeUsers[hash_active] = username;
          active_count ++;
          load_factor_active = float (active_count) / float (activetableSize) ;
          if ( (load_factor_active) > MAX_LOAD_FACTOR) {
            rehash_table_active();
          }
          return 1;
        }
        else
        {
          index_active++;
        }
      }
      return 0;
    }
  }

int AccessControl::logout(std::string username) {
    for (int i=0; i < activetableSize; i++) {
        if(activeUsers[i] == username) {
            activeUsers[i] = DELETED;
            active_count--;
            return 1;
        }
    }
    return 0;
    
}


float AccessControl::printActiveUsers() {
  int i;
  for (i=0; i < activetableSize; i++ ) {
    cout << activeUsers[i] << endl;
  }
  return load_factor_active;

}

float AccessControl::printPasswords() {
  int i;
  for (i=0; i < regtableSize; i++) {
      cout << regUsers[i].first << " " <<  regUsers[i].second << endl;
  }
  return load_factor_reg;

}
