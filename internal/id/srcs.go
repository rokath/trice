// SPDX-License-Identifier: MIT

package id

// ArrayFlag is a slice type for multi flag
type ArrayFlag []string

// String method is the needed for interface satisfaction.
func (i *ArrayFlag) String() string {
	return ""
}

// Set is a needed method for multi flags.
func (i *ArrayFlag) Set(value string) error {
	*i = append(*i, value)
	return nil
}
