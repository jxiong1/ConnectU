/*
 * PROJECT: CONNECT-U (Starter Code)
 * Course: ECE367L Data Structures & Algorithms
 * 
 * SAFETY UPDATE:
 * The saveData() function call in main() is currently COMMENTED OUT.
 * This prevents you from accidentally wiping your 'posts.csv' file
 * if your Lab 1 implementation is incomplete.
 * * ONLY uncomment saveData() after you have verified Lab 1 works!
 */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>
#include <cctype>

using namespace std;

// ==========================================
// MODELS & DATA STRUCTURES
// ==========================================

//This is a change

struct Post {
    int postId;
    int userId;
    string content;
    int likes;
    long timestamp;
    Post* next; 

    Post(int pid, int uid, string txt, int lk, long time) 
        : postId(pid), userId(uid), content(txt), likes(lk), timestamp(time), next(nullptr) {}
        
    // TODO: LAB 3 - Implement Scoring Logic
    double getScore() {
        double hoursOld = (time(0)-timestamp) / 3600;
        double score = (likes * 10) + (1000/hoursOld + 1);
        return score; 
    }
};

// TODO: LAB 1 - Linked List
class Timeline {
public:
    Post* head;
    Timeline() : head(nullptr) {}

    // Task: Add a new post to the FRONT of the list (O(1))
    void addPost(int pid, int uid, string content, int likes, long time) {
        // TODO: LAB 1
        Post* newPost = new Post(pid, uid, content, likes, time); //new pointer to a new Post object

        Post* oldPost = head; //Insert newPost in head node, push oldPost back
        head = newPost;
        
        newPost->next = oldPost;
    }

    void printTimeline() {
        Post* current = head;
        if (!current) { cout << "  (No posts yet)" << endl; return; }
        
        // Task: Traverse the linked list and print content
        // TODO: LAB 1

        while(current != nullptr){ //Prints content for every post in the linked list
            cout << current->content << endl;
            current = current->next;
        }

    }

};

// Forward Declaration
class User;

// TODO: LAB 4 - Binary Search Tree
struct BSTNode {
    User* user;
    BSTNode* left;
    BSTNode* right;
    BSTNode(User* u) : user(u), left(nullptr), right(nullptr) {}
};

class FriendBST {
public:
    BSTNode* root;
    FriendBST() : root(nullptr) {}

    BSTNode* insert(BSTNode* node, User* u) ;

    void printInOrder(BSTNode* node);

    void addFriend(User* u) { root = insert(root, u); }

    void printFriends() {
        if (root == nullptr) cout << "  (No friends yet)" << endl;
        else printInOrder(root);
    }
};

class User {
public:
    int userId;
    string username;
    int techScore, artScore, sportScore;
    
    Timeline timeline;       // Lab 1
    vector<User*> friends;   // Graph
    FriendBST friendTree;    // Lab 4

    User(int id, string name, int t, int a, int s) 
        : userId(id), username(name), techScore(t), artScore(a), sportScore(s) {}

    void addPost(int pid, string content, int likes, long time) {
        timeline.addPost(pid, userId, content, likes, time);
    }

    void addFriend(User* u) {
        friends.push_back(u);       
        friendTree.addFriend(u);    
    }
    
    vector<User*> getFriendsList() { return friends; }
};

// BST Implementation
BSTNode* FriendBST::insert(BSTNode* node, User* u) {
    // TODO: LAB 4
    if(node == nullptr){
        return new BSTNode(u);
    }

    //Converts the username string into lower case for ASCII comparison
    string currentName = (node->user->username);

    for(char& c : currentName){
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }

    string newName = u->username;

    for(char& c : newName){
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }

    //If the current node's ASCII is bigger, then traverse to the left child and run the check again
    if(currentName > newName){
        node->left = insert(node->left, u);
    }
    //If the current node's ASCII is smaller, then traverse to the right child and run the check again
    else{
        node->right = insert(node->right, u);
    }

    return node;
}
void FriendBST::printInOrder(BSTNode* node) {
    // TODO: LAB 4
    //Return if it reached a nullptr
    if(node == nullptr){
        return;
    }
    
    //Go all the way left of the tree
    printInOrder(node->left);
    //Print out the username after the previous recursive call returns
    cout<<node->user->username+"\n";
    //Go to the right if available to print out the right child
    printInOrder(node->right);
}

// TODO: LAB 3 - Max Heap
class FeedHeap {
private:
    Post* heap[1000]; 
    int size;

    void heapifyDown(int index) { 
        /* TODO: LAB 3 */
        int leftChildIndex = 2*index + 1;
        int rightChildIndex = 2*index + 2;

        int childCandidate;
        //If both child exist, find the bigger child and assign as candidate for comparison
        if(leftChildIndex < size && rightChildIndex < size){ 
                if(heap[leftChildIndex]->getScore() > heap[rightChildIndex]->getScore()){
                    childCandidate = leftChildIndex;
                }
                else{
                    childCandidate = rightChildIndex;
                }
            }
        //If only one child exist, assign that child for comparison
        else if(leftChildIndex < size){
            childCandidate = leftChildIndex;
        }
        else if(rightChildIndex < size){
            childCandidate = rightChildIndex;
        }
        //If no child exist, this mean the current node is leaf, then return
        else{
            return;
        }

        //If parent smaller than the biggest child, then swap
        if(heap[index]->getScore() < heap[childCandidate]->getScore()){
            Post* temp = heap[index];
            heap[index] = heap[childCandidate];
            heap[childCandidate] = temp;
        }
        //Return/stops recursive if parent is greater than child
        else{
            return;
        }

        /*Recursive function to ensure that the original index value 
        keep going down until the max heap condition is met */
        return heapifyDown(childCandidate);

    }
    void heapifyUp(int index) { 
        /* TODO: LAB 3 */
        //Gets the parent index
        int parentIndex = (index-1)/2;

        /*Stops the recursive if the original index value is at root
          or parent node does not exist*/
        if(parentIndex < 0 || heap[parentIndex] == nullptr){
            return;
        }
        //if the current parameter index's score is greater than the parent,
        //then, swap the value of the parentIndex and the parameter index
        if(heap[index]->getScore() > heap[parentIndex]->getScore()){
            Post* temp = heap[index];
            heap[index] = heap[parentIndex];
            heap[parentIndex] = temp;
        }
        //Stops recursive if parent is greater
        else{
            return;
        }
        return heapifyUp(parentIndex);
    }

public:
    FeedHeap() : size(0) {}
    void push(Post* p) { 
        /* TODO: LAB 3 */
        //Sets the new Post as the last node of the heap
        heap[size] = p;
        size++;

        //heapify up to complete the max heap structure
        heapifyUp(size-1);
     }
    Post* popMax() { 
        /* TODO: LAB 3 */ 
        if(size == 0){
            return nullptr;
        }
        Post* max = heap[0];
        //Set lastIndex as new root of heap
        Post* lastIndex = heap[size - 1];
        heap[0] = heap[size-1];
        size --;
        //Push that node down to make the heap valid
        heapifyDown(0);

        //Returns the max value / old root of heap
        return max;  
    }
    bool isEmpty() { return size == 0; }
};

vector<User*> allUsers;

// TODO: LAB 2 - Hash Map
struct HashNode {
    string key;
    User* value;
    HashNode* next;
    HashNode(string k, User* v) : key(k), value(v), next(nullptr) {}
};

class UserMap {
private:
    static const int TABLE_SIZE = 10007; 
    HashNode** table;

    //Polynomial Rolling Hash
    unsigned long hashFunction(string key) {
        unsigned long hash = 0;
        unsigned long prime = 31;
        for(char c : key){
            hash = hash * prime + c;
        }
        return hash; 
    }

public:
    UserMap() {
        table = new HashNode*[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++) table[i] = nullptr;
    }

    //Inserts and update User* in the hash table
    void put(string key, User* user) {
        //Converts key into index
        unsigned long i = hashFunction(key) % TABLE_SIZE;
        //Gets head of linked list
        HashNode* currUser = table[i];

        //Traverses linked list
        while(currUser != nullptr) {
            //Checks if key exist (collision happens), update value if it does
            if(currUser->key == key) {
                currUser->value = user;
                return;
            }
            currUser = currUser->next;
        }
        //Adds new user node if no collsion happens
        HashNode* node = new HashNode(key, user);
        node->next = table[i];
        table[i] = node;
    }

    //Searches for a key in the hash table and returns the associated User*
    User* get(string key) {
        // TODO: LAB 2 - REPLACE ABOVE WITH HASH LOOKUP
        //Converts key into index
        unsigned long i = hashFunction(key) % TABLE_SIZE;
        //get head of linked list
        HashNode* currUser = table[i];

        //Traveses linked list
        while(currUser != nullptr) {

            //if key matches, return Users*
            if(currUser->key == key){
                return currUser->value;
            }
            currUser = currUser->next;
        }
        return nullptr;
    }
};

UserMap userMap;

// ==========================================
// UTILITY FUNCTIONS
// ==========================================

vector<string> split(string s) {
    vector<string> tokens;
    string token;
    bool inQuotes = false;
    
    for (char c : s) {
        if (c == '"') {
            inQuotes = !inQuotes; // Toggle quote state
            continue; // Skip the quote character itself
        }
        
        if (c == ',' && !inQuotes) {
            // Found a delimiter outside of quotes -> New Token
            tokens.push_back(token);
            token.clear();
        } else {
            // Regular character or comma inside quotes
            token += c;
        }
    }
    tokens.push_back(token); // Add last token
    return tokens;
}


int GLOBAL_POST_ID_COUNTER = 1;

Post* findPostById(int id) {
    for (User* u : allUsers) {
        Post* curr = u->timeline.head;
        while (curr != nullptr) {
            if (curr->postId == id) return curr;
            curr = curr->next;
        }
    }
    return nullptr;
}

void createNewPost(User* author, string content) {
    int postId = GLOBAL_POST_ID_COUNTER++;
    long timestamp = time(0); 
    author->addPost(postId, content, 0, timestamp);
    cout << "\n[SUCCESS] Post saved to timeline." << endl;
}

void registerNewUser(string username, int tech, int art, int sport) {
    int newId = allUsers.size() + 1; 
    User* newUser = new User(newId, username, tech, art, sport);
    allUsers.push_back(newUser);
    userMap.put(username, newUser);
    cout << "\n[SUCCESS] Account created." << endl;
}

void addFriendship(User* requester, User* target) {
    requester->addFriend(target);
    target->addFriend(requester);
    cout << "\n[SUCCESS] You are now friends with @" << target->username << endl;
}

// TODO: LAB 5 - Breadth First Search
void recommendFriends(User* startUser) {
    cout << "\n[GRAPH ANALYSIS] Finding friends of friends..." << endl;
    // TODO: LAB 5

    if(!startUser) return;

    queue<pair<User*, int>> q;
    set<int>visited;

    //Mark startUSer (yourself) as visited
    visited.insert(startUser->userId);

    //Enqueue your direct friends, mark them as visited
    for(User* friendUser : startUser->friends){
        q.push({friendUser, 1});
        visited.insert(friendUser->userId);
    }

    /*BFS, while q is not empty, traverse the a level at a time
      by adding its children node (mutuals) to the end of the q*/
    while(!q.empty()){
        User* current = q.front().first;
        int level = q.front().second;
        q.pop();

        //Print only at the second level
        if(level == 2){
            cout<<"\n"<<current->username<< " - "<< current->userId << endl;
            continue;
        }

        //Add children nodes to the q before the second level
        if(level < 2){
            for(User* mutual : current->friends){
                if(visited.find(mutual->userId) == visited.end()){
                    q.push({mutual, level + 1});
                    visited.insert(mutual->userId);
                }
            }

        }
    }
   
}

// ==========================================
// FILE I/O 
// ==========================================

void loadData() {
    cout << "Loading data from CSV files..." << endl;
    ifstream userFile("users.csv");
    string line;
    if (userFile.is_open()) {
        getline(userFile, line); 
        while (getline(userFile, line)) {
            vector<string> row = split(line);
            if (row.size() < 5) continue;
            User* newUser = new User(stoi(row[0]), row[1], stoi(row[2]), stoi(row[3]), stoi(row[4]));
            allUsers.push_back(newUser);
            userMap.put(row[1], newUser); 
        }
        userFile.close();
    }
    ifstream relFile("relations.csv");
    if (relFile.is_open()) {
        getline(relFile, line); 
        while (getline(relFile, line)) {
            vector<string> row = split(line);
            if (row.size() < 2) continue;
            int u1 = stoi(row[0]); int u2 = stoi(row[1]);
            if (u1 <= allUsers.size() && u2 <= allUsers.size()) {
                allUsers[u1-1]->addFriend(allUsers[u2-1]);
                allUsers[u2-1]->addFriend(allUsers[u1-1]);
            }
        }
        relFile.close();
    }
    ifstream postFile("posts.csv");
    if (postFile.is_open()) {
        getline(postFile, line); 
        while (getline(postFile, line)) {
            vector<string> row = split(line);
            if (row.size() < 5) continue;
            int pid = stoi(row[0]); int uid = stoi(row[1]);
            if (uid <= allUsers.size()) {
                allUsers[uid-1]->addPost(pid, row[2], stoi(row[3]), stol(row[4])); 
                if (pid >= GLOBAL_POST_ID_COUNTER) GLOBAL_POST_ID_COUNTER = pid + 1;
            }
        }
        postFile.close();
    }
}

void saveData() {
    // SAFETY CHECK: If no users exist, do not overwrite files!
    if (allUsers.empty()) {
        cout << "[SAFETY] No data in memory. Skipping save to prevent file wipe." << endl;
        return;
    }

    cout << "Saving data..." << endl;
    ofstream userFile("users.csv");
    userFile << "user_id,username,tech_score,art_score,sport_score\n";
    for (User* u : allUsers) {
        userFile << u->userId << "," << u->username << "," << u->techScore << "," << u->artScore << "," << u->sportScore << "\n";
    }
    userFile.close();

    ofstream relFile("relations.csv");
    relFile << "user_id_1,user_id_2\n";
    for (User* u : allUsers) {
        for (User* f : u->friends) {
            if (u->userId < f->userId) relFile << u->userId << "," << f->userId << "\n";
        }
    }
    relFile.close();

    ofstream postFile("posts.csv");
    postFile << "post_id,user_id,content,likes,timestamp\n";
    for (User* u : allUsers) {
        vector<Post*> temp;
        Post* curr = u->timeline.head;
        while (curr) { temp.push_back(curr); curr = curr->next; }
        for (int i = temp.size() - 1; i >= 0; i--) {
            Post* p = temp[i];
            string safeContent = p->content;
            if (safeContent.find(',') != string::npos) {
                safeContent = "\"" + safeContent + "\"";
            } 
            postFile << p->postId << "," << p->userId << "," << safeContent << "," << p->likes << "," << p->timestamp << "\n";
        }
    }
    postFile.close();
    cout << "Done." << endl;
}

// ==========================================
// MAIN MENU (UI)
// ==========================================

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void showUserDashboard(User* currentUser) {
    int choice = 0;
    while (choice != 7) {
        cout << "\n--- Welcome, @" << currentUser->username << " ---" << endl;
        cout << "1. View My Post (Lab 1)" << endl;
        cout << "2. Create New Post (Lab 1)" << endl;
        cout << "3. Add Friend (Lab 2)" << endl;
        cout << "4. Algorithmic Feed (Lab 3)" << endl;
        cout << "5. View Friends Sorted (Lab 4)" << endl;
        cout << "6. Get Friend Recommendations (Lab 5)" << endl;
        cout << "7. Logout" << endl;
        cout << "Select >> ";
        cin >> choice;

        if (choice == 1) {
            cout << "\n[MY POST]" << endl;
            currentUser->timeline.printTimeline();
        }
        else if (choice == 2) {
            cout << "\nEnter post content: ";
            cin.ignore(); 
            string content;
            getline(cin, content);
            createNewPost(currentUser, content);
        }
        else if (choice == 3) {
            string friendName;
            cout << "Enter username to add: "; cin >> friendName;
            User* target = userMap.get(friendName);
            if(target && target != currentUser) addFriendship(currentUser, target);
            else cout << "Invalid user (or Hash Map not implemented)." << endl;
        }
        else if (choice == 4) {
            cout << "\n[ALGORITHMIC FEED]" << endl;
            FeedHeap feed;
            vector<User*> friends = currentUser->getFriendsList();
            for(User* f : friends) {
                Post* p = f->timeline.head;
                int limit = 0;
                while(p != nullptr && limit < 5) {
                    feed.push(p);
                    p = p->next;
                    limit++;
                }
            }
            int count = 0;
            while(!feed.isEmpty() && count < 10) {
                Post* top = feed.popMax();
                if(top)
                    cout << "  > [ID: " << top->postId << "] [Score: " << (int)top->getScore() << "] @" 
                         << allUsers[top->userId - 1]->username << ": " << top->content 
                         << " (" << top->likes << " likes)" << endl;
                count++;
            }
            if(count == 0) cout << "  No posts found." << endl;
            else {
                cout << "\nDo you want to like a post? (y/n): ";
                char resp; cin >> resp;
                if (resp == 'y' || resp == 'Y') {
                    int pid; cout << "Enter Post ID: "; cin >> pid;
                    Post* p = findPostById(pid);
                    if (p) { p->likes++; cout << "Liked!" << endl; }
                }
            }
        }
        else if (choice == 5) {
            cout << "\n[MY FRIENDS]" << endl;
            currentUser->friendTree.printFriends();
        }
        else if (choice == 6) {
             recommendFriends(currentUser);
        }
        else if (choice == 7) {
            cout << "Logging out..." << endl;
        }
    }
}

void showMainMenu() {
    int choice = 0;
    while (choice != 3) {
        cout << "\n=== CONNECT-U ===" << endl;
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "3. Exit & Save" << endl;
        cout << "Select >> ";
        cin >> choice;

        if (choice == 1) {
            string username;
            cout << "Username: "; cin >> username;
            User* user = userMap.get(username);
            if (user) showUserDashboard(user);
            else cout << "User not found." << endl;
        } 
        else if (choice == 2) {
            string username;
            int t, a, s;
            cout << "Username: "; cin >> username;
            cout << "Tech/Art/Sport (1-10): "; cin >> t >> a >> s;
            registerNewUser(username, t, a, s);
        }
        else if (choice == 3) {
            // SAFETY: Commented out to prevent data loss on initial run.
            // Students must uncomment this ONLY when Lab 1 is complete.
            saveData(); 
            cout << "Goodbye! " << endl;
        }
    }
}

int main() {
    loadData(); 
    clearScreen();
    showMainMenu();
    return 0;
}