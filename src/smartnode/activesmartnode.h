// Copyright (c) 2014-2017 The Dash Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ACTIVESMARTNODE_H
#define ACTIVESMARTNODE_H

#include "../net.h"
#include "../key.h"
#include "../wallet/wallet.h"

class CActiveSmartnode;

static const int ACTIVE_SMARTNODE_INITIAL          = 0; // initial state
static const int ACTIVE_SMARTNODE_SYNC_IN_PROCESS  = 1;
static const int ACTIVE_SMARTNODE_INPUT_TOO_NEW    = 2;
static const int ACTIVE_SMARTNODE_NOT_CAPABLE      = 3;
static const int ACTIVE_SMARTNODE_STARTED          = 4;

extern CActiveSmartnode activeSmartnode;

// Responsible for activating the Smartnode and pinging the network
class CActiveSmartnode
{
public:
    enum smartnode_type_enum_t {
        SMARTNODE_UNKNOWN = 0,
        SMARTNODE_REMOTE  = 1,
        SMARTNODE_LOCAL   = 2
    };

private:
    // critical section to protect the inner data structures
    mutable CCriticalSection cs;

    smartnode_type_enum_t eType;

    bool fPingerEnabled;

    /// Ping Smartnode
    bool SendSmartnodePing();

public:
    // Keys for the active Smartnode
    CPubKey pubKeySmartnode;
    CKey keySmartnode;

    // Initialized while registering Smartnode
    CTxIn vin;
    CService service;

    int nState; // should be one of ACTIVE_SMARTNODE_XXXX
    std::string strNotCapableReason;

    CActiveSmartnode()
        : eType(SMARTNODE_UNKNOWN),
          fPingerEnabled(false),
          pubKeySmartnode(),
          keySmartnode(),
          vin(),
          service(),
          nState(ACTIVE_SMARTNODE_INITIAL)
    {}

    /// Manage state of active Smartnode
    void ManageState();

    std::string GetStateString() const;
    std::string GetStatus() const;
    std::string GetTypeString() const;

private:
    void ManageStateInitial();
    void ManageStateRemote();
    void ManageStateLocal();
};

#endif
