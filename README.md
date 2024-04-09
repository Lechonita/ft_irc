## [IRC](https://datatracker.ietf.org/doc/html/rfc1459) - Internet Relay Chat



### 🗺️ Project Description

This project consists in creating an IRC Server to which multiple clients can connect to and converse, privately or in channels.
A client's actions should not block the server. All clients can perform actions simutaneously on the server.




### 🎮 Key Commands



### Connexion commands

| Command | Expected arguments | Outcome |
| :-------------: | ------------- | ----------- |
| PASS | `<password>`  | Password is necessary to properly connect to server |
| NICK  | `<nickname>` | Setup your nickname |
| USER  | `<username>` `<mode>` `<server>` :`<real first name>` `<real last name>` | Setup username and real name (mode and server are not taken into account) |



## Basic commands

| Command | Expected arguments | Outcome |
| :-------------: | ------------- | ----------- |
| JOIN | `<channelname>` | Join a channel. If the channel does not exist yet, it creates the server. Channel creator automatically becomes channel operator |
| PART  | none | Quits the channel |
| PRIVMSG | `<channelname or nickname of receiver>` `<message>` | Send a message to a channel or to another client |
| QUIT | none | Quits the server |
| WHOIS | `<nickname>` | Gives information about the requested client |



## Operator Commands (can only be performed by channel operators)

| Command | Expected arguments | Outcome |
| :-------------: | ------------- | ----------- |
| KICK | ``<nickname>`` | Kicks a client out of the channel |
| TOPIC  | `<channelname>` / can also add `<topic>` | See the channel's topic. Adding a `<topic>` will change the channel's topic |
| INVITE | `<nickname>` `<channelname>` | Invite a client to the channel |
| MODE | `<channelname>` `<mode>` | Changes the mode of the channel |



## Channel modes

| Mode | Expected arguments | Outcome |
| :-------------: | ------------- | ----------- |
| +i or -i | none |  Set/remove Invite-only channel |
| +t or -t | none |  Set/remove the restrictions of the TOPIC command to channel operators |
| +k or -k | `<password>` | Set/remove the channel key (password) |
| +o or -o | `<nickname>` | Give/take channel operator privilege |
| +l or -l | `<number>` | Set/remove the user limit to channel |






### 🚀 Usage



#### 1. Clone the repository

```shell
git clone https://github.com/Lechonita/ft_irc.git
```



#### 2. Compile the ```ircserv``` program

```shell
make
```



#### 3. Execute with a port number and a password as parameters

```shell
./ircserv <port number> <password>

# Example
./ircserv 1025 password123
```



#### 4. Connect a client to the server

```shell
/!\ In a new terminal

# with netcat
nc localhost <port number> <password>

# with irssi
irssi
/connect localhost <port number> <password>
```
