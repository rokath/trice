# abc_tx_host

> Retired documentation from `_test/abc_tx_host/README.md`. Use [Test Internals](../../../../../rm/target_test_internals.md#add-or-change-a-test) and [Running the Tests](../../../../../rm/target_tests.md). The original text below is retained as history.

This test package checks the Trice ABC transmit macros on the host side.

It compiles a small C fixture with `TRICE_TX_ABC_SUPPORT 1`, captures the
raw bytes produced by selected `triceC`/`TriceC`/`TRiceC` and
`trice8C`/`trice16C`/`trice32C` calls, and compares them against fixed byte
fixtures.

The intent is to verify the emitted ABC wire format only. It is not a receive
test and it does not require a target-side ABC handler table.
