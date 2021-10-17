-- =============================================================================
-- The following directives assign pins to the locations specific for the
-- CY8CKIT-001 kit with PSoC 3 module and CY8CKIT-030 kit.
-- =============================================================================

-- === UART ===
attribute port_location of \SW_Tx_UART:tx(0)\ : label is "PORT(0,5)";