#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <functional>

using namespace std;

struct User {
    string name;
    string email;
    string password;
    char gender;
    string dateOfBirth;
    vector<string> friends;
    vector<string> posts;
    vector<pair<string, string>> privateMessages;
    unordered_set<string> friendSet;
    unordered_map<string, User*> friendsGraph;
};

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void displayMainMenu() {
    cout << "Welcome to Facebook Lite!" << endl;
    cout << "1. Login" << endl;
    cout << "2. Register" << endl;
    cout << "3. Exit" << endl;
    cout << "Enter your choice: ";
}

User registerUser() {
    User newUser;

    cout << "Enter your name: ";
    cin >> newUser.name;

    cout << "Enter your email: ";
    cin >> newUser.email;

    cout << "Enter your password: ";
    cin >> newUser.password;

    cout << "Enter your gender (M/F): ";
    cin >> newUser.gender;

    cout << "Enter your date of birth (YYYY-MM-DD): ";
    clearInputBuffer();
    getline(cin, newUser.dateOfBirth);

    cout << "Registration successful! Welcome, " << newUser.name << "!" << endl;

    return newUser;
}

User loginUser(const vector<User>& users) {
    string enteredEmail, enteredPassword;

    cout << "Enter your email: ";
    clearInputBuffer();
    getline(cin, enteredEmail);

    cout << "Enter your password: ";
    getline(cin, enteredPassword);

    for (const User& user : users) {
        if (user.email == enteredEmail && user.password == enteredPassword) {
            cout << "Login successful! Welcome back, " << user.name << "!" << endl;
            return user;
        }
    }

    cout << "Invalid email or password. Please enter correct details." << endl;
    return User();
}

void addFriendAndUpdatePosts(User& loggedInUser, User& newFriend) {
    loggedInUser.friends.push_back(newFriend.name);
    newFriend.friends.push_back(loggedInUser.name);

    loggedInUser.posts.insert(loggedInUser.posts.end(), newFriend.posts.begin(), newFriend.posts.end());
    newFriend.posts.insert(newFriend.posts.end(), loggedInUser.posts.begin(), loggedInUser.posts.end());

    loggedInUser.friendSet.insert(newFriend.name);
    newFriend.friendSet.insert(loggedInUser.name);

    loggedInUser.friendsGraph[newFriend.name] = &newFriend;
    newFriend.friendsGraph[loggedInUser.name] = &loggedInUser;
}

unordered_set<string> findCommonFriendsDFS(const User& user1, const User& user2) {
    unordered_set<string> commonFriends;

    function<void(const User&, unordered_set<string>&)> dfs = [&](const User& current, unordered_set<string>& visited) {
        visited.insert(current.name);

        for (const auto& pair : current.friendsGraph) {
            const User& friendUser = *pair.second;
            if (visited.find(friendUser.name) == visited.end()) {
                commonFriends.insert(friendUser.name);
                dfs(friendUser, visited);
            }
        }
    };

    unordered_set<string> visited;
    dfs(user1, visited);

    return commonFriends;
}

vector<string> recommendFriends(const User& loggedInUser) {
    unordered_set<string> loggedInUserFriends(loggedInUser.friends.begin(), loggedInUser.friends.end());
    unordered_map<string, int> mutualFriendCounts;

    for (const string& friendName : loggedInUser.friends) {
        const User& friendUser = *loggedInUser.friendsGraph.at(friendName);

        for (const string& mutualFriend : friendUser.friends) {
            if (loggedInUserFriends.find(mutualFriend) == loggedInUserFriends.end()) {
                mutualFriendCounts[mutualFriend]++;
            }
        }
    }

    priority_queue<pair<int, string>> pq;
    for (const auto& pair : mutualFriendCounts) {
        pq.push({pair.second, pair.first});
    }

    vector<string> recommendations;
    while (!pq.empty()) {
        recommendations.push_back(pq.top().second);
        pq.pop();
    }

    return recommendations;
}

void sendPrivateMessage(User& sender, User& recipient, const string& message) {
    sender.privateMessages.push_back({recipient.name, message});
    recipient.privateMessages.push_back({sender.name, message});
}

void loggedInUserOptions(User& loggedInUser, vector<User>& users) {
    int choice;

    do {
        cout << "Logged in as: " << loggedInUser.name << endl;
        cout << "1. View Profile" << endl;
        cout << "2. Edit Profile" << endl;
        cout << "3. Post a Message/Status" << endl;
        cout << "4. View Friends/Connections" << endl;
        cout << "5. Add Friends" << endl;
        cout << "6. Private Messaging" << endl;
        cout << "7. View Private Messages" << endl;
        cout << "8. View Friends' Posts" << endl;
        cout << "9. Logout" << endl;
        cout << "Enter your choice: ";

        cin >> choice;
        if (cin.fail()) {
            cout << "Invalid input. Please enter a valid option." << endl;
            clearInputBuffer();
            continue;
        }

        switch (choice) {
            case 1:
                // View Profile
                cout << "Name: " << loggedInUser.name << endl;
                cout << "Email: " << loggedInUser.email << endl;
                cout << "Gender: " << loggedInUser.gender << endl;
                cout << "Date of Birth: " << loggedInUser.dateOfBirth << endl;
                break;

            case 2:
                // Edit Profile
                cout << "Enter your new name: ";
                cin >> loggedInUser.name;

                cout << "Enter your new password: ";
                cin >> loggedInUser.password;

                cout << "Profile updated successfully!" << endl;
                break;

            case 3:
                // Post a Message/Status
                {
                    string status;
                    cout << "Enter your status: ";
                    cin.ignore();
                    getline(cin, status);
                    loggedInUser.posts.push_back(loggedInUser.name + ": " + status);
                    cout << "Status posted: " << status << endl;
                }
                break;

            case 4:
                // View Friends/Connections
                cout << "Your Friends/Connections:" << endl;
                for (const string& buddy : loggedInUser.friends) {
                    cout << buddy << endl;
                }
                break;

            case 5:
                // Add Friends
                {
                    string friendName;
                    cout << "Enter the name of the friend you want to add: ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    getline(cin, friendName);

                    auto friendIter = find_if(users.begin(), users.end(), [friendName](const User& u) {
                        return u.name == friendName;
                    });

                    if (friendIter != users.end()) {
                        addFriendAndUpdatePosts(loggedInUser, *friendIter);
                        cout << friendName << " added to your friends list!" << endl;
                    } else {
                        cout << friendName << " is not registered on the network. You can only add registered users." << endl;
                    }
                }
                break;

            case 6:
                // Private Messaging
                {
                    string recipientName;
                    cout << "Enter the name of the friend you want to message: ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    getline(cin, recipientName);

                    auto friendIter = find(loggedInUser.friends.begin(), loggedInUser.friends.end(), recipientName);

                    if (friendIter != loggedInUser.friends.end()) {
                        string message;
                        cout << "Enter your message to " << recipientName << ": ";
                        getline(cin, message);

                        auto recipientIter = find_if(users.begin(), users.end(), [recipientName](const User& u) {
                            return u.name == recipientName;
                        });

                        if (recipientIter != users.end()) {
                            sendPrivateMessage(loggedInUser, *recipientIter, message);
                            cout << "Message sent to " << recipientName << ": " << message << endl;
                        } else {
                            cout << "Error: Recipient not found." << endl;
                        }
                    } else {
                        cout << "You can only message friends. Please add " << recipientName << " to your friends list." << endl;
                    }
                }
                break;

            case 7:
                // View Private Messages
                cout << "Your Private Messages:" << endl;
                for (const auto& message : loggedInUser.privateMessages) {
                    cout << "From: " << message.first << ", Message: " << message.second << endl;
                }
                break;

            case 8:
                // View Friends' Posts
                cout << "Posts from your friends:" << endl;
                for (const string& friendName : loggedInUser.friends) {
                    auto friendIter = find_if(users.begin(), users.end(), [friendName](const User& u) {
                        return u.name == friendName;
                    });

                    if (friendIter != users.end()) {
                        cout << "Posts from " << friendName << ":" << endl;
                        for (const string& post : friendIter->posts) {
                            cout << post << endl;
                        }
                        cout << "-----------------------" << endl;
                    }
                }
                // Include the user's own posts
                cout << "Your Posts:" << endl;
                for (const string& post : loggedInUser.posts) {
                    cout << post << endl;
                }
                break;

            case 9:
                // Logout
                cout << "Logging out. Goodbye, " << loggedInUser.name << "!" << endl;
                break;

            default:
                cout << "Invalid choice. Please enter a valid option." << endl;
        }
    } while (choice != 9);
}

int main() {
    int choice;
    vector<User> users;
    User loggedInUser;

    do {
        displayMainMenu();

        cin >> choice;
        if (cin.fail()) {
            cout << "Invalid input. Please enter a valid option." << endl;
            clearInputBuffer();
            continue;
        }

        switch (choice) {
            case 1:
                // Login
                loggedInUser = loginUser(users);
                if (!loggedInUser.name.empty()) {
                    loggedInUserOptions(loggedInUser, users);
                }
                break;

            case 2:
                // Register a new user
                users.push_back(registerUser());
                break;

            case 3:
                cout << "Exiting the program. Goodbye!" << endl;
                break;

            default:
                cout << "Invalid choice. Please enter a valid option." << endl;
        }

    } while (choice != 3);

    return 0;
}
