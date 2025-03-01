// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWAnyAddress.h>
#include "Cosmos/Address.h"
#include "proto/Cosmos.pb.h"
#include "HexCoding.h"

#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;

TEST(THORChainTWAnySigner, SignTx) {
    auto privateKey = parse_hex("7105512f0c020a1dd759e14b865ec0125f59ac31e34d7a2807a228ed50cb343e");
    Cosmos::Proto::SigningInput input;
    input.set_account_number(593);
    input.set_chain_id("thorchain");
    input.set_sequence(2);
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_memo("");

    auto fromAddress = "thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r";
    auto toAddress = "thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn";

    auto msg = input.add_messages();
    auto& message = *msg->mutable_send_coins_message();
    message.set_from_address(fromAddress);
    message.set_to_address(toAddress);
    auto amountOfTx = message.add_amounts();
    amountOfTx->set_denom("rune");
    amountOfTx->set_amount(10000000);

    auto& fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("rune");
    amountOfFee->set_amount(2000000);

    Cosmos::Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeTHORChain);

    ASSERT_EQ(output.json(), R"({"mode":"block","tx":{"fee":{"amount":[{"amount":"2000000","denom":"rune"}],"gas":"200000"},"memo":"","msg":[{"type":"cosmos-sdk/MsgSend","value":{"amount":[{"amount":"10000000","denom":"rune"}],"from_address":"thor1z53wwe7md6cewz9sqwqzn0aavpaun0gw0exn2r","to_address":"thor1e2ryt8asq4gu0h6z2sx9u7rfrykgxwkmr9upxn"}}],"signatures":[{"pub_key":{"type":"tendermint/PubKeySecp256k1","value":"A+2Zfjls9CkvX85aQrukFZnM1dluMTFUp8nqcEneMXx3"},"signature":"FehJoB+jbdUVXfEOFb9VKf5IpZF/HxIpOMPXe5Y3gb97PmpwLsASn7JJYBEU8OtW8z64W5rNo6UgCAt6IEv8fA=="}]}})");
}
