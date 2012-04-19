/*
 * CommProtocol.h
 *
 *  Created on: Mar 18, 2012
 *      Author: db538
 */

#ifndef COMMPROTOCOL_H_
#define COMMPROTOCOL_H_

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/thread/thread.hpp>

using boost::asio::ip::tcp;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

namespace CommProtocol {

/*
 * Communication:
 *  - slave sends HANDSHAKE with the last byte set to his version
 *  - master sends back MSG_ACK if versions match, MSG_REJECT with his version otherwise
 */

const char HANDSHAKE[] = { 'R', 'A', 'Y', 'T', 'R', 'A', 'C', 'E', 'v' };

const char MSG_ACK = 1;
const char MSG_ERR = 2;
const char MSG_TASK = 3;
const char MSG_WAIT = 4;
const char MSG_OK = 5;
const char MSG_FINISHED = 6;

}

#endif /* COMMPROTOCOL_H_ */
