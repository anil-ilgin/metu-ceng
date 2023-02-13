#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "VideoShare.hpp"
#include "User.hpp"

using namespace std;

void VideoShare::printAllVideos() {
    videos.print();
}

void VideoShare::printAllUsers() {
    users.print();
}

/* TO-DO: method implementations below */

VideoShare::VideoShare() {

}
VideoShare::~VideoShare() {

}

void VideoShare::createUser(const string & userName, const string & name, const string & surname) {
  User user_1 = User(userName,name,surname);
  users.insertNode(users.getHead(),user_1);
  return;
}


void VideoShare::loadUsers(const string & fileName) {
  ifstream file;
  string line;

  file.open(fileName.c_str());
  getline(file, line);
  string delimiter = ";";
  string str1,str2,str3;
    while (file.good()) {
        size_t pos = 0;
        pos = line.find(delimiter);
        str1 = line.substr(0, pos);
        line.erase(0, pos + delimiter.length());
        if (str1.length() < 1 ) {
          line.erase();
        getline(file, line);
        }
        pos = line.find(delimiter);
        str2 = line.substr(0,pos);
        line.erase(0, pos + delimiter.length());
        pos = line.find(delimiter);
        str3 = line.substr(0,pos);
        line.erase(0, pos + delimiter.length());
        createUser(str1,str2,str3);
        getline(file, line);
      }
    file.close();
    return;
}

void VideoShare::createVideo(const string & title, const string & genre) {
  Video video_1 = Video(title,genre);
  videos.insertNode(videos.getHead(),video_1);
  return;

}


void VideoShare::loadVideos(const string & fileName) {
  ifstream file;
  string line;

  file.open(fileName.c_str());
  string str1,str2;
  getline(file, line);
  string delimiter = ";";
  while (file.good()) {
      size_t pos = 0;
      pos = line.find(delimiter);
      str1 = line.substr(0, pos);
      if (str1.length() < 1) {
        line.erase();
        getline(file, line);
      }
      line.erase(0, pos + delimiter.length());
      pos = line.find(delimiter);
      str2 = line.substr(0,pos);
      line.erase(0, pos + delimiter.length());
      createVideo(str1,str2);
      getline(file, line);
    }
  file.close();
  return;
}

void VideoShare::addFriendship(const string & userName1, const string & userName2) {
    if(users.findNode(userName1) == NULL ||
        users.findNode(userName2) == NULL) {
            return;
    }
  User* user_1 = users.findNode(userName1)->getDataPtr();
  User* user_2 = users.findNode(userName2)->getDataPtr();
  user_1->addFriend(user_2);
  user_2->addFriend(user_1);
  return;
}

void VideoShare::removeFriendship(const string & userName1, const string & userName2) {
    if(users.findNode(userName1) == NULL ||
        users.findNode(userName2) == NULL) {return;}
  User* user_1 = users.findNode(userName1)->getDataPtr();
  User* user_2 = users.findNode(userName2)->getDataPtr();
  user_1->removeFriend(user_2);
  user_2->removeFriend(user_1);
  return;
}

void VideoShare::updateUserStatus(const string & userName, Status newStatus) {
  User* user_1 = users.findNode(userName)->getDataPtr();
  user_1->updateStatus(newStatus);
  return;
}

void VideoShare::subscribe(const string & userName, const string & videoTitle) {
    if(users.findNode(userName) == NULL) {return;}
    if(videos.findNode(videoTitle) == NULL) {return;}
    if(users.findNode(userName)->getDataPtr()->getStatus() == SUSPENDED) {
    return;
  }
  User* user_1 = users.findNode(userName)->getDataPtr();
  Video* video_1 = videos.findNode(videoTitle)->getDataPtr();
  user_1->subscribe(video_1);
  return;
}

void VideoShare::unSubscribe(const string & userName, const string & videoTitle) {
    if(users.findNode(userName) == NULL) {return;}
    if(videos.findNode(videoTitle) == NULL) {return;}
    if(users.findNode(userName)->getDataPtr()->getStatus() == SUSPENDED) {
    return;
  }
  User* user_1 = users.findNode(userName)->getDataPtr();
  Video* video_1 = videos.findNode(videoTitle)->getDataPtr();
  user_1->unSubscribe(video_1);
  return;
}

void VideoShare::deleteUser(const string & userName) {
    if(users.findNode(userName) == NULL) {return;}
  User* user_1 = users.findNode(userName)->getDataPtr();
  Node<User*>* user_2 = user_1->getFriends()->first();
  while(user_2) {
    user_2->getData()->removeFriend(user_1);
    user_2 = user_2->getNext();
  }
 users.deleteNode(users.findPrev(userName));
 return;
}

void VideoShare::sortUserSubscriptions(const string & userName) {
    if(users.findNode(userName) == NULL) {return;}
  User * user_1 = users.findNode(userName)->getDataPtr();
  Node<Video *>* video_1 = user_1->getSubscriptions()->first();
  if (video_1 == NULL) {return;}
  int track = 0;
  while (video_1->getNext() != NULL ) {
    if ( video_1->getData()->getTitle() > video_1->getNext()->getData()->getTitle()) {
      user_1->getSubscriptions()->swap(track,track+1);
      video_1 = user_1->getSubscriptions()->first();
      track = 0;
    }
    else {
      video_1 = video_1->getNext();
      track++;
      if ( track == user_1->getSubscriptions()->getLength() ) { break;}
      continue; }
  }
  return;
}

void VideoShare::printUserSubscriptions(const string & userName){
    if(users.findNode(userName) == NULL) {return;}
  User * user_1 = users.findNode(userName)->getDataPtr();
  user_1->printSubscriptions();
}

void VideoShare::printFriendsOfUser(const string & userName) {
    if(users.findNode(userName) == NULL) {return;}
  User * user_1 = users.findNode(userName)->getDataPtr();
  user_1->printFriends();
  return;
}
void VideoShare::printCommonSubscriptions(const string & userName1, const string & userName2) {
  User * user_1 = users.findNode(userName1)->getDataPtr();
  Node<Video *>* video_1 = user_1->getSubscriptions()->first();
  User * user_2 = users.findNode(userName2)->getDataPtr();
  Node<Video *>* video_2 = user_2->getSubscriptions()->first();
  while (video_2 != NULL) {
    while(video_1 != NULL) {
      if ( video_1->getData()->getTitle() == video_2->getData()->getTitle()) {
          std::cout << *(video_1->getData());
          video_1 = video_1->getNext();
          continue;
      }
      else {
        video_1 = video_1->getNext();
      continue;
      }
    }
    video_1 = user_1->getSubscriptions()->first();
    video_2 = video_2->getNext();
  }
  cout << endl;
  return;
}
void VideoShare::printFriendSubscriptions(const string & userName) {
  User * user_1 = users.findNode(userName)->getDataPtr();
  Node<Video *>* video_1 = user_1->getSubscriptions()->first();
  if(users.findNode(userName)->getDataPtr()->getFriends()->first() == NULL) {
    return;
  }
//  cout<< "node user_1 " << user_1->getUsername() << endl;
  User * user_2 = user_1->getFriends()->first()->getData();

  Node<Video *>* video_2 = user_2->getSubscriptions()->first();
//  cout<< "node user_2 " << user_2->getUsername() << endl;
  LinkedList<Video*>* new_list = new LinkedList<Video*>;
  int track = 0;
  int count = 0;
  while(user_1->getFriends()->findPrev(user_2)->getNext()->getNext()) {
  //  cout<< "while user_2 " << user_2->getUsername() << endl;
    video_1 = user_2->getSubscriptions()->first();
    while(video_1) {
    //  cout<< "while video_1 " << video_1->getData()->getTitle() << endl;
      if (new_list->findNode(video_1->getData())) {
    //    cout<< "if video_1 " << video_1->getData()->getTitle() << endl;
        video_1 = video_1->getNext();
        continue;
      }
      else {
    //   cout<< "else video_1 " << video_1->getData()->getTitle() << endl;
        new_list->insertNode(new_list->getHead(), video_1->getData());
        video_1 = video_1->getNext();
        continue;
      }
    }
    user_2 = user_1->getFriends()->findPrev(user_2)->getNext()->getNext()->getData();
  }

  video_1 = new_list->first();
  if (video_1 == NULL ) { delete new_list; return; }
  while(video_1->getNext()) {
    if(video_1->getData()->getTitle() > video_1->getNext()->getData()->getTitle()) {
      new_list->swap(track,track+1);
      count = 0;
      video_1 = new_list->first();
      track = 0;
    }
    else {
        video_1 = video_1->getNext();
        count++;
        track++;
        if (count == new_list->getLength() ) {break;}
        }
   continue;
    }

    video_1 = new_list->first();
    while (video_1) {
    cout << *(video_1->getData());
    video_1 = video_1->getNext();
    }
    cout << endl;
    delete new_list;
    return;
}
bool VideoShare::isConnected(const string & userName1, const string & userName2){
  User * user_1 = users.findNode(userName1)->getDataPtr();
  User * user_2 = users.findNode(userName2)->getDataPtr();
  if (user_1->getFriends()->findNode(user_2)) {return true;}
  while(1) {
    if (user_1->getFriends()->findNode(user_2)) {return true;}
    else {
      user_2 = user_2->getFriends()->first()->getData();
      isConnected(userName1,user_2->getUsername());
    }
    user_2 = user_2->getFriends()->first()->getNext()->getData();
  }
  return false;
  /*  if(users.findNode(userName1) == NULL ||
        users.findNode(userName2) == NULL) {return;}

*/
}
