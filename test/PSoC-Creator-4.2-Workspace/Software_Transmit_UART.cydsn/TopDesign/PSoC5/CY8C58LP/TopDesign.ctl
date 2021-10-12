-- =============================================================================
-- The following directives assign pins to the locations specific for the
-- CY8CKIT-001 kit with PSoC 5LP module and CY8CKIT-050 kit.
-- =============================================================================

-- === UART ===
attribute port_location of \SW_Tx_UART:tx(0)\ : label is "PORT(0,5)";