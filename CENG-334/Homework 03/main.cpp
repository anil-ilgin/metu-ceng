#include <iostream>
#include "fat32.h"
#include "parser.h"
#include "stringTokenizer.h"
#include <cstdio>
#include <fstream>
#include <algorithm>
#include <utility>
#include <chrono>

using namespace std;

BPB_struct bpbStruct;
fstream* image;
uint32_t curr_cluster;
uint32_t parent_cluster;
uint32_t cluster_size;
uint32_t start_of_FAT;
uint32_t start_of_c2;
string curser;
vector<FatFileEntry> directory;
vector<string> current_directory;
vector<uint32_t> cluster_list;
vector<vector<uint32_t>> FATs;
uint32_t sizeofFAT ;
uint32_t cluster;
uint32_t offsetIndex;
FatFileEntry lastFoundEntry;
uint32_t  sector_size;
uint32_t endSignal;
string fileName;

vector<string> printListBasic;
vector<string> printListDetailed;


void getCluster(uint32_t clusterNo) {
    uint32_t offset = start_of_c2 + (clusterNo - 2) * cluster_size;
    image->seekg(offset,ios_base::beg);
}

string getName(FatFileEntry fatFileEntry) {
    string name;
    for(unsigned short i : fatFileEntry.lfn.name1) { if (i) name += (char) i; else return name; }
    for(unsigned short i : fatFileEntry.lfn.name2) { if (i) name += (char) i; else return name; }
    for(unsigned short i : fatFileEntry.lfn.name3) { if (i) name += (char) i; else return name; }
    return name;


}

string getFullName(FatFileEntry fatFileEntry) {
    FatFileEntry dummy = fatFileEntry;
    string name = "";
    while (dummy.lfn.attributes == 15) {
        name += getName(dummy);
        image->read((char*)&dummy, sizeof(dummy));
    }
    if (dummy.msdos.attributes == 16) return name;
    else {
        if (fatFileEntry.msdos.extension[0] == 0) return name;
        name += ".";
        for (unsigned short i: fatFileEntry.msdos.extension) { if (i) name += (char) i; else return name; }
    }
    return name;
}


string getLastModifiedDateTime(FatFileEntry fatFileEntry) {
    string ret;
    uint16_t date = fatFileEntry.msdos.modifiedDate;
    char d[80];
    uint16_t month = date & 0x1E0;
    month = month >> 5;
    if (month == 1) {
        ret += "January";
    }
    else if (month == 2) {
        ret += "February";
    }
    else if (month == 3) {
        ret += "March";
    }
    else if (month == 4) {
        ret += "April";
    }
    else if (month == 5) {
        ret += "May";
    }
    else if (month == 6) {
        ret += "June";
    }
    else if (month == 7) {
        ret += "July";
    }
    else if (month == 8) {
        ret += "August";
    }
    else if (month == 9) {
        ret += "September";
    }
    else if (month == 10) {
        ret += "October";
    }
    else if (month == 11) {
        ret += "November";
    }
    else if (month == 12) {
        ret += "December";
    }

    ret += " ";

    uint16_t day = date & 0x1F;
    ret += to_string(day);
    ret += " ";

    date = fatFileEntry.msdos.modifiedTime;
    uint16_t hour = date & 0xF800;
    hour = hour >> 11;
    ret += to_string(hour);
    ret += ":";

    uint16_t minute = date & 0x7E0;
    minute = minute >> 5;
    ret += to_string(minute);

    return ret;
}

string getFullNameDetailed(FatFileEntry fatFileEntry) {
    FatFileEntry dummy = fatFileEntry;
    string name = "";
    while (dummy.lfn.attributes == 15) {
        name += getName(dummy);
        image->read((char*)&dummy, sizeof(dummy));
    }
    if (dummy.msdos.attributes == 16) {
        string ret_string = "drwx------ 1 root root 0 ";
        ret_string += getLastModifiedDateTime(dummy);
        ret_string += " ";
        ret_string += name;
        return ret_string;
    }
    else {
        string ret_string = "-rwx------ 1 root root ";
        if (fatFileEntry.msdos.extension[0] != 0)  name += ".";
        for (unsigned short i: fatFileEntry.msdos.extension) { if (i) name += (char) i; else break; }
        string fileSize = to_string(dummy.msdos.fileSize);
        ret_string +=  fileSize;
        ret_string += " ";
        ret_string += getLastModifiedDateTime(dummy);
        ret_string += " ";
        ret_string += name;
        return ret_string;
    }
}


uint32_t getFirstCluster(FatFileEntry entry) {
    uint32_t firstOrder = entry.msdos.eaIndex;
    uint32_t secondOrder = entry.msdos.firstCluster;
    firstOrder = firstOrder << 16;
    uint32_t final = firstOrder + secondOrder;
    return final;
}


void printScreen() {
    // for every cluster in clusterList, write print screen.
    string name = "";
    for(int i = 0; i < current_directory.size(); i++) {
        name += current_directory[i];
        if (i != current_directory.size() - 1) name += "/";
    }
    if (current_directory.empty()) {
        name = "/> ";
    }
    else name += "> ";
    cout << name;
}



uint32_t getFATEntry(uint32_t entryNo, uint32_t FATIndex) {
    uint32_t index = start_of_FAT +  4 * entryNo;
    image->seekg(index, ios_base::beg);
    uint32_t x;
    unsigned char arr[4];
    image->read((char*)&arr,4);
    uint32_t next = 0;
    for(int i = 0; i < 4; i++) {
        uint32_t dummy = arr[i];
        next += (dummy << 8*i);
    }
    return next;
}



bool findInDirectory (uint32_t clusterNo, const string& target) {
    FatFileEntry  entry;
    uint32_t limit = cluster_size / sizeof (entry);
    uint32_t currCluster = clusterNo;
    int fatIndex = 0;

    while(1) {
        if (currCluster < 0xFFFFFF8) getCluster(currCluster);
        else return false;
        string name = "";
        uint32_t lfn_count = 0;
        for (uint32_t j = 0; j < limit; j++) {
            image->read((char*)&entry, sizeof(entry));
            if (entry.lfn.sequence_number == 46) continue;
            if (entry.lfn.attributes == 0) return false;
            if (entry.lfn.attributes == 15) {
                lfn_count++;
                name += getName(entry);
                continue;
            }
            else {
                if (name == target) {
                    // FOUND
                    cluster = currCluster;
                    offsetIndex = j - lfn_count;
                    lastFoundEntry = entry;
                    return true;
                }
                else {
                    name.clear();
                    lfn_count = 0;
                    continue;
                }
            }
        }
        currCluster = getFATEntry(clusterNo,fatIndex);
    }

}


void printLs(uint32_t clusterNo) {
    FatFileEntry  entry;
    uint32_t limit = cluster_size / sizeof (entry);
    uint32_t currCluster = clusterNo;
    int fatIndex = 0;
    while(1) {
        if (currCluster < 0xFFFFFF8) getCluster(currCluster);
        else return ;
        string name ;
        uint32_t lfn_count = 0;
        for (uint32_t j = 0; j < limit; j++) {
            image->read((char*)&entry, sizeof(entry));
            if (entry.lfn.attributes == 0) return;
            if (entry.lfn.sequence_number == 46) continue;
            name = getFullName(entry);
            printListBasic.push_back(name);
        }
        currCluster = getFATEntry(clusterNo,fatIndex);
    }

}

void printLsDetailed(uint32_t clusterNo) {
    FatFileEntry  entry;
    uint32_t limit = cluster_size / sizeof (entry);
    uint32_t currCluster = clusterNo;
    int fatIndex = 0;
    while(1) {
        if (currCluster < 0xFFFFFF8) getCluster(currCluster);
        else return ;
        string name ;
        uint32_t lfn_count = 0;
        for (uint32_t j = 0; j < limit; j++) {
            image->read((char*)&entry, sizeof(entry));

            if (entry.lfn.attributes == 0) return;
            if (entry.lfn.sequence_number == 46) continue;
            name = getFullNameDetailed(entry);
            printListDetailed.push_back(name);
        }
        currCluster = getFATEntry(clusterNo,fatIndex);
    }
}

uint32_t getFirstFreeFAT() {
    uint32_t index = start_of_FAT;
    image->seekg(index, ios_base::beg);
    unsigned char arr[4];
    bool notfound = true;
    uint32_t ind = 0;
    while( notfound ) {
        image->read((char*)&arr,4);
        uint32_t next = 0;
        for(int i = 0; i < 4; i++) {
            uint32_t dummy = arr[i];
            next += (dummy << 8*i);
        }
        if (next == 0){
            notfound = false;
        }
        else {
            ind++;
        }
    }
    return ind;

}


vector<FatFileEntry> findPath(string path, int mode) {
    vector<string> list;
    vector<string> dummy_directory;
    vector<FatFileEntry> dummy_ffe;

    uint32_t currCluster;
    if (directory.empty()) currCluster = 2;
    else currCluster = getFirstCluster(directory.back());

    if (! path.empty()) {
        list = tokenizeStringPath(path);
        if (mode == 1) {
            list.pop_back();
            if (list.empty()) {
                //we are looking for current
                for(FatFileEntry elem : directory) {
                    dummy_ffe.push_back(elem);
                }
                return dummy_ffe;
            }
        }
    }


    if (list[0] != "") {
        for(const string& elem : current_directory)
        {
            dummy_directory.push_back(elem);
        }
        for(const FatFileEntry& ffe : directory) {
            dummy_ffe.push_back(ffe);
        }
    }


    for(const string& elem: list) {
        if (elem == ".") {
            continue;
        }
        else if (elem == "..") {
            dummy_directory.pop_back();
            dummy_ffe.pop_back();

        }
        else if (elem == "") {
            currCluster = 2;
        }
        else {

            bool isItFound = findInDirectory(currCluster, elem);
            if (isItFound) {
                FatFileEntry fatFileEntry;
                getCluster(cluster);
                if (offsetIndex) image->seekg(offsetIndex*sizeof(fatFileEntry), ios_base::cur);
                image->read((char*)&fatFileEntry, sizeof (fatFileEntry));
                string dir_name = getFullName(fatFileEntry);
                dummy_ffe.push_back(lastFoundEntry);
                dummy_directory.push_back(dir_name);
                currCluster = getFirstCluster(lastFoundEntry);
            }
            else {
                dummy_ffe.clear();
                return dummy_ffe;
            }
        }

    }
    return dummy_ffe;

}


vector<string> PtokenizeStringPath(string p){
    int pos = -1;
    vector<string> v;
    while((pos = p.find('.'))!=-1){
        string dir = p.substr(0, pos);
        v.push_back(dir);
        p = p.substr(pos+1);
    };

    //* uncomment if you want to have / for paths starting from root. Currently it is ""(empty string) instead of "/"
    // if(v.size()){
    // 	if(v[0] == ""){
    // 		v[0]="/";
    // 	}
    // }

    if(p.size()){
        v.push_back(p);
    }

    return v;
}

void editModifiedAndCreationTime(FatFile83 &fatFileEntry) {
    const time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    uint16_t year = localtime(&now)->tm_year - 80;
    uint16_t mon = localtime(&now)->tm_mon + 1;
    uint16_t day = localtime(&now)->tm_mday;
    uint16_t hour = localtime(&now)->tm_hour;
    uint16_t minute = localtime(&now)->tm_min;
    uint16_t sec = localtime(&now)->tm_sec / 2;
    fatFileEntry.creationTime = sec + (minute << 5) + (hour << 11);
    fatFileEntry.creationDate = day + (year << 9) + (mon << 5);
    fatFileEntry.modifiedTime = sec + (minute << 5) + (hour << 11);
    fatFileEntry.modifiedDate = day + (year << 9) + (mon << 5);
}

void editModifiedTime(FatFileEntry& fatFileEntry) {
    const time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    uint16_t year = localtime(&now)->tm_year - 80;
    uint16_t mon = localtime(&now)->tm_mon + 1;
    uint16_t day = localtime(&now)->tm_mday;
    uint16_t hour = localtime(&now)->tm_hour;
    uint16_t minute = localtime(&now)->tm_min;
    uint16_t sec = localtime(&now)->tm_sec / 2;
    fatFileEntry.msdos.modifiedTime = sec + (minute << 5) + (hour << 11);
    fatFileEntry.msdos.modifiedDate = day + (year << 9) + (mon << 5);
}



vector<FatFileLFN> createLFN(string name) {
    vector<FatFileLFN> ret;
    int count = (int)((name.length() - 1) / 13);
    int x = 0;
    for(int i = 0; i <= count; i++) {
        FatFileLFN fatFileLfn;
        fatFileLfn.attributes = 15;

        //Assign dates.


        for(int j = 0; j < 5; j++) {
            if( x < name.length() ) {
                fatFileLfn.name1[j] =  name[x];
                x++;
            }
            else {
                fatFileLfn.name1[j] = 0;
            }
        }

        for(int j = 0; j < 6; j++) {
            if( x < name.length() ) {
                fatFileLfn.name2[j] =  name[x];
                x++;
            }
            else {
                fatFileLfn.name2[j] = 0;
            }
        }

        for(int j = 0; j < 2; j++) {
            if( x < name.length() ) {
                fatFileLfn.name3[j] =  name[x];
                x++;
            }
            else {
                fatFileLfn.name3[j] = 0;
            }
        }
        fatFileLfn.reserved = 0;
        fatFileLfn.firstCluster = 0;

        ret.push_back(fatFileLfn);
    }

    return ret;
}


FatFile83 create83Dir(const string& name) {
    FatFile83 fatFile83;
    uint32_t clusterNo = getFirstFreeFAT();
    fatFile83.attributes = 16;
    fatFile83.fileSize = 0;
    fatFile83.eaIndex = (uint16_t ) (clusterNo >> 16);
    fatFile83.firstCluster = (uint16_t) (clusterNo);

    uint32_t offset = start_of_FAT + (clusterNo)*4;
    image->seekp(offset, ios_base::beg);
    for(int i= 0 ; i < 4; i++) {
        uint8_t toWrite = (endSignal >> 8*i);
        image->write((char*)&toWrite, 1);
    }

    return fatFile83;
}

FatFile83 create83File(string name) {
    vector<string> tokenized = PtokenizeStringPath(name);
    FatFile83 fatFile83;
    for(int i = 0; i < 3; i++) {
        fatFile83.extension[i] = tokenized[1][i];
    }
    uint32_t clusterNo = getFirstFreeFAT();
    fatFile83.eaIndex = (uint16_t ) (clusterNo >> 16);
    fatFile83.firstCluster = (uint16_t) (clusterNo);

    uint32_t offset = start_of_FAT + (clusterNo)*4;
    image->seekp(offset, ios_base::beg);
    for(int i= 0 ; i < 4; i++) {
        uint8_t toWrite = (endSignal >> 8*i);
        image->write((char*)&toWrite, 1);
    }

    fatFile83.fileSize = 0;
    return fatFile83;

}






void commandCD(parsed_input* parsedInput) {
    char* string1 = parsedInput->arg1;
    string path(string1);
    vector<string> list;
    vector<string> dummy_directory;
    vector<FatFileEntry> dummy_ffe;

    uint32_t currCluster;
    if (directory.empty()) currCluster = 2;
    else currCluster = getFirstCluster(directory.back());

    if (! path.empty()) {
        list = tokenizeStringPath(path);
    }

    if (list[0] != "") {
        for(const string& elem : current_directory)
        {
            dummy_directory.push_back(elem);
        }
        for(const FatFileEntry& ffe : directory) {
            dummy_ffe.push_back(ffe);
        }
    }



    for(const string& elem: list) {
        if (elem == ".") {
            continue;
        }
        else if (elem == "..") {
            dummy_directory.pop_back();
            dummy_ffe.pop_back();

        }
        else if (elem == "") {
            currCluster = 2;
        }
        else {

            bool isItFound = findInDirectory(currCluster, elem);
            if (isItFound) {
                FatFileEntry fatFileEntry;
                getCluster(cluster);
                if (offsetIndex) image->seekg(offsetIndex*sizeof(fatFileEntry), ios_base::cur);
                image->read((char*)&fatFileEntry, sizeof (fatFileEntry));
                string dir_name = getFullName(fatFileEntry);
                dummy_ffe.push_back(lastFoundEntry);
                dummy_directory.push_back(dir_name);
                currCluster = getFirstCluster(lastFoundEntry);
            }
            else {
                return;
            }
        }

    }

    current_directory.clear();
    directory.clear();
    for(const string& elem : dummy_directory) current_directory.push_back(elem);
    for(FatFileEntry elem: dummy_ffe) directory.push_back(elem);
    curr_cluster = getFirstCluster(directory.back());
    // cd into given directory, then call printScreen

}



void commandLS(parsed_input* parsedInput) {
    // LS the current list, with given detailed argument.
    char* arg1 = parsedInput->arg1;
    char* arg2 = parsedInput->arg2;
    if (arg1 == nullptr)
    {
        uint32_t clusterNo;
        if (directory.empty()) clusterNo = 2;
        else {
            FatFileEntry entry = directory.back();
            clusterNo = getFirstCluster(entry);
        }
        printLs(clusterNo);
        if ( !printListBasic.empty() ) {
            for (int i = 0 ; i < printListBasic.size(); i++) {
                cout << printListBasic[i];
                if (i != printListBasic.size() - 1) cout << " ";
            }
            cout << endl;
        }
        printListBasic.clear();
        return;
        // Just ls current
    }
    else
    {
        string s_arg1(arg1);

        if(s_arg1 != "-l")
        {
            vector<FatFileEntry> dummy_directory = findPath(s_arg1,0);
            if (dummy_directory.empty()) return;
            FatFileEntry back = dummy_directory.back();
            uint32_t clusterNo = getFirstCluster(back);
            printLs(clusterNo);
            if ( !printListBasic.empty() ) {
                for (int i = 0 ; i < printListBasic.size(); i++) {
                    cout << printListBasic[i];
                    if (i != printListBasic.size() - 1) cout << " ";
                }
                cout << endl;
            }
            printListBasic.clear();
            return;
            // ls less with given path.

        }
        else
        {
            if (arg2 == nullptr)
            {
                // detailed LS
                uint32_t clusterNo;
                if (directory.empty()) clusterNo = 2;
                else {
                    FatFileEntry entry = directory.back();
                    clusterNo = getFirstCluster(entry);
                }
                printLsDetailed(clusterNo);
                if ( !printListDetailed.empty() ) {
                    for (int i = 0 ; i < printListDetailed.size(); i++) {
                        cout << printListDetailed[i] << endl;
                    }
                }
                printListDetailed.clear();
                return;
            }
            else
            {
                string s_arg2(arg2);
                vector<FatFileEntry> dummy_directory = findPath(s_arg2,0);
                if (dummy_directory.empty()) return;
                FatFileEntry back = dummy_directory.back();
                uint32_t clusterNo = getFirstCluster(back);
                printLsDetailed(clusterNo);
                if ( !printListDetailed.empty() ) {
                    for (int i = 0 ; i < printListDetailed.size(); i++) {
                        cout << printListDetailed[i] << endl;
                    }
                }
                printListDetailed.clear();
                return;

                // ls full with given path.
            }
        }
    }

}

void writeToCluster(uint32_t firstCluster, vector<FatFileLFN>& LFNList, FatFile83 dir ) {

    uint32_t currCluster = firstCluster;
    uint32_t  requiredSize = (1 + LFNList.size())* 32;
    uint32_t reqSize = (1 + LFNList.size());
    bool isNotDone = true;
    uint32_t prevCluster = currCluster;
    while (isNotDone) {
        if (currCluster == endSignal) {
            uint32_t freeCluster = getFirstFreeFAT();
            uint32_t offset = start_of_FAT + (prevCluster)*4;
            image->seekp(offset, ios_base::beg);
            for(int i = 0 ; i < 4; i++) {
                uint8_t toWrite = (freeCluster >> 8*i);
                image->write((char*)&toWrite, 1);
            }
            offset = start_of_FAT + (freeCluster)*4;
            image->seekp(offset, ios_base::beg);
            for(int i= 0 ; i < 4; i++) {
                uint8_t toWrite = (endSignal >> 8*i);
                image->write((char*)&toWrite, 1);
            }
            currCluster = freeCluster;
            // allocate new cluster and update the FatTable.
        }
        getCluster(currCluster);
        uint32_t counter = 0;
        bool enoughSpace = false;
        uint32_t index = 0;
        FatFileEntry dummy;
        uint32_t range = cluster_size / sizeof(FatFileEntry);
        for(int32_t i = 0; i < range; i++) {
            image->read((char*)&dummy, sizeof(FatFileEntry));
            if (dummy.msdos.filename[0] == 0) {
                counter++;
            }
            else if(dummy.lfn.sequence_number == 0) {
                counter++;
            }
            else {
                counter = 0;
            }

            if (counter == reqSize) {
                enoughSpace = true;
                index = i - (reqSize-1);
                break;
            }
        }
/*
        for(uint32_t i = 0; i < cluster_size;) {
            if (arr[0] != 0) {
                counter = 0;
            }
            else {
                counter++;
            }
            if (counter == requiredSize) {
                enoughSpace = true;
                index = i - 31;
                break;
            }
        }
*/
        if (enoughSpace) {
            isNotDone = false;
            uint32_t off = start_of_c2 + (currCluster - 2) * cluster_size + index* sizeof(FatFileEntry);
            image->seekp(off, ios_base::beg);
            for(FatFileLFN elem : LFNList) {
                image->write((char*)&elem, sizeof(elem));
            }
            image->write((char*)& dir, sizeof(dir));

            // write to here.
        }
        else  {
            prevCluster = currCluster;
            currCluster = getFATEntry(currCluster, 0);
            //go to next cluster, do it again.
        }

        //traverse the insides
    }


}


void commandMKDIR(parsed_input* parsedInput) {
    // create a directory under given arg
    string path(parsedInput->arg1);
    vector<string> tokenized = tokenizeStringPath(path);
    if (tokenized.size() == 1 && directory.empty()) {
        // we are in the root.
        string name = tokenized.back();
        vector<FatFileLFN> LFNList = createLFN(name);
        FatFile83 dir = create83Dir(name);
        editModifiedAndCreationTime(dir);
        uint32_t firstEmpty = getFirstFreeFAT();

        uint32_t firstCluster = 2;

        writeToCluster(firstCluster, LFNList, dir);

    }
    else {
        //store the old directory
        vector<FatFileEntry> parentDirectory;
        parentDirectory = findPath(path,1);
        if (parentDirectory.empty()) return;
        string name = tokenized.back();
        vector<FatFileLFN> LFNList = createLFN(name);
        FatFile83 dir = create83Dir(name);
        editModifiedAndCreationTime(dir);
        uint32_t firstEmpty = getFirstFreeFAT();

        FatFileEntry parent = parentDirectory.back();
        uint32_t firstCluster = getFirstCluster(parent);

        writeToCluster(firstCluster, LFNList, dir);

    }


}

void commandTOUCH(parsed_input* parsedInput) {
    // create a directory under given arg
    string path(parsedInput->arg1);
    vector<string> tokenized = tokenizeStringPath(path);
    if (tokenized.size() == 1 && directory.empty()) {
        // we are in the root.
        string name = tokenized.back();
        vector<FatFileLFN> LFNList = createLFN(name);
        FatFile83 file83 = create83File(name);
        editModifiedAndCreationTime(file83);
        uint32_t firstEmpty = getFirstFreeFAT();

        uint32_t firstCluster = 2;

        writeToCluster(firstCluster, LFNList, file83);

    }
    else {
        //store the old directory
        vector<FatFileEntry> parentDirectory;
        parentDirectory = findPath(path,1);
        if (parentDirectory.empty()) return;
        string name = tokenized.back();
        vector<FatFileLFN> LFNList = createLFN(name);
        FatFile83 file83 = create83File(name);
        editModifiedAndCreationTime(file83);
        uint32_t firstEmpty = getFirstFreeFAT();

        FatFileEntry parent = parentDirectory.back();
        uint32_t firstCluster = getFirstCluster(parent);

        writeToCluster(firstCluster, LFNList, file83);

    }

}

void commandMV(parsed_input* parsedInput) {
    // move the given arg1 to arg2
}

void commandCAT(parsed_input* parsedInput) {
    // print out the files.
    string s(parsedInput->arg1);
    vector<FatFileEntry> ffe = findPath(s,0);
    if (ffe.empty()) return;
    FatFileEntry entry = ffe.back();
    uint32_t clusterNo = getFirstCluster(entry);
    string output;
    while (clusterNo != endSignal) {
        getCluster(clusterNo);
        char x = 0;
        for(int i = 0; i < cluster_size; i++) {
            image->read((char*)&x, 1);
            if (x == 0) break;
            else output+= x;
        }
        clusterNo = getFATEntry(clusterNo, 0);
    }
    cout << output << endl;

}



int main(int argc, char* argv []) {
    // Read image file and fillout bpbStruct
    image = new fstream (argv[1], ios_base::in | ios_base::out | ios::binary);
    image->read((char*)&bpbStruct, sizeof(bpbStruct));
    //  PrintBPB(bpbStruct);
    string s(argv[1]);
    fileName = s;
    curr_cluster = bpbStruct.extended.RootCluster;
    parent_cluster = curr_cluster;
    sector_size = bpbStruct.BytesPerSector;
    cluster_size = bpbStruct.BytesPerSector * bpbStruct.SectorsPerCluster;
    start_of_FAT = bpbStruct.BytesPerSector * bpbStruct.ReservedSectorCount;
    start_of_c2 = start_of_FAT + bpbStruct.NumFATs * bpbStruct.extended.FATSize * bpbStruct.BytesPerSector;
    endSignal = getFATEntry(0,0);
    uint32_t x = getFATEntry(2,0);
    printScreen();


    while(true) {
        parsed_input parsedInput ;
        string inp;
        getline(cin, inp);
        inp += "\n";
        char* line = strdup(inp.c_str());
        parse(&parsedInput, line);
       // cout << parsedInput.arg1 << " " << parsedInput.arg2 << endl;
       if ( parsedInput.type == CD) {
           commandCD(&parsedInput);
           printScreen();
       }
       else if (parsedInput.type == LS) {
           commandLS(&parsedInput);
           printScreen();
       }
       else if (parsedInput.type == MKDIR) {
           commandMKDIR(&parsedInput);
           printScreen();
       }
       else if (parsedInput.type == TOUCH) {
           commandTOUCH(&parsedInput);
           printScreen();
       }
       else if (parsedInput.type == MV) {
           commandMV(&parsedInput);
           printScreen();
       }
       else if (parsedInput.type == CAT) {
           commandCAT(&parsedInput);
           printScreen();
       }
       else if (parsedInput.type == QUIT) {
           image->close();
           clean_input(&parsedInput);
           return 0;
       }


       clean_input(&parsedInput);
    }



  return 0;
}
