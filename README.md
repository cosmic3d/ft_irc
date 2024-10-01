<img src="https://github.com/user-attachments/assets/aefd0832-b75d-4325-b2b0-449c5d3cd19e" alt="drawing" width="600"/>

# FT_IRC

## Introduction


FT_IRC is a simple IRC (Internet Relay Chat) server written in C++. It's a project developed by students of the 42 Barcelona school as part of their curriculum. The goal of this project is to create a functional IRC server that can handle multiple clients, channels, and messages.

## How to Use

To use the FT_IRC server, follow these steps:

### 1.  Compile the project
Compile the project using the provided Makefile by running the following command:
```
make
```

### 2. Run the server
Run the server by executing the compiled binary with the following command, replacing `<port>` and `<password>` with your desired values:
```
./ircserv <port> <password>
```

### 3. Use an IRC client
Use an IRC client (such as HexChat or mIRC) to connect to the server.

### Important Note:

- The server does **not** support IRCv3 capabilities.
- The correct way to register after connecting to the server is by using the following sequence of commands:
```
PASS <password>
NICK <nickname>
USER <username> * 0 :<realname>
```
**Make sure to replace `<password>` with the server password, `<nickname>` with your unique nickname, `<username>` with your unique username and `<realname>` with your real name**
- If not using an IRC client, make sure to add the [Carriage Return - Line Feed](https://datatracker.ietf.org/doc/html/rfc2812#section-2.3) character pair after every command

### 4. YOU CAN PLAY NOW
Once registered, you can join channels, send messages, and interact with other users

## Features

- Handles multiple clients and channels
- Supports basic IRC commands (JOIN, PART, PRIVMSG, NOTICE, etc.)
- Allows users to create and manage channels
- Implements user authentication and authorization
- Supports channel modes (e.g., +i, +t, +l)
- Handles user kicks and bans
- Provides a basic user interface for managing channels and users

## Resources

- IRC Protocol Documentation
- IRC Client Documentation
- C++ Documentation

## Contributions

Contributions are welcome! If you'd like to contribute to the FT_IRC project, please follow these steps:

1. Fork the repository on GitHub.
2. Create a new branch for your feature or bug fix.
3. Make your changes and commit them.
4. Open a pull request to merge your changes into the main branch.

## Known Issues

- The server does not handle errors properly in some cases.
- The user interface is basic and could be improved.
- Some IRC commands are not implemented.

## Future Development

- Implement more IRC commands and features.
- Improve the user interface and user experience.
- Add support for SSL/TLS encryption.
- Optimize the server for better performance.

## License

The FT_IRC project is licensed under the MIT License.

## Authors

- Jenavarr
- Damendez

## Acknowledgments

We would like to thank the 42 Barcelona school for providing us with the opportunity to work on this project. We would also like to thank the IRC community for their documentation and resources.
