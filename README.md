# Luckycoin Core [LKY, L]
# ERC-20 Support Wallet 0x83F06EF8AAfE1844b0dB4f413f760621A21F6C56

![Luckycoin](https://github.com/luiselzate/luckycoin_1.14.3/blob/main/src/qt/res/icons/wallet_bgcoin.png?raw=true)

Luckycoin is a cryptocurrency like Bitcoin, it does not use SHA256 as
its proof of work (POW). Taking development cues from Tenebrix and Litecoin,
Luckycoin currently employs a simplified variant of scrypt.
- **Website:** [luckycoinblockexplorer.com.](https://luckycoinblockexplorer.com/)

## License
Luckycoin Core is released under the terms of the MIT license. See
[COPYING](COPYING) for more information or see
[opensource.org](https://opensource.org/licenses/MIT)

## Development and contributions – developers
Development is ongoing, and the development team, as well as other volunteers,
can freely work in their own trees and submit pull requests when features or
bug fixes are ready.

#### Version strategy
Version numbers are following ```major.minor.patch``` semantics.

#### Branches
There are 3 types of branches in this repository:

- **master:** Stable, contains the latest version of the latest *major.minor* release.
- **maintenance:** Stable, contains the latest version of previous releases, which are still under active maintenance. Format: ```<version>-maint```
- **development:** Unstable, contains new code for planned releases. Format: ```<version>-dev```

*Master and maintenance branches are exclusively mutable by release. Planned*
*releases will always have a development branch and pull requests should be*
*submitted against those. Maintenance branches are there for **bug fixes only,***
*please submit new features against the development branch with the highest version.*

#### Contributions ✍️

Developers are strongly encouraged to write [unit tests](src/test/README.md) for new code, and to
submit new unit tests for old code. Unit tests can be compiled and run
(assuming they weren't disabled in configure) with: `make check`. Further details on running
and extending unit tests can be found in [/src/test/README.md](/src/test/README.md).

There are also [regression and integration tests](/qa) of the RPC interface, written
in Python, that are run automatically on the build server.
These tests can be run (if the [test dependencies](/qa) are installed) with: `qa/pull-tester/rpc-tests.py`

Changes should be tested by somebody other than the developer who wrote the
code. This is especially important for large or high-risk changes. It is useful
to add a test plan to the pull request description if testing the changes is
not straightforward.

## Frequently Asked Questions ❓

### How much LKY can exist?
Approximately 20,000,000 coins.
Each subsequent block will grant 0.0013 coins to encourage miners to continue to
secure the network and make up for lost wallets on hard drives/phones/lost
encryption passwords/etc.


### Mining information ⛏

Luckycoin uses a simplified variant of the scrypt key derivation function as its
proof of work with a target time of one minute per block and difficulty
readjustment after every block. The block rewards are fixed.

Originally, a different payout scheme was envisioned with block rewards being
determined by taking the maximum reward as per the block schedule and applying
the result of a Mersenne Twister pseudo-random number generator to arrive at a
number between 0 and the maximum reward.

Difficulty retargeting was also changed from four hours to once per block (every minute),
implementing an algorithm courtesy of the DigiByte Coin development team, to
lessen the impact of sudden increases and decreases of network hashing rate.

### make luckycoind/luckycoin-cli/luckycoin-qt

  The following are developer notes on how to build luckycoin on your native platform. They are not complete guides, but include notes on the necessary libraries, compile flags, etc.

  - [OSX Build Notes](doc/build-osx.md)
  - [Unix Build Notes](doc/build-unix.md)
  - [Windows Build Notes](doc/build-windows.md)

### ports

- RPC 9918
- P2P 9917
