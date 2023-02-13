#ifndef _HW3_H
#define _HW3_H
#include <iostream>
#include <string>
#include <vector>
#include <utility>  // use just for std::pair< >
#include <fstream>

#define MAX_LOAD_FACTOR 0.65
#define EMPTY "EMPTY"
#define DELETED "DELETED"


class AccessControl
{
public:
	AccessControl(int table1Size, int table2Size);
	~AccessControl();

	int addUser(std::string username, std::string pass);
	int addUsers(std::string filePath);
	int delUser(std::string username, std::vector<std::string>& oldPasswords);
	int changePass(std::string username, std::string oldpass, std::string newpass);

	int login(std::string username, std::string pass);
	int logout(std::string username);

	float printActiveUsers();
	float printPasswords();
private:
	std::vector<std::pair<std::string, std::string > > regUsers;
	std::vector<std::string> activeUsers;
	std::vector<std::pair<std::string, int > > user_pass_count;
	// user_pass_count holds pairs of <username,integer> which indicate 
	// how many times each user registered to regUsers via addUser or changePassword 
	
	int user_count;                             // how many users registered,
	int reg_count;                              // how many entries in regUsers
	int active_count;                           // how many users logged in
	int regtableSize, activetableSize;          // current size of regUsers and activeUsers
	double load_factor_reg;                     // percentage of loadness in regUsers
	double load_factor_active;                  // percentage of loadness in activeUsers;
	
	//<<your hash class>> activeUsers;	// hash(username) -> username
	//<<your hash class>> regUsers;  		// hash(username) -> username,password

	int hashFunction(std::string key, int tableSize, int i) {
	   int length = key.length();
	   int newK = 0;
	   for (int i = 0; i < length; i++)
	     newK += (int) key[i];

		 // hash function 1 -> (newK % tableSize)
		 // hash function 2 -> (newK * tableSize - 1) % tableSize)

	   return ((newK % tableSize) + i * ((newK * tableSize - 1) % tableSize)) % tableSize;
	}

    // getPrime returns the first prime number which is greater than 2*(tablesize)
	int getPrime(int tableSize) {
		int isPrime = 0;
		int start = tableSize*2;
		int divisor = 2;
		if (tableSize == 1) {return 3;}
		while(1)
		{
			for (int i = 0; i<(start/2 + 1); i++)
			{
				if (start%divisor == 0) {	isPrime = 0;	break;    	}
				else                    { isPrime = 1;	divisor++; 	}
			}
			if (isPrime) { return start;                    }
			else         { start++; divisor = 2; continue;	}
		}
	}
    
    // pass_counter records how many times a username registered in regUser via addUser or changePasswords by
    // changing or adding the value in user_pass_count.
	void pass_counter(std::string username) {
		int i;
		for(i=0; i < user_count; i++) {
			if (user_pass_count[i].first == username) {
				user_pass_count[i].second += 1;
				return;
			}
		}
		user_pass_count[i].first = username;
		user_pass_count[i].second += 1;
		return;
	}

 
    
    // rehash_table_reg() creates a new table with new size 
    // then rehashes all the values in original to new table for regUser
    
	void rehash_table_reg() {
		std::vector<std::pair<std::string, std::string > > temp_regUsers;
		// control_vector is holding strings of username to control if it rehashed every value of given user_name or not
		std::vector<std::string> control_vector; 
		int new_size = getPrime(regtableSize);
		int hash, hash_new, i, k, index, new_index, count,size_of_control_vector, control,l;
		temp_regUsers.resize(new_size);
		std::string user_name;
		//creates new empty table size of min(prime_number) > 2*tablesize
		for(i=0; i<new_size; i++) {
			temp_regUsers[i].first = EMPTY;
			temp_regUsers[i].second = EMPTY;
		}
		size_of_control_vector = 0;
		//will iterate until end of the original table
		for(i=0; i < regtableSize; i++) {
			user_name = regUsers[i].first;
			if ( user_name != EMPTY && user_name != DELETED ) {
				count = 0;
				//checking how many times this user iterated in regUser
				for(int x=0; x < user_count; x++) {
					if(user_pass_count[x].first == user_name) {
						count = user_pass_count[x].second;
					}
				}
				index = 0; new_index = 0; k = 0; control = 0;
				//checking if every element of same username is rehashed to new one, if so control changes to 1
				for(l=0; l < size_of_control_vector; l++ ) {
	                if (user_name == control_vector[l] ) {
	                    control = 1;
	                }
	            }
	            if (control == 1) {
	                continue;
	          
	            }
	            //iterate until we rehashed every element of same username
				while(k < count )
				{
					hash = hashFunction(user_name,regtableSize, index);
					if (regUsers[hash].first == user_name)
					{   
					    //iterate until find an empty spot 
						while(new_index < new_size)
						{
							hash_new = hashFunction(user_name, new_size, new_index);
							if(temp_regUsers[hash_new].first == EMPTY )
							{
								temp_regUsers[hash_new].first = regUsers[hash].first;
								temp_regUsers[hash_new].second = regUsers[hash].second;
								index++;
								k++;
								break;
							}
							else
							{
								new_index++;
								if (new_index > new_size) {
									break;
								}
							}
						}
					}
					else 
					{
						index++;
						if(index > regtableSize) {
							break;
						    }
					}
				}
				//rehashed every element of same username
					control_vector.push_back(user_name);
					size_of_control_vector = control_vector.size();
			}
		}
		//rehashed the whole original table
		user_pass_count.resize(new_size);
		for (int m = user_count; m < new_size; m++) {
		    user_pass_count[m].first = EMPTY;
		    user_pass_count[m].second = 0;
		}
		regUsers.clear();
		regUsers.resize(new_size);
		regUsers = temp_regUsers;
		regtableSize = new_size;
		load_factor_reg = float(reg_count) / float(regtableSize);
		//updated every data of regUsers
		return;
	}
	
     // rehash_table_active() creates a new table with new size 
     // then rehashes all the values in original to new table for activeUsers
	void rehash_table_active () {
		std::vector< std::string > temp_activeUsers;
		int new_size = getPrime(activetableSize);
		temp_activeUsers.resize(new_size);
		int hash,i,j,k,index = 0;
		//creates new empty table with size of min(prime_number) > 2*tablesize 
		for(i=0; i<new_size; i++) {
			temp_activeUsers[i] = EMPTY;
		}
		//iterates until we rehash every member
		for(i=0; i < activetableSize; i++) {
			index = 0;
			//iterates until rehash the current element
			while (index < activetableSize) {
				if(activeUsers[i] != DELETED && activeUsers[i] != EMPTY) {
					hash = hashFunction(activeUsers[i],new_size,index);
					if (temp_activeUsers[hash] == EMPTY) {
						temp_activeUsers[hash] = activeUsers[i];
						break;
					}
					else {
						index++;
						continue;
					}
				}
				index++;
			}
		}
		//updates data of activeUser
		activeUsers.clear();
		activeUsers.resize(new_size);
		activeUsers = temp_activeUsers;
		activetableSize = new_size;
		load_factor_active = float(active_count) / float(activetableSize);
		return;
	}
    //checks if user is logged in
	int check_login(std::string user_name) {
		int i;
		for(i=0; i < activetableSize; i++) {
			if (activeUsers[i] == user_name) {
				return 1;
			}
			else {
				continue;
			}
		}
		return 0;
	}

};
#endif
