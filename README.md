# rshell
CS100 Assignment 2

#Instructions
1) git clone https://github.com/LoneLogical/rshell.git

2) cd rshell

3) git checkout hw2

4) make 

5) bin/rshell 

#Intro
To write a basic command shell, my partner and I used the composite pattern we learned in lecture to simplify the organizational structure of our code. The entire command line argument simplifies down to commands, command parameters, and connectors. We decided to make an Abstract Base class with no data field but with a virtual function called execute() which returns a Boolean value. We would create a Command class and a Connector class that both inherit from Base. The Command Class objects are the leaves of the design pattern, and the Connector Class objects are the composite nodes, which hold Base pointers. Each connector, has its own subclass of the Connector class, and we included a special Command subclass specifically for the Exit command. All other bin commands would fall into the general Command class. The connector class pointers would create a tree hierarchy much like a lop-sided Binary search tree. The execute function when called on the root node should percolate down the structure until it reaches a LHS pointer pointing to a command object. At which point, the command object’s execute would be performed and return a truth value. This process would then percolate back up the tree in a linear fashion but also in comliance with the logical structure of the connectors.

#Bugs
1) There are some definite memory issues that need reworking. Possible memory leaks occur when dynamically creating new Command and COnnector objects for building the structure. Each time the user enters a command, we're not totally certain that the memory devoted to the previous command line is given back to the heap properly.

2) The parsing part of the program does not restrict users from entering connectors without commands. For example, "echo hello; " would throw an error. Likewise, if commands are commented out, a similar error arises, such as in "echo hello && # ls -a". This is basically the same case because we've treated the text after '#' to be whitespace.



#Authors
Destin Raymundo: [https://github.com/draym001]

Andrew Whitaker: [https://github.com/LoneLogical]
