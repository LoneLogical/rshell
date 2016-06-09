# rshell
CS100 Assignment 4

#Instructions
1) git clone https://github.com/LoneLogical/rshell.git

2) cd rshell

3) git checkout hw4

4) make 

5) bin/rshell 

#Intro
To write a basic command shell, my partner and I used the composite pattern we learned in lecture to simplify the organizational structure of our code. The entire command line argument simplifies down to commands, command parameters, and connectors. We decided to make an Abstract Base class with no data field but with a virtual function called execute() which returns a Boolean value. We would create a Command class and a Connector class that both inherit from Base. The Command Class objects are the leaves of the design pattern, and the Connector Class objects are the composite nodes, which hold Base pointers. Each connector, has its own subclass of the Connector class, and we included a special Command subclass specifically for the Exit command. All other bin commands would fall into the general Command class. The connector class pointers would create a tree hierarchy much like a lop-sided Binary search tree. The execute function when called on the root node should percolate down the structure until it reaches a LHS pointer pointing to a command object. At which point, the command object’s execute would be performed and return a truth value. This process would then percolate back up the tree in a linear fashion but also in comliance with the logical structure of the connectors.

#HW4 Updates
1) Can now use features like input redirection <, output redirection >> >, and piping|. Input redirection allows a command to take input from a file. Output redirection allows the output of a command, normally intended for standard output, to be redirected to a file instead. Piping allows the standard output of one command to be fed into the input of another commannd.

2) The Input Redirect, Output Redirect, Output Append, and Pipe classes have been built. These classes inherit from our Connector class. These classes allow the user to input the connectors, (<, >, >>, |) into the command line.

#Bugs

#Authors
Destin Raymundo: [https://github.com/draym001]

Andrew Whitaker: [https://github.com/LoneLogical]
