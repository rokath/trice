-- =============================================================================
-- The following directives assign pins to the locations specific for the
-- CY8CKIT-046 kit.
-- =============================================================================

-- === UART ===
attribute port_location of \SW_Tx_UART:tx(0)\ : label is "PORT(3,1)";