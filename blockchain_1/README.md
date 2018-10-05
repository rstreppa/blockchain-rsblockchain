# Roberto's BlockChain, version 0.1

*This program is a simple but (hopefully) realistic enough implementation of a BlockChain, loosely based on the Bitcoin BlockChain*

Compile and build on Windows 10 using the msvc compiler (Visual Studio Express 2015), then launch:
**C:\executable_location\blockchain_1**

## Main Features
-  Written mostly in C++11, some parts in C (implementation of SHA512)   
-  Compiled with the msvc compiler (Visual Studio Express 2015) on Windows 10
-  Employs a very simple Peer2Peer logic where each Node:
  -  runs a thread for the server (listening to incoming Messages from the Network, decoding them and invoking the controlling Node member function) 
  -  runs a thread to create transactions randomly, package them in a Message and send over the Network
  -  runs a thread to mine Blocks, i.e. solve the computational puzzle for the nonce, collect transactions into the new Block, generate CoinBase transaction, 
  package a Message and send over the Network 
- implement and employ SHA512 hashing function instead of the more common SHA256

## Dependencies (sourced libraries)
-  boost libraries 1.61.0   https://www.boost.org/
-  json_spirit_v4.08 Json Spirit library to convert Messages to Json, the only library I found to work well on Windows 10 
https://www.codeproject.com/Articles/20027/JSON-Spirit-A-C-JSON-Parser-Generator-Implemented
-  Aaron Gifford's C Implementations of SHA-512 http://www.aarongifford.com/computers/sha.html
-  @halx99 wonderful mini-asio for peer-to-peer networking, extremely easy to use https://github.com/halx99/mini-asio
    
## Acknowledgments
From Timothy Ko @tko22 from whom I borrowed the logic of expressing the BlockChain as a vector of unique_ptr 
plus a general understanding on how to organize the code https://github.com/tko22/simple-blockchain

I analyzed in-depth Jack Schultz's @jackschultz Python BlockChain again to understand the structure of a BlockChain  https://github.com/jackschultz/jbc

I essentially took @gubatron Merkle Tree implementation, which is cristalline clear and simply made it a template https://gist.github.com/gubatron/00cb7358720c97d69b3d 

Last but not definitely not least, I am indebted to @nemopublius because his wonderful series of posts allowed me to understand Bitcoin, Blockchain and many other things
https://self-evident.org/

## TODO

Implement a ZKP (Zero Knowledge Proof) protocol. There is little around in the literature both theoretically and in terms of available code. Wikipedia is a good place to start and the math involved appears not too difficult to implement.   
