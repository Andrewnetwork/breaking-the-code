// CrackProtocolConstants.h
// Andrew Ribeiro 
// http://www.AndrewRibeiro.com
// April 10, 2011
/*******************************************************
** This code was created by Andrew Ribeiro whom        *
** retains the rights to all the material presented in *
** this document, unless stated otherwise. The usage   *
** of this code is granded with no liability to the    *
** creator of this material.                           *
********************************************************/

/*******************DESCRIPTION*************************
 * This file contains the CRACK protocol verbs         *
 * used for communication between the client and       *
 * server. These constants are used in the protocol    *
 * classes: CrackProtocolClient & CrackProtocolServer. *
 *******************************************************/

#pragma once

#include <string>
using std::string;

const string KEY_START_PROCESS = "SP";
const string KEY_GET_PROCESS_STATUS = "PS";
const string KEY_RANGE_COMPLETE = "RC";
const string KEY_REQUEST_INFO = "RI";
const string KEY_GET_RANGE = "GR";
const string KEY_GET_VERIFICATION_ARCHIVE = "GA";
const char   BLANK_CHAR = 125;

// Server -> Client Keywords
const string RANGE_YEILDED_NO_RESULTS = "";

// Server <-> Client
const string KEY_UNSUPPORTED_OP = "UO";