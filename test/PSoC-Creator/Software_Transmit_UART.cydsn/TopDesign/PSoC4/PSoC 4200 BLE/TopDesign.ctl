-- =============================================================================
-- The following directives assign pins to the locations specific for the
-- CY8CKIT-042-BLE and CY8CKIT-042-BLE-A kits.
-- =============================================================================

-- === UART ===
attribute port_location of \SW_Tx_UART:tx(0)\ : label is "PORT(1,5)";