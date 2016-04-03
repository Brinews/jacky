1. ChatServer

$./chatserver port

where port is the udp server binding port.

In the server, we keep a list of clients. When a user client login, we add it
to the list; when a user client logout, we remove it. Also, when user query
for the online users, we just traverse the list and return them. When a user
want to talk to some other user client, we just return the target client's
listen hostname and port.

2. ChatClient

$./chatclient chatserver-hostname chatserver-port self-listening-port

In the client, we can interact with the chatserver and also interact with the
other chatclient.

Interact with chatserver:
1. login username self-listening-port
When login success, the chatserver will store the chatclient's information
including hostname, listening port, username.

One user cannot login when he has already logged-in.

2. who username

Here the username the login-user-name, used to valid the query user.

3. talk username to-talk-username

Here the username is the login-user-name, and the to-talk-username is the
other user logged-in.

4. quit username

Here the username is the login-user-name.

Interact with chatclient:
1. Input message from console
2. Send message to remote chatclient;
3. Get reply from remote chatclient;
4. If input 'exit', then exit the chat process;

