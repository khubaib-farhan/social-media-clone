# Facebook Lite

Facebook Lite is a simple, text-based social network application implemented in C++. It provides basic functionality similar to popular social networking platforms, allowing users to create profiles, connect with friends, post messages, and communicate privately.

## Features

- User Registration and Login
- Profile Management
- Status Updates and Posts
- Friend Connections
- Private Messaging
- View Friends' Posts

## Getting Started

### Prerequisites

- C++ compiler (supporting C++11 or later)
- Standard C++ libraries

### Compilation

To compile the program, use the following command:

```
g++ -std=c++11 main.cpp -o fb_lite_clone
```

### Running the Program

After compilation, run the executable:

```
./fb_lite_clone
```

## Usage

1. **Main Menu**:
   - Login
   - Register
   - Exit

2. **User Options** (after login):
   - View Profile
   - Edit Profile
   - Post a Message/Status
   - View Friends/Connections
   - Add Friends
   - Private Messaging
   - View Private Messages
   - View Friends' Posts
   - Logout

## Implementation Details

- The program uses a `vector` of `User` structs to store user information.
- Friend connections are managed using an `unordered_set` and `unordered_map` for efficient lookups.
- The application implements a simple depth-first search (DFS) algorithm to find common friends.
- Friend recommendations are generated based on mutual connections.

## Limitations

- Data is not persistent and is lost when the program terminates.
- There's no encryption for sensitive information like passwords.
- The application runs in a single session and doesn't support multiple concurrent users.

## Future Improvements

- Implement data persistence (e.g., file I/O or database integration)
- Add password hashing and security features
- Develop a graphical user interface
- Implement more advanced social networking algorithms and features

## Contributing

Feel free to fork this project and submit pull requests with any improvements or additional features.

## License

This project is open source and available under the [MIT License](LICENSE).
