readme.txt
Andrew Ribeiro
Andrew@andrewribeiro.com
March 5, 2011
Version 1.2


_______ _________ _______  _______  _        _______  _______  _______  _______  _        _______ _________
(  ____ \\__   __/(       )(  ____ )( \      (  ____ \(  ____ \(  ___  )(  ____ \| \    /\(  ____ \\__   __/
| (    \/   ) (   | () () || (    )|| (      | (    \/| (    \/| (   ) || (    \/|  \  / /| (    \/   ) (   
| (_____    | |   | || || || (____)|| |      | (__    | (_____ | |   | || |      |  (_/ / | (__       | |   
(_____  )   | |   | |(_)| ||  _____)| |      |  __)   (_____  )| |   | || |      |   _ (  |  __)      | |   
      ) |   | |   | |   | || (      | |      | (            ) || |   | || |      |  ( \ \ | (         | |   
/\____) |___) (___| )   ( || )      | (____/\| (____/\/\____) || (___) || (____/\|  /  \ \| (____/\   | |   
\_______)\_______/|/     \||/       (_______/(_______/\_______)(_______)(_______/|_/    \/(_______/   )_(   


Dependancies
+ wsock32.lib
	- Windows Sockets Library

Library Contents 
+ SimpleSocketsLibrary.lib
+ SocketCommunicator.h
+ SimpleSocket.h
+ SimpleServerSocket.h
+ SimpleServerSocket.h
+ SocketCommunicator.h


Content Descriptions 

+ SimpleSocketsLibrary.lib

	- Static library, contains the implementation of the interfaces ( header files ). 


+ Interfaces 

	- SocketCommunicator.h 
		* This class is used to read/write on windows sockets. 

	- SimpleSocket.
		* This is a base class that is inherited by the two types of sockets in this library.
		* Contains general socket inililization procedures and data members. 

	- SimpleServerSocket.h
		* Abstracts the creation of a server socket. Contains one method, listenForConnection. 

	- SimpleClientSocket.h
		* Abstracts the creation of a client socket. Contains one method, connectToServer. 


Build Instructions ( Visual Studio 2010 )

+ In order to use this library you must folow the Instructions bellow.

	1.) Add the headers to your project. 

	2.) Add the SimpleSocketsLibrary.lib to your project. 

	3.) Add the SimpleSocketsLibrary.lib to the aditional dependencies of the linker. 
		* Project Properties -> Configuration Properties -> Linker -> Input
		    -> type SimpleSocketsLibrary.lib in the additional dependancies text box. 

	4.) Add the wsock32.lib to the aditional dependencies of the linker. 
		* Project Properties -> Configuration Properties -> Linker -> Input
		    -> type wsock32.lib in the additional dependancies text box. 

+ For more information on how to use Static Libraries in your C++ projects, please see the url bellow 
	- http://msdn.microsoft.com/en-us/library/ms235627.aspx