/*
    Democrit - atomic trades for XAYA games
    Copyright (C) 2020  Autonomous Worlds Ltd

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef DEMOCRIT_TESTUTILS_HPP
#define DEMOCRIT_TESTUTILS_HPP

#include "proto/orders.pb.h"

#include <gloox/jid.h>

#include <glog/logging.h>
#include <gmock/gmock.h>

#include <google/protobuf/text_format.h>
#include <google/protobuf/util/message_differencer.h>

#include <string>

namespace democrit
{

/**
 * Data for one of the test accounts that we use.
 */
struct TestAccount
{

  /** The username for the XMPP server.  */
  const char* name;

  /** The password for logging into the server.  */
  const char* password;

};

/**
 * Full set of "server configuration" used for testing.
 */
struct ServerConfiguration
{

  /** The XMPP server used.  */
  const char* server;

  /** The MUC service.  */
  const char* muc;

  /** The test accounts.  */
  TestAccount accounts[3];

};

/**
 * Returns the ServerConfiguration instance that should be used throughout
 * testing.
 *
 * This expects a local environment (with server running on localhost)
 * as it is set up e.g. by Charon's test/env Docker scripts.
 */
const ServerConfiguration& GetServerConfig ();

/**
 * Returns the JID of the n-th test account from the selected server config.
 * Optinally adds a specified resource.
 */
gloox::JID GetTestJid (unsigned n, const std::string& res = "");

/**
 * Returns the password for the n-th test account.
 */
std::string GetPassword (unsigned n);

/**
 * Returns the full room JID (including server) to use in tests for
 * a given local room name.
 */
gloox::JID GetRoom (const std::string& nm);

/**
 * Sleeps some short amount of time, which we use to let the server process
 * some things in tests.
 */
void SleepSome ();

/**
 * Parses a protocol buffer from text format.
 */
template <typename Proto>
  Proto
  ParseTextProto (const std::string& str)
{
  Proto res;
  CHECK (google::protobuf::TextFormat::ParseFromString (str, &res));
  return res;
}

MATCHER_P (EqualsOrdersForAsset, str, "")
{
  const auto expected = ParseTextProto<proto::OrderbookForAsset> (str);
  if (google::protobuf::util::MessageDifferencer::Equals (arg, expected))
    return true;

  *result_listener << "actual: " << arg.DebugString ();
  return false;
}

MATCHER_P (EqualsOrdersByAsset, str, "")
{
  const auto expected = ParseTextProto<proto::OrderbookByAsset> (str);
  if (google::protobuf::util::MessageDifferencer::Equals (arg, expected))
    return true;

  *result_listener << "actual: " << arg.DebugString ();
  return false;
}

MATCHER_P (EqualsOrdersOfAccount, str, "")
{
  const auto expected = ParseTextProto<proto::OrdersOfAccount> (str);
  if (google::protobuf::util::MessageDifferencer::Equals (arg, expected))
    return true;

  *result_listener << "actual: " << arg.DebugString ();
  return false;
}

} // namespace democrit

#endif // DEMOCRIT_TESTUTILS_HPP
