# rshell
CS100 Assignment 2

#Instructions
1) git clone https://github.com/LoneLogical/rshell.git
2) cd rshell
3) git checkout hw2
4) make
5) bin/rshell

#Intro
To write a basic command shell, my partner and I intend to use the composite pattern we learned in lecture to simplify the organizational structure of our code. The entire command line argument simplifies down to commands, command parameters, and connectors. We decided to make an Abstract Base class with no data field but with a virtual function called execute ( ) which returns a Boolean value. We would create a Command class and a Connector class that both inherit from Base. The Command Class would be the leaves of the design pattern, and the Connector Class would be the composite, which holds Base pointers. Each connector, would have its own subclass of the Connector class, and we would include a special Command subclass specifically for the Exit command. All other bin commands would fall into the general Command class. The connector class pointers would create a tree hierarchy much like a lop-sided Binary search tree. The execute function when called on the root node should percolate down the structure until it reaches a LHS pointer pointing to a command object. At which point, the command object’s execute would be performed and return a truth value. This process would then percolate back up the tree.

#Bugs
1) Good news first, single commands work extremely well. With that being said...this thing sucks.

#Authors
Destin Raymundo
Andrew Whitaker
