// SocketCommunicator.cpp
// Andrew Ribeiro 
// March 4, 2011

#include "Stdafx.h"

namespace AndrewRibeiro
{

	SocketCommunicator::SocketCommunicator( SOCKET sock )
	{
		comStream = sock;
	}

	bool SocketCommunicator::write( string message )
	{
		// # Send message size. 
		char sizeOfIncMessage[ MAX_DIGIT_MESSAGE_SIZE ] = { '\0' }; 
		char ack[3];
		int nBytesSent, nBytesRecvd;

		
		// # Converting message length to a c string. 

		// Compute how many digits are in the message length. 
		int digitsInMessageLength = ( int )floor( log10( (double)message.length() ) ) + 1;
	

		// Buffer for the messageLength + end string character. 
		char * messageLength = new char[digitsInMessageLength + 1];

		// Convert message length to a c string. 
		itoa(message.length() , messageLength, 10 );

		

		// Add end string character.
		messageLength[ digitsInMessageLength ] = '\0';

		// # Send the message length 
		nBytesSent = send ( comStream, messageLength , MAX_DIGIT_MESSAGE_SIZE, 0);

		
		delete [] messageLength;

		if ((nBytesSent == 0) || (nBytesSent == SOCKET_ERROR))
		{
			// No message write.
			return false;
		}

		// #Confirm Message Received.
		nBytesRecvd = recv (comStream, ack, 3 , 0);

		if ((nBytesRecvd == 0) || (nBytesRecvd == SOCKET_ERROR))
		{
			// No message read. 
			return false;
		}
		else
		{
			// # Send the message. 

			nBytesSent = send ( comStream, message.c_str() , message.length(), 0);

			if ((nBytesSent  == 0) || (nBytesSent  == SOCKET_ERROR))
			{
				// No message write. 
				return false;
			}

			return true;
		}
		
		
	}

	string SocketCommunicator::read()
	{
		// Get message size. 
		char sizeOfIncMessage[ MAX_DIGIT_MESSAGE_SIZE ] = { '\0' }; 
		char ack[3];
		int nBytesRecvd, nBytesSent;

	

		nBytesRecvd = recv (comStream, sizeOfIncMessage, MAX_DIGIT_MESSAGE_SIZE, 0);


		if ((nBytesRecvd == 0) || (nBytesRecvd == SOCKET_ERROR))
		{
			// No message read. 
			return NO_MESSAGE_RECEIVED;
		}

		// Send message received acknowlege.
		nBytesSent = send ( comStream, ack , sizeof(ack), 0);

		//delete [] messageLength;
 
			
		if ((nBytesSent == 0) || (nBytesSent == SOCKET_ERROR))
		{
			// Did not send the ack message.
			return "";
		}

		// Read message. 
		int iSizeOfIncMessage = atoi( sizeOfIncMessage );
		char * incMessage = new char[ iSizeOfIncMessage + 1 ]; 

		nBytesRecvd = recv (comStream, incMessage, iSizeOfIncMessage, 0);

		incMessage [ iSizeOfIncMessage ] = '\0';


		if ((nBytesRecvd == 0) || (nBytesRecvd == SOCKET_ERROR))
		{
			// No message read. 
			return NO_MESSAGE_RECEIVED;
		}

		string outStr;

		for( int i = 0; i < iSizeOfIncMessage; i++)
		{
			outStr.push_back( incMessage[i] );
		}

		delete [] incMessage; 

		return outStr;

	}
}