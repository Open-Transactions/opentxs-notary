/************************************************************
 *
 *  Transactor.cpp
 *
 */

/************************************************************
 -----BEGIN PGP SIGNED MESSAGE-----
 Hash: SHA1

 *                 OPEN TRANSACTIONS
 *
 *       Financial Cryptography and Digital Cash
 *       Library, Protocol, API, Server, CLI, GUI
 *
 *       -- Anonymous Numbered Accounts.
 *       -- Untraceable Digital Cash.
 *       -- Triple-Signed Receipts.
 *       -- Cheques, Vouchers, Transfers, Inboxes.
 *       -- Basket Currencies, Markets, Payment Plans.
 *       -- Signed, XML, Ricardian-style Contracts.
 *       -- Scripted smart contracts.
 *
 *  Copyright (C) 2010-2013 by "Fellow Traveler" (A pseudonym)
 *
 *  EMAIL:
 *  FellowTraveler@rayservers.net
 *
 *  BITCOIN:  1NtTPVVjDsUfDWybS4BwvHpG2pdS9RnYyQ
 *
 *  KEY FINGERPRINT (PGP Key in license file):
 *  9DD5 90EB 9292 4B48 0484  7910 0308 00ED F951 BB8E
 *
 *  OFFICIAL PROJECT WIKI(s):
 *  https://github.com/FellowTraveler/Moneychanger
 *  https://github.com/FellowTraveler/Open-Transactions/wiki
 *
 *  WEBSITE:
 *  http://www.OpenTransactions.org/
 *
 *  Components and licensing:
 *   -- Moneychanger..A Java client GUI.....LICENSE:.....GPLv3
 *   -- otlib.........A class library.......LICENSE:...LAGPLv3
 *   -- otapi.........A client API..........LICENSE:...LAGPLv3
 *   -- opentxs/ot....Command-line client...LICENSE:...LAGPLv3
 *   -- otserver......Server Application....LICENSE:....AGPLv3
 *  Github.com/FellowTraveler/Open-Transactions/wiki/Components
 *
 *  All of the above OT components were designed and written by
 *  Fellow Traveler, with the exception of Moneychanger, which
 *  was contracted out to Vicky C (bitcointrader4@gmail.com).
 *  The open-source community has since actively contributed.
 *
 *  -----------------------------------------------------
 *
 *   LICENSE:
 *   This program is free software: you can redistribute it
 *   and/or modify it under the terms of the GNU Affero
 *   General Public License as published by the Free Software
 *   Foundation, either version 3 of the License, or (at your
 *   option) any later version.
 *
 *   ADDITIONAL PERMISSION under the GNU Affero GPL version 3
 *   section 7: (This paragraph applies only to the LAGPLv3
 *   components listed above.) If you modify this Program, or
 *   any covered work, by linking or combining it with other
 *   code, such other code is not for that reason alone subject
 *   to any of the requirements of the GNU Affero GPL version 3.
 *   (==> This means if you are only using the OT API, then you
 *   don't have to open-source your code--only your changes to
 *   Open-Transactions itself must be open source. Similar to
 *   LGPLv3, except it applies to software-as-a-service, not
 *   just to distributing binaries.)
 *
 *   Extra WAIVER for OpenSSL, Lucre, and all other libraries
 *   used by Open Transactions: This program is released under
 *   the AGPL with the additional exemption that compiling,
 *   linking, and/or using OpenSSL is allowed. The same is true
 *   for any other open source libraries included in this
 *   project: complete waiver from the AGPL is hereby granted to
 *   compile, link, and/or use them with Open-Transactions,
 *   according to their own terms, as long as the rest of the
 *   Open-Transactions terms remain respected, with regard to
 *   the Open-Transactions code itself.
 *
 *   Lucre License:
 *   This code is also "dual-license", meaning that Ben Lau-
 *   rie's license must also be included and respected, since
 *   the code for Lucre is also included with Open Transactions.
 *   See Open-Transactions/src/otlib/lucre/LUCRE_LICENSE.txt
 *   The Laurie requirements are light, but if there is any
 *   problem with his license, simply remove the Lucre code.
 *   Although there are no other blind token algorithms in Open
 *   Transactions (yet. credlib is coming), the other functions
 *   will continue to operate.
 *   See Lucre on Github:  https://github.com/benlaurie/lucre
 *   -----------------------------------------------------
 *   You should have received a copy of the GNU Affero General
 *   Public License along with this program.  If not, see:
 *   http://www.gnu.org/licenses/
 *
 *   If you would like to use this software outside of the free
 *   software license, please contact FellowTraveler.
 *   (Unfortunately many will run anonymously and untraceably,
 *   so who could really stop them?)
 *
 *   DISCLAIMER:
 *   This program is distributed in the hope that it will be
 *   useful, but WITHOUT ANY WARRANTY; without even the implied
 *   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *   PURPOSE.  See the GNU Affero General Public License for
 *   more details.

 -----BEGIN PGP SIGNATURE-----
 Version: GnuPG v1.4.9 (Darwin)

 iQIcBAEBAgAGBQJRSsfJAAoJEAMIAO35UbuOQT8P/RJbka8etf7wbxdHQNAY+2cC
 vDf8J3X8VI+pwMqv6wgTVy17venMZJa4I4ikXD/MRyWV1XbTG0mBXk/7AZk7Rexk
 KTvL/U1kWiez6+8XXLye+k2JNM6v7eej8xMrqEcO0ZArh/DsLoIn1y8p8qjBI7+m
 aE7lhstDiD0z8mwRRLKFLN2IH5rAFaZZUvj5ERJaoYUKdn4c+RcQVei2YOl4T0FU
 LWND3YLoH8naqJXkaOKEN4UfJINCwxhe5Ke9wyfLWLUO7NamRkWD2T7CJ0xocnD1
 sjAzlVGNgaFDRflfIF4QhBx1Ddl6wwhJfw+d08bjqblSq8aXDkmFA7HeunSFKkdn
 oIEOEgyj+veuOMRJC5pnBJ9vV+7qRdDKQWaCKotynt4sWJDGQ9kWGWm74SsNaduN
 TPMyr9kNmGsfR69Q2Zq/FLcLX/j8ESxU+HYUB4vaARw2xEOu2xwDDv6jt0j3Vqsg
 x7rWv4S/Eh18FDNDkVRChiNoOIilLYLL6c38uMf1pnItBuxP3uhgY6COm59kVaRh
 nyGTYCDYD2TK+fI9o89F1297uDCwEJ62U0Q7iTDp5QuXCoxkPfv8/kX6lS6T3y9G
 M9mqIoLbIQ1EDntFv7/t6fUTS2+46uCrdZWbQ5RjYXdrzjij02nDmJAm2BngnZvd
 kamH0Y/n11lCvo1oQxM+
 =uSzz
 -----END PGP SIGNATURE-----
**************************************************************/

#include "Transactor.hpp"
#include "OTServer.hpp"
#include <opentxs/core/OTIdentifier.hpp>
#include <opentxs/core/OTPseudonym.hpp>
#include <opentxs/core/OTString.hpp>
#include <opentxs/core/OTLog.hpp>

namespace opentxs
{

Transactor::Transactor(OTServer* server)
    : transactionNumber_(0)
    , server_(server)
{
}

/// Just as every request must be accompanied by a request number, so
/// every transaction request must be accompanied by a transaction number.
/// The request numbers can simply be incremented on both sides (per user.)
/// But the transaction numbers must be issued by the server and they do
/// not repeat from user to user. They are unique to transaction.
///
/// Users must ask the server to send them transaction numbers so that they
/// can be used in transaction requests. The server keeps an internal counter
/// and maintains a data file to store the latest one.
///
/// More specifically, the server file itself stores the latest transaction
/// number
/// (So it knows what number to issue and increment when the next request comes
/// in.)
///
/// But once it issues the next number, that number needs to be recorded in the
/// nym file
/// for the user it was issued to, so that it can be verified later when he
/// submits it
/// for a transaction--and so it can be removed once the transaction is complete
/// (so it
/// won't work twice.)
///
/// The option to bSaveTheNumber defaults to true for this reason. But sometimes
/// it
/// will be sent to false, in cases where the number doesn't need to be saved
/// because
/// it's never going to be verified. For example, if the server creates a
/// transaction
/// number so it can put a transaction into your inbox, it's never going to have
/// to verify
/// that it actually put it into the inbox by checking it's own nymfile for that
/// transaction
/// number. Instead it would just check its own server signature on the inbox.
/// But I digress...
///
bool Transactor::issueNextTransactionNumber(OTPseudonym& theNym,
                                            int64_t& lTransactionNumber,
                                            bool bStoreTheNumber)
{
    OTIdentifier NYM_ID(theNym), SERVER_NYM_ID(server_->m_nymServer);

    // If theNym has the same ID as server_->m_nymServer, then we'll use
    // server_->m_nymServer
    // instead of theNym.  (Since it's the same nym anyway, we'll stick to the
    // one we already loaded so any changes don't get overwritten later.)
    OTPseudonym* pNym = nullptr;

    if (NYM_ID == SERVER_NYM_ID)
        pNym = &server_->m_nymServer;
    else
        pNym = &theNym;

    // transactionNumber_ stores the last VALID AND ISSUED transaction number.
    // So first, we increment that, since we don't want to issue the same number
    // twice.
    transactionNumber_++;

    // Next, we save it to file.
    if (!server_->SaveMainFile()) {
        OTLog::Error("Error saving main server file.\n");
        transactionNumber_--;
        return false;
    }

    // Each Nym stores the transaction numbers that have been issued to it.
    // (On client AND server side.)
    //
    // So whenever the server issues a new number, it's to a specific Nym, then
    // it is recorded in his Nym file before being sent to the client (where it
    // is also recorded in his Nym file.)  That way the server always knows
    // which
    // numbers are valid for each Nym.
    else if (bStoreTheNumber &&
             (false == pNym->AddTransactionNum(
                           server_->m_nymServer, server_->m_strServerID,
                           transactionNumber_, true))) // bSave = true
    {
        OTLog::Error("Error adding transaction number to Nym file.\n");
        transactionNumber_--;
        server_->SaveMainFile(); // Save it back how it was, since we're not
                                 // issuing this
                                 // number after all.
        return false;
    }

    // SUCCESS?
    // Now the server main file has saved the latest transaction number,
    // and the number has been stored on the relevant nym file.
    // NOW we set it onto the parameter and return true.
    else {
        lTransactionNumber = transactionNumber_;
        return true;
    }
}

/// Transaction numbers are now stored in the nym file (on client and server
/// side) for whichever nym
/// they were issued to. This function verifies whether or not the transaction
/// number is present and valid
/// for any specific nym (i.e. for the nym passed in.)
bool Transactor::verifyTransactionNumber(
    OTPseudonym& theNym, const int64_t& lTransactionNumber) // passed by
                                                            // reference for
                                                            // speed, but not a
                                                            // return value.
{
    OTIdentifier NYM_ID(theNym), SERVER_NYM_ID(server_->m_nymServer);

    // If theNym has the same ID as server_->m_nymServer, then we'll use
    // server_->m_nymServer
    // instead of theNym.  (Since it's the same nym anyway, we'll stick to the
    // one we already loaded so any changes don't get overwritten later.)
    OTPseudonym* pNym = nullptr;

    if (NYM_ID == SERVER_NYM_ID)
        pNym = &server_->m_nymServer;
    else
        pNym = &theNym;
    if (pNym->VerifyTransactionNum(server_->m_strServerID, lTransactionNumber))
        return true;
    else {
        const OTString strNymID(NYM_ID);
        const OTString strIssued(
            pNym->VerifyIssuedNum(server_->m_strServerID, lTransactionNumber)
                ? "(However, that number IS issued to that Nym... He must have "
                  "already used it.)\n"
                : "(In fact, that number isn't even issued to that Nym, though "
                  "perhaps it was at some time in the past?)\n");

        OTLog::vError("%s: %ld not available for Nym %s to use. \n%s",
                      __FUNCTION__,
                      //                    " Oh, and FYI, tangentially, the
                      // current Trns# counter is: %ld\n",
                      lTransactionNumber, strNymID.Get(), strIssued.Get());
        //                    transactionNumber_);
    }

    return false;
}

/// Remove a transaction number from the Nym record once it's officially
/// used/spent.
bool Transactor::removeTransactionNumber(OTPseudonym& theNym,
                                         const int64_t& lTransactionNumber,
                                         bool bSave)
{
    OTIdentifier NYM_ID(theNym), SERVER_NYM_ID(server_->m_nymServer);

    // If theNym has the same ID as server_->m_nymServer, then we'll use
    // server_->m_nymServer
    // instead of theNym.  (Since it's the same nym anyway, we'll stick to the
    // one we already loaded so any changes don't get overwritten later.)
    OTPseudonym* pNym = nullptr;

    if (NYM_ID == SERVER_NYM_ID)
        pNym = &server_->m_nymServer;
    else
        pNym = &theNym;

    bool bRemoved = false;

    if (bSave)
        bRemoved = pNym->RemoveTransactionNum(
            server_->m_nymServer, server_->m_strServerID,
            lTransactionNumber); // the version that passes in a signer nym --
                                 // saves to local storage.
    else
        bRemoved = pNym->RemoveTransactionNum(
            server_->m_strServerID,
            lTransactionNumber); // the version that doesn't save.

    return bRemoved;
}

/// Remove an issued number from the Nym record once that nym accepts the
/// receipt from his inbox.
bool Transactor::removeIssuedNumber(OTPseudonym& theNym,
                                    const int64_t& lTransactionNumber,
                                    bool bSave)
{
    OTIdentifier NYM_ID(theNym), SERVER_NYM_ID(server_->m_nymServer);

    // If theNym has the same ID as server_->m_nymServer, then we'll use
    // server_->m_nymServer
    // instead of theNym.  (Since it's the same nym anyway, we'll stick to the
    // one we already loaded so any changes don't get overwritten later.)
    OTPseudonym* pNym = nullptr;

    if (NYM_ID == SERVER_NYM_ID)
        pNym = &server_->m_nymServer;
    else
        pNym = &theNym;

    bool bRemoved =
        pNym->RemoveIssuedNum(server_->m_nymServer, server_->m_strServerID,
                              lTransactionNumber, bSave);

    return bRemoved;
}

} // namespace opentxs
